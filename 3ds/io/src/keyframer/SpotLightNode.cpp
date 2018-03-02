// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/io/keyframer/SpotLightNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (SpotLightNode)

                SpotLightNode::SpotLightNode (util::ui16 id, const Light &light) : Node (id, light.name) {
                    assert (light.spot.get ());
                    positionTrack.keys.push_back (Point3Track::Key (0, light.position));
                    colorTrack.keys.push_back (
                        Point3Track::Key (0, blas::Point3 (light.color.r, light.color.g, light.color.b)));
                    hotSpotTrack.keys.push_back (f32Track::Key (0, light.spot->hotSpot));
                    fallOffTrack.keys.push_back (f32Track::Key (0, light.spot->fallOff));
                    rollTrack.keys.push_back (f32Track::Key (0, light.spot->roll));
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
