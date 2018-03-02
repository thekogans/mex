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
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/blas/EulerAngles.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Matrix3 Matrix3::Translate (const Point3 &pt) {
                Matrix3 matrix (Matrix3::Identity);
                matrix.t.x = pt.x;
                matrix.t.y = pt.y;
                matrix.t.z = pt.z;
                return matrix;
            }

            Matrix3 Matrix3::RotateX (util::f32 angle) {
                Matrix3 matrix (Matrix3::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.y.y = c;
                matrix.y.z = s;
                matrix.z.y = -s;
                matrix.z.z = c;
                return matrix;
            }

            Matrix3 Matrix3::RotateY (util::f32 angle) {
                Matrix3 matrix (Matrix3::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.x.x = c;
                matrix.x.z = -s;
                matrix.z.x = s;
                matrix.z.z = c;
                return matrix;
            }

            Matrix3 Matrix3::RotateZ (util::f32 angle) {
                Matrix3 matrix (Matrix3::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.x.x = c;
                matrix.x.y = s;
                matrix.y.x = -s;
                matrix.y.y = c;
                return matrix;
            }

            Matrix3 Matrix3::Scale (const Point3 &pt) {
                Matrix3 matrix (Matrix3::Zero);
                matrix.x.x = pt.x;
                matrix.y.y = pt.y;
                matrix.z.z = pt.z;
                return matrix;
            }

            Matrix3 Matrix3::Skew (util::f32 x, util::f32 y) {
                Matrix3 matrix (Matrix3::Identity);
                matrix.y.x = x;
                matrix.x.y = y;
                return matrix;
            }

            Matrix3 Matrix3::Shear (const Point3 &pt) {
                Matrix3 matrix (Matrix3::Identity);
                matrix.y.x = pt.x;
                matrix.z.x = pt.y;
                matrix.z.y = pt.z;
                return matrix;
            }

            Matrix3 Matrix3::Aim2D (const Point3 &p1, const Point3 &p2) {
                util::f32 scale = Point3 (p1.x, p1.y, 0.0f).Length ();
                if (util::IS_ZERO (scale)) {
                    return Matrix3::Zero;
                }
                scale = Point3 (p2.x, p2.y, 0.0f).Length () / scale;

                util::f32 angle1 = atan2f (p1.y, p1.x);
                if (angle1 < 0.0f) {
                    angle1 += util::TWOPI;
                }

                util::f32 angle2 = atan2f (p2.y, p2.x);
                if (angle2 < 0.0f) {
                    angle2 += util::TWOPI;
                }

                return RotateZ (angle2 - angle1) * Scale (Point3 (scale, scale, 1.0f));
            }

            Matrix3 Matrix3::AimZ (const Point3 &pt) {
                // vector zero length, collapse the world!
                util::f32 r = pt.Length ();
                if (util::IS_ZERO (r)) {
                    return Matrix3::Identity;
                }

                // vector already points in the z direction
                if (util::IS_ZERO (pt.x) && util::IS_ZERO (pt.y)) {
                    return pt.z > 0.0f ? Matrix3::Identity : Matrix3::MirrorYZ;
                }

                util::f32 theta = atan2f (pt.y, pt.x);
                util::f32 fee = acosf (pt.z / r);
                util::f32 ct = cosf (theta);
                util::f32 st = sinf (theta);
                util::f32 cf = cosf (fee);
                util::f32 sf = sinf (fee);

                return Matrix3 (
                    Point3 (-st, -ct * cf, ct * sf),
                    Point3 (ct, -st * cf, st * sf),
                    Point3 (0.0f, sf, cf),
                    Point3 (0.0f, 0.0f, 0.0f));
            }

            Matrix3 Matrix3::AimViewer (
                const Point3 &position, const Point3 &target, util::f32 fov, util::f32 roll) {
                util::f32 scale = 1.0f / tanf (fov * 0.5f);
                return
                    Translate (-position) *
                    AimZ (target - position) *
                    Matrix3::MirrorX *  // turn to left-handed
                    RotateZ (roll) *
                    Scale (Point3 (scale, scale, 1.0f));
            }

            Matrix3 Matrix3::LookAt (
                const Point3 &position, const Point3 &target, const Point3 &up, util::f32 roll) {
                Point3 f = Normalize (target - position);
                Point3 s = Normalize (Cross (f, up));
                Point3 u = Normalize (Cross (s, f));
                return
                    Translate (-position) *
                    Matrix3 (
                        Point3 (s.x, u.x, -f.x),
                        Point3 (s.y, u.y, -f.y),
                        Point3 (s.z, u.z, -f.z),
                        Point3::Zero) *
                    RotateZ (roll);
            }

    #if 1
            Matrix3 Matrix3::FromQuaternion (const Quaternion &quat) {
                util::f32 s = 2.0f / Dot (quat, quat);

                util::f32 xs = quat.x * s;
                util::f32 ys = quat.y * s;
                util::f32 zs = quat.z * s;

                util::f32 wx = quat.w * xs;
                util::f32 wy = quat.w * ys;
                util::f32 wz = quat.w * zs;

                util::f32 xx = quat.x * xs;
                util::f32 xy = quat.x * ys;
                util::f32 xz = quat.x * zs;

                util::f32 yy = quat.y * ys;
                util::f32 yz = quat.y * zs;
                util::f32 zz = quat.z * zs;

                Matrix3 matrix;

                matrix.x.x = 1.0f - (yy + zz);
                matrix.x.y = xy - wz;
                matrix.x.z = xz + wy;

                matrix.y.x = xy + wz;
                matrix.y.y = 1.0f - (xx + zz);
                matrix.y.z = yz - wx;

                matrix.z.x = xz - wy;
                matrix.z.y = yz + wx;
                matrix.z.z = 1.0f - (xx + yy);

                matrix.t.x = 0.0f;
                matrix.t.y = 0.0f;
                matrix.t.z = 0.0f;

                return matrix;
            }
    #else
            // This version is functionally equivalent to the one above. It
            // should execute faster due to register pairing.
            Matrix3 Matrix3::FromQuaternion (const Quaternion &quat) {
                Matrix3 matrix;
                util::f32 s = 2.0f / Dot (quat, quat);
                util::f32 x2 = quat.x * s;
                util::f32 y2 = quat.y * s;
                util::f32 z2 = quat.z * s;
                {
                    util::f32 xx2 = quat.x * x2;
                    util::f32 yy2 = quat.y * y2;
                    util::f32 zz2 = quat.z * z2;
                    matrix.x.x = 1.0f - yy2 - zz2;
                    matrix.y.y = 1.0f - xx2 - zz2;
                    matrix.z.z = 1.0f - xx2 - yy2;
                }
                {
                    util::f32 yz2 = quat.y * z2;
                    util::f32 wx2 = quat.w * x2;
                    matrix.z.y = yz2 + wx2;
                    matrix.y.z = yz2 - wx2;
                }
                {
                    util::f32 xy2 = quat.x * y2;
                    util::f32 wz2 = quat.w * z2;
                    matrix.y.x = xy2 + wz2;
                    matrix.x.y = xy2 - wz2;
                }
                {
                    util::f32 xz2 = quat.x * z2;
                    util::f32 wy2 = quat.w * y2;
                    matrix.x.z = xz2 + wy2;
                    matrix.z.x = xz2 - wy2;
                }
                matrix.t.x = 0.0f;
                matrix.t.y = 0.0f;
                matrix.t.z = 0.0f;
                return matrix;
            }
    #endif

            Matrix3 Matrix3::FromEulerAngles (const EulerAngles &eulerAngles) {
                return
                    RotateX (eulerAngles.angles.x) *
                    RotateY (eulerAngles.angles.y) *
                    RotateZ (eulerAngles.angles.z);
            }

            Matrix3 Matrix3::FromAngleAxis (const AngleAxis &angleAxis) {
                return FromQuaternion (Quaternion::FromAngleAxis (angleAxis));
            }

            Matrix3 &Matrix3::operator *= (const Matrix3 &matrix) {
                return *this = *this * matrix;
            }

            Matrix3 &Matrix3::operator *= (util::f32 s) {
                x *= s;
                y *= s;
                z *= s;
                t *= s;
                return *this;
            }

            namespace {
                // Fast inversion routine for 3x3 matrices.
                //
                // This takes 30 multiplies/divides, as opposed to 39 for Cramer's Rule.
                // The algorithm consists of performing fast Gaussian elimination, by never
                // doing any operations where the result is guaranteed to be zero, or where
                // one operand is guaranteed to be zero. This is done at the cost of clarity,
                // alas.
                //
                // Returns true if the inverse was successful, false if it failed.
                bool Invert3x3 (Matrix3 &matrix1, Matrix3 &matrix2) {
                    // attempt to find the largest entry in first column to use as pivot
                    util::f32 a = matrix1[0][0];
                    if (a < 0.0f) {
                        a = -a;
                    }
                    util::f32 b = matrix1[1][0];
                    if (b < 0.0f) {
                        b = -b;
                    }
                    util::f32 c = matrix1[2][0];
                    if (c < 0.0f) {
                        c = -c;
                    }

                    util::i32 row0;
                    if (a > b) {
                        if (a > c) {
                            row0 = 0;
                        }
                        else {
                            row0 = 2;
                        }
                    }
                    else {
                        if (b > c) {
                            row0 = 1;
                        }
                        else {
                            row0 = 2;
                        }
                    }

                    // Scale row0 of matrix1
                    if (util::IS_ZERO (matrix1[row0][0])) {
                        return false;
                    }

                    util::f32 temp = 1.0f / matrix1[row0][0];
                    matrix1[row0][0] = 1.0f;
                    matrix1[row0][1] *= temp;
                    matrix1[row0][2] *= temp;

                    // Scale row0 of inverse
                    matrix2[row0][row0] = temp; // other entries are zero -- no effort

                    {
                        // Clear column zero of matrix1, and make corresponding changes in inverse
                        for (util::i32 i = 0; i < 3; ++i) {
                            if (i != row0) {    // for i = all rows but row0
                                temp = -matrix1[i][0];
                                matrix1[i][0] = 0.0f;
                                matrix1[i][1] += temp * matrix1[row0][1];
                                matrix1[i][2] += temp * matrix1[row0][2];
                                matrix2[i][row0] = temp * matrix2[row0][row0];
                            }
                        }
                    }

                    // We've now done Gaussian elimination so that the matrix1 and
                    // inverse look like this:
                    //
                    //  1 * *       * 0 0
                    //  0 * *       * 1 0
                    //  0 * *       * 0 1
                    //
                    // We now proceed to do elimination on the second column.

                    // Find which row to use
                    util::i32 i1, i2;
                    if (row0 == 0) {
                        i1 = 1;
                        i2 = 2;
                    }
                    else if (row0 == 1) {
                        i1 = 0;
                        i2 = 2;
                    }
                    else {
                        i1 = 0;
                        i2 = 1;
                    }

                    // Find which is larger in abs. val.:the entry in [i1][1] or [i2][1]
                    // and use that value for pivoting.
                    a = matrix1[i1][1];
                    if (a < 0) {
                        a = -a;
                    }
                    b = matrix1[i2][1];
                    if (b < 0) {
                        b = -b;
                    }
                    util::i32 row1;
                    if (a > b) {
                        row1 = i1;
                    }
                    else {
                        row1 = i2;
                    }
                    util::i32 row2 = row1 == i1 ? i2 : i1;

                    // Scale row1 in matrix1
                    if (util::IS_ZERO (matrix1[row1][1])) {
                        return false;
                    }
                    temp = 1.0f / matrix1[row1][1];
                    matrix1[row1][1] = 1.0f;
                    matrix1[row1][2] *= temp;   // matrix1[row1][0] is zero already

                    // Scale row1 in matrix2
                    matrix2[row1][row1] = temp; // it used to be a 1.0
                    matrix2[row1][row0] *= temp;

                    {
                        // Clear column one, matrix1, and make corresponding changes in matrix2
                        for (util::i32 i = 0; i < 3; ++i) {
                            if (i != row1) {    // for i = all rows but row1
                                temp = -matrix1[i][1];
                                matrix1[i][1] = 0.0f;
                                matrix1[i][2] += temp * matrix1[row1][2];

                                matrix2[i][row1] = temp * matrix2[row1][row1];
                                matrix2[i][row0] += temp * matrix2[row1][row0];
                            }
                        }
                    }

                    // Scale row2 in matrix1
                    if (util::IS_ZERO (matrix1[row2][2])) {
                        return false;
                    }

                    temp = 1.0f / matrix1[row2][2];
                    matrix1[row2][2] = 1.0f;    // matrix1[row2][*] is zero already

                    // Scale row2 in matrix2
                    matrix2[row2][row2]  = temp;    // it used to be a 1.0
                    matrix2[row2][row0] *= temp;
                    matrix2[row2][row1] *= temp;

                    // Clear column one, matrix1, and make corresponding changes in matrix2
                    for (util::i32 i = 0; i < 3; ++i) {
                        if (i != row2) {    // for i = all rows but row2
                            temp = -matrix1[i][2];
                            matrix1[i][2] = 0.0f;
                            matrix2[i][row0] += temp * matrix2[row2][row0];
                            matrix2[i][row1] += temp * matrix2[row2][row1];
                            matrix2[i][row2] += temp * matrix2[row2][row2];
                        }
                    }

                    // Now all is done except that the inverse needs to have its rows shuffled.
                    // row0 needs to be moved to matrix2[0][*], row1 to matrix2[1][*], etc.
                    // We *didn't* do the swapping before the elimination so that we could more
                    // easily keep track of what ops are needed to be done in the inverse.

                    if (row0 != 0) {
                        if (row1 == 0) {
                            std::swap (matrix2[row0][0], matrix2[row1][0]);
                            std::swap (matrix2[row0][1], matrix2[row1][1]);
                            std::swap (matrix2[row0][2], matrix2[row1][2]);
                            std::swap (row0, row1);
                        }
                        else {
                            std::swap (matrix2[row0][0], matrix2[row2][0]);
                            std::swap (matrix2[row0][1], matrix2[row2][1]);
                            std::swap (matrix2[row0][2], matrix2[row2][2]);
                            std::swap (row0, row2);
                        }
                    }

                    if (row1 != 1) {
                        std::swap (matrix2[row1][0], matrix2[row2][0]);
                        std::swap (matrix2[row1][1], matrix2[row2][1]);
                        std::swap (matrix2[row1][2], matrix2[row2][2]);
                    }

                    return true;
                }
            }

            // This returns the inverse of the given matrix.
            // Fast inversion routine for 4 x 3 matrices.
            // Returns Matrix3::Zero if the inverse failed.
            Matrix3 Matrix3::Invert () const {
                Matrix3 matrix1 (*this);
                Matrix3 matrix2 (Identity);
                if (Invert3x3 (matrix1, matrix2)) {
                    matrix2[3][0] -= matrix1[3][0] * matrix2[0][0] + matrix1[3][1] * matrix2[1][0] + matrix1[3][2] * matrix2[2][0];
                    matrix2[3][1] -= matrix1[3][0] * matrix2[0][1] + matrix1[3][1] * matrix2[1][1] + matrix1[3][2] * matrix2[2][1];
                    matrix2[3][2] -= matrix1[3][0] * matrix2[0][2] + matrix1[3][1] * matrix2[1][2] + matrix1[3][2] * matrix2[2][2];
                    return matrix2;
                }
                return Matrix3::Zero;
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

            util::f32 Matrix3::Determinant () const {
                util::f32 a1 = x.x;
                util::f32 b1 = x.y;
                util::f32 c1 = x.z;
                util::f32 d1 = 0.0f;

                util::f32 a2 = y.x;
                util::f32 b2 = y.y;
                util::f32 c2 = y.z;
                util::f32 d2 = 0.0f;

                util::f32 a3 = z.x;
                util::f32 b3 = z.y;
                util::f32 c3 = z.z;
                util::f32 d3 = 0.0f;

                util::f32 a4 = t.x;
                util::f32 b4 = t.y;
                util::f32 c4 = t.z;
                util::f32 d4 = 1.0f;

                return
                    a1 * DET3X3 (b2, b3, b4, c2, c3, c4, d2, d3, d4) -
                    b1 * DET3X3 (a2, a3, a4, c2, c3, c4, d2, d3, d4) +
                    c1 * DET3X3 (a2, a3, a4, b2, b3, b4, d2, d3, d4) -
                    d1 * DET3X3 (a2, a3, a4, b2, b3, b4, c2, c3, c4);
            }

            bool Matrix3::Decompose (Point3 &translation, Quaternion &rotation, Point3 &scale) const {
                if (util::IS_ZERO (Determinant ())) {
                    return false;
                }

                // Take care of translation.
                translation = t;

                Matrix3 matrix (x, y, z, Point3::Zero);

                // Compute X scale factor and normalize first row.
                scale.x = matrix.x.Normalize ();

                // Make 2nd row orthogonal to 1st.
                matrix.y -= matrix.x * Dot (matrix.x, matrix.y);

                // Compute Y scale and normalize 2nd row.
                scale.y = matrix.y.Normalize ();

                // Make 3rd row orthogonal to 1st and 2nd.
                matrix.z -= matrix.x * Dot (matrix.x, matrix.z);
                matrix.z -= matrix.y * Dot (matrix.y, matrix.z);

                // Compute Z scale and normalize 3rd row.
                scale.z = matrix.z.Normalize ();

                // At this point, matrix is orthonormal.
                // Check for a coordinate system flip.  If the determinant
                // is -1, then negate the matrix, and the scaling factors.
                if (Dot (matrix.x, Cross (matrix.y, matrix.z)) < 0.0f) {
                    scale *= -1.0f;
                    matrix *= -1.0f;
                }

                rotation = Quaternion::FromMatrix3 (matrix);

                return true;
            }

            Point3 Matrix3::XformNormal (const Point3 &normal) const {
                return Point3 (
                    normal.x * x.x + normal.y * y.x + normal.z * z.x,
                    normal.x * x.y + normal.y * y.y + normal.z * z.y,
                    normal.x * x.z + normal.y * y.z + normal.z * z.z);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Matrix3 _LIB_THEKOGANS_MEX_BLAS_API operator * (const Matrix3 &m1, const Matrix3 &m2) {
                return Matrix3 (
                    Point3 (
                        m1.x.x * m2.x.x + m1.x.y * m2.y.x + m1.x.z * m2.z.x,
                        m1.x.x * m2.x.y + m1.x.y * m2.y.y + m1.x.z * m2.z.y,
                        m1.x.x * m2.x.z + m1.x.y * m2.y.z + m1.x.z * m2.z.z),
                    Point3 (
                        m1.y.x * m2.x.x + m1.y.y * m2.y.x + m1.y.z * m2.z.x,
                        m1.y.x * m2.x.y + m1.y.y * m2.y.y + m1.y.z * m2.z.y,
                        m1.y.x * m2.x.z + m1.y.y * m2.y.z + m1.y.z * m2.z.z),
                    Point3 (
                        m1.z.x * m2.x.x + m1.z.y * m2.y.x + m1.z.z * m2.z.x,
                        m1.z.x * m2.x.y + m1.z.y * m2.y.y + m1.z.z * m2.z.y,
                        m1.z.x * m2.x.z + m1.z.y * m2.y.z + m1.z.z * m2.z.z),
                    Point3 (
                        m1.t.x * m2.x.x + m1.t.y * m2.y.x + m1.t.z * m2.z.x + m2.t.x,
                        m1.t.x * m2.x.y + m1.t.y * m2.y.y + m1.t.z * m2.z.y + m2.t.y,
                        m1.t.x * m2.x.z + m1.t.y * m2.y.z + m1.t.z * m2.z.z + m2.t.z));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
