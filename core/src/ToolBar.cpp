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

#include <cassert>
#include <cctype>
#include "thekogans/util/Buffer.h"
#include "thekogans/util/ByteSwap.h"
#include "thekogans/util/XercesUtils.h"
#include "thekogans/util/Exception.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/ToolBar.h"

namespace thekogans {
    namespace mex {
        namespace core {

            THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (ToolBar::Item)

            namespace {
                // Believe it or not, but just declaring map static
                // does not guarantee proper ctor call order!? Wrapping
                // it in an accessor function does.
                ToolBar::Item::EventHandler::Map &GetMap () {
                    static ToolBar::Item::EventHandler::Map map;
                    return map;
                }
            }

            ToolBar::Item::EventHandler *ToolBar::Item::EventHandler::Get (
                const std::string &eventHandler, Module &module) {
                Map::iterator it = GetMap ().find (eventHandler);
                return it != GetMap ().end () ? it->second (module) : 0;
            }

            ToolBar::Item::EventHandler::MapInitializer::MapInitializer (
                const std::string &eventHandler, Factory factory) {
                std::pair<Item::EventHandler::Map::iterator, bool> result =
                    GetMap ().insert (Item::EventHandler::Map::value_type (
                            eventHandler, factory));
                assert (result.second);
            }

            void ToolBar::Item::ToolEventHandler::OnClickLeft () {
                module.viewLayout->SetTool (&tool);
            }

            void ToolBar::Item::ToolEventHandler::OnUpdateUI (UI &ui) {
                ui.SetEnabled (true);
                ui.SetCheckable (true);
                ui.SetChecked (module.viewLayout->tool == &tool);
            }

            namespace {
                inline util::ui8 GetNumber (char ch) {
                    return isdigit (ch) ? (ch - '0') : (10 + tolower (ch) - 'a');
                }

                _3ds::opengl::Image::UniquePtr GetImage (const XERCES_CPP_NAMESPACE::DOMNode &node) {
                    XERCES_CPP_NAMESPACE::DOMNodeList *children = node.getChildNodes ();
                    if (children->getLength () == 1) {
                        const XERCES_CPP_NAMESPACE::DOMNode *child = children->item (0);
                        if (child->getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::TEXT_NODE) {
                            std::string str = util::XMLChTostring (child->getNodeValue ());
                            std::vector<util::ui8> bits;
                            for (std::size_t i = 0, count = str.size (); i < count;) {
                                if (str[i] == '0' && str[i + 1] == 'x' &&
                                        isxdigit (str[i + 2]) && isxdigit (str[i + 3])) {
                                    bits.push_back (GetNumber (str[i + 2]) * 16 + GetNumber (str[i + 3]));
                                    i += 4;
                                }
                                else {
                                    ++i;
                                }
                            }
                            if (!bits.empty ()) {
                                util::TenantReadBuffer buffer (util::LittleEndian, &bits[0], bits.size ());
                                _3ds::opengl::Image::UniquePtr image (new _3ds::opengl::Image ());
                                if (image->Load (buffer)) {
                                    return image;
                                }
                            }
                        }
                        // FIXME: log error
                        assert (0);
                    }
                    // FIXME: log error
                    assert (0);
                    return _3ds::opengl::Image::UniquePtr ();
                }

                void ParseItem (
                        const XERCES_CPP_NAMESPACE::DOMNode &node,
                        Module &module,
                        std::vector<ToolBar::Item *> &items) {
                    if (util::XMLChTostring (node.getNodeName ()) == "item") {
                        const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes = node.getAttributes ();
                        std::string toolTip = util::GetAttributeValue ("toolTip", attributes);
                        std::string statusTip = util::GetAttributeValue ("statusTip", attributes);
                        _3ds::opengl::Image::UniquePtr image = GetImage (node);
                        assert (image.get () != 0);
                        std::string eventHandlerName = module.GetType () +
                            util::GetAttributeValue ("eventHandler", attributes);
                        ToolBar::Item::EventHandler *eventHandler =
                            ToolBar::Item::EventHandler::Get (eventHandlerName, module);
                        if (eventHandler == 0) {
                            assert (UI::Instance ()->consoleWindow != 0);
                            UI::Instance ()->consoleWindow->Print (
                                "ToolBar: Unable to locate handler for '%s'.\n", eventHandlerName.c_str ());
                            struct DummyEventHandler : public ToolBar::Item::EventHandler {
                                virtual void OnClickLeft () {}
                                virtual void OnUpdateUI (UI &ui) {
                                    ui.SetEnabled (false);
                                    ui.SetCheckable (false);
                                    ui.SetChecked (false);
                                }
                            } static dummyEventHandler;
                            eventHandler = &dummyEventHandler;
                        }
                        items.push_back (new ToolBar::Item (toolTip, statusTip, std::move (image), eventHandler));
                    }
                    else if (util::XMLChTostring (node.getNodeName ()) == "separator") {
                        // separator
                        items.push_back (0);
                    }
                    else {
                        assert (0);
                    }
                }
            }

            ToolBar::ToolBar (const std::string &path, Module &module) :
                    width (0),
                    height (0) {
                util::XercesPlatformInit xercesPlatformInit;
                util::XercesErrorHandler errorHandler;
                XERCES_CPP_NAMESPACE::XercesDOMParser parser;
                parser.setErrorHandler (&errorHandler);
                try {
                    parser.parse (path.c_str ());
                    const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes =
                        parser.getDocument ()->getDocumentElement ()->getAttributes ();
                    width = util::stringToui32 (util::GetAttributeValue ("width", attributes).c_str ());
                    height = util::stringToui32 (util::GetAttributeValue ("height", attributes).c_str ());
                    const XERCES_CPP_NAMESPACE::DOMNodeList *children =
                        parser.getDocument ()->getDocumentElement ()->getChildNodes ();
                    for (std::size_t i = 0, count = children->getLength (); i < count; ++i) {
                        const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                        if (child.getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                            (util::XMLChTostring (child.getNodeName ()) == "item" ||
                                util::XMLChTostring (child.getNodeName ()) == "separator")) {
                            ParseItem (child, module, items);
                        }
                    }
                }
                catch (const std::string &message) {
                    assert (UI::Instance ()->consoleWindow != 0);
                    UI::Instance ()->consoleWindow->Print (
                        "ToolBar: Unable to load '%s'.\n\t%s\n",
                        path.c_str (), message.c_str ());
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
