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

#if !defined (__thekogans_mex_core_ToolBar_h)
#define __thekogans_mex_core_ToolBar_h

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/opengl/Image.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct Tool;
            struct Module;

            struct _LIB_THEKOGANS_MEX_CORE_DECL ToolBar {
                typedef std::unique_ptr<ToolBar> UniquePtr;

                struct _LIB_THEKOGANS_MEX_CORE_DECL Item {
                    THEKOGANS_UTIL_DECLARE_HEAP (Item)

                    struct _LIB_THEKOGANS_MEX_CORE_DECL EventHandler {
                        typedef EventHandler *(*Factory) (Module &);
                        typedef std::map<std::string, Factory> Map;

                        static EventHandler *Get (const std::string &eventHandler, Module &module);

                        struct _LIB_THEKOGANS_MEX_CORE_DECL MapInitializer {
                            MapInitializer (const std::string &eventHandler, Factory factory);
                        };

                        virtual ~EventHandler () {}

                        virtual void OnClickLeft () = 0;
                        virtual void OnClickRight () {}

                        struct UI {
                            Item &item;

                            UI (Item &item_) : item (item_) {}
                            virtual ~UI () {}

                            virtual void SetEnabled (bool) = 0;
                            virtual void SetCheckable (bool) = 0;
                            virtual void SetChecked (bool) = 0;
                            virtual void SetImage () = 0;
                        };

                        virtual void OnUpdateUI (UI &) {}
                    };

                    struct _LIB_THEKOGANS_MEX_CORE_DECL ToolEventHandler : public EventHandler {
                    protected:
                        Module &module;
                        Tool &tool;

                    public:
                        ToolEventHandler (Module &module_, Tool &tool_) :
                            module (module_), tool (tool_) {}

                        virtual void OnClickLeft ();
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

                    std::string toolTip;
                    std::string statusTip;
                    _3ds::opengl::Image::UniquePtr image;
                    EventHandler *eventHandler;

                    Item (const std::string &toolTip_,
                            const std::string &statusTip_,
                            _3ds::opengl::Image::UniquePtr image_,
                            EventHandler *eventHandler_) :
                            toolTip (toolTip_),
                            statusTip (statusTip_),
                            image (std::move (image_)),
                            eventHandler (eventHandler_) {
                        assert (image.get () != 0);
                        assert (eventHandler != 0);
                    }

                private:
                    Item (const Item &);
                    Item &operator = (const Item &);
                };

                util::ui32 width;
                util::ui32 height;
                util::OwnerVector<Item> items;

                ToolBar (const std::string &path, Module &module);
            };

            #define THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER(type)\
            public:\
                static thekogans::mex::core::ToolBar::Item::EventHandler::MapInitializer mapInitializer;\
                static thekogans::mex::core::ToolBar::Item::EventHandler *Create (\
                        thekogans::mex::core::Module &module) {\
                    static type eventHandler (module);\
                    return &eventHandler;\
                }

            #define THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER(type, module)\
                thekogans::mex::core::ToolBar::Item::EventHandler::MapInitializer type::mapInitializer (\
                    #module #type, type::Create);

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_ToolBar_h)
