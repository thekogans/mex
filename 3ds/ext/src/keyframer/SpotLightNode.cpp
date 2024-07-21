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

#include "thekogans/mex/3ds/ext/keyframer/SpotLightNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (SpotLightNode)

                io::Light::UniquePtr SpotLightNode::Snapshot () const {
                    io::Light::UniquePtr newLight (new io::Light (spotLightHeadNode.light));
                    assert (newLight.get () != 0);
                    assert (newLight->spot.get () != 0);
                    if (newLight.get () != 0 && newLight->spot.get () != 0) {
                        newLight->position = spotLightHeadNode.position;
                        newLight->color = opengl::f32Color (spotLightHeadNode.color.x,
                            spotLightHeadNode.color.y, spotLightHeadNode.color.z);
                        newLight->spot->target = spotLightTargetNode.target;
                        newLight->spot->hotSpot = spotLightHeadNode.hotSpot;
                        newLight->spot->fallOff = spotLightHeadNode.fallOff;
                        newLight->spot->roll = spotLightHeadNode.roll;
                    }
                    return newLight;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
