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

#include <algorithm>
#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/io/command/BlasSimpleTypeCommands.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool ViewZoomInCommand::Execute () {
                        return view.zoom * 2.0f < 1000000.0f &&
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new f32SetCommand (view.zoom, view.zoom * 2.0f)));
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewZoomInCommand)

                    bool ViewZoomOutCommand::Execute () {
                        return view.zoom * 0.5f > 0.0000001f &&
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new f32SetCommand (view.zoom, view.zoom * 0.5f)));
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewZoomOutCommand)

                    bool ViewZoomBoundCommand::Execute () {
                        if ((view.IsOrthographic () || view.IsShape ()) && !bound.IsEmpty ()) {
                            blas::Point2 extents = bound.Extents ();
                            util::f32 zoom = (view.IsShape () ? ext::View::ShapeWidth :
                                ext::View::OrthographicWidth) / std::max (extents.x, extents.y) * 0.9f;
                            if (zoom > 1000000.0f) {
                                zoom = 1000000.0f;
                            }
                            ExecuteAndAddCommand (Command::UniquePtr (new f32SetCommand (view.zoom, zoom)));
                            blas::Matrix3 xform (
                                blas::Matrix3::RotateX (-util::RAD (90.0f)) *
                                blas::Matrix3::RotateY (-util::RAD (ext::View::GetHorizontalAngle (view))) *
                                blas::Matrix3::RotateX (util::RAD (ext::View::GetVerticalAngle (view))));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new Point3SetCommand (
                                        view.worldOrigin,
                                        blas::Point3 (bound.Center (), (view.worldOrigin * xform).z) * xform.Invert ())));
                            return true;
                        }
                        return false;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewZoomBoundCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
