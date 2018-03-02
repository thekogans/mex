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

#if !defined (__thekogans_mex_3ds_ext_keyframer_Point3Key_h)
#define __thekogans_mex_3ds_ext_keyframer_Point3Key_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/keyframer/Track.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct Point3Track;

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Point3Key : public io::Point3Track::Key {
                    blas::Point3 ds;    // Incoming tangent.
                    blas::Point3 dd;    // Outgoing tangent.

                    explicit Point3Key (const io::Point3Track::Key &key) :
                        io::Point3Track::Key (key), ds (blas::Point3::Zero), dd (blas::Point3::Zero) {}

                private:
                    void CompDeriv (const Point3Key &prev, const Point3Key &next);
                    void CompDerivFirst (const Point3Key &next);
                    void CompDerivLast (const Point3Key &prev);
                    void CompDerivFirstLoop (const Point3Key &prev, const Point3Key &next, util::f32 segmentLength);
                    void CompDerivLastLoop (const Point3Key &prev, const Point3Key &next, util::f32 segmentLength);
                    void CompDerivTwo (Point3Key &next);

                    friend struct Point3Track;
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_Point3Key_h)
