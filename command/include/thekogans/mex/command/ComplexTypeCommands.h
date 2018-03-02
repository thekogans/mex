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

#if !defined (__thekogans_mex_command_ComplexTypeCommands_h)
#define __thekogans_mex_command_ComplexTypeCommands_h

#include "thekogans/util/Heap.h"
#include "thekogans/mex/command/Command.h"

namespace thekogans {
    namespace mex {
        namespace command {

            template<typename T>
            struct ComplexTypeSwapCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ComplexTypeSwapCommand<T>)

            private:
                T &t1;
                T &t2;

            public:
                ComplexTypeSwapCommand (T &t1_, T &t2_) :
                    t1 (t1_),
                    t2 (t2_) {}

                virtual bool Execute () {
                    return Do ();
                }
                virtual bool Undo () {
                    return Do ();
                }
                virtual bool Redo () {
                    return Do ();
                }

            private:
                inline bool Do () {
                    t1.Swap (t2);
                    return true;
                }
            };

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_command_ComplexTypeCommands_h)
