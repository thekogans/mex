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

#include <cmath>
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/keyframer/Util.h"
#include "thekogans/mex/3ds/ext/keyframer/Point3Track.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                Point3Track::Point3Track (const io::Point3Track &track_, const io::Keyframer::Segment &segment) : track (track_) {
                    {
                        // vectorize
                        for (std::size_t i = 0, count = track.keys.size (); i < count; ++i) {
                            keys.push_back (Point3Key (track.keys[i]));
                        }
                    }
                    if (keys.size () > 2) {
                        std::size_t i, count;
                        for (i = 1, count = keys.size () - 1; i < count; ++i) {
                            keys[i].CompDeriv (keys[i - 1], keys[i + 1]);
                        }
                        if (track.IsLoops ()) {
                            keys[0].CompDerivFirstLoop (keys[i - 1], keys[1], (util::f32)segment.length);
                            keys[i].CompDerivLastLoop (keys[i - 1], keys[1], (util::f32)segment.length);
                        }
                        else {
                            keys[0].CompDerivFirst (keys[1]);
                            keys[i].CompDerivLast (keys[i - 1]);
                        }
                    }
                    else if (keys.size () == 2) {
                        keys[0].CompDerivTwo (keys[1]);
                    }
                    {
                        tweenCache.resize (segment.length + 1);
                        for (util::ui32 frame = 0, count = segment.length + 1; frame < count; ++frame) {
                            tweenCache[frame] = GetTweenHelper (frame);
                        }
                    }
                }

                blas::Point3 Point3Track::GetTweenHelper (util::ui32 frame) const {
                    if (keys.empty ()) {
                        return blas::Point3::Zero;
                    }
                    if (track.IsRepeats ()) {
                        frame = keys[0].frame + frame % (keys.back ().frame - keys[0].frame);
                    }
                    if (frame < keys[0].frame) {
                        if (!track.IsLoops ()) {
                            return keys[0].value;
                        }
                        frame = keys.back ().frame - keys[0].frame + frame;
                    }
                    else if (frame > keys.back ().frame) {
                        if (!track.IsLoops ()) {
                            return keys.back ().value;
                        }
                        frame = keys[0].frame + frame % keys.back ().frame;
                    }
                    std::size_t kidx = 0;
                    while (kidx < keys.size () && keys[kidx].frame < frame) {
                        ++kidx;
                    }
                    if (kidx == keys.size () || (kidx && frame != keys[kidx].frame)) {
                        --kidx;
                    }
                    if (frame == keys[kidx].frame) {
                        return keys[kidx].value;
                    }
                    util::f32 t = Ease (static_cast<util::f32> (frame - keys[kidx].frame) / (keys[kidx + 1].frame - keys[kidx].frame),
                        keys[kidx].easeFrom, keys[kidx + 1].easeTo);
                    return GetHermiteSplinePoint (t, keys[kidx].value, keys[kidx + 1].value, keys[kidx].dd, keys[kidx + 1].ds);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
