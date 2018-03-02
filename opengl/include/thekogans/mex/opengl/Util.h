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

#if !defined (__thekogans_mex_opengl_Util_h)
#define __thekogans_mex_opengl_Util_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API SetPixel (
                util::i32 x,
                util::i32 y);
            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Line (
                util::i32 x0,
                util::i32 y0,
                util::i32 x1,
                util::i32 y1);
            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API HLine (
                util::i32 x0,
                util::i32 x1,
                util::i32 y);
            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API VLine (
                util::i32 x,
                util::i32 y0,
                util::i32 y1);
            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Rectangle (
                util::i32 x,
                util::i32 y,
                util::i32 width,
                util::i32 height);

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Point22glPoint (
                const blas::Point2 &point,
                util::f32 glPoint[]);
            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Point32glPoint (
                const blas::Point3 &point,
                util::f32 glPoint[]);

            _LIB_THEKOGANS_MEX_OPENGL_DECL void _LIB_THEKOGANS_MEX_OPENGL_API Matrix32glMatrix (
                const blas::Matrix3 &xform,
                util::f32 glxform[]);

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Util_h)
