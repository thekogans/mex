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
#include "thekogans/mex/core/MenuBar.h"

namespace thekogans {
    namespace mex {
        namespace core {

            THEKOGANS_UTIL_IMPLEMENT_HEAP (MenuBar::Item)

            namespace {
                // Believe it or not, but just declaring map static
                // does not guarantee proper ctor call order!? Wrapping
                // it in an accessor function does.
                MenuBar::Item::EventHandler::Map &GetMap () {
                    static MenuBar::Item::EventHandler::Map map;
                    return map;
                }
            }

            MenuBar::Item::EventHandler *MenuBar::Item::EventHandler::Get (const std::string &name, Module &module) {
                Map::iterator it = GetMap ().find (name);
                return it != GetMap ().end () ? it->second (module) : 0;
            }

            MenuBar::Item::EventHandler::MapInitializer::MapInitializer (const std::string &eventHandler, Factory factory) {
                std::pair<Item::EventHandler::Map::iterator, bool> result =
                    GetMap ().insert (Item::EventHandler::Map::value_type (eventHandler, factory));
                assert (result.second);
            }

            void MenuBar::Item::ToolEventHandler::OnSetFocus () {
                module.viewLayout->SetTool (&tool);
            }

            void  MenuBar::Item::ToolEventHandler::OnUpdateUI (UI &ui) {
                ui.SetEnabled (true);
                ui.SetCheckable (true);
                ui.SetChecked (module.viewLayout->tool == &tool);
            }

            namespace {
                void ParseItem (const XERCES_CPP_NAMESPACE::DOMNode &node, MenuBar::Item *item, Module &module) {
                    const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes = node.getAttributes ();
                    if (util::XMLChTostring (node.getNodeName ()) == "popup") {
                        std::string text = util::GetAttributeValue ("text", attributes);
                        std::string statusTip = util::GetAttributeValue ("statusTip", attributes);
                        item = new MenuBar::Item (MenuBar::Item::TYPE_POPUP,
                            text, statusTip, "no", 0, item);
                    }
                    else if (util::XMLChTostring (node.getNodeName ()) == "item") {
                        std::string text = util::GetAttributeValue ("text", attributes);
                        std::string statusTip = util::GetAttributeValue ("statusTip", attributes);
                        std::string modules = util::GetAttributeValue ("modules", attributes);
                        std::string eventHandlerName = module.GetType () +
                            util::GetAttributeValue ("eventHandler", attributes);
                        MenuBar::Item::EventHandler *eventHandler =
                            MenuBar::Item::EventHandler::Get (eventHandlerName, module);
                        if (eventHandler == 0) {
                            assert (UI::Instance ().consoleWindow != 0);
                            UI::Instance ().consoleWindow->Print (
                                "MenuBar: Unable to locate handler for '%s'.\n", eventHandlerName.c_str ());
                            struct DummyCommandEventHandler : public MenuBar::Item::EventHandler {
                                virtual void OnSetFocus () {}
                                virtual void OnUpdateUI (UI &ui) {
                                    ui.SetEnabled (false);
                                    ui.SetCheckable (false);
                                    ui.SetChecked (false);
                                }
                            } static dummyCommandEventHandler;
                            eventHandler = &dummyCommandEventHandler;
                        }
                        item = new MenuBar::Item (MenuBar::Item::TYPE_ITEM,
                            text, statusTip, modules, eventHandler, item);
                    }
                    else if (util::XMLChTostring (node.getNodeName ()) == "separator") {
                        item = new MenuBar::Item (MenuBar::Item::TYPE_SEPARATOR,
                            "", "", "no", 0, item);
                    }
                    else {
                        assert (0);
                    }
                    XERCES_CPP_NAMESPACE::DOMNodeList *children = node.getChildNodes ();
                    for (util::ui32 i = 0, count = children->getLength (); i < count; ++i) {
                        const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                        if (child.getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                            (util::XMLChTostring (child.getNodeName ()) == "popup" ||
                                util::XMLChTostring (child.getNodeName ()) == "item" ||
                                util::XMLChTostring (child.getNodeName ()) == "separator")) {
                            ParseItem (child, item, module);
                        }
                    }
                }
            }

            MenuBar::MenuBar (const std::string &path, Module &module) {
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
                                util::XMLChTostring (child.getNodeName ()) == "item" ||
                                util::XMLChTostring (child.getNodeName ()) == "separator")) {
                            ParseItem (child, &root, module);
                        }
                    }
                }
                catch (const std::string &message) {
                    assert (UI::Instance ().consoleWindow != 0);
                    UI::Instance ().consoleWindow->Print (
                        "MenuBar: Unable to load '%s'.\n\t%s\n",
                        path.c_str (), message.c_str ());
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
