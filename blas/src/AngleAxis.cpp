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
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            AngleAxis AngleAxis::FromQuaternion (const Quaternion &quat) {
                util::f32 halfAngle = acosf (quat.w);
                util::f32 sinHalfAngle = sinf (halfAngle);
                util::f32 angle = 0.0f;
                Point3 axis (Point3::Zero);
                if (!util::IS_ZERO (sinHalfAngle)) {
                    angle = 2.0f * halfAngle;
                    util::f32 oneOverSinHalfAngle = 1.0f / sinHalfAngle;
                    axis.x = quat.x * oneOverSinHalfAngle;
                    axis.y = quat.y * oneOverSinHalfAngle;
                    axis.z = quat.z * oneOverSinHalfAngle;
                }
                return AngleAxis (angle, axis);
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
