// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_command.
//
// libthekogans_mex_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_command_Command_h)
#define __thekogans_mex_command_Command_h

#include <memory>
#include <typeinfo>
#include "thekogans/util/RefCounted.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace command {

            struct _LIB_THEKOGANS_MEX_COMMAND_DECL Command : public util::RefCounted {
                /// \brief
                /// Declare \see{util::RefCounted} pointers.
                THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Command)

                Command ();
                virtual ~Command ();

                virtual const char *GetName () const = 0;

                virtual bool Execute () = 0;
                virtual bool Undo () = 0;
                virtual bool Redo () = 0;
            };

            #define THEKOGANS_MEX_COMMAND_DECLARE_COMMAND(type)\
                THEKOGANS_UTIL_DECLARE_HEAP (type)\
            public:\
                virtual const char *GetName () const {\
                    return typeid (*this).name ();\
                }

            #define THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND(type)\
                THEKOGANS_UTIL_IMPLEMENT_HEAP (type)
            #define THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX(type, minItemsInPage)\
                THEKOGANS_UTIL_IMPLEMENT_HEAP_EX (type, minItemsInPage)
            #define THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T(type)\
                THEKOGANS_UTIL_IMPLEMENT_HEAP_T (type)
            #define THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T(type, minItemsInPage)\
                THEKOGANS_UTIL_IMPLEMENT_HEAP_EX_T (type, minItemsInPage)

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_command_Command_h)
