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

#if !defined (__thekogans_mex_blas_Matrix4_h)
#define __thekogans_mex_blas_Matrix4_h

#include <cassert>
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point4.h"
#include "thekogans/mex/blas/Matrix.h"
#include "thekogans/mex/blas/Matrix3.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<typename T>
            struct Bound;

            template<>
            struct _LIB_THEKOGANS_MEX_BLAS_DECL Matrix<Point4> {
                Point4 x;
                Point4 y;
                Point4 z;
                Point4 t;

                Matrix<Point4> () {}
                Matrix<Point4> (const Point4 &x_, const Point4 &y_, const Point4 &z_, const Point4 &t_) :
                    x (x_), y (y_), z (z_), t (t_) {}
                Matrix<Point4> (const Matrix<Point4> &matrix) :
                    x (matrix.x), y (matrix.y), z (matrix.z), t (matrix.t) {}
                explicit Matrix<Point4> (const Matrix3 &matrix) :
                    x (matrix.x, 0.0f), y (matrix.y, 0.0f), z (matrix.z, 0.0f), t (matrix.t, 1.0f) {}

                static const Matrix<Point4> Zero;
                static const Matrix<Point4> Identity;
                static const Matrix<Point4> MirrorX;
                static const Matrix<Point4> MirrorY;
                static const Matrix<Point4> MirrorZ;
                static const Matrix<Point4> MirrorXY;
                static const Matrix<Point4> MirrorXZ;
                static const Matrix<Point4> MirrorYZ;
                static const Matrix<Point4> MirrorXYZ;

                // IMPORTANT: All angles are in radians.
                static Matrix<Point4> Translate (const Point3 &pt);
                static Matrix<Point4> RotateX (util::f32 angle);
                static Matrix<Point4> RotateY (util::f32 angle);
                static Matrix<Point4> RotateZ (util::f32 angle);
                static Matrix<Point4> Scale (const Point3 &pt);
                static Matrix<Point4> Skew (util::f32 x, util::f32 y);
                static Matrix<Point4> Shear (const Point3 &pt);
                static Matrix<Point4> LookAt (const Point3 &position, const Point3 &target,
                    const Point3 &up = Point3::Z, util::f32 roll = 0.0f);
                static Matrix<Point4> Orthographic (const blas::Bound<Point3> &frustum);
                static Matrix<Point4> Perspective(util::f32 fovy,
                    util::f32 aspect, util::f32 zNear, util::f32 zFar);

                inline const Point4 &operator [] (util::ui32 index) const {
                    assert (index < 4);
                    return ((const Point4 *)this)[index];
                }
                inline Point4 &operator [] (util::ui32 index) {
                    assert (index < 4);
                    return ((Point4 *)this)[index];
                }

                inline Matrix<Point4> &operator = (const Matrix<Point4> &matrix) {
                    if (&matrix != this) {
                        x = matrix.x;
                        y = matrix.y;
                        z = matrix.z;
                        t = matrix.t;
                    }
                    return *this;
                }

                Matrix<Point4> &operator *= (const Matrix<Point4> &matrix);

                inline Matrix<Point4> Transpose () const {
                    return Matrix<Point4> (
                        Point4 (x.x, y.x, z.x, t.x),
                        Point4 (x.y, y.y, z.y, t.y),
                        Point4 (x.z, y.z, z.z, t.z),
                        Point4 (x.w, y.w, z.w, t.w));
                }
            };

            _LIB_THEKOGANS_MEX_BLAS_DECL Matrix4 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                const Matrix4 &m1, const Matrix4 &m2);

            inline util::Serializer &operator << (util::Serializer &serializer, const Matrix4 &matrix) {
                serializer << matrix.x << matrix.y << matrix.z << matrix.t;
                return serializer;
            }

            inline util::Serializer &operator >> (util::Serializer &serializer, Matrix4 &matrix) {
                serializer >> matrix.x >> matrix.y >> matrix.z >> matrix.t;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Matrix4_h)
