// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext_command.
//
// libthekogans_mex_3ds_ext_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext_command. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/ext/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/ext/command/keyframer/KeyframerCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {
                namespace command {

                    bool KeyframerValidateAfterOpenCommand::Execute () {
                        ExecuteAndAddCommand (
                            Command::UniquePtr (
                                new extKeyframerComplexTypeSwapCommand (keyframer, newKeyframer)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (KeyframerValidateAfterOpenCommand)

                } // namespace command
            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
