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
#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/io/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool ShaperValidateAfterOpenCommand::Execute () {
                        // BezierPolygon2
                        {
                            // Shapes are 2D entities, but for some reason
                            // 3D Studio saves them out as 3D. I found a bunch
                            // of *.shp files with z != 0.0f. That wrecked
                            // havock with OpenGL view frustum (near/far planes
                            // clipping out parts of polygons). Setting z to
                            // 0.0f here avoids a whole lot of grief down
                            // the road.
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new OwnerVectorBezierPolygon2clearCommand (
                                        shaper.polygons2)));
                            for (std::size_t i = 0,
                                    count = shaper.polygons3.size (); i < count; ++i) {
                                ExecuteAndAddCommand (
                                    Command::SharedPtr (
                                        new OwnerVectorBezierPolygon2push_backCommand (
                                            shaper.polygons2,
                                            io::BezierPolygon3To2 (*shaper.polygons3[i]))));
                            }
                        }
                        // ViewLayout
                        // Guard against brain dead exporters that don't write ViewLayout.
                        // Reset to 3DStudio single view layout.
                        // Check layout sanity.
                        if (!shaper.viewLayout.layout.IsValid () ||
                                shaper.viewLayout.views3.size () != 5) {
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewLayoutlayoutSetCommand (
                                        shaper.viewLayout.layout,
                                        io::ViewLayout::Layout (
                                            io::ViewLayout::Layout::Single))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewclearCommand (
                                        shaper.viewLayout.views3)));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        shaper.viewLayout.views3,
                                        io::ShapeView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        shaper.viewLayout.views3, io::View ())));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        shaper.viewLayout.views3, io::View ())));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        shaper.viewLayout.views3, io::View ())));
                            // Place holder used in maximize/minimize logic.
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        shaper.viewLayout.views3, io::View ())));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewLayoutSetSizeCommand (
                                        shaper.viewLayout,
                                        blas::Size (0, 0, 1024, 768))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewZoomBoundCommand (
                                        shaper.viewLayout[0],
                                        ext::Shaper (shaper).GetViewBound (
                                            ext::View (shaper.viewLayout[0])))));
                        }
                        else {
                            bool maximized = shaper.viewLayout.layout.maximized;
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::SharedPtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            shaper.viewLayout)));
                            }
                            for (std::size_t i = 0,
                                    count = shaper.viewLayout.GetViewCount (); i < count; ++i) {
                                if (!shaper.viewLayout[i].IsValid () ||
                                        !shaper.viewLayout[i].IsShape ()) {
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new stdVectorViewassignCommand (
                                                shaper.viewLayout.views3, i,
                                                io::ShapeView (blas::Size::Empty))));
                                }
                            }
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::SharedPtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            shaper.viewLayout)));
                            }
                        }
                        // From here on out we will write only newer views3.
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new stdVectorViewclearCommand (
                                    shaper.viewLayout.views)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ShaperValidateAfterOpenCommand)

                    bool ShaperValidateBeforeSaveCommand::Execute () {
                        // BezierPolygon3
                        {
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new OwnerVectorBezierPolygon3clearCommand (
                                        shaper.polygons3)));
                            for (std::size_t i = 0,
                                    count = shaper.polygons2.size (); i < count; ++i) {
                                ExecuteAndAddCommand (
                                    Command::SharedPtr (
                                        new OwnerVectorBezierPolygon3push_backCommand (
                                            shaper.polygons3,
                                            io::BezierPolygon2To3 (*shaper.polygons2[i]))));
                            }
                        }
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ShaperValidateBeforeSaveCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
