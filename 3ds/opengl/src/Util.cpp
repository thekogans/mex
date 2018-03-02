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

#include <algorithm>
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawLine2 (
                        const blas::Point2 &p1, const blas::Point2 &p2) {
                    glBegin (GL_LINES);
                    glVertex2f (p1.x, p1.y);
                    glVertex2f (p2.x, p2.y);
                    glEnd ();
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawLine3 (
                        const blas::Point3 &p1, const blas::Point3 &p2) {
                    glBegin (GL_LINES);
                    glVertex3f (p1.x, p1.y, p1.z);
                    glVertex3f (p2.x, p2.y, p2.z);
                    glEnd ();
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPolygon (
                        const blas::Polygon2 &polygon) {
                    if (!polygon.points.empty ()) {
                        ClientState clientState (ClientState::VERTEX_ARRAY);
                        glVertexPointer (2, GL_FLOAT, 0, &polygon.points[0]);
                        glDrawArrays (polygon.closed ? GL_LINE_LOOP : GL_LINE_STRIP, 0, polygon.points.size ());
                    }
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPolygon (
                        const blas::Polygon3 &polygon) {
                    if (!polygon.points.empty ()) {
                        ClientState clientState (ClientState::VERTEX_ARRAY);
                        glVertexPointer (3, GL_FLOAT, 0, &polygon.points[0]);
                        glDrawArrays (polygon.closed ? GL_LINE_LOOP : GL_LINE_STRIP, 0, polygon.points.size ());
                    }
                }

                namespace {
                    inline bool IsFacingViewer (const io::View &view,
                            const blas::Point3 &a, const blas::Point3 &b, const blas::Point3 &c) {
                        blas::Point3 t1 = b - a;
                        blas::Point3 t2 = c - a;
                        return view.IsPerspective () ?
                            blas::Dot (blas::Cross (t1, t2), a) > 0.0f :
                            t1.x * t2.y - t1.y * t2.x > 0.0f;
                    }

                    inline void DrawLine (const blas::Point3 &p1, const blas::Point3 &p2) {
                        glVertex3f (p1.x, p1.y, p1.z);
                        glVertex3f (p2.x, p2.y, p2.z);
                    }

                    void DrawBox (const ext::View &extView, const blas::Point3 world[8]) {
                        blas::Point3 view[8];
                        {
                            for (util::ui32 i = 0; i < 8; ++i) {
                                view[i] = world[i] * extView.xform;
                            }
                        }
                        bool edge[12];
                        {
                            for (util::ui32 i = 0; i < 12; ++i) {
                                edge[i] = false;
                            }
                        }
                        glBegin (GL_LINES);
                        {
                            // left
                            if (IsFacingViewer (extView.view, view[0], view[4], view[3])) {
                                DrawLine (world[0], world[3]);
                                edge[3] = true;
                                DrawLine (world[0], world[4]);
                                edge[8] = true;
                                DrawLine (world[3], world[7]);
                                edge[11] = true;
                                DrawLine (world[4], world[7]);
                                edge[7] = true;
                            }
                        }
                        {
                            // right
                            if (IsFacingViewer (extView.view, view[1], view[2], view[5])) {
                                DrawLine (world[1], world[2]);
                                edge[1] = true;
                                DrawLine (world[1], world[5]);
                                edge[9] = true;
                                DrawLine (world[2], world[6]);
                                edge[10] = true;
                                DrawLine (world[5], world[6]);
                                edge[5] = true;
                            }
                        }
                        {
                            // front
                            if (IsFacingViewer (extView.view, view[0], view[1], view[4])) {
                                DrawLine (world[0], world[1]);
                                edge[0] = true;
                                DrawLine (world[4], world[5]);
                                edge[4] = true;
                                if (edge[8] == false) {
                                    DrawLine (world[0], world[4]);
                                    edge[8] = true;
                                }
                                if (edge[9] == false) {
                                    DrawLine (world[1], world[5]);
                                    edge[9] = true;
                                }
                            }
                        }
                        {
                            // back
                            if (IsFacingViewer (extView.view, view[2], view[3], view[6])) {
                                DrawLine (world[2], world[3]);
                                edge[2] = true;
                                DrawLine (world[6], world[7]);
                                edge[6] = true;
                                if (edge[10] == false) {
                                    DrawLine (world[2], world[6]);
                                    edge[10] = true;
                                }
                                if (edge[11] == false) {
                                    DrawLine (world[3], world[7]);
                                    edge[11] = true;
                                }
                            }
                        }
                        {
                            // top
                            if (IsFacingViewer (extView.view, view[4], view[5], view[7])) {
                                if (edge[4] == false) {
                                    DrawLine (world[4], world[5]);
                                }
                                if (edge[5] == false) {
                                    DrawLine (world[5], world[6]);
                                }
                                if (edge[6] == false) {
                                    DrawLine (world[6], world[7]);
                                }
                                if (edge[7] == false) {
                                    DrawLine (world[7], world[4]);
                                }
                            }
                        }
                        {
                            // bottom
                            if (IsFacingViewer (extView.view, view[0], view[3], view[1])) {
                                if (edge[0] == false) {
                                    DrawLine (world[0], world[1]);
                                }
                                if (edge[1] == false) {
                                    DrawLine (world[1], world[2]);
                                }
                                if (edge[2] == false) {
                                    DrawLine (world[2], world[3]);
                                }
                                if (edge[3] == false) {
                                    DrawLine (world[3], world[0]);
                                }
                            }
                        }
                        glEnd ();
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawBox (
                        const ext::View &view, const blas::Bound3 &bound, const blas::Matrix3 &xform) {
                    blas::Point3 world[8];
                    world[0] = blas::Point3 (bound.min.x, bound.min.y, bound.min.z) * xform;
                    world[1] = blas::Point3 (bound.max.x, bound.min.y, bound.min.z) * xform;
                    world[2] = blas::Point3 (bound.max.x, bound.max.y, bound.min.z) * xform;
                    world[3] = blas::Point3 (bound.min.x, bound.max.y, bound.min.z) * xform;
                    world[4] = blas::Point3 (bound.min.x, bound.min.y, bound.max.z) * xform;
                    world[5] = blas::Point3 (bound.max.x, bound.min.y, bound.max.z) * xform;
                    world[6] = blas::Point3 (bound.max.x, bound.max.y, bound.max.z) * xform;
                    world[7] = blas::Point3 (bound.min.x, bound.max.y, bound.max.z) * xform;
                    DrawBox (view, world);
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPath (
                        const std::vector<blas::Point3> &path, const std::vector<util::ui32> &keys,
                        const io::Keyframer::Segment &segment) {
                    {
                        // Path
                        Color color (DefaultPalette[RED]);
                        ClientState clientState (ClientState::VERTEX_ARRAY);
                        glVertexPointer (3, GL_FLOAT, 0, &path[0]);
                        glDrawArrays (GL_LINE_STRIP, 0, path.size ());
                    }
                    {
                        // Frames
                        Color color (DefaultPalette[YELLOW]);
                        glDrawArrays (GL_POINTS, 0, path.size ());
                        // Current frame
                        if (segment.currFrame >= segment.startFrame &&
                            segment.currFrame <= segment.endFrame) {
                            DrawIcon (BOX_3x3, path[segment.currFrame - segment.startFrame]);
                        }
                    }
                    {
                        // Key frames
                        Color color (DefaultPalette[WHITE]);
                        for (std::size_t i = 0, count = keys.size (); i < count; ++i) {
                            if (keys[i] >= segment.startFrame && keys[i] <= segment.endFrame) {
                                DrawIcon (BOX_5x5, path[keys[i] - segment.startFrame]);
                            }
                        }
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPathWithColor (
                        const std::vector<blas::Point3> &path, const std::vector<util::ui32> &keys,
                        const io::Keyframer::Segment &segment, const thekogans::mex::opengl::ui8Color &color) {
                    Color _color (color);
                    {
                        // Path
                        ClientState clientState (ClientState::VERTEX_ARRAY);
                        glVertexPointer (3, GL_FLOAT, 0, &path[0]);
                        glDrawArrays (GL_LINE_STRIP, 0, path.size ());
                    }
                    {
                        // Frames
                        glDrawArrays (GL_POINTS, 0, path.size ());
                        // Current frame
                        if (segment.currFrame >= segment.startFrame &&
                            segment.currFrame <= segment.endFrame) {
                            DrawIcon (BOX_3x3, path[segment.currFrame - segment.startFrame]);
                        }
                    }
                    {
                        // Key frames
                        for (std::size_t i = 0, count = keys.size (); i < count; ++i) {
                            if (keys[i] >= segment.startFrame && keys[i] <= segment.endFrame) {
                                DrawIcon (BOX_5x5, path[keys[i] - segment.startFrame]);
                            }
                        }
                    }
                }

                namespace {
                    bool ColorPart2glColor (const io::Color::Parts &part, util::f32 glColor[]) {
                        switch (part.type) {
                            case io::Color::Parts::NO_COLOR:
                                return false;
                            case io::Color::Parts::UI8_COLOR:
                                glColor[0] = static_cast<util::f32> (part.byteColor.r) / 255.0f;
                                glColor[1] = static_cast<util::f32> (part.byteColor.g) / 255.0f;
                                glColor[2] = static_cast<util::f32> (part.byteColor.b) / 255.0f;
                                glColor[3] = 1.0f;
                                return true;
                            case io::Color::Parts::F32_COLOR:
                                // FIXME: verify 0.0 <= part.floatColor <= 1.0
                                glColor[0] = part.floatColor.r;
                                glColor[1] = part.floatColor.g;
                                glColor[2] = part.floatColor.b;
                                glColor[3] = 1.0f;
                                return true;
                        }
                        // FIXME: throw something appropriate
                        assert (0);
                        return false;
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API Color2glColor (
                        const io::Color &color, util::f32 glColor[], const util::f32 defaultglColor[]) {
                    // Try extracting gamma corrected first, followed by raw.
                    if (!ColorPart2glColor (color.gammaCorrected, glColor) && !ColorPart2glColor (color.raw, glColor)) {
                        // Use default as last resort.
                        glColor[0] = defaultglColor[0];
                        glColor[1] = defaultglColor[1];
                        glColor[2] = defaultglColor[2];
                        glColor[3] = defaultglColor[3];
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API f32Color2glColor (
                        const thekogans::mex::opengl::f32Color &color, util::f32 glColor[]) {
                    glColor[0] = color.r;
                    glColor[1] = color.g;
                    glColor[2] = color.b;
                    glColor[3] = 1.0f;
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API SetupLights (
                        const io::Color &ambientLight, const util::OwnerVector<io::Light> &lights) {
                    glEnable (GL_LIGHTING);
                    util::f32 ambientColor[4];
                    const util::f32 defaultAmbientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
                    Color2glColor (ambientLight, ambientColor, defaultAmbientColor);
                    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambientColor);
                    // FIXME: Perhaps add support for GL_LIGHT_MODEL_TWO_SIDE and GL_LIGHT_MODEL_LOCAL_VIEWER
                    const util::f32 zeroColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
                    for (std::size_t i = 0, count = std::min ((int)lights.size (), GL_MAX_LIGHTS); i < count; ++i) {
                        glEnable (GL_LIGHT0 + i);
                        glLightfv (GL_LIGHT0 + i, GL_AMBIENT, zeroColor);
                        util::f32 diffuseColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
                        f32Color2glColor (lights[i]->color, diffuseColor);
                        glLightfv (GL_LIGHT0 + i, GL_DIFFUSE, diffuseColor);
                        glLightfv (GL_LIGHT0 + i, GL_SPECULAR, diffuseColor);
                        util::f32 position[] = {0.0f, 0.0f, 0.0f, 1.0f};
                        Point32glPoint (lights[i]->position, position);
                        glLightfv (GL_LIGHT0 + i, GL_POSITION, position);
                        if (lights[i]->spot.get ()) {
                            util::f32 direction[] = {0.0f, 0.0f, 0.0f, 1.0f};
                            Point32glPoint (lights[i]->spot->target - lights[i]->position, direction);
                            glLightfv (GL_LIGHT0 + i, GL_SPOT_DIRECTION, direction);
                            // glLightfv (GL_LIGHT0 + i, GL_SPOT_EXPONENT, );
                            // FIXME: lights[i]->spot->fallOff?
                            glLightf (GL_LIGHT0 + i, GL_SPOT_CUTOFF, lights[i]->spot->hotSpot);
                        }
                        // glLightfv (GL_LIGHT0 + i, GL_CONSTANT_ATTENUATION, );
                        // glLightfv (GL_LIGHT0 + i, GL_LINEAR_ATTENUATION, );
                        // glLightfv (GL_LIGHT0 + i, GL_QUADRATIC_ATTENUATION, );
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
