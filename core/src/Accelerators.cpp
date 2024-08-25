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
#include "thekogans/util/XercesUtils.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/Accelerators.h"

namespace thekogans {
    namespace mex {
        namespace core {

            THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Accelerators::Item)

            namespace {
                // Believe it or not, but just declaring map static
                // does not guarantee proper ctor call order!? Wrapping
                // it in an accessor function does.
                Accelerators::Item::EventHandler::Map &GetMap () {
                    static Accelerators::Item::EventHandler::Map map;
                    return map;
                }
            }

            Accelerators::Item::EventHandler *Accelerators::Item::EventHandler::Get (
                    const std::string &eventHandler, Module &module) {
                Map::iterator it = GetMap ().find (eventHandler);
                return it != GetMap ().end () ? it->second (module) : 0;
            }

            Accelerators::Item::EventHandler::MapInitializer::MapInitializer (
                    const std::string &eventHandler, Factory factory) {
                std::pair<Item::EventHandler::Map::iterator, bool> result =
                    GetMap ().insert (Item::EventHandler::Map::value_type (eventHandler, factory));
                assert (result.second);
            }

            namespace {
                util::ui32 ParseCh (const std::string str) {
                    if (!str.empty ()) {
                        return tolower (str[0]);
                    }
                    return 0;
                }

                util::ui32 ParseFlags (const std::string str) {
                    util::ui32 flags = 0;
                    if (strstr (str.c_str (), "Ctrl")) {
                        flags |= FLAG_CTRL;
                    }
                    if (strstr (str.c_str (), "Alt")) {
                        flags |= FLAG_ALT;
                    }
                    if (strstr (str.c_str (), "Shift")) {
                        flags |= FLAG_SHIFT;
                    }
                    return flags;
                }

                void ParseItem (
                        const XERCES_CPP_NAMESPACE::DOMNode &node,
                        Module &module,
                        std::vector<Accelerators::Item *> &items) {
                    const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes = node.getAttributes ();
                    util::ui32 ch = ParseCh (util::GetAttributeValue ("ch", attributes));
                    util::ui32 flags = ParseFlags (util::GetAttributeValue ("flags", attributes));
                    std::string eventHandlerName = module.GetType () +
                        util::GetAttributeValue ("eventHandler", attributes);
                    Accelerators::Item::EventHandler *eventHandler =
                        Accelerators::Item::EventHandler::Get (eventHandlerName, module);
                    if (eventHandler == 0) {
                        assert (UI::Instance ()->consoleWindow != 0);
                        UI::Instance ()->consoleWindow->Print (
                            "Accelerators: Unable to locate handler for '%s'.\n",
                            eventHandlerName.c_str ());
                        struct DummyEventHandler : public Accelerators::Item::EventHandler {
                            virtual void OnSetFocus () {}
                        } static dummyEventHandler;
                        eventHandler = &dummyEventHandler;
                    }
                    items.push_back (new Accelerators::Item (ch, flags, eventHandler));
                }
            }

            Accelerators::Accelerators (const std::string &path, Module &module) {
                util::XercesPlatformInit xercesPlatformInit;
                util::XercesErrorHandler errorHandler;
                XERCES_CPP_NAMESPACE::XercesDOMParser parser;
                parser.setErrorHandler (&errorHandler);
                try {
                    parser.parse (path.c_str ());
                    const XERCES_CPP_NAMESPACE::DOMNodeList *children =
                        parser.getDocument ()->getDocumentElement ()->getChildNodes ();
                    for (std::size_t i = 0, count = children->getLength (); i < count; ++i) {
                        const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                        if (child.getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                                util::XMLChTostring (child.getNodeName ()) == "item") {
                            ParseItem (child, module, items);
                        }
                    }
                }
                catch (const std::string &message) {
                    assert (UI::Instance ()->consoleWindow != 0);
                    UI::Instance ()->consoleWindow->Print (
                        "Accelerators: Unable to load '%s'.\n\t%s\n",
                        path.c_str (), message.c_str ());
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
