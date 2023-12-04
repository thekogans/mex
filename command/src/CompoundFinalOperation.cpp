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
#include "thekogans/mex/command/CompoundFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace command {

            bool CompoundFinalOperation::ExecuteAndAddFinalOperation (
                    FinalOperation::SharedPtr finalOperation) {
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
