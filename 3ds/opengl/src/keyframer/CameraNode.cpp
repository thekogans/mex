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
#include "thekogans/mex/3ds/opengl/editor/Camera.h"
#include "thekogans/mex/3ds/opengl/keyframer/CameraNode.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (CameraNode)

                CameraNode::CameraNode (
                        const ext::CameraHeadNode &cameraHeadNode_,
                        const ext::CameraTargetNode &cameraTargetNode_,
                        const io::Project &project_) :
                        cameraHeadNode (cameraHeadNode_),
                        cameraTargetNode (cameraTargetNode_),
                        project (project_) {
                    if (cameraHeadNode.cameraNode.IsHasPath ()) {
                        cameraHeadNode.GetPath (project.keyframer.segment, headPath, headKeys);
                    }
                    if (cameraTargetNode.targetNode.IsHasPath ()) {
                        cameraTargetNode.GetPath (project.keyframer.segment, targetPath, targetKeys);
                    }
                }

                void CameraNode::Draw (const View &view) const {
                    if (view.view.name.value != cameraHeadNode.camera.name) {
                        if (cameraHeadNode.cameraNode.IsHasPath ()) {
                            DrawPath (headPath, headKeys, project.keyframer.segment);
                        }
                        if (cameraTargetNode.targetNode.IsHasPath ()) {
                            DrawPath (targetPath, targetKeys, project.keyframer.segment);
                        }
                        Camera (io::Camera (cameraHeadNode.camera.name, cameraHeadNode.position, cameraTargetNode.target,
                            cameraHeadNode.roll, io::Camera::FOV2Lens (cameraHeadNode.fov), cameraHeadNode.camera.showCone,
                                cameraHeadNode.camera.ranges)).Draw (view);
                        if (cameraHeadNode.cameraNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                            Color color (DefaultPalette[WHITE]);
                            DrawIcon (BOX_4x4, cameraHeadNode.position);
                        }
                        if (cameraTargetNode.targetNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                            Color color (DefaultPalette[WHITE]);
                            DrawIcon (BOX_3x3, cameraTargetNode.target);
                        }
                    }
                }

                void CameraNode::DrawWithColor (const View &view, const thekogans::mex::opengl::ui8Color &color) const {
                    if (view.view.name.value != cameraHeadNode.camera.name) {
                        if (cameraHeadNode.cameraNode.IsHasPath ()) {
                            DrawPathWithColor (headPath, headKeys, project.keyframer.segment, color);
                        }
                        if (cameraTargetNode.targetNode.IsHasPath ()) {
                            DrawPathWithColor (targetPath, targetKeys, project.keyframer.segment, color);
                        }
                        Camera (io::Camera (cameraHeadNode.camera.name, cameraHeadNode.position, cameraTargetNode.target,
                            cameraHeadNode.roll, io::Camera::FOV2Lens (cameraHeadNode.fov), cameraHeadNode.camera.showCone,
                                cameraHeadNode.camera.ranges)).DrawWithColor (view, color);
                        if (cameraHeadNode.cameraNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                            Color _color (color);
                            DrawIcon (BOX_4x4, cameraHeadNode.position);
                        }
                        if (cameraTargetNode.targetNode.IsKeyFrame (project.keyframer.segment.currFrame)) {
                            Color _color (color);
                            DrawIcon (BOX_3x3, cameraTargetNode.target);
                        }
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
