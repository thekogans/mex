// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/opengl/Util.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API SetPixel (
                    util::i32 x,
                    util::i32 y) {
                glBegin (GL_POINTS);
                glVertex2i (x, y);
                glEnd ();
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Line (
                    util::i32 x0,
                    util::i32 y0,
                    util::i32 x1,
                    util::i32 y1) {
                glBegin (GL_LINES);
                glVertex2i (x0, y0);
                glVertex2i (x1, y1);
                glEnd ();
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API HLine (
                    util::i32 x0,
                    util::i32 x1,
                    util::i32 y) {
                glBegin (GL_LINES);
                glVertex2i (x0, y);
                glVertex2i (x1, y);
                glEnd ();
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API VLine (
                    util::i32 x,
                    util::i32 y0,
                    util::i32 y1) {
                glBegin (GL_LINES);
                glVertex2i (x, y0);
                glVertex2i (x, y1);
                glEnd ();
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Rectangle (
                    util::i32 x,
                    util::i32 y,
                    util::i32 width,
                    util::i32 height) {
                util::i32 points[] = {
                    x, y,
                    x + width - 1, y,
                    x + width - 1, y + height - 1,
                    x, y + height - 1
                };
                ClientState clientState (ClientState::VERTEX_ARRAY);
                glVertexPointer (2, GL_INT, 0, points);
                glDrawArrays (GL_LINE_LOOP, 0, 4);
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Point22glPoint (
                    const blas::Point2 &point,
                    util::f32 glPoint[]) {
                glPoint[0] = point.x;
                glPoint[1] = point.y;
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Point32glPoint (
                    const blas::Point3 &point,
                    util::f32 glPoint[]) {
                glPoint[0] = point.x;
                glPoint[1] = point.y;
                glPoint[2] = point.z;
            }

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Matrix32glMatrix (
                    const blas::Matrix3 &xform,
                    util::f32 glxform[]) {
                glxform[0] = xform.x.x;
                glxform[1] = xform.x.y;
                glxform[2] = xform.x.z;
                glxform[3] = 0.0f;
                glxform[4] = xform.y.x;
                glxform[5] = xform.y.y;
                glxform[6] = xform.y.z;
                glxform[7] = 0.0f;
                glxform[8] = xform.z.x;
                glxform[9] = xform.z.y;
                glxform[10] = xform.z.z;
                glxform[11] = 0.0f;
                glxform[12] = xform.t.x;
                glxform[13] = xform.t.y;
                glxform[14] = xform.t.z;
                glxform[15] = 1.0f;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
