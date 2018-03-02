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

#if !defined (__thekogans_mex_blas_Point3_h)
#define __thekogans_mex_blas_Point3_h

#include <cassert>
#include <cmath>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Matrix.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Point2;
            template<typename T>
            struct Bound;
            template<typename T>
            struct Polygon;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Point3 {
                union {
                    struct {
                        util::f32 x;
                        util::f32 y;
                        util::f32 z;
                    };
                    struct {
                        util::f32 xyz[3];
                    };
                };

                static const Point3 Zero;
                static const Point3 One;
                static const Point3 Big;
                static const Point3 NegBig;
                static const Point3 Small;
                static const Point3 NegSmall;
                static const Point3 X;
                static const Point3 Y;
                static const Point3 Z;

                Point3 () {}
                Point3 (util::f32 x_, util::f32 y_, util::f32 z_) :
                    x (x_), y (y_), z (z_) {}
                Point3 (const util::f32 xyz[]) :
                    x (xyz[0]), y (xyz[1]), z (xyz[2]) {}
                Point3 (const Point3 &pt) :
                    x (pt.x), y (pt.y), z (pt.z) {}
                Point3 (const Point2 &pt, util::f32 z_ = 0.0f);

                inline const util::f32 &operator [] (std::size_t index) const {
                    assert (index < 3);
                    return xyz[index];
                }
                inline util::f32 &operator [] (std::size_t index) {
                    assert (index < 3);
                    return xyz[index];
                }

                inline Point3 &operator = (const Point3 &pt) {
                    if (&pt != this) {
                        x = pt.x;
                        y = pt.y;
                        z = pt.z;
                    }
                    return *this;
                }

                Point3 &operator = (const Point2 &pt);

                inline Point3 &operator += (const Point3 &pt) {
                    x += pt.x;
                    y += pt.y;
                    z += pt.z;
                    return *this;
                }

                inline Point3 &operator -= (const Point3 &pt) {
                    x -= pt.x;
                    y -= pt.y;
                    z -= pt.z;
                    return *this;
                }

                Point3 &operator *= (const Matrix3 &matrix);

                inline Point3 &operator *= (util::f32 s) {
                    x *= s;
                    y *= s;
                    z *= s;
                    return *this;
                }

                inline Point3 &operator /= (util::f32 s) {
                    s = 1.0f / s;
                    x *= s;
                    y *= s;
                    z *= s;
                    return *this;
                }

                inline Point3 Perpendicular () const {
                    if (x == 0.0f && y == 0.0f) {
                        return z == 0.0f ? Zero : X;
                    }
                    return Point3 (-y, x, 0.0f);
                }

                util::f32 Normalize (util::f32 eps = util::EPSILON);

                inline util::f32 Length () const {
                    return sqrtf (x * x + y * y + z * z);
                }

                inline bool IsZero (util::f32 eps = util::EPSILON) const {
                    return util::IS_ZERO (x, eps) && util::IS_ZERO (y, eps) && util::IS_ZERO (z, eps);
                }

                inline bool IsBetweenEq (const Point3 &p1, const Point3 &p2, util::f32 eps = util::EPSILON) const {
                    return
                        util::IS_BETWEEN_EQ (x, p1.x, p2.x, eps) &&
                        util::IS_BETWEEN_EQ (y, p1.y, p2.y, eps) &&
                        util::IS_BETWEEN_EQ (z, p1.z, p2.z, eps);
                }

                bool InBound (const Bound<Point2> &bound) const;
                bool InBound (const Bound<Point3> &bound) const;
                bool InPolygon (const Polygon<Point3> &polygon) const;

                Point2 Project (bool perspective, util::f32 eps = util::EPSILON) const;
            };

            inline bool operator == (const Point3 &p1, const Point3 &p2) {
                return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
            }

            inline bool operator != (const Point3 &p1, const Point3 &p2) {
                return p1.x != p2.x || p1.y != p2.y || p1.z != p2.z;
            }

            inline Point3 operator + (const Point3 &p1, const Point3 &p2) {
                return Point3 (p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
            }

            inline Point3 operator - (const Point3 &p1, const Point3 &p2) {
                return Point3 (p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
            }

            inline Point3 operator - (const Point3 &pt) {
                return Point3 (-pt.x, -pt.y, -pt.z);
            }

            inline Point3 operator * (util::f32 s, const Point3 &pt) {
                return Point3 (s * pt.x, s * pt.y, s * pt.z);
            }

            inline Point3 operator * (const Point3 &pt, util::f32 s) {
                return Point3 (pt.x * s, pt.y * s, pt.z * s);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point3 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                const Point3 &pt,
                const Matrix3 &matrx);

            inline Point3 operator / (const Point3 &pt, util::f32 s) {
                return pt * (1.0f / s);
            }

            inline Point3 operator / (const Point3 &p1, const Point3 &p2) {
                return Point3 (p1.x / p2.x, p1.y / p2.y, p1.z / p2.z);
            }

            inline Point3 Mid (const Point3 &p1, const Point3 &p2) {
                return util::LERP (0.5f, p1, p2);
            }

            inline util::f32 Distance (const Point3 &p1, const Point3 &p2) {
                return (p1 - p2).Length ();
            }

            inline Point3 Normalize (const Point3 &pt, util::f32 eps = util::EPSILON) {
                util::f32 length = pt.Length ();
                return util::IS_ZERO (length, eps) ? Point3::Zero : pt / length;
            }

            inline util::f32 Dot (const Point3 &p1, const Point3 &p2) {
                return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
            }

            inline Point3 Cross (const Point3 &p1, const Point3 &p2) {
                return Point3 (
                    p1.y * p2.z - p1.z * p2.y,
                    p1.z * p2.x - p1.x * p2.z,
                    p1.x * p2.y - p1.y * p2.x);
            }

            // Reflect incident vector (i) using normal vector (n).
            inline Point3 Reflect (const Point3 &i, const Point3 &n) {
                return i - 2.0f * Dot (i, n) * n;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 _LIB_THEKOGANS_MEX_BLAS_API Angle (
                const Point3 &p1,
                const Point3 &p2,
                util::f32 eps = util::EPSILON);
            _LIB_THEKOGANS_MEX_BLAS_DECL Point3 _LIB_THEKOGANS_MEX_BLAS_API Bisect (
                const Point3 &p1,
                const Point3 &p2,
                util::f32 eps = util::EPSILON);

            // Sutherland-Kohen line clipper.
            // Orthographic
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API CLIPt (
                util::f32 denom,
                util::f32 num,
                util::f32 &tE,
                util::f32 &tL);
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip2D (
                Point3 &p1,
                Point3 &p2,
                const Bound<Point3> &bound);
            // Perspective
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API ClipNearFar (
                Point3 &p1,
                Point3 &p2,
                util::f32 nearPlane,
                util::f32 farPlane);
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip3DW (
                Point3 &p1,
                Point3 &p2,
                util::f32 aspect);
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip3DH (
                Point3 &p1,
                Point3 &p2,
                util::f32 aspect);
            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip3D (
                Point3 &p1,
                Point3 &p2);

            inline util::Serializer &operator << (util::Serializer &serializer, const Point3 &pt) {
                serializer << pt.x << pt.y << pt.z;
                return serializer;
            }

            inline util::Serializer &operator >> (util::Serializer &serializer, Point3 &pt) {
                serializer >> pt.x >> pt.y >> pt.z;
                return serializer;
            }

            const util::ui32 POINT3_SIZE = sizeof (Point3);

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Point3_h)
