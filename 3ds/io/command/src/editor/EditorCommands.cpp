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

#include <cassert>
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/io/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/editor/EditorCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool EditorValidateAfterOpenCommand::Execute () {
                        // ViewLayout
                        // Guard against brain dead exporters that don't write ViewLayout.
                        // Reset to 3DStudio four view layout.
                        // Check layout sanity.
                        if (!editor.viewLayout.layout.IsValid () ||
                                editor.viewLayout.views3.size () != 5) {
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewLayoutlayoutSetCommand (
                                        editor.viewLayout.layout,
                                        io::ViewLayout::Layout (
                                            io::ViewLayout::Layout::Four))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewclearCommand (
                                        editor.viewLayout.views3)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        editor.viewLayout.views3,
                                        io::TopView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        editor.viewLayout.views3,
                                        io::FrontView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        editor.viewLayout.views3,
                                        io::LeftView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        editor.viewLayout.views3,
                                        io::UserView (blas::Size::Empty))));
                            // Place holder used in maximize/minimize logic.
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        editor.viewLayout.views3, io::View ())));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewLayoutSetSizeCommand (
                                        editor.viewLayout,
                                        blas::Size (0, 0, 1024, 768))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomBoundCommand (
                                        editor.viewLayout[0],
                                        ext::Editor (editor).GetViewBound (
                                            ext::View (editor.viewLayout[0])))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomBoundCommand (editor.
                                        viewLayout[1],
                                        ext::Editor (editor).GetViewBound (
                                            ext::View (editor.viewLayout[1])))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomBoundCommand (
                                        editor.viewLayout[2],
                                        ext::Editor (editor).GetViewBound (
                                            ext::View (editor.viewLayout[2])))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomBoundCommand (
                                        editor.viewLayout[3],
                                        ext::Editor (editor).GetViewBound (
                                            ext::View (editor.viewLayout[3])))));
                        }
                        else {
                            bool maximized = editor.viewLayout.layout.maximized;
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            editor.viewLayout)));
                            }
                            // Make sure Camera/SpotLight views have associated Camera/SpotLights.
                            for (std::size_t i = 0; i < editor.viewLayout.GetViewCount (); ++i) {
                                if (!editor.viewLayout[i].IsValid ()) {
                                    ExecuteAndAddCommand (
                                        Command::UniquePtr (
                                            new stdVectorViewassignCommand (
                                                editor.viewLayout.views3, i, io::View ())));
                                }
                                if (editor.viewLayout[i].IsCamera ()) {
                                    if (editor.GetCamera (editor.viewLayout[i].name.value) == 0) {
                                        ExecuteAndAddCommand (
                                            Command::UniquePtr (
                                                new stdVectorViewassignCommand (
                                                    editor.viewLayout.views3, i, io::View ())));
                                    }
                                }
                                else if (editor.viewLayout[i].IsSpotLight ()) {
                                    const io::Light *light =
                                        editor.GetLight (editor.viewLayout[i].name.value);
                                    if (light == 0 || light->spot.get () == 0) {
                                        ExecuteAndAddCommand (
                                            Command::UniquePtr (
                                                new stdVectorViewassignCommand (
                                                    editor.viewLayout.views3, i, io::View ())));
                                    }
                                }
                            }
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            editor.viewLayout)));
                            }
                        }
                        // From here on out we will write only newer views3. 
                        ExecuteAndAddCommand (
                            Command::UniquePtr (
                                new stdVectorViewclearCommand (
                                    editor.viewLayout.views)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (EditorValidateAfterOpenCommand)

                    bool EditorValidateBeforeSaveCommand::Execute () {
                        // FIXME: implement
                        assert (0);
                        return false;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (EditorValidateBeforeSaveCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
