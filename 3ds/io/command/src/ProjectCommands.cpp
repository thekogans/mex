// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/io/command/shaper/ShaperCommands.h"
#include "thekogans/mex/3ds/io/command/lofter/LofterCommands.h"
#include "thekogans/mex/3ds/io/command/editor/EditorCommands.h"
#include "thekogans/mex/3ds/io/command/keyframer/KeyframerCommands.h"
//#include "thekogans/mex/3ds/io/command/materialseditor/MaterialsEditorCommands.h"
//#include "thekogans/mex/3ds/io/command/videopost/VideoPostCommands.h"
#include "thekogans/mex/3ds/io/command/ProjectCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool ProjectValidateAfterOpenCommand::Execute () {
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new ShaperValidateAfterOpenCommand (project.shaper)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new LofterValidateAfterOpenCommand (project.lofter)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new EditorValidateAfterOpenCommand (project.editor)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new KeyframerValidateAfterOpenCommand (project.keyframer)));
                        //ExecuteAndAddCommand (
                        //    Command::SharedPtr (
                        //        new MaterialsEditorValidateAfterOpenCommand (project.materialsEditor)));
                        //ExecuteAndAddCommand (
                        //    Command::SharedPtr (
                        //        new VideoPostValidateAfterOpenCommand (project.videoPost)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ProjectValidateAfterOpenCommand)

                    bool ProjectValidateBeforeSaveCommand::Execute () {
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new ShaperValidateBeforeSaveCommand (project.shaper)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new LofterValidateBeforeSaveCommand (project.lofter)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new EditorValidateBeforeSaveCommand (project.editor)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new KeyframerValidateBeforeSaveCommand (project.keyframer)));
                        //ExecuteAndAddCommand (
                        //    Command::SharedPtr (
                        //        new MaterialsEditorValidateBeforeSaveCommand (project.materialsEditor)));
                        //ExecuteAndAddCommand (
                        //    Command::SharedPtr (
                        //        new VideoPostValidateBeforeSaveCommand (project.videoPost)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ProjectValidateBeforeSaveCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
