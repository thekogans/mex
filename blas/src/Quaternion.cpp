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
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Quaternion Quaternion::FromAngleAxis (const AngleAxis &angleAxis) {
                util::f32 length = angleAxis.axis.Length ();
                if (!util::IS_ZERO (length)) {
                    util::f32 ha = angleAxis.angle * 0.5f;
                    util::f32 st = sinf (ha);
                    length = 1.0f / length;
                    return Quaternion (
                        st * angleAxis.axis.x * length,
                        st * angleAxis.axis.y * length,
                        st * angleAxis.axis.z * length,
                        cosf (ha));
                }
                return Quaternion (0.0f, 0.0f, 0.0f, 1.0f);
            }

            Quaternion Quaternion::FromMatrix3 (const Matrix3 &matrix) {
                Quaternion quat;
                util::f32 trace = matrix.x.x + matrix.y.y + matrix.z.z;
                if (trace > 0.0f) {
                    util::f32 s = sqrtf (trace + 1.0f);
                    quat.w = s * 0.5f;
                    s = 0.5f / s;
                    quat.x = (matrix.z.y - matrix.y.z) * s;
                    quat.y = (matrix.x.z - matrix.z.x) * s;
                    quat.z = (matrix.y.x - matrix.x.y) * s;
                }
                else {
                    util::f32 matrix3x3[3][3];
                    matrix3x3[0][0] = matrix.x.x;
                    matrix3x3[0][1] = matrix.x.y;
                    matrix3x3[0][2] = matrix.x.z;
                    matrix3x3[1][0] = matrix.y.x;
                    matrix3x3[1][1] = matrix.y.y;
                    matrix3x3[1][2] = matrix.y.z;
                    matrix3x3[2][0] = matrix.z.x;
                    matrix3x3[2][1] = matrix.z.y;
                    matrix3x3[2][2] = matrix.z.z;

                    util::i32 i = 0;
                    if (matrix3x3[1][1] > matrix3x3[0][0]) {
                        i = 1;
                    }
                    if (matrix3x3[2][2] > matrix3x3[i][i]) {
                        i = 2;
                    }

                    static const util::i32 next[3] = {1, 2, 0};
                    util::i32 j = next[i];
                    util::i32 k = next[j];

                    util::f32 s = sqrtf (matrix3x3[i][i] - matrix3x3[j][j] - matrix3x3[k][k] + 1.0f);
                    util::f32 xyzw[4];
                    xyzw[i] = s * 0.5f;
                    s = 0.5f / s;

                    xyzw[3] = (matrix3x3[k][j] - matrix3x3[j][k]) * s;
                    xyzw[j] = (matrix3x3[j][i] + matrix3x3[i][j]) * s;
                    xyzw[k] = (matrix3x3[k][i] + matrix3x3[i][k]) * s;

                    quat.x = xyzw[0];
                    quat.y = xyzw[1];
                    quat.z = xyzw[2];
                    quat.w = xyzw[3];
                }
                return quat;
            }

            Quaternion &Quaternion::operator *= (const Quaternion &quat) {
                util::f32 x_ = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
                util::f32 y_ = w * quat.y + y * quat.w + z * quat.x - x * quat.z;
                util::f32 z_ = w * quat.z + z * quat.w + x * quat.y - y * quat.x;
                util::f32 w_ = w * quat.w - x * quat.x - y * quat.y - z * quat.z;

                x = x_;
                y = y_;
                z = z_;
                w = w_;

                return *this;
            }

            // Exponentiate quaternion, assuming scalar part 0.
            Quaternion Quaternion::Exp () const {
                util::f32 angle = sqrtf (x * x + y * y + z * z);
                util::f32 scale = util::IS_ZERO (angle) ? 1.0f : sinf (angle) / angle;
                return Quaternion (scale * x, scale * y, scale * z, cosf (angle));
            }

            // Take the natural logarithm of unit quaternion.
            Quaternion Quaternion::Log () const {
                util::f32 scale = sqrtf (x * x + y * y + z * z);
                util::f32 angle = atan2f (scale, w);
                if (!util::IS_ZERO (scale)) {
                    scale = angle / scale;
                }
                return Quaternion (scale * x, scale * y, scale * z, 0.0f);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Quaternion _LIB_THEKOGANS_MEX_BLAS_API Slerp (util::f32 t, const Quaternion &quat1, const Quaternion &quat2) {
                util::f32 cosom = quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;

                if ((1.0f + cosom) > util::EPSILON) {
                    util::f32 sclp;
                    util::f32 sclq;
                    if ((1.0f - cosom) > util::EPSILON) {
                        // usual case
                        util::f32 omega = acosf (cosom);
                        util::f32 sinom = sinf (omega);
                        sclp = sinf ((1.0f - t) * omega) / sinom;
                        sclq = sinf (t * omega) / sinom;
                    }
                    else {
                        // ends very close
                        sclp = 1.0f - t;
                        sclq = t;
                    }
                    return Quaternion (
                        sclp * quat1.x + sclq * quat2.x,
                        sclp * quat1.y + sclq * quat2.y,
                        sclp * quat1.z + sclq * quat2.z,
                        sclp * quat1.w + sclq * quat2.w);
                }

                // ends nearly opposite
                util::f32 sclp = sinf ((1.0f - t) * util::HALFPI);
                util::f32 sclq = sinf (t * util::HALFPI);

                return Quaternion (
                    sclp * quat1.x + sclq * -quat1.y,
                    sclp * quat1.y + sclq * quat1.x,
                    sclp * quat1.z + sclq -quat1.w,
                    quat1.z);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Quaternion _LIB_THEKOGANS_MEX_BLAS_API SquadRev (
                util::f32 t,                // parameter, in range [0.0, 1.0]
                util::f32 angle,            // angle of rotation
                const Point3 &axis,         // the axis of rotation
                const Quaternion &quat1,    // start quaternion
                const Quaternion &t1,       // start tangent quaternion
                const Quaternion &t2,       // end tangent quaternion
                const Quaternion &quat2) {  // end quaternion
                util::f32 omega = angle * 0.5f;
                if (omega < util::PI - util::EPSILON) {
                    return Squad (t, quat1, t1, t2, quat2);
                }
                util::f32 nrevs = 0.0f;
                while (omega > util::PI - util::EPSILON) {
                    omega -= util::PI;
                    nrevs += 1.0f;
                }
                if (omega < 0.0f) {
                    omega = 0.0f;
                }
                // in first 90 degrees
                util::f32 s = t * angle / util::PI;       // 2 * t * (omega + Npi) / pi
                if (s < 1.0f) {
                    Quaternion pp = quat1 * Quaternion (axis.x, axis.y, axis.z, 0.0f);
                    return Squad (s, quat1, t1, pp, pp);
                }
                // middle part, on great circle (quat1, q)
                util::f32 v = s + 1.0f - 2.0f * (nrevs + (omega / util::PI));
                if (v <= 0.0f) {
                    while (s >= 2.0f) {
                        s -= 2.0f;
                    }
                    return Slerp (s, quat1, quat1 * Quaternion (axis.x, axis.y, axis.z, 0.0f));
                }
                // in last 90 degrees
                Quaternion qq = -quat2 * Quaternion (axis.x, axis.y, axis.z, 0.0f);
                return Squad (v, qq, qq, t2, quat2);
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
