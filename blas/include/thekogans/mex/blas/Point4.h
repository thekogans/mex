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

#if !defined (__thekogans_mex_blas_Point4_h)
#define __thekogans_mex_blas_Point4_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Point4;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Point4 {
                union {
                    struct {
                        util::f32 x;
                        util::f32 y;
                        util::f32 z;
                        util::f32 w;
                    };
                    struct {
                        util::f32 xyzw[4];
                    };
                };

                static const Point4 Zero;
                static const Point4 One;
                static const Point4 Big;
                static const Point4 NegBig;
                static const Point4 Small;
                static const Point4 NegSmall;
                static const Point4 X;
                static const Point4 Y;
                static const Point4 Z;

                Point4 () {}
                Point4 (util::f32 x_, util::f32 y_, util::f32 z_, util::f32 w_) :
                    x (x_), y (y_), z (z_), w (w_) {}
                Point4 (util::f32 xyzw[]) :
                    x (xyzw[0]), y (xyzw[1]), z (xyzw[2]), w (xyzw[3]) {}
                Point4 (const Point4 &pt) :
                    x (pt.x), y (pt.y), z (pt.z), w (pt.w) {}
                Point4 (const Point2 &pt, util::f32 z_ = 0, util::f32 w_ = 0) :
                    x (pt.x), y (pt.y), z (z_), w (w_) {}
                Point4 (const Point3 &pt, util::f32 w_ = 0) :
                    x (pt.x), y (pt.y), z (pt.z), w (w_) {}

                inline const util::f32 &operator [] (util::ui32 index) const {
                    assert (index < 4);
                    return xyzw[index];
                }
                inline util::f32 &operator [] (util::ui32 index) {
                    assert (index < 4);
                    return xyzw[index];
                }

                inline Point4 &operator = (const Point4 &pt) {
                    if (&pt != this) {
                        x = pt.x;
                        y = pt.y;
                        z = pt.z;
                        w = pt.w;
                    }
                    return *this;
                }

                inline Point4 &operator += (const Point4 &pt) {
                    util::f32 scale = w / pt.w;
                    x += pt.x * scale;
                    y += pt.y * scale;
                    z += pt.z * scale;
                    return *this;
                }

                inline Point4 &operator -= (const Point4 &pt) {
                    util::f32 scale = w / pt.w;
                    x -= pt.x * scale;
                    y -= pt.y * scale;
                    z -= pt.z * scale;
                    return *this;
                }

                inline Point4 &operator *= (util::f32 s) {
                    x *= s;
                    y *= s;
                    z *= s;
                    return *this;
                }

                inline Point4 &operator /= (util::f32 s) {
                    s = 1.0f / s;
                    x *= s;
                    y *= s;
                    z *= s;
                    return *this;
                }

                inline util::f32 Length () const {
                    util::f32 scale = 1.0f / w;
                    util::f32 x_ = x * scale;
                    util::f32 y_ = y * scale;
                    util::f32 z_ = z * scale;
                    return sqrtf (x_ * x_ + y_ * y_ + z_ * z_);
                }

                Point4 &operator *= (const Matrix4 &matrix);
            };

            inline util::f32 Dot (const Point4 &p1, const Point4 &p2) {
                return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z + p1.w * p2.w;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point4 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                const Point4 &pt,
                const Matrix4 &matrx);

            inline util::Serializer &operator << (util::Serializer &serializer, const Point4 &pt) {
                serializer << pt.x << pt.y << pt.z << pt.w;
                return serializer;
            }

            inline util::Serializer &operator >> (util::Serializer &serializer, Point4 &pt) {
                serializer >> pt.x >> pt.y >> pt.z >> pt.w;
                return serializer;
            }

            const util::ui32 POINT4_SIZE = sizeof (Point4);

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Point4_h)
