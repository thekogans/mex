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

#include <cassert>
#include "thekogans/mex/command/CompoundCommand.h"

namespace thekogans {
    namespace mex {
        namespace command {

            // NOTE: Without a full blown transaction log, it is
            // extremely difficult to guarntee transactional
            // semantics. While, in theory, Undo/Redo don't
            // provide these guarantees, in practice, most
            // compound commands are implemented using mostly
            // SimpleTypeCommand's. These don't throw, ever.

            bool CompoundCommand::Undo () {
                {
                    for (std::size_t i = commands.size (); i--;) {
                        if (!commands[i]->Undo ()) {
                            for (; ++i < commands.size ();) {
                                commands[i]->Redo ();
                            }
                            return false;
                        }
                    }
                }
                {
                    for (std::size_t i = 0, count = finalOperations.size (); i < count; ++i) {
                        finalOperations[i]->Execute ();
                    }
                }
                return true;
            }

            bool CompoundCommand::Redo () {
                {
                    for (std::size_t i = 0; i < commands.size (); ++i) {
                        if (!commands[i]->Redo ()) {
                            for (; i--;) {
                                commands[i]->Undo ();
                            }
                            return false;
                        }
                    }
                }
                {
                    for (std::size_t i = 0, count = finalOperations.size (); i < count; ++i) {
                        finalOperations[i]->Execute ();
                    }
                }
                return true;
            }

            bool CompoundCommand::ExecuteAndAddCommand (Command::Ptr command) {
                assert (command.Get () != 0);
                if (command.Get () != 0) {
                    if (command->Execute ()) {
                        commands.push_back (command);
                        return true;
                    }
                    command->Undo ();
                }
                return false;
            }

            bool CompoundCommand::ExecuteAndAddFinalOperation (FinalOperation::Ptr finalOperation) {
                assert (finalOperation.Get () != 0);
                if (finalOperation.Get () != 0) {
                    if (finalOperation->Execute ()) {
                        finalOperations.push_back (finalOperation);
                        return true;
                    }
                }
                return false;
            }

        } // namespace command
    } // namespace mex
} // namespace thekogans
