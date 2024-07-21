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

#if !defined (__thekogans_mex_command_Transaction_h)
#define __thekogans_mex_command_Transaction_h

#include <memory>
#include <string>
#include "thekogans/util/Heap.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace command {

            struct _LIB_THEKOGANS_MEX_COMMAND_DECL Transaction : public CompoundCommand {
                /// \brief
                /// Declare \see{util::RefCounted} pointers.
                THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Transaction)

                THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                std::string name;
                bool undoable;
                bool committing;

                Transaction (
                    const std::string &name_,
                    bool undoable_ = true,
                    bool committing_ = false) :
                    name (name_),
                    undoable (undoable_),
                    committing (committing_) {}

                virtual const char *GetName () const {
                    return name.c_str ();
                }

                virtual bool Execute ();

                virtual void Optimize () {}

                virtual bool IsUndoable () const {
                    return undoable;
                }
                virtual bool IsCommitting () const {
                    return committing;
                }

                void AddCommand (Command::SharedPtr command);
                void AddFinalOperation (FinalOperation::SharedPtr finalOperation);
            };

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_command_Transaction_h)
