// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/blas/EulerAngles.h"
#include "thekogans/mex/3ds/ext/keyframer/Util.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL util::f32 _LIB_THEKOGANS_MEX_3DS_EXT_API Ease (
                        util::f32 u, util::f32 a, util::f32 b) {
                    util::f32 s = a + b;
                    if (u == 0.0f || u == 1.0f || s == 0.0f) {
                        return u;
                    }
                    if (s > 1.0f) {
                        s = 1.0f / s;
                        a *= s;
                        b *= s;
                    }
                    util::f32 k = 1.0f / (2.0f - a - b);
                    if (u < a) {
                        return (k / a) * u * u;
                    }
                    if (u < 1.0f - b) {
                        return k * (2.0f * u - a);
                    }
                    u = 1.0f - u;
                    return 1.0f - (k / b) * u * u;
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL blas::Matrix3 _LIB_THEKOGANS_MEX_3DS_EXT_API GetPRSFrameXform (
                        const Point3Track &positionTrack, const QuaternionTrack &rotationTrack,
                        const Point3Track &scaleTrack, util::ui32 frame, const blas::Matrix3 &parentXform) {
                    blas::Matrix3 xform =
                        blas::Matrix3::Scale (scaleTrack.GetTween (frame)) *
                        blas::Matrix3::FromQuaternion (rotationTrack.GetTween (frame)) *
                        blas::Matrix3::Translate (positionTrack.GetTween (frame));
                    if (rotationTrack.track.IsNoLink () || scaleTrack.track.IsNoLink ()) {
                        blas::Point3 translation;
                        blas::Quaternion rotation;
                        blas::Point3 scale;
                        parentXform.Decompose (translation, rotation, scale);
                        if (rotationTrack.track.IsNoLink ()) {
                            blas::EulerAngles eulerAngles (blas::EulerAngles::FromQuaternion (rotation));
                            if (rotationTrack.track.IsNoLinkX ()) {
                                eulerAngles.angles.x = 0.0f;
                            }
                            if (rotationTrack.track.IsNoLinkY ()) {
                                eulerAngles.angles.y = 0.0f;
                            }
                            if (rotationTrack.track.IsNoLinkZ ()) {
                                eulerAngles.angles.z = 0.0f;
                            }
                            rotation = blas::Quaternion::FromMatrix3 (
                                blas::Matrix3::FromEulerAngles (eulerAngles));
                        }
                        if (scaleTrack.track.IsNoLinkX ()) {
                            scale.x = 1.0f;
                        }
                        if (scaleTrack.track.IsNoLinkY ()) {
                            scale.y = 1.0f;
                        }
                        if (scaleTrack.track.IsNoLinkZ ()) {
                            scale.z = 1.0f;
                        }
                        // Put in whatever scale and rotation are left.
                        blas::Matrix3 matrix = blas::Matrix3::Scale (scale) *
                            blas::Matrix3::FromQuaternion (rotation);
                        // Find where the position would have been...
                        blas::Point3 pdelta = positionTrack.GetTween (frame);
                        // Set the position to the position we would have been at if
                        // we inherited everything and then subtract the delta so
                        // when the position controller adds it in it will cancel.
                        matrix.t = pdelta * parentXform;
                        matrix.t = -pdelta * matrix;
                        xform *= matrix;
                    }
                    else {
                        xform *= parentXform;
                    }
                    return xform;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
