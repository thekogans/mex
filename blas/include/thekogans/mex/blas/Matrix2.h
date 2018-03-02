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

#if !defined (__thekogans_mex_blas_Matrix2_h)
#define __thekogans_mex_blas_Matrix2_h

#include <cassert>
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<>
            struct _LIB_THEKOGANS_MEX_BLAS_DECL Matrix<Point2> {
                Point2 x;
                Point2 y;
                Point2 t;

                Matrix<Point2> () {}
                Matrix<Point2> (const Point2 &x_, const Point2 &y_, const Point2 &t_) :
                    x (x_), y (y_), t (t_) {}
                Matrix<Point2> (const Matrix<Point2> &matrix) :
                    x (matrix.x), y (matrix.y), t (matrix.t) {}

                static const Matrix<Point2> Zero;
                static const Matrix<Point2> Identity;
                static const Matrix<Point2> MirrorX;
                static const Matrix<Point2> MirrorY;
                static const Matrix<Point2> MirrorXY;

                // IMPORTANT: All angles are in radians.
                static Matrix<Point2> Translate (const Point2 &pt);
                static Matrix<Point2> RotateZ (util::f32 angle);
                static Matrix<Point2> Scale (const Point2 &pt);
                static Matrix<Point2> Skew (util::f32 x, util::f32 y);
                // Takes two points in the XY plane and computes a matrix
                // which when applied to p1 rotates and scales it in to p2.
                static Matrix<Point2> Aim2D (const Point2 &p1, const Point2 &p2);

                inline const Point2 &operator [] (util::ui32 index) const {
                    assert (index < 3);
                    return ((const Point2 *)this)[index];
                }
                inline Point2 &operator [] (util::ui32 index) {
                    assert (index < 3);
                    return ((Point2 *)this)[index];
                }

                inline Matrix<Point2> &operator = (const Matrix<Point2> &matrix) {
                    if (&matrix != this) {
                        x = matrix.x;
                        y = matrix.y;
                        t = matrix.t;
                    }
                    return *this;
                }

                Matrix<Point2> &operator *= (const Matrix<Point2> &matrix);
                Matrix<Point2> &operator *= (util::f32 s);

                Matrix<Point2> Invert () const;
                util::f32 Determinant () const;
                bool Decompose (Point2 &translation, util::f32 &rotation, Point2 &scale) const;

                inline Matrix<Point2> Transpose () const {
                    return Matrix<Point2> (Point2 (x.x, y.x), Point2 (x.y, y.y), t);
                }

                Point2 XformNormal (const Point2 &normal) const;
            };

            _LIB_THEKOGANS_MEX_BLAS_DECL Matrix2 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                const Matrix2 &m1,
                const Matrix2 &m2);

            inline util::Serializer &operator << (util::Serializer &serializer, const Matrix2 &matrix) {
                serializer << matrix.x << matrix.y << matrix.t;
                return serializer;
            }

            inline util::Serializer &operator >> (util::Serializer &serializer, Matrix2 &matrix) {
                serializer >> matrix.x >> matrix.y >> matrix.t;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Matrix2_h)
