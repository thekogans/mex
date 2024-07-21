// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/editor/Light.h"
#include "thekogans/mex/3ds/opengl/keyframer/SpotLightNode.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (SpotLightNode)

                SpotLightNode::SpotLightNode (const ext::SpotLightHeadNode &spotLightHeadNode_,
                    const ext::SpotLightTargetNode &spotLightTargetNode_, const io::Project &project_) :
                    spotLightHeadNode (spotLightHeadNode_), spotLightTargetNode (spotLightTargetNode_),
                    project (project_) {
                    if (spotLightHeadNode.spotLightNode.IsHasPath ()) {
                        spotLightHeadNode.GetPath (project.keyframer.segment, headPath, headKeys);
                    }
                    if (spotLightTargetNode.targetNode.IsHasPath ()) {
                        spotLightTargetNode.GetPath (project.keyframer.segment, targetPath, targetKeys);
                    }
                }

                void SpotLightNode::Draw (const View &view) const {
                    if (spotLightHeadNode.spotLightNode.IsHasPath ()) {
                        DrawPath (headPath, headKeys, project.keyframer.segment);
                    }
                    if (spotLightTargetNode.targetNode.IsHasPath ()) {
                        DrawPath (targetPath, targetKeys, project.keyframer.segment);
                    }
                    io::Light::Spot spot (spotLightTargetNode.target, spotLightHeadNode.hotSpot,
                        spotLightHeadNode.fallOff, spotLightHeadNode.roll, spotLightHeadNode.light.spot->cone,
                        spotLightHeadNode.light.spot->shadow, spotLightHeadNode.light.spot->projector);
                    Light (io::Light (spotLightHeadNode.light.name, spotLightHeadNode.position, spotLightHeadNode.light.color,
                        spotLightHeadNode.light.multiplier, spotLightHeadNode.light.attenuation, spotLightHeadNode.light.excludes,
                        spotLightHeadNode.light.on, &spot)).Draw (view);
                    if (spotLightHeadNode.spotLightNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                        Color color (DefaultPalette[WHITE]);
                        DrawIcon (BOX_9x9, spotLightHeadNode.position);
                    }
                    if (spotLightTargetNode.targetNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                        Color color (DefaultPalette[WHITE]);
                        DrawIcon (BOX_3x3, spotLightTargetNode.target);
                    }
                }

                void SpotLightNode::DrawWithColor (const View &view, const thekogans::mex::opengl::ui8Color &color) const {
                    if (spotLightHeadNode.spotLightNode.IsHasPath ()) {
                        DrawPathWithColor (headPath, headKeys, project.keyframer.segment, color);
                    }
                    if (spotLightTargetNode.targetNode.IsHasPath ()) {
                        DrawPathWithColor (targetPath, targetKeys, project.keyframer.segment, color);
                    }
                    io::Light::Spot spot (spotLightTargetNode.target, spotLightHeadNode.hotSpot,
                        spotLightHeadNode.fallOff, spotLightHeadNode.roll, spotLightHeadNode.light.spot->cone,
                        spotLightHeadNode.light.spot->shadow, spotLightHeadNode.light.spot->projector);
                    Light (io::Light (spotLightHeadNode.light.name, spotLightHeadNode.position, spotLightHeadNode.light.color,
                        spotLightHeadNode.light.multiplier, spotLightHeadNode.light.attenuation, spotLightHeadNode.light.excludes,
                        spotLightHeadNode.light.on, &spot)).DrawWithColor (view, color);
                    if (spotLightHeadNode.spotLightNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                        Color _color (color);
                        DrawIcon (BOX_9x9, spotLightHeadNode.position);
                    }
                    if (spotLightTargetNode.targetNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                        Color _color (color);
                        DrawIcon (BOX_3x3, spotLightTargetNode.target);
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
