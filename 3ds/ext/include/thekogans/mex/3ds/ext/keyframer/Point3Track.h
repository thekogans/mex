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

#if !defined (__thekogans_mex_3ds_ext_keyframer_Point3Track_h)
#define __thekogans_mex_3ds_ext_keyframer_Point3Track_h

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/keyframer/Track.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/keyframer/Point3Key.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Point3Track {
                    const io::Point3Track &track;
                    std::vector<Point3Key> keys;
                    // Running some Shark traces revealed a hot spot in
                    // GetTween. This cache is calculated in the ctor,
                    // and is used to eliminate that logic at run time.
                    std::vector<blas::Point3> tweenCache;

                    Point3Track (const io::Point3Track &track_, const io::Keyframer::Segment &segment);

                    inline blas::Point3 GetTween (util::ui32 frame) const {
                        return frame < tweenCache.size () ? tweenCache[frame] : blas::Point3::Zero;
                    }

                private:
                    blas::Point3 GetTweenHelper (util::ui32 frame) const;
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_Point3Track_h)
