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

#if !defined (__thekogans_mex_3ds_ext_keyframer_f32Key_h)
#define __thekogans_mex_3ds_ext_keyframer_f32Key_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/keyframer/Track.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct f32Track;

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL f32Key : public io::f32Track::Key {
                    util::f32 ds;   // Incoming tangent.
                    util::f32 dd;   // Outgoing tangent.

                    explicit f32Key (const io::f32Track::Key &key) :
                        io::f32Track::Key (key), ds (0.0f), dd (0.0f) {}

                private:
                    void CompDeriv (const f32Key &prev, const f32Key &next);
                    void CompDerivFirst (const f32Key &next);
                    void CompDerivLast (const f32Key &prev);
                    void CompDerivFirstLoop (const f32Key &prev, const f32Key &next, util::f32 segmentLength);
                    void CompDerivLastLoop (const f32Key &prev, const f32Key &next, util::f32 segmentLength);
                    void CompDerivTwo (f32Key &next);

                    friend struct f32Track;
                };

            } // namespace ext
        } // namespace _ext_keyframer
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_f32Key_h)
