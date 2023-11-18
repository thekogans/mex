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

#include "thekogans/mex/3ds/ext/keyframer/LightNode.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (LightNode)

                LightNode::LightNode (
                    const io::Light &light_,
                    const io::LightNode &lightNode_,
                    const io::Keyframer::Segment &segment) :
                    Node (lightNode_),
                    light (light_),
                    lightNode (lightNode_),
                    positionTrack (lightNode.positionTrack, segment),
                    colorTrack (lightNode.colorTrack, segment) {}

                io::Light::UniquePtr LightNode::Snapshot () const {
                    io::Light::UniquePtr newLight (new io::Light (light));
                    assert (newLight.get () != 0);
                    if (newLight.get () != 0) {
                        newLight->position = position;
                        newLight->color = opengl::f32Color (color.x, color.y, color.z);
                    }
                    return newLight;
                }

                blas::Bound2 LightNode::GetBound2 (const blas::Matrix3 &xform) const {
                    blas::Point2 pt (position * xform);
                    return blas::Bound2 (pt, pt);
                }

                void LightNode::SetCurrentFrame (util::ui32 frame, const blas::Matrix3 &parentXform) {
                    position = positionTrack.GetTween (frame) * parentXform;
                    color = colorTrack.GetTween (frame);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
