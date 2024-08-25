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

#include "thekogans/mex/command/Transaction.h"

namespace thekogans {
    namespace mex {
        namespace command {

            THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS_EX (
                Transaction,
                util::SpinLock,
                512,
                util::DefaultAllocator::Instance ())

            bool Transaction::Execute () {
                {
                    for (std::size_t i = 0,
                            count = commands.size (); i < count; ++i) {
                        commands[i]->Execute ();
                    }
                }
                {
                    for (std::size_t i = 0,
                            count = finalOperations.size (); i < count; ++i) {
                        finalOperations[i]->Execute ();
                    }
                }
                return true;
            }

            void Transaction::AddCommand (Command::SharedPtr command) {
                assert (command.Get () != 0);
                if (command.Get () != 0) {
                    commands.push_back (command);
                }
            }

            void Transaction::AddFinalOperation (FinalOperation::SharedPtr finalOperation) {
                assert (finalOperation.Get () != 0);
                if (finalOperation.Get () != 0) {
                    finalOperations.push_back (finalOperation);
                }
            }

        } // namespace command
    } // namespace mex
} // namespace thekogans
