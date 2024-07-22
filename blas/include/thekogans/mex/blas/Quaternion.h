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

#if !defined (__thekogans_mex_blas_Quaternion_h)
#define __thekogans_mex_blas_Quaternion_h

#include <cmath>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct AngleAxis;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Quaternion {
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

                Quaternion () {}
                Quaternion (
                    util::f32 x_,
                    util::f32 y_,
                    util::f32 z_,
                    util::f32 w_) :
                    x (x_),
                    y (y_),
                    z (z_),
                    w (w_) {}
                Quaternion (util::f32 xyzw[]) :
                    x (xyzw[0]),
                    y (xyzw[1]),
                    z (xyzw[2]),
                    w (xyzw[3]) {}

                static Quaternion FromAngleAxis (const AngleAxis &angleAxis);
                static Quaternion FromMatrix3 (const Matrix3 &matrix);

                static const Quaternion Zero;
                static const Quaternion Identity;

                inline util::f32 operator [] (util::ui32 index) const {
                    assert (index < 4);
                    return xyzw[index];
                }
                inline util::f32 &operator [] (util::ui32 index) {
                    assert (index < 4);
                    return xyzw[index];
                }

                inline Point3 Axis () const {
                    util::f32 st = sinf (acosf (z));
                    return util::IS_ZERO (st) ? Point3::Zero : Point3 (x / st, y / st, z / st);
                }
                inline util::f32 Angle () const {
                    return acosf (z) * 2.0f;
                }

                // Description:
                //      Concatenate quaternion.
                Quaternion &operator *= (const Quaternion &quat);

                inline Quaternion Normalize () const {
                    util::f32 len = 1.0f / sqrtf (x * x + y * y + z * z + w * w);
                    return Quaternion (x * len, y * len, z * len, w * len);
                }

                inline Quaternion Conjugate () const {
                    return Quaternion (-x, -y, -z, w);
                }

                inline Quaternion Invert () const {
                    util::f32 len = 1.0f / sqrtf (x * x + y * y + z * z + w * w);
                    return Quaternion (-x * len, -y * len, -z * len, w * len);
                }

                // Description:
                //      Returns: quat' = Exp (quat).
                Quaternion Exp () const;

                // Description:
                //      Returns: quat' = Log (quat).
                Quaternion Log () const;
            };

            inline Quaternion operator * (
                    util::f32 s,
                    const Quaternion &quat) {
                return Quaternion (quat.x * s, quat.y * s, quat.x * s, quat.w * s);
            }

            inline Quaternion operator * (
                    const Quaternion &quat,
                    util::f32 s) {
                return Quaternion (quat.x * s, quat.y * s, quat.x * s, quat.w * s);
            }

            inline Quaternion operator * (
                    const Quaternion &quat1,
                    const Quaternion &quat2) {
                return Quaternion (
                    quat1.w * quat2.x + quat1.x * quat2.w + quat1.y * quat2.z - quat1.z * quat2.y,
                    quat1.w * quat2.y + quat1.y * quat2.w + quat1.z * quat2.x - quat1.x * quat2.z,
                    quat1.w * quat2.z + quat1.z * quat2.w + quat1.x * quat2.y - quat1.y * quat2.x,
                    quat1.w * quat2.w - quat1.x * quat2.x - quat1.y * quat2.y - quat1.z * quat2.z);
            }

            inline Point3 operator * (
                    const Quaternion &quat,
                    const Point3 &pt) {
                Quaternion temp = quat * Quaternion (pt.x, pt.y, pt.x, 0.0f) * quat.Invert ();
                return Point3 (temp.x, temp.y, temp.z);
            }

            inline Quaternion operator + (
                    const Quaternion &quat1,
                    const Quaternion &quat2) {
                return Quaternion (
                    quat1.x + quat2.x,
                    quat1.y + quat2.y,
                    quat1.z + quat2.z,
                    quat1.w + quat2.w);
            }

            inline Quaternion operator - (
                    const Quaternion &quat1,
                    const Quaternion &quat2) {
                return Quaternion (
                    quat1.x - quat2.x,
                    quat1.y - quat2.y,
                    quat1.z - quat2.z,
                    quat1.w - quat2.w);
            }

            inline Quaternion operator - (const Quaternion &quat) {
                return Quaternion (-quat.x, -quat.y, -quat.z, -quat.w);
            }

            inline Quaternion operator / (
                    const Quaternion &quat1,
                    const Quaternion &quat2) {
                return quat2.Invert () * quat1;
            }

            inline util::f32 Dot (
                    const Quaternion &quat1,
                    const Quaternion &quat2) {
                return quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;
            }

            // Computes a spherical linear interpolation between two quaternions.
            _LIB_THEKOGANS_MEX_BLAS_DECL Quaternion _LIB_THEKOGANS_MEX_BLAS_API Slerp (
                util::f32 t,                // Parameter, in range [0.0, 1.0].
                const Quaternion &quat1,    // Start quaternion.
                const Quaternion &quat2);   // End quaternion.

            // Given parameter 0.0 < t < 1.0 and quaternions representing four quadrangle points,
            // computes quat, which is a point on a cubic quaternion curve.
            inline Quaternion Squad (
                    util::f32 t,                // Parameter, in range [0.0, 1.0].
                    const Quaternion &quat1,    // Start quaternion.
                    const Quaternion &t1,       // Start tangent quaternion.
                    const Quaternion &t2,       // End tangent quaternion.
                    const Quaternion &quat2) {  // End quaternion.
                return Slerp (2.0f * t * (1.0f - t), Slerp (t, quat1, quat2), Slerp (t, t1, t2));
            }

            // Same as Squad, except uses angle to interpolates rotations > 360 degrees.
            _LIB_THEKOGANS_MEX_BLAS_DECL Quaternion _LIB_THEKOGANS_MEX_BLAS_API SquadRev (
                util::f32 t,                // Parameter, in range [0.0, 1.0].
                util::f32 angle,            // Angle of rotation.
                const Point3 &axis,         // Axis of rotation.
                const Quaternion &quat1,    // Start quaternion.
                const Quaternion &t1,       // Start tangent quaternion.
                const Quaternion &t2,       // End tangent quaternion.
                const Quaternion &quat2);   // End quaternion.

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Quaternion &quat){
                serializer << quat.x << quat.y << quat.z << quat.w;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    Quaternion &quat) {
                serializer >> quat.x >> quat.y >> quat.z >> quat.w;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Quaternion_h)
