// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_core_Util_h)
#define __thekogans_mex_core_Util_h

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            enum {
                FLAG_CTRL = 0x00000001,
                FLAG_ALT = 0x00000002,
                FLAG_SHIFT = 0x00000004,
            };

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawLine2 (
                const _3ds::opengl::View &view,
                const blas::Point2 &from,
                const blas::Point2 &to,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawBound2 (
                const _3ds::opengl::View &view,
                const blas::Bound2 &bound,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawBound2 (
                const _3ds::opengl::View &view,
                const blas::Bound2 &bound,
                const blas::Matrix2 &xform,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawPolygon2 (
                const _3ds::opengl::View &view,
                const blas::Polygon2 &polygon,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawArrow2 (
                const _3ds::opengl::View &view,
                const blas::Point2 &from,
                const blas::Point2 &to,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawCircle2 (
                const _3ds::opengl::View &view,
                const blas::Circle &circle,
                util::f32 angle,
                util::ui32 segs,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawStar2 (
                const _3ds::opengl::View &view,
                const blas::Point2 &center,
                util::f32 radius,
                util::f32 angle,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawArc2 (
                const _3ds::opengl::View &view,
                const blas::Point2 &center,
                util::f32 radius,
                util::f32 startAngle,
                util::f32 spanAngle,
                bool clockWise,
                bool drawRadius,
                util::ui32 steps,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawEllipse2 (
                const _3ds::opengl::View &view,
                const blas::Ellipse &ellipse,
                util::ui32 steps,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawBezierCubic2 (
                const _3ds::opengl::View &view,
                const blas::BezierCubic2 &bezierCubic,
                util::ui32 steps,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_Util_h)
