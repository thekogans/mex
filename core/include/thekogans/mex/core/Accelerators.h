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

#if !defined (__thekogans_mex_core_Accelerators_h)
#define __thekogans_mex_core_Accelerators_h

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct Module;

            struct _LIB_THEKOGANS_MEX_CORE_DECL Accelerators {
                typedef std::unique_ptr<Accelerators> UniquePtr;

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

                        virtual void OnSetFocus () = 0;
                    };

                private:
                    util::ui32 ch;
                    util::ui32 flags;
                    EventHandler *eventHandler;

                public:
                    Item (util::ui32 ch_, util::ui32 flags_, EventHandler *eventHandler_) :
                            ch (ch_), flags (flags_), eventHandler (eventHandler_) {
                        assert (eventHandler != 0);
                    }

                    inline bool HandleEvent (util::ui32 ch_, util::ui32 flags_) {
                        if (ch == ch_ && (flags & flags_) == flags) {
                            assert (eventHandler != 0);
                            eventHandler->OnSetFocus ();
                            return true;
                        }
                        return false;
                    }
                };

            protected:
                util::OwnerVector<Item> items;

            public:
                Accelerators (const std::string &path, Module &module);

                inline const std::vector<Item *> &GetItems () const {return items;}
            };

            #define THEKOGANS_MEX_CORE_DECLARE_ACCELERATORS_ITEM_EVENT_HANDLER(type)\
            public:\
                static thekogans::mex::core::Accelerators::Item::EventHandler::MapInitializer mapInitializer;\
                static thekogans::mex::core::Accelerators::Item::EventHandler *Create (\
                    thekogans::mex::core::Module &module) {\
                    static type eventHandler (module);\
                    return &eventHandler;\
                }

            #define THEKOGANS_MEX_CORE_IMPLEMENT_ACCELERATORS_ITEM_EVENT_HANDLER(type, module)\
                thekogans::mex::core::Accelerators::Item::EventHandler::MapInitializer type::mapInitializer (\
                    #module #type, type::Create);

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_Accelerators_h)
