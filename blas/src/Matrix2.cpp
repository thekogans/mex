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

#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Matrix2 Matrix2::Translate (const Point2 &pt) {
                Matrix2 matrix (Matrix2::Identity);
                matrix.t.x = pt.x;
                matrix.t.y = pt.y;
                return matrix;
            }

            Matrix2 Matrix2::RotateZ (util::f32 angle) {
                Matrix2 matrix (Matrix2::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.x.x = c;
                matrix.x.y = s;
                matrix.y.x = -s;
                matrix.y.y = c;
                return matrix;
            }

            Matrix2 Matrix2::Scale (const Point2 &pt) {
                Matrix2 matrix (Matrix2::Zero);
                matrix.x.x = pt.x;
                matrix.y.y = pt.y;
                return matrix;
            }

            Matrix2 Matrix2::Skew (util::f32 x, util::f32 y) {
                Matrix2 matrix (Matrix2::Identity);
                matrix.y.x = x;
                matrix.x.y = y;
                return matrix;
            }

            Matrix2 Matrix2::Aim2D (const Point2 &p1, const Point2 &p2) {
                util::f32 scale = p1.Length ();
                if (util::IS_ZERO (scale)) {
                    return Matrix2::Zero;
                }
                scale = p2.Length () / scale;

                util::f32 angle1 = atan2f (p1.y, p1.x);
                if (angle1 < 0.0f) {
                    angle1 += TWOPI;
                }

                util::f32 angle2 = atan2f (p2.y, p2.x);
                if (angle2 < 0.0f) {
                    angle2 += TWOPI;
                }

                return RotateZ (angle2 - angle1) * Scale (Point2 (scale, scale));
            }

            Matrix2 &Matrix2::operator *= (const Matrix2 &matrix) {
                return *this = *this * matrix;
            }

            Matrix2 &Matrix2::operator *= (util::f32 s) {
                x *= s;
                y *= s;
                t *= s;
                return *this;
            }

            Matrix2 Matrix2::Invert () const {
                // FIXME: implement
                assert (0);
                return Matrix2::Zero;
            }

            namespace {
                // Calculate the determinant of a 2x2 matrix.
                //     |a, b|
                //     |c, d|
                inline util::f32 DET2X2 (util::f32 a, util::f32 b, util::f32 c, util::f32 d) {
                    return a * d - b * c;
                }

                // Calculate the determinant of a 3x3 matrix:
                //     |a1, b1, c1|
                //     |a2, b2, c2|
                //     |a3, b3, c3|
                inline util::f32 DET3X3 (
                    util::f32 a1, util::f32 a2, util::f32 a3,
                    util::f32 b1, util::f32 b2, util::f32 b3,
                    util::f32 c1, util::f32 c2, util::f32 c3) {
                    return
                        a1 * DET2X2 (b2, b3, c2, c3) -
                        b1 * DET2X2 (a2, a3, c2, c3) +
                        c1 * DET2X2 (a2, a3, b2, b3);
                }
            }

            util::f32 Matrix2::Determinant () const {
                util::f32 a1 = x.x;
                util::f32 b1 = x.y;
                util::f32 c1 = 0.0f;

                util::f32 a2 = y.x;
                util::f32 b2 = y.y;
                util::f32 c2 = 0.0f;

                util::f32 a3 = t.x;
                util::f32 b3 = t.y;
                util::f32 c3 = 1.0f;

                // FIXME: verify
                return DET3X3 (a1, a2, a3, b1, b2, b3, c1, c2, c3);
            }

            bool Matrix2::Decompose (Point2 &translation, util::f32 &rotation, Point2 &scale) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            Point2 Matrix2::XformNormal (const Point2 &normal) const {
                return Point2 (
                    normal.x * x.x + normal.y * y.x,
                    normal.x * x.y + normal.y * y.y);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Matrix2 _LIB_THEKOGANS_MEX_BLAS_API operator * (const Matrix2 &m1, const Matrix2 &m2) {
                return Matrix2 (
                    Point2 (
                        m1.x.x * m2.x.x + m1.x.y * m2.y.x,
                        m1.x.x * m2.x.y + m1.x.y * m2.y.y),
                    Point2 (
                        m1.y.x * m2.x.x + m1.y.y * m2.y.x,
                        m1.y.x * m2.x.y + m1.y.y * m2.y.y),
                    Point2 (
                        m1.t.x * m2.x.x + m1.t.y * m2.y.x + m2.t.x,
                        m1.t.x * m2.x.y + m1.t.y * m2.y.y + m2.t.y));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
