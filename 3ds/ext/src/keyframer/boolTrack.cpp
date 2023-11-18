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

#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/keyframer/boolTrack.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                boolTrack::boolTrack (
                        const io::boolTrack &track_,
                        const io::Keyframer::Segment &segment) :
                        track (track_) {
                    tweenCache.resize (segment.length + 1);
                    for (util::ui32 frame = 0, count = segment.length + 1; frame < count; ++frame) {
                        tweenCache[frame] = GetTweenHelper (frame);
                    }
                }

                bool boolTrack::GetTweenHelper (util::ui32 frame) const {
                    if (track.keys.empty ()) {
                        return true;
                    }
                    if (track.IsRepeats ()) {
                        frame = track.keys[0].frame + frame % (track.keys.back ().frame - track.keys[0].frame);
                    }
                    if (frame < track.keys[0].frame) {
                        if (!track.IsLoops ()) {
                            return true;
                        }
                        frame = track.keys.back ().frame - track.keys[0].frame + frame;
                    }
                    else if (frame > track.keys.back ().frame) {
                        if (!track.IsLoops ()) {
                            return !(track.keys.size () & 1);
                        }
                        frame = track.keys[0].frame + frame % track.keys.back ().frame;
                    }
                    std::size_t kidx = 0;
                    while (kidx < track.keys.size () && track.keys[kidx].frame < frame) {
                        ++kidx;
                    }
                    if (kidx == track.keys.size () || (kidx && frame != track.keys[kidx].frame)) {
                        --kidx;
                    }
                    return kidx & 1;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
