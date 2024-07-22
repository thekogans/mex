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

#if !defined (__thekogans_mex_blas_Point2_h)
#define __thekogans_mex_blas_Point2_h

#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Matrix.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Point3;
            struct Circle;
            struct Ellipse;
            template<typename T>
            struct Bound;
            template<typename T>
            struct Polygon;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Point2 {
                union {
                    struct {
                        util::f32 x;
                        util::f32 y;
                    };
                    struct {
                        util::f32 xy[2];
                    };
                };

                static const Point2 Zero;
                static const Point2 One;
                static const Point2 Big;
                static const Point2 NegBig;
                static const Point2 Small;
                static const Point2 NegSmall;
                static const Point2 X;
                static const Point2 Y;

                Point2 () {}
                Point2 (
                    util::f32 x_,
                    util::f32 y_) :
                    x (x_),
                    y (y_) {}
                Point2 (util::f32 xy[]) :
                    x (xy[0]),
                    y (xy[1]) {}
                Point2 (const Point2 &pt) :
                    x (pt.x),
                    y (pt.y) {}
                // Loose the third dimension. It is explicit to
                // prevent unintended mistakes that can be a
                // nightmare to debug.
                explicit Point2 (const Point3 &pt);

                inline const util::f32 &operator [] (util::ui32 index) const {
                    assert (index < 2);
                    return xy[index];
                }
                inline util::f32 &operator [] (util::ui32 index) {
                    assert (index < 2);
                    return xy[index];
                }

                inline Point2 &operator = (const Point2 &pt) {
                    if (&pt != this) {
                        x = pt.x;
                        y = pt.y;
                    }
                    return *this;
                }

                inline Point2 &operator += (const Point2 &pt) {
                    x += pt.x;
                    y += pt.y;
                    return *this;
                }

                inline Point2 &operator -= (const Point2 &pt) {
                    x -= pt.x;
                    y -= pt.y;
                    return *this;
                }

                Point2 &operator *= (const Matrix2 &matrix);

                inline Point2 &operator *= (util::f32 s) {
                    x *= s;
                    y *= s;
                    return *this;
                }

                inline Point2 &operator /= (util::f32 s) {
                    s = 1.0f / s;
                    x *= s;
                    y *= s;
                    return *this;
                }

                inline Point2 Perpendicular () const {
                    return Point2 (-y, x);
                }

                util::f32 AngleWithXAxis (util::f32 eps = EPSILON) const;
                util::f32 Normalize (util::f32 eps = EPSILON);

                inline util::f32 Length () const {
                    return sqrtf (x * x + y * y);
                }

                inline bool IsZero (util::f32 eps = EPSILON) const {
                    return util::IS_ZERO (x, eps) && util::IS_ZERO (y, eps);
                }

                bool InCircle (const Circle &circle) const;
                bool InEllipse (const Ellipse &ellipse) const;
                bool InBound (const Bound<Point2> &bound) const;
                bool InBound (const Bound<Point3> &bound) const;
                bool InPolygon (const Polygon<Point2> &polygon) const;

                inline bool IsBetweenEq (
                        const Point2 &p1,
                        const Point2 &p2,
                        util::f32 eps = EPSILON) const {
                    return
                        util::IS_BETWEEN_EQ (x, p1.x, p2.x, eps) &&
                        util::IS_BETWEEN_EQ (y, p1.y, p2.y, eps);
                }
            };

            _LIB_THEKOGANS_MEX_BLAS_DECL Point2 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                const Point2 &pt,
                const Matrix2 &matrx);

            inline bool operator == (
                    const Point2 &p1,
                    const Point2 &p2) {
                return p1.x == p2.x && p1.y == p2.y;
            }

            inline bool operator != (
                    const Point2 &p1,
                    const Point2 &p2) {
                return p1.x != p2.x || p1.y != p2.y;
            }

            inline Point2 operator + (
                    const Point2 &p1,
                    const Point2 &p2) {
                return Point2 (p1.x + p2.x, p1.y + p2.y);
            }

            inline Point2 operator - (
                    const Point2 &p1,
                    const Point2 &p2) {
                return Point2 (p1.x - p2.x, p1.y - p2.y);
            }

            inline Point2 operator - (const Point2 &pt) {
                return Point2 (-pt.x, -pt.y);
            }

            inline Point2 operator * (
                    util::f32 s,
                    const Point2 &pt) {
                return Point2 (s * pt.x, s * pt.y);
            }

            inline Point2 operator * (
                    const Point2 &pt,
                    util::f32 s) {
                return Point2 (pt.x * s, pt.y * s);
            }

            inline Point2 operator / (
                    const Point2 &pt,
                    util::f32 s) {
                util::f32 t = 1.0f / s;
                return Point2 (pt.x * t, pt.y * t);
            }

            inline Point2 Mid (
                    const Point2 &p1,
                    const Point2 &p2) {
                return util::LERP (0.5f, p1, p2);
            }

            inline util::f32 Distance (
                    const Point2 &p1,
                    const Point2 &p2) {
                return (p1 - p2).Length ();
            }

            inline Point2 Normalize (
                    const Point2 &pt,
                    util::f32 eps = EPSILON) {
                util::f32 length = pt.Length ();
                if (!util::IS_ZERO (length, eps)) {
                    length = 1.0f / length;
                    return Point2 (pt.x * length, pt.y * length);
                }
                return Point2::Zero;
            }

            inline util::f32 Dot (
                    const Point2 &p1,
                    const Point2 &p2) {
                return p1.x * p2.x + p1.y * p2.y;
            }

            // The order in which p1 and p2 are given is very important.
            // The algorithm assumes counter-clockwise ordering and will
            // produce an interior (left) angle.
            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 _LIB_THEKOGANS_MEX_BLAS_API Angle (
                const Point2 &p1,
                const Point2 &p2,
                util::f32 eps = EPSILON);
            _LIB_THEKOGANS_MEX_BLAS_DECL Point2 _LIB_THEKOGANS_MEX_BLAS_API Bisect (
                const Point2 &p1,
                const Point2 &p2,
                util::f32 eps = EPSILON);
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip2 (
                Point2 &p1,
                Point2 &p2,
                const Bound<Point2> &bound);

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Point2 &pt) {
                serializer << pt.x << pt.y;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    Point2 &pt) {
                serializer >> pt.x >> pt.y;
                return serializer;
            }

            const util::ui32 POINT2_SIZE = sizeof (Point2);

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Point2_h)
