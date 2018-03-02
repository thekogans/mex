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
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/io/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/lofter/LofterCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool LofterValidateAfterOpenCommand::Execute () {
                        // Path
                        {
                            std::vector<io::BezierPolygon3::Vertex> &vertices = lofter.path.polygon.vertices;
                            if (vertices.empty ()) {
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new stdVectorBezierPolygon3Vertexpush_backCommand (
                                            vertices,
                                            io::BezierPolygon3::Vertex (
                                                blas::Point3::Zero,
                                                blas::Point3::Zero,
                                                blas::Point3::Zero,
                                                io::BezierPolygon3::Vertex::First))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new stdVectorBezierPolygon3Vertexpush_backCommand (
                                            vertices,
                                            io::BezierPolygon3::Vertex (
                                                blas::Point3 (0.0f, 100.0f, 0.0f),
                                                blas::Point3::Zero,
                                                blas::Point3::Zero,
                                                io::BezierPolygon3::Vertex::Last))));
                            }
                        }
                        // Shapes
                        {
                            for (std::size_t i = 0, count = lofter.shapes.size (); i < count; ++i) {
                                std::vector<io::BezierPolygon3::Vertex> &vertices =
                                    lofter.shapes[i]->polygons.vertices;
                                // Shapes are 2D entities, but for some reason
                                // 3D Studio saves them out as 3D. I fond a bunch
                                // of *.lft files with z != 0.0f. That wrecked
                                // havock with ext::Lofter. Setting z to
                                // 0.0f here avoids a whole lot of grief down
                                // the road.
                                for (std::size_t j = 0; j < vertices.size (); ++j) {
                                    if (vertices[j].pt.z != 0.0f ||
                                            vertices[j].red.z != 0.0f ||
                                            vertices[j].yellow.z != 0.0f) {
                                        ExecuteAndAddCommand (
                                            Command::UniquePtr (
                                                new stdVectorBezierPolygon3VertexassignCommand (
                                                    vertices,
                                                    j,
                                                    io::BezierPolygon3::Vertex (
                                                        blas::Point3 (vertices[j].pt.x, vertices[j].pt.y, 0.0f),
                                                        blas::Point3 (vertices[j].red.x, vertices[j].red.y, 0.0f),
                                                        blas::Point3 (vertices[j].yellow.x, vertices[j].yellow.y, 0.0f),
                                                        vertices[j].flags))));
                                    }
                                }
                            }
                        }
                        // ViewLayout
                        // Guard against brain dead exporters that don't write ViewLayout.
                        // Reset to 3DStudio four-left view layout.
                        // Check layout sanity.
                        if (!lofter.viewLayout.layout.IsValid (io::ViewLayout::Layout::FourLeft) ||
                            lofter.viewLayout.views3.size () != 5) {
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewLayoutlayoutSetCommand (
                                        lofter.viewLayout.layout,
                                        io::ViewLayout::Layout (io::ViewLayout::Layout::FourLeft))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewclearCommand (
                                        lofter.viewLayout.views3)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        lofter.viewLayout.views3,
                                        io::TopView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        lofter.viewLayout.views3,
                                        io::FrontView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        lofter.viewLayout.views3,
                                        io::UserView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        lofter.viewLayout.views3,
                                        io::ShapeView (blas::Size::Empty))));
                            // Place holder used in maximize/minimize logic.
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new stdVectorViewpush_backCommand (
                                        lofter.viewLayout.views3,
                                        io::View ())));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewLayoutSetSizeCommand (
                                        lofter.viewLayout,
                                        blas::Size (0, 0, 1024, 768))));
                            ext::Lofter extLofter (lofter);
                            const ext::Lofter::Shape *shape = extLofter.GetCurrShape ();
                            if (shape != 0) {
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewZoomBoundCommand (
                                            lofter.viewLayout[0],
                                            extLofter.GetViewBound (ext::View (lofter.viewLayout[0])))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewZoomBoundCommand (
                                            lofter.viewLayout[1],
                                            extLofter.GetViewBound (ext::View (lofter.viewLayout[1])))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewZoomBoundCommand (
                                            lofter.viewLayout[2],
                                            extLofter.GetViewBound (ext::View (lofter.viewLayout[2])))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewZoomBoundCommand (
                                            lofter.viewLayout[3],
                                            shape->GetBound2 (ext::View (lofter.viewLayout[3]).xform))));
                            }
                        }
                        else {
                            bool maximized = lofter.viewLayout.layout.maximized;
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            lofter.viewLayout)));
                            }
                            for (std::size_t i = 0; i < lofter.viewLayout.GetViewCount (); ++i) {
                                if (!lofter.viewLayout[i].IsValid ()) {
                                    ExecuteAndAddCommand (
                                        Command::UniquePtr (
                                            new stdVectorViewassignCommand (
                                                lofter.viewLayout.views3,
                                                i,
                                                io::View ())));
                                }
                            }
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            lofter.viewLayout)));
                            }
                        }
                        // From here on out we will write only newer views3. 
                        ExecuteAndAddCommand (
                            Command::UniquePtr (
                                new stdVectorViewclearCommand (
                                    lofter.viewLayout.views)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (LofterValidateAfterOpenCommand)

                    bool LofterValidateBeforeSaveCommand::Execute () {
                        // FIXME: implement
                        assert (0);
                        return false;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (LofterValidateBeforeSaveCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
