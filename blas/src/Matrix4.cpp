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

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Matrix4 Matrix4::Translate (const Point3 &pt) {
                Matrix4 matrix (Matrix4::Identity);
                matrix.t.x = pt.x;
                matrix.t.y = pt.y;
                matrix.t.z = pt.z;
                return matrix;
            }

            Matrix4 Matrix4::RotateX (util::f32 angle) {
                Matrix4 matrix (Matrix4::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.y.y = c;
                matrix.y.z = s;
                matrix.z.y = -s;
                matrix.z.z = c;
                return matrix;
            }

            Matrix4 Matrix4::RotateY (util::f32 angle) {
                Matrix4 matrix (Matrix4::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.x.x = c;
                matrix.x.z = -s;
                matrix.z.x = s;
                matrix.z.z = c;
                return matrix;
            }

            Matrix4 Matrix4::RotateZ (util::f32 angle) {
                Matrix4 matrix (Matrix4::Identity);
                util::f32 c = cosf (angle);
                util::f32 s = sinf (angle);
                matrix.x.x = c;
                matrix.x.y = s;
                matrix.y.x = -s;
                matrix.y.y = c;
                return matrix;
            }

            Matrix4 Matrix4::Scale (const Point3 &pt) {
                Matrix4 matrix (Matrix4::Zero);
                matrix.x.x = pt.x;
                matrix.y.y = pt.y;
                matrix.z.z = pt.z;
                return matrix;
            }

            Matrix4 Matrix4::Skew (util::f32 x, util::f32 y) {
                Matrix4 matrix (Matrix4::Identity);
                matrix.y.x = x;
                matrix.x.y = y;
                return matrix;
            }

            Matrix4 Matrix4::Shear (const Point3 &pt) {
                Matrix4 matrix (Matrix4::Identity);
                matrix.y.x = pt.x;
                matrix.z.x = pt.y;
                matrix.z.y = pt.z;
                return matrix;
            }

            Matrix4 Matrix4::LookAt (const Point3 &position, const Point3 &target,
                    const Point3 &up, util::f32 roll) {
                Point3 f = Normalize (target - position);
                Point3 s = Normalize (Cross (f, up));
                Point3 u = Normalize (Cross (s, f));
                return
                    Translate (-position) *
                    Matrix4 (
                        Point4 (s.x, u.x, -f.x, 0.0f),
                        Point4 (s.y, u.y, -f.y, 0.0f),
                        Point4 (s.z, u.z, -f.z, 0.0f),
                        Point4::Zero) *
                    RotateZ (roll);
            }

            Matrix4 Matrix4::Orthographic (const blas::Bound3 &frustum) {
                blas::Point3 extents = frustum.Extents ();
                return Matrix4 (
                    blas::Point4 (2.0f / extents.x, 0.0f, 0.0f, 0.0f),
                    blas::Point4 (0.0f, 2.0f / extents.y, 0.0f, 0.0f),
                    blas::Point4 (0.0f, 0.0f, -2.0f / extents.z, 0.0f),
                    blas::Point4 (
                        -(frustum.min.x + frustum.max.x) / extents.x,
                        -(frustum.min.y + frustum.max.y) / extents.y,
                        -(frustum.min.z + frustum.max.z) / extents.z, 1.0f));
            }

            Matrix4 Matrix4::Perspective (util::f32 fovy, util::f32 aspect, util::f32 zNear, util::f32 zFar) {
                fovy *= 0.5f;
                fovy = 1.0f / tanf (fovy);
                return Matrix4 (
                    blas::Point4 (fovy / aspect, 0.0f, 0.0f, 0.0f),
                    blas::Point4 (0.0f, fovy, 0.0f, 0.0f),
                    blas::Point4 (0.0f, 0.0f, -(zNear + zFar) / (zFar - zNear), -1.0f),
                    blas::Point4 (0.0f, 0.0f, -2.0f * zNear * zFar / (zFar - zNear), 0.0f));
            }

            Matrix4 &Matrix4::operator *= (const Matrix4 &matrix) {
                return *this = *this * matrix;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Matrix4 _LIB_THEKOGANS_MEX_BLAS_API operator * (const Matrix4 &m1, const Matrix4 &m2) {
                return Matrix4 (
                    Point4 (
                        m1.x.x * m2.x.x + m1.x.y * m2.y.x + m1.x.z * m2.z.x + m1.x.w * m2.t.x,
                        m1.x.x * m2.x.y + m1.x.y * m2.y.y + m1.x.z * m2.z.y + m1.x.w * m2.t.y,
                        m1.x.x * m2.x.z + m1.x.y * m2.y.z + m1.x.z * m2.z.z + m1.x.w * m2.t.z,
                        m1.x.x * m2.x.w + m1.x.y * m2.y.w + m1.x.z * m2.z.w + m1.x.w * m2.t.w),
                    Point4 (
                        m1.y.x * m2.x.x + m1.y.y * m2.y.x + m1.y.z * m2.z.x + m1.y.w * m2.t.x,
                        m1.y.x * m2.x.y + m1.y.y * m2.y.y + m1.y.z * m2.z.y + m1.y.w * m2.t.y,
                        m1.y.x * m2.x.z + m1.y.y * m2.y.z + m1.y.z * m2.z.z + m1.y.w * m2.t.z,
                        m1.y.x * m2.x.w + m1.y.y * m2.y.w + m1.y.z * m2.z.w + m1.y.w * m2.t.w),
                    Point4 (
                        m1.z.x * m2.x.x + m1.z.y * m2.y.x + m1.z.z * m2.z.x + m1.z.w * m2.t.x,
                        m1.z.x * m2.x.y + m1.z.y * m2.y.y + m1.z.z * m2.z.y + m1.z.w * m2.t.y,
                        m1.z.x * m2.x.z + m1.z.y * m2.y.z + m1.z.z * m2.z.z + m1.z.w * m2.t.z,
                        m1.z.x * m2.x.w + m1.z.y * m2.y.w + m1.z.z * m2.z.w + m1.z.w * m2.t.w),
                    Point4 (
                        m1.t.x * m2.x.x + m1.t.y * m2.y.x + m1.t.z * m2.z.x + m1.t.w * m2.t.x,
                        m1.t.x * m2.x.y + m1.t.y * m2.y.y + m1.t.z * m2.z.y + m1.t.w * m2.t.y,
                        m1.t.x * m2.x.z + m1.t.y * m2.y.z + m1.t.z * m2.z.z + m1.t.w * m2.t.z,
                        m1.t.x * m2.x.w + m1.t.y * m2.y.w + m1.t.z * m2.z.w + m1.t.w * m2.t.w));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
