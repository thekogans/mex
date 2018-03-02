// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/util/XercesUtils.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/SpeedBar.h"

namespace thekogans {
    namespace mex {
        namespace core {

            THEKOGANS_UTIL_IMPLEMENT_HEAP (SpeedBar::Item)

            namespace {
                // Believe it or not, but just declaring map static
                // does not guarantee proper ctor call order!? Wrapping
                // it in an accessor function does.
                SpeedBar::Item::EventHandler::Map &GetMap () {
                    static SpeedBar::Item::EventHandler::Map map;
                    return map;
                }
            }

            SpeedBar::Item::EventHandler *SpeedBar::Item::EventHandler::Get (
                    const std::string &name, Module &module) {
                Map::iterator it = GetMap ().find (name);
                return it != GetMap ().end () ? it->second (module) : 0;
            }

            SpeedBar::Item::EventHandler::MapInitializer::MapInitializer (
                    const std::string &eventHandler, Factory factory) {
                std::pair<Item::EventHandler::Map::iterator, bool> result =
                    GetMap ().insert (Item::EventHandler::Map::value_type (eventHandler, factory));
                assert (result.second);
            }

            void SpeedBar::Item::ToolEventHandler::OnSetFocus () {
                module.viewLayout->SetTool (&tool);
            }

            void SpeedBar::Item::ToolEventHandler::OnUpdateUI (UI &ui) {
                ui.SetSticky (true);
                ui.SetEnabled (module.viewLayout->tool == &tool);
                ui.SetChecked (false);
            }

            namespace {
                void ParseItem (const XERCES_CPP_NAMESPACE::DOMNode &node,
                        SpeedBar::Item *item, Module &module) {
                    const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes = node.getAttributes ();
                    std::string text = util::GetAttributeValue ("text", attributes);
                    std::string eventHandlerName = module.GetType () +
                        util::GetAttributeValue ("eventHandler", attributes);
                    SpeedBar::Item::EventHandler *eventHandler =
                        SpeedBar::Item::EventHandler::Get (eventHandlerName, module);
                    if (eventHandler == 0) {
                        if (util::XMLChTostring (node.getNodeName ()) == "popup") {
                            eventHandler = new SpeedBar::Item::PopupEventHandler (item);
                        }
                        else {
                            assert (UI::Instance ().consoleWindow != 0);
                            UI::Instance ().consoleWindow->Print (
                                "SpeedBar: Unable to locate handler for '%s'.\n",
                                eventHandlerName.c_str ());
                            struct DummyCommandEventHandler :
                                    public SpeedBar::Item::CommandEventHandler {
                                virtual void OnSetFocus () {}
                            } static dummyCommandEventHandler;
                            eventHandler = &dummyCommandEventHandler;
                        }
                    }
                    item = new SpeedBar::Item (text, eventHandler, item);
                    XERCES_CPP_NAMESPACE::DOMNodeList *children = node.getChildNodes ();
                    for (util::ui32 i = 0, count = children->getLength (); i < count; ++i) {
                        const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                        if (child.getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                            (util::XMLChTostring (child.getNodeName ()) == "popup" ||
                                util::XMLChTostring (child.getNodeName ()) == "item")) {
                            ParseItem (child, item, module);
                        }
                    }
                }
            }

            SpeedBar::SpeedBar (const std::string &path, Module &module) :
                    origin (blas::Point::Zero) {
                util::XercesPlatformInit xercesPlatformInit;
                util::XercesErrorHandler errorHandler;
                XERCES_CPP_NAMESPACE::XercesDOMParser parser;
                parser.setErrorHandler (&errorHandler);
                try {
                    parser.parse (path.c_str ());
                    XERCES_CPP_NAMESPACE::DOMNodeList *children =
                        parser.getDocument ()->getDocumentElement ()->getChildNodes ();
                    for (util::ui32 i = 0, count = children->getLength (); i < count; ++i) {
                        const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                        if (child.getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                            (util::XMLChTostring (child.getNodeName ()) == "popup" ||
                                util::XMLChTostring (child.getNodeName ()) == "item")) {
                            ParseItem (child, &root, module);
                        }
                    }
                }
                catch (const std::string &message) {
                    assert (UI::Instance ().consoleWindow != 0);
                    UI::Instance ().consoleWindow->Print (
                        "SpeedBar: Unable to load '%s'.\n\t%s\n",
                        path.c_str (), message.c_str ());
                }
            }

            util::ui32 SpeedBar::GetItemCount () const {
                util::ui32 nitems = 0;
                for (const Item *item = &root; item != 0; item = item->GetCurrChild ()) {
                    nitems += item->children.size ();
                }
                return nitems;
            }

            void SpeedBar::SetCurrItem (util::ui32 itemIndex) {
                for (Item *item = &root; item != 0; item = item->GetCurrChild ()) {
                    const std::vector<Item *> &children = item->children;
                    if (itemIndex < children.size ()) {
                        class UI : public Item::EventHandler::UI {
                        public:
                            bool sticky;
                            bool enabled;
                            bool checked;

                            UI (Item &item) : SpeedBar::Item::EventHandler::UI (item),
                                sticky (false), enabled (false), checked (false) {}

                            virtual void SetSticky (bool sticky_) {sticky = sticky_;}
                            virtual void SetEnabled (bool enabled_) {enabled = enabled_;}
                            virtual void SetChecked (bool checked_) {checked = checked_;}
                        } ui (*children[itemIndex]);
                        children[itemIndex]->eventHandler->OnUpdateUI (ui);
                        if (ui.sticky) {
                            std::vector<Item *> parents;
                            for (Item *parent = item->GetCurrChild ();
                                    parent != 0; parent = parent->GetCurrChild ()) {
                                parents.push_back (parent);
                            }
                            for (std::size_t i = parents.size (); i--;) {
                                parents[i]->eventHandler->OnKillFocus ();
                            }
                            item->currChild = itemIndex;
                            for (item = item->GetCurrChild ();
                                    item != 0; item = item->GetCurrChild ()) {
                                item->eventHandler->OnSetFocus ();
                            }
                        }
                        else {
                            children[itemIndex]->eventHandler->OnSetFocus ();
                        }
                        return;
                    }
                    itemIndex -= children.size ();
                    /*
                      if (item->GetCurrChild () != 0) {
                      item->GetCurrChild ()->eventHandler->OnSetFocus ();
                      }
                    */
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
