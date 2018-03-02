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

#if !defined (__thekogans_mex_3ds_ext_keyframer_QaternionKey_h)
#define __thekogans_mex_3ds_ext_keyframer_QaternionKey_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/3ds/io/keyframer/Track.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL QuaternionKey : public io::AngleAxisTrack::Key {
                    blas::Quaternion quat;
                    blas::Quaternion ds;    // Incoming tangent.
                    blas::Quaternion dd;    // Outgoing tangent.

                    explicit QuaternionKey (const io::AngleAxisTrack::Key &key) :
                        io::AngleAxisTrack::Key (key),
                        quat (blas::Quaternion::FromAngleAxis (key.value)),
                        ds (blas::Quaternion::Identity),
                        dd (blas::Quaternion::Identity) {}
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_QaternionKey_h)
