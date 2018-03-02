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

#include "thekogans/mex/command/GroupFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace command {

            THEKOGANS_MEX_COMMAND_IMPLEMENT_FINAL_OPERATION (GroupFinalOperation)

            bool GroupFinalOperation::Execute () {
                for (std::size_t i = 0,
                        count = finalOperations.size (); i < count; ++i) {
                    finalOperations[i]->Execute ();
                }
                return true;
            }

            void GroupFinalOperation::AddFinalOperation (
                    FinalOperation::UniquePtr finalOperation) {
                assert (finalOperation.get () != 0);
                if (finalOperation.get () != 0) {
                    finalOperations.push_back (finalOperation.get ());
                    finalOperation.release ();
                }
            }

        } // namespace command
    } // namespace mex
} // namespace thekogans
