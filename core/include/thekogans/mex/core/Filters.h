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

#if !defined (__thekogans_mex_core_Filters_h)
#define __thekogans_mex_core_Filters_h

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/io/Progress.h"
#include "thekogans/mex/core/Config.h"
#include "thekogans/mex/core/Model.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct Module;

            struct _LIB_THEKOGANS_MEX_CORE_DECL Filters {
                typedef std::unique_ptr<Filters> UniquePtr;

                struct _LIB_THEKOGANS_MEX_CORE_DECL Item {
                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    struct _LIB_THEKOGANS_MEX_CORE_DECL EventHandler {
                        typedef EventHandler *(*Factory) (Module &);
                        typedef std::map<std::string, Factory> Map;

                        static EventHandler *Get (const std::string &name, Module &module);

                        struct _LIB_THEKOGANS_MEX_CORE_DECL MapInitializer {
                            MapInitializer (const std::string &eventHandler, Factory factory);
                        };

                        virtual ~EventHandler () {}

                        virtual _3ds::io::Project::FileType OnRead (
                                const std::string &path,
                                _3ds::io::Project &project,
                                _3ds::io::Progress *progress = 0) {
                            return _3ds::io::Project::Unknown;
                        }
                        virtual _3ds::io::Project::FileType OnWrite (
                                const std::string &path,
                                _3ds::io::Project &project,
                                _3ds::io::Progress *progress = 0) {
                            return _3ds::io::Project::Unknown;
                        }
                    };

                    std::string filter;
                    std::string ext;
                    enum {
                        Read = 1,
                        Write = 2
                    };
                    util::Flags32 flags;
                    EventHandler *eventHandler;

                    Item (const std::string &filter_,
                            const std::string &ext_,
                            util::ui32 flags_,
                            EventHandler *eventHandler_) :
                            filter (filter_),
                            ext (ext_),
                            flags (flags_),
                            eventHandler (eventHandler_) {
                        assert (!filter.empty ());
                        assert (!ext.empty ());
                        assert (eventHandler != 0);
                    }

                    inline bool CanRead () const {
                        return flags.Test (Read);
                    }
                    inline bool CanWrite () const {
                        return flags.Test (Write);
                    }
                };

            protected:
                util::OwnerVector<Item> items;

            public:
                Filters (const std::string &path, Module &module);

                inline const std::vector<Item *> &GetItems () const {return items;}
            };

            #define THEKOGANS_MEX_CORE_DECLARE_FILTERS_ITEM_EVENT_HANDLER(type)\
            public:\
                static thekogans::mex::core::Filters::Item::EventHandler::MapInitializer mapInitializer;\
                static thekogans::mex::core::Filters::Item::EventHandler *Create (\
                        thekogans::mex::core::Module &module) {\
                    static type eventHandler (module);\
                    return &eventHandler;\
                }

            #define THEKOGANS_MEX_CORE_IMPLEMENT_FILTERS_ITEM_EVENT_HANDLER(type, module)\
                thekogans::mex::core::Filters::Item::EventHandler::MapInitializer type::mapInitializer (\
                    #module #type, type::Create);

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_Filters_h)
