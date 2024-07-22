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

#if !defined (__thekogans_mex_blas_Matrix3_h)
#define __thekogans_mex_blas_Matrix3_h

#include <cassert>
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix.h"
#include "thekogans/mex/blas/Matrix2.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Quaternion;
            struct EulerAngles;
            struct AngleAxis;

            template<>
            struct _LIB_THEKOGANS_MEX_BLAS_DECL Matrix<Point3> {
                Point3 x;
                Point3 y;
                Point3 z;
                Point3 t;

                Matrix<Point3> () {}
                Matrix<Point3> (
                    const Point3 &x_,
                    const Point3 &y_,
                    const Point3 &z_,
                    const Point3 &t_) :
                    x (x_),
                    y (y_),
                    z (z_),
                    t (t_) {}
                Matrix<Point3> (const Matrix<Point3> &matrix) :
                    x (matrix.x),
                    y (matrix.y),
                    z (matrix.z),
                    t (matrix.t) {}
                Matrix<Point3> (
                    const Matrix<Point2> &matrix,
                    const Point3 z_ = Point3::Zero) :
                    x (matrix.x),
                    y (matrix.y),
                    z (z_),
                    t (matrix.t) {}

                static const Matrix<Point3> Zero;
                static const Matrix<Point3> Identity;
                static const Matrix<Point3> MirrorX;
                static const Matrix<Point3> MirrorY;
                static const Matrix<Point3> MirrorZ;
                static const Matrix<Point3> MirrorXY;
                static const Matrix<Point3> MirrorXZ;
                static const Matrix<Point3> MirrorYZ;
                static const Matrix<Point3> MirrorXYZ;

                // IMPORTANT: All angles are in radians.
                static Matrix<Point3> Translate (const Point3 &pt);
                static Matrix<Point3> RotateX (util::f32 angle);
                static Matrix<Point3> RotateY (util::f32 angle);
                static Matrix<Point3> RotateZ (util::f32 angle);
                static Matrix<Point3> Scale (const Point3 &pt);
                static Matrix<Point3> Skew (
                    util::f32 x,
                    util::f32 y);
                static Matrix<Point3> Shear (const Point3 &pt);

                // Takes two points in the XY plane and computes a matrix
                // which when applied to p1 rotates and scales it in to p2.
                static Matrix<Point3> Aim2D (
                    const Point3 &p1,
                    const Point3 &p2);

                // Takes a point and computes a matrix which when applied
                // to point rotates it in to the Z axis.
                static Matrix<Point3> AimZ (const Point3 &pt);

                // Takes viewer parameter and computes a matrix which when
                // applied to points, transforms them in to viewer space.
                // IMPORTANT: Assumes fov and roll are in radians.
                static Matrix<Point3> AimViewer (
                    const Point3 &position,
                    const Point3 &target,
                    util::f32 fov,
                    util::f32 roll);

                static Matrix<Point3> LookAt (
                    const Point3 &position,
                    const Point3 &target,
                    const Point3 &up = Point3::Z,
                    util::f32 roll = 0.0f);

                static Matrix<Point3> FromQuaternion (const Quaternion &quat);
                static Matrix<Point3> FromEulerAngles (const EulerAngles &eulerAngles);
                static Matrix<Point3> FromAngleAxis (const AngleAxis &angleAxis);

                inline const Point3 &operator [] (util::ui32 index) const {
                    assert (index < 4);
                    return ((const Point3 *)this)[index];
                }
                inline Point3 &operator [] (util::ui32 index) {
                    assert (index < 4);
                    return ((Point3 *)this)[index];
                }

                inline Matrix<Point3> &operator = (const Matrix<Point3> &matrix) {
                    if (&matrix != this) {
                        x = matrix.x;
                        y = matrix.y;
                        z = matrix.z;
                        t = matrix.t;
                    }
                    return *this;
                }

                Matrix<Point3> &operator *= (const Matrix<Point3> &matrix);
                Matrix<Point3> &operator *= (util::f32 s);

                Matrix<Point3> Invert () const;
                util::f32 Determinant () const;
                bool Decompose (
                    Point3 &translation,
                    Quaternion &rotation,
                    Point3 &scale) const;

                Point3 XformNormal (const Point3 &normal) const;

                inline Matrix<Point3> Transpose () const {
                    return Matrix<Point3> (
                        Point3 (x.x, y.x, z.x),
                        Point3 (x.y, y.y, z.y),
                        Point3 (x.z, y.z, z.z), t);
                }

                inline bool IsLeftHanded () const {
                    return Dot (Cross (x, y), z) < 0.0f;
                }
            };

            _LIB_THEKOGANS_MEX_BLAS_DECL Matrix3 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                const Matrix3 &m1,
                const Matrix3 &m2);

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Matrix3 &matrix) {
                serializer << matrix.x << matrix.y << matrix.z << matrix.t;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    Matrix3 &matrix) {
                serializer >> matrix.x >> matrix.y >> matrix.z >> matrix.t;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Matrix3_h)
