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

#if !defined (__thekogans_mex_3ds_ext_keyframer_Util_h)
#define __thekogans_mex_3ds_ext_keyframer_Util_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/keyframer/Point3Track.h"
#include "thekogans/mex/3ds/ext/keyframer/QuaternionTrack.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                template <typename T>
                inline T GetHermiteSplinePoint (util::f32 t, const T &p1, const T &p2, const T &d1, const T &d2) {
                    util::f32 t2 = t * t;
                    util::f32 t3 = t2 * t;
                    return
                        (2.0f * t3 - 3.0f * t2 + 1.0f) * p1 +
                        (-2.0f * t3 + 3.0f * t2) * p2 +
                        (t3 - 2.0f * t2 + t) * d1 +
                        (t3 - t2) * d2;
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL util::f32 _LIB_THEKOGANS_MEX_3DS_EXT_API Ease (
                    util::f32 u,
                    util::f32 a,
                    util::f32 b);

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL blas::Matrix3 _LIB_THEKOGANS_MEX_3DS_EXT_API GetPRSFrameXform (
                    const Point3Track &positionTrack,
                    const QuaternionTrack &rotationTrack,
                    const Point3Track &scaleTrack,
                    util::ui32 frame,
                    const blas::Matrix3 &parentXform);

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_Util_h)
