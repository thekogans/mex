// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_blas.
//
// libthekogans_mex_blas is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_blas is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_blas. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/blas/BezierCubic.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InCircle (const Circle &circle) const {
                return start.InCircle (circle) && end.InCircle (circle);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossCircle (const Circle &circle) const {
                util::f32 Px = end.x - start.x;
                util::f32 Py = end.y - start.y;
                util::f32 Cx = circle.center.x - start.x;
                util::f32 Cy = circle.center.y - start.y;
                util::f32 m = util::IS_ZERO (Px) ? 1.0f : Py / Px;
                util::f32 a = m * m + 1.0f;
                util::f32 b = -2.0f * (Cx + Cy * m);
                util::f32 c = Cx * Cx + Cy * Cy - circle.radius * circle.radius;
                util::f32 d = b * b - 4.0f * a * c;
                // roots are imaginary, quick reject
                if (d < EPSILON) {
                    return false;
                }
            #define SAME_SIGN(a, b) (((a) * (b)) > 0.0f)
                // first root
                util::f32 x = (-b + sqrtf (d)) / (2.0f * a);
                util::f32 y = m * x;
                if (SAME_SIGN (x, Px) && SAME_SIGN (y, Py) &&
                        fabsf (x) <= fabsf (Px) && fabsf (y) <= fabsf (Py)) {
                    return true;
                }
                // second root
                x = (-b - sqrtf (d)) / (2.0f * a);
                y = m * x;
                if (SAME_SIGN (x, Px) && SAME_SIGN (y, Py) &&
                        fabsf (x) <= fabsf (Px) && fabsf (y) <= fabsf (Py)) {
                    return true;
                }
            #undef SAME_SIGN
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InEllipse (const Ellipse &ellipse) const {
                return start.InEllipse (ellipse) && end.InEllipse (ellipse);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossEllipse (const Ellipse &ellipse) const {
                return start.InEllipse (ellipse) || end.InEllipse (ellipse);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InBound (const Bound2 &bound) const {
                return start.InBound (bound) && end.InBound (bound);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InPolygon (const Polygon2 &polygon) const {
                return start.InPolygon (polygon) && end.InPolygon (polygon) && !CrossPolygon (polygon);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossPolygon (const Polygon2 &polygon) const {
                for (std::size_t i = 0, count = polygon.points.size () - 1; i < count; ++i) {
                    if (CrossLine (Line2 (polygon.points[i], polygon.points[i + 1]))) {
                        return true;
                    }
                }
                return polygon.closed && CrossLine (Line2 (polygon.points.back (), polygon.points[0]));
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossBound (const Bound2 &bound) const {
                util::f32 dx = end.x - start.x;
                util::f32 dy = end.y - start.y;
                if (util::IS_ZERO (dx) && util::IS_ZERO (dy)) {
                    return start.InBound (bound);
                }
                util::f32 tE = 0.0f;
                util::f32 tL = 1.0f;
                return
                    CLIPt (dx, bound.min.x - start.x, tE, tL) &&
                    CLIPt (-dx, start.x - bound.max.x, tE, tL) &&
                    CLIPt (dy, bound.min.y - start.y, tE, tL) &&
                    CLIPt (-dy, start.y - bound.max.y, tE, tL);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line3::CrossBound (const Bound3 &bound) const {
                util::f32 dx = end.x - start.x;
                util::f32 dy = end.y - start.y;
                util::f32 dz = end.z - start.z;
                if (util::IS_ZERO (dx) && util::IS_ZERO (dy) && util::IS_ZERO (dz)) {
                    return start.InBound (bound);
                }
                util::f32 tE = 0.0f;
                util::f32 tL = 1.0f;
                return
                    CLIPt (dx, bound.min.x - start.x, tE, tL) &&
                    CLIPt (-dx, start.x - bound.max.x, tE, tL) &&
                    CLIPt (dy, bound.min.y - start.y, tE, tL) &&
                    CLIPt (-dy, start.y - bound.max.y, tE, tL) &&
                    CLIPt (dz, bound.min.z - start.z, tE, tL) &&
                    CLIPt (-dz, start.z - bound.max.z, tE, tL);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossLine (const Line2 &line, Point2 &pt) const {
                util::f32 Ax = end.x - start.x;
                util::f32 Ay = end.y - start.y;
                util::f32 Bx = line.start.x - line.end.x;
                util::f32 By = line.start.y - line.end.y;
                util::f32 d = Ay * Bx - Ax * By;
                if (util::IS_ZERO (d)) {
                    return false;
                }
                util::f32 Cx = start.x - line.start.x;
                util::f32 Cy = start.y - line.start.y;
                util::f32 t = (By * Cx - Bx * Cy) / d;
                if (t < 0.0f || t > 1.0f) {
                    return false;
                }
                t = (Ax * Cy - Ay * Cx) / d;
                if (t < 0.0f || t > 1.0f) {
                    return false;
                }
                pt = util::LERP (t, line.start, line.end);
                return true;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossBezierCubic (
                    const BezierCubic2 &bezierCubic, util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 p1 = bezierCubic.p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 p2 = bezierCubic.GetPoint (t);
                    if (CrossLine (Line2 (p1, p2))) {
                        return true;
                    }
                    p1 = p2;
                }
                return false;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
