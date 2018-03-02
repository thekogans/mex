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

#if !defined (__thekogans_mex_core_MenuBar_h)
#define __thekogans_mex_core_MenuBar_h

#include <cassert>
#include <memory>
#include <string>
#include <map>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct Tool;
            struct Module;

            struct _LIB_THEKOGANS_MEX_CORE_DECL MenuBar {
                typedef std::unique_ptr<MenuBar> UniquePtr;

                struct _LIB_THEKOGANS_MEX_CORE_DECL Item {
                    THEKOGANS_UTIL_DECLARE_HEAP (Item)

                    struct _LIB_THEKOGANS_MEX_CORE_DECL EventHandler {
                        typedef EventHandler *(*Factory) (Module &);
                        typedef std::map<std::string, Factory> Map;

                        static EventHandler *Get (const std::string &name, Module &module);

                        struct _LIB_THEKOGANS_MEX_CORE_DECL MapInitializer {
                            MapInitializer (const std::string &eventHandler, Factory factory);
                        };

                        virtual ~EventHandler () {}

                        virtual void OnSetFocus () = 0;
                        virtual void OnKillFocus () {}

                        struct UI {
                            Item &item;

                            UI (Item &item_) : item (item_) {}
                            virtual ~UI () {}

                            virtual void SetEnabled (bool) = 0;
                            virtual void SetCheckable (bool) = 0;
                            virtual void SetChecked (bool) = 0;
                        };

                        virtual void OnUpdateUI (UI &) = 0;
                    };

                    struct _LIB_THEKOGANS_MEX_CORE_DECL ToolEventHandler : public EventHandler {
                    protected:
                        Module &module;
                        Tool &tool;

                    public:
                        ToolEventHandler (Module &module_, Tool &tool_) :
                            module (module_), tool (tool_) {}

                        virtual void OnSetFocus ();
                        virtual void OnUpdateUI (UI &ui);
                    };

                    struct _LIB_THEKOGANS_MEX_CORE_DECL CommandEventHandler : public EventHandler {
                        virtual void OnUpdateUI (UI &ui) {
                            ui.SetEnabled (true);
                            ui.SetCheckable (false);
                            ui.SetChecked (false);
                        }
                    };

                    struct _LIB_THEKOGANS_MEX_CORE_DECL ButtonEventHandler : public EventHandler {
                        virtual void OnUpdateUI (UI &ui) {
                            ui.SetEnabled (true);
                            ui.SetCheckable (true);
                            ui.SetChecked (IsChecked ());
                        }

                        virtual bool IsChecked () const = 0;
                    };

                    enum {
                        TYPE_POPUP,
                        TYPE_ITEM,
                        TYPE_SEPARATOR
                    };
                    util::ui32 type;
                    std::string text;
                    std::string statusTip;
                    std::string modules;
                    EventHandler *eventHandler;
                    Item *parent;
                    util::OwnerVector<Item> children;

                    Item () :
                        type (TYPE_POPUP),
                        eventHandler (0),
                        parent (0) {}
                    Item (util::ui32 type_,
                            const std::string &text_,
                            const std::string &statusTip_,
                            const std::string &modules_,
                            EventHandler *eventHandler_,
                            Item *parent_) :
                            type (type_),
                            text (text_),
                            statusTip (statusTip_),
                            modules (modules_),
                            eventHandler (eventHandler_),
                            parent (parent_) {
                        assert (parent != 0);
                        parent->children.push_back (this);
                    }
                };

                Item root;

                MenuBar () {}
                MenuBar (const std::string &path, Module &module);
            };

            #define THEKOGANS_MEX_CORE_DECLARE_MENUBAR_ITEM_EVENT_HANDLER(type)\
            public:\
                static thekogans::mex::core::MenuBar::Item::EventHandler::MapInitializer mapInitializer;\
                static thekogans::mex::core::MenuBar::Item::EventHandler *Create (\
                        thekogans::mex::core::Module &module) {\
                    static type eventHandler (module);\
                    return &eventHandler;\
                }

            #define THEKOGANS_MEX_CORE_IMPLEMENT_MENUBAR_ITEM_EVENT_HANDLER(type, module)\
                thekogans::mex::core::MenuBar::Item::EventHandler::MapInitializer type::mapInitializer (\
                    #module #type, type::Create);

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_MenuBar_h)
