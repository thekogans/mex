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
#include "thekogans/mex/3ds/opengl/editor/Light.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Light)

                namespace {
                    void DrawCone (const blas::Point3 &position, const blas::Point3 &target,
                        util::f32 hotSpot, util::f32 fallOff, util::f32 roll,
                        const io::Light::Spot::Cone &cone, bool baseOnly) {
                        {
                            std::vector<blas::Point3> points;
                            if (cone.CalcPoints (position, target, hotSpot, roll, points)) {
                                glBegin (GL_LINE_LOOP);
                                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                                    glVertex3f (points[i].x, points[i].y, points[i].z);
                                }
                                glEnd ();
                            }
                        }
                        {
                            std::vector<blas::Point3> points;
                            if (cone.CalcPoints (position, target, fallOff, roll, points)) {
                                glBegin (GL_LINE_LOOP);
                                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                                    glVertex3f (points[i].x, points[i].y, points[i].z);
                                }
                                glEnd ();
                            }
                            if (!baseOnly) {
                                glBegin (GL_LINES);
                                for (util::ui32 i = 0; i < 4; ++i) {
                                    glVertex3f (position.x, position.y, position.z);
                                    util::ui32 index = cone.type == io::Light::Spot::Cone::Rectangular ? i : i * 4;
                                    glVertex3f (points[index].x, points[index].y, points[index].z);
                                }
                                glEnd ();
                            }
                        }
                    }
                }

                void Light::Draw (const View &view) const {
                    DrawWithColor (view, DefaultPalette[YELLOW]);
                }

                void Light::DrawWithColor (const View &view, const thekogans::mex::opengl::ui8Color &color) const {
                    Color _color (color);
                    if (light.spot.get () != 0) {
                        if (view.view.name.value == light.name) {
                            DrawCone (light.position, light.spot->target, light.spot->hotSpot,
                                light.spot->fallOff, light.spot->roll, light.spot->cone, true);
                        }
                        else {
                            DrawIcon (LIGHT_HEAD, light.position);
                            DrawIcon (BOX_5x5, light.spot->target);
                            DrawLine3 (light.position, light.spot->target);
                            if (light.spot->cone.show) {
                                DrawCone (light.position, light.spot->target, light.spot->hotSpot,
                                    light.spot->fallOff, light.spot->roll, light.spot->cone, false);
                            }
                        }
                    }
                    else {
                        DrawIcon (LIGHT_HEAD, light.position);
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
