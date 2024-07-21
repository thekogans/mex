// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#include <cassert>
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/editor/Camera.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Camera)

                namespace {
                    void DrawCone (const blas::Point3 &position, const blas::Point3 &target,
                        util::f32 roll, util::f32 fov, bool baseOnly, const thekogans::mex::opengl::ui8Color &color) {
                        std::vector<blas::Point3> points;
                        if (io::Light::Spot::Cone (io::Light::Spot::Cone::Rectangular, 1.0f, true, true).CalcPoints (
                            position, target, fov, roll, points)) {
                            Color _color (color);
                            {
                                glBegin (GL_LINE_LOOP);
                                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                                    glVertex3f (points[i].x, points[i].y, points[i].z);
                                }
                                glEnd ();
                            }
                            if (!baseOnly) {
                                glBegin (GL_LINES);
                                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                                    glVertex3f (position.x, position.y, position.z);
                                    glVertex3f (points[i].x, points[i].y, points[i].z);
                                }
                                glEnd ();
                            }
                        }
                    }
                }

                void Camera::Draw (const View &view) const {
                    if (view.view.name.value == camera.name) {
                        DrawCone (camera.position, camera.target, camera.roll,
                            io::Camera::Lens2FOV (camera.lens), true, DefaultPalette[CYAN]);
                    }
                    else {
                        Color color (DefaultPalette[BLUE]);
                        DrawIcon (CAMERA_HEAD, camera.position);
                        DrawIcon (BOX_5x5, camera.target);
                        DrawLine3 (camera.position, camera.target);
                        if (camera.showCone) {
                            DrawCone (camera.position, camera.target, camera.roll,
                                io::Camera::Lens2FOV (camera.lens), false, DefaultPalette[CYAN]);
                        }
                    }
                }

                void Camera::DrawWithColor (const View &view, const thekogans::mex::opengl::ui8Color &color) const {
                    if (view.view.name.value == camera.name) {
                        DrawCone (camera.position, camera.target, camera.roll,
                            io::Camera::Lens2FOV (camera.lens), true, color);
                    }
                    else {
                        Color _color (color);
                        DrawIcon (CAMERA_HEAD, camera.position);
                        DrawIcon (BOX_5x5, camera.target);
                        DrawLine3 (camera.position, camera.target);
                        if (camera.showCone) {
                            DrawCone (camera.position, camera.target, camera.roll,
                                io::Camera::Lens2FOV (camera.lens), false, color);
                        }
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
