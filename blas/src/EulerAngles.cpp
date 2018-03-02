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

#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/blas/EulerAngles.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            EulerAngles EulerAngles::FromMatrix3 (const Matrix3 &matrix) {
                util::f32 sinY = matrix.x.z;
                if (sinY < -1.0f) {
                    sinY = -1.0f;
                }
                else if (sinY > 1.0f) {
                    sinY = 1.0f;
                }
                util::f32 cosY = sqrtf (1.0f - sinY * sinY);
                if (matrix.x.x < 0.0f && matrix.z.z < 0.0f) {
                    cosY = -cosY;
                }
                return fabsf (cosY) > util::EPSILON ?
                    EulerAngles (Point3 (
                        atan2f (matrix.y.z / cosY, matrix.z.z / cosY),
                        -atan2f (sinY, cosY),
                        atan2f (matrix.x.y / cosY, matrix.x.x / cosY))) :
                    EulerAngles (Point3 (
                        atan2f (-matrix.z.y, matrix.y.y),
                        -asinf (sinY),
                        0.0f));
            }

            EulerAngles EulerAngles::FromQuaternion (const Quaternion &quat) {
                // FIXME: Find a direct route from Quaternion -> EulerAngles.
                return EulerAngles::FromMatrix3 (Matrix3::FromQuaternion (quat));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
