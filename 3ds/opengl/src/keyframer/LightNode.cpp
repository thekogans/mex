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
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/keyframer/LightNode.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (LightNode)

                LightNode::LightNode (const ext::LightNode &lightNode_,
                    const io::Project &project_) : lightNode (lightNode_), project (project_) {
                    if (lightNode.lightNode.IsHasPath ()) {
                        lightNode.GetPath (project.keyframer.segment, path, keys);
                    }
                }

                void LightNode::Draw (const View &) const {
                    if (lightNode.lightNode.IsHasPath ()) {
                        DrawPath (path, keys, project.keyframer.segment);
                    }
                    Color color (DefaultPalette[YELLOW]);
                    DrawIcon (LIGHT_HEAD, lightNode.position);
                    if (lightNode.lightNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                        Color color (DefaultPalette[WHITE]);
                        DrawIcon (BOX_9x9, lightNode.position);
                    }
                }

                void LightNode::DrawWithColor (const View &view, const thekogans::mex::opengl::ui8Color &color) const {
                    if (lightNode.lightNode.IsHasPath ()) {
                        DrawPathWithColor (path, keys, project.keyframer.segment, color);
                    }
                    Color _color (color);
                    DrawIcon (LIGHT_HEAD, lightNode.position);
                    if (lightNode.lightNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                        DrawIcon (BOX_9x9, lightNode.position);
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
