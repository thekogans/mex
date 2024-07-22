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

#include <cmath>
#include <cassert>
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Point2::Point2 (const Point3 &pt) : x (pt.x), y (pt.y) {}

            util::f32 Point2::AngleWithXAxis (util::f32 eps) const {
                util::f32 length = Length ();
                if (util::IS_ZERO (length, eps)) {
                    return 0.0f;
                }
                length = 1.0f / length;
                util::f32 angle = atan2f (y * length, x * length);
                while (angle < 0.0f) {
                    angle += TWOPI;
                }
                return angle;
            }

            Point2 &Point2::operator *= (const Matrix2 &matrix) {
                util::f32 x_ = x * matrix.x.x + y * matrix.y.x + matrix.t.x;
                util::f32 y_ = x * matrix.x.y + y * matrix.y.y + matrix.t.y;
                x = x_;
                y = y_;
                return *this;
            }

            util::f32 Point2::Normalize (util::f32 eps) {
                util::f32 length = Length ();
                if (!util::IS_ZERO (length, eps)) {
                    util::f32 oneOverLength = 1.0f / length;
                    x *= oneOverLength;
                    y *= oneOverLength;
                }
                return length;
            }

            bool Point2::InCircle (const Circle &circle) const {
                util::f32 x_ = x - circle.center.x;
                util::f32 y_ = y - circle.center.y;
                return x_ * x_ + y_ * y_ <= circle.radius * circle.radius;
            }

            bool Point2::InEllipse (const Ellipse &ellipse) const {
                Point2 center = ellipse.bound.Center ();
                Point2 axis = ellipse.bound.max - center;
                Point2 axisSquared (axis.x * axis.x, axis.y * axis.y);
                util::f32 x_ = x - center.x;
                util::f32 y_ = y - center.y;
                // The standard equation for ellipse is x2 / a2 + y2 / b2 = 1
                // (where a and b are major and minor axis respectively).
                // To remove the devision, I multiplied both sides by a2b2.
                return x_ * x_ * axisSquared.y + y_ * y_ * axisSquared.x <= axisSquared.x * axisSquared.y;
            }

            bool Point2::InBound (const Bound2 &bound) const {
                return !(x < bound.min.x || x > bound.max.x ||
                    y < bound.min.y || y > bound.max.y);
            }

            bool Point2::InBound (const Bound3 &bound) const {
                return !(x < bound.min.x || x > bound.max.x ||
                    y < bound.min.y || y > bound.max.y ||
                    0.0f < bound.min.z || 0.0f > bound.max.z);
            }

            bool Point2::InPolygon (const Polygon2 &polygon) const {
                if (!polygon.closed) {
                    return false;
                }
                util::i32 nc = 0;
                util::f32 va = polygon.points[0].y - y;
                util::i32 sva = util::SIGN2 (va);
                for (std::size_t i = 0, count = polygon.points.size (); i < count; ++i) {
                    util::f32 vb = polygon.points[(i + 1) % polygon.points.size ()].y - y;
                    util::i32 svb = util::SIGN2 (vb);
                    if (sva != svb) {
                        util::f32 ua = polygon.points[i].x - x;
                        util::f32 ub = polygon.points[(i + 1) % polygon.points.size ()].x - x;
                        if ((ua > 0.0f && ub > 0.0f) || ((ua > 0.0f || ub > 0.0f) && (ua - va * (ub - ua) / (vb - va) > 0.0f))) {
                            ++nc;
                        }
                        sva = svb;
                    }
                    va = vb;
                }
                return (nc & 1) == 1;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point2 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                    const Point2 &pt,
                    const Matrix2 &matrix) {
                return Point2 (
                    pt.x * matrix.x.x + pt.y * matrix.y.x + matrix.t.x,
                    pt.x * matrix.x.y + pt.y * matrix.y.y + matrix.t.y);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 _LIB_THEKOGANS_MEX_BLAS_API Angle (
                    const Point2 &p1,
                    const Point2 &p2,
                    util::f32 eps) {
                util::f32 angle = p1.AngleWithXAxis (eps) - p2.AngleWithXAxis (eps);
                while (angle < 0.0f) {
                    angle += TWOPI;
                }
                return angle;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point2 _LIB_THEKOGANS_MEX_BLAS_API Bisect (
                    const Point2 &p1,
                    const Point2 &p2,
                    util::f32 eps) {
                // Compute bisecting angle.
                util::f32 angle = Angle (p1, p2, eps) * 0.5f;
                // Rotate p2 angle / 2 in to p1 around z axis.
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                return Point2 (p2.x * c - p2.y * s, p2.x * s + p2.y * c);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip2 (
                    Point2 &p1,
                    Point2 &p2,
                    const Bound2 &bound) {
                util::f32 dx = p2.x - p1.x;
                util::f32 dy = p2.y - p1.y;
                if (dx == 0.0f && dy == 0.0f) {
                    return p1.InBound (bound);
                }
                util::f32 t1 = 0.0f;
                util::f32 t2 = 1.0f;
                if (CLIPt ( dx, bound.min.x - p1.x, t1, t2) &&
                    CLIPt (-dx, p1.x - bound.max.x, t1, t2) &&
                    CLIPt ( dy, bound.min.y - p1.y, t1, t2) &&
                    CLIPt (-dy, p1.y - bound.max.y, t1, t2)) {
                    if (t2 < 1.0f) {
                        p2.x = p1.x + t2 * dx;
                        p2.y = p1.y + t2 * dy;
                    }
                    if (t1 > 0.0f) {
                        p1.x = p1.x + t1 * dx;
                        p1.y = p1.y + t1 * dy;
                    }
                    return true;
                }
                return false;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
