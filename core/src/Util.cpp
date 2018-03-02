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

#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/Tool.h"

namespace thekogans {
    namespace mex {
        namespace core {

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawLine2 (
                    const _3ds::opengl::View &view,
                    const blas::Point2 &from,
                    const blas::Point2 &to,
                    const opengl::ui8Color &color) {
                Tool::Viewport2D viewport2D (view, color);
                _3ds::opengl::DrawLine2 (from, to);
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawBound2 (
                    const _3ds::opengl::View &view,
                    const blas::Bound2 &bound,
                    const opengl::ui8Color &color) {
                std::vector<blas::Point2> points;
                {
                    points.push_back (bound.min);
                    points.push_back (blas::Point2 (bound.min.x, bound.max.y));
                    points.push_back (bound.max);
                    points.push_back (blas::Point2 (bound.max.x, bound.min.y));
                }
                {
                    Tool::Viewport2D viewport2D (view, color);
                    _3ds::opengl::DrawPolygon (blas::Polygon2 (points, true));
                }
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawBound2 (
                    const _3ds::opengl::View &view,
                    const blas::Bound2 &bound,
                    const blas::Matrix2 &xform,
                    const opengl::ui8Color &color) {
                std::vector<blas::Point2> points;
                {
                    points.push_back (bound.min * xform);
                    points.push_back (blas::Point2 (bound.min.x, bound.max.y) * xform);
                    points.push_back (bound.max * xform);
                    points.push_back (blas::Point2 (bound.max.x, bound.min.y) * xform);
                }
                {
                    Tool::Viewport2D viewport2D (view, color);
                    _3ds::opengl::DrawPolygon (blas::Polygon2 (points, true));
                }
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawPolygon2 (
                    const _3ds::opengl::View &view,
                    const blas::Polygon2 &polygon,
                    const opengl::ui8Color &color) {
                Tool::Viewport2D viewport2D (view, color);
                _3ds::opengl::DrawPolygon (polygon);
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawArrow2 (
                   const _3ds::opengl::View &view,
                   const blas::Point2 &from,
                   const blas::Point2 &to,
                   const opengl::ui8Color &color) {
                blas::Point2 pt = to - from;
                if (!pt.IsZero ()) {
                    Tool::Viewport2D viewport2D (view, color);
                    _3ds::opengl::DrawLine2 (from, to);
                    util::f32 angle = blas::atan2f (pt.y, pt.x);
                    _3ds::opengl::DrawLine2 (
                        blas::Point2 (15.0f * view.oneOverScale, 0.0f) *
                        blas::Matrix2::RotateZ (angle - util::RAD (210.0f)) *
                        blas::Matrix2::Translate (blas::Point2 (to.x, to.y)), to);
                    _3ds::opengl::DrawLine2 (
                        blas::Point2 (15.0f * view.oneOverScale, 0.0f) *
                        blas::Matrix2::RotateZ (angle - util::RAD (150.0f)) *
                        blas::Matrix2::Translate (blas::Point2 (to.x, to.y)), to);
                }
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawCircle2 (
                    const _3ds::opengl::View &view,
                    const blas::Circle &circle,
                    util::f32 angle,
                    util::ui32 segs,
                    const opengl::ui8Color &color) {
                std::vector<blas::Point2> points;
                {
                    const util::f32 dangle = util::TWOPI / segs;
                    for (util::ui32 i = 0; i < segs; ++i) {
                        points.push_back (circle.center +
                            blas::Point2 (circle.radius * cosf (angle), circle.radius * sinf (angle)));
                        angle += dangle;
                    }
                }
                DrawPolygon2 (view, blas::Polygon2 (points, true), color);
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawStar2 (
                    const _3ds::opengl::View &view,
                    const blas::Point2 &center,
                    util::f32 radius,
                    util::f32 angle,
                    const opengl::ui8Color &color) {
                std::vector<blas::Point2> points;
                {
                    points.resize (10);
                    const util::f32 dangle = util::TWOPI / 5.0f;
                    for (util::ui32 i = 0; i < 5; ++i) {
                        points[i * 2] = center +
                            blas::Point2 (radius * cosf (angle), radius * sinf (angle));
                        angle += dangle;
                    }
                    blas::Line2 (points[0], points[4]).CrossLine (blas::Line2 (points[2], points[8]), points[1]);
                    blas::Line2 (points[0], points[4]).CrossLine (blas::Line2 (points[2], points[6]), points[3]);
                    blas::Line2 (points[4], points[8]).CrossLine (blas::Line2 (points[2], points[6]), points[5]);
                    blas::Line2 (points[4], points[8]).CrossLine (blas::Line2 (points[0], points[6]), points[7]);
                    blas::Line2 (points[0], points[6]).CrossLine (blas::Line2 (points[2], points[8]), points[9]);
                }
                DrawPolygon2 (view, blas::Polygon2 (points, true), color);
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawArc2 (
                    const _3ds::opengl::View &view,
                    const blas::Point2 &center,
                    util::f32 radius,
                    util::f32 startAngle,
                    util::f32 spanAngle,
                    bool clockWise,
                    bool drawRadius,
                    util::ui32 steps,
                    const opengl::ui8Color &color) {
                std::vector<blas::Point2> points;
                {
                    spanAngle /= steps - 1;
                    if (clockWise) {
                        spanAngle *= -1;
                    }
                    for (util::ui32 i = 0; i < steps; ++i) {
                        points.push_back (center +
                            blas::Point2 (radius * cosf (startAngle), radius * sinf (startAngle)));
                        startAngle += spanAngle;
                    }
                }
                DrawPolygon2 (view, blas::Polygon2 (points, false), color);
                if (drawRadius) {
                    DrawLine2 (view, center, points.back (), color);
                }
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawEllipse2 (
                    const _3ds::opengl::View &view,
                    const blas::Ellipse &ellipse,
                    util::ui32 steps,
                    const opengl::ui8Color &color) {
                DrawPolygon2 (view, ellipse.ToPolygon (4 * (steps + 1)), color);
            }

            _LIB_THEKOGANS_MEX_CORE_DECL void _LIB_THEKOGANS_MEX_CORE_API DrawBezierCubic2 (
                    const _3ds::opengl::View &view,
                    const blas::BezierCubic2 &bezierCubic,
                    util::ui32 steps,
                    const opengl::ui8Color &color) {
                DrawPolygon2 (view, bezierCubic.ToPolygon (steps, blas::BezierCubic2::High), color);
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
