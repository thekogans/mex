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

#include "thekogans/mex/3ds/ext/keyframer/CameraNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (CameraNode)

                io::Camera::UniquePtr CameraNode::Snapshot () const {
                    io::Camera::UniquePtr newCamera (new io::Camera (cameraHeadNode.camera));
                    assert (newCamera.get () != 0);
                    if (newCamera.get () != 0) {
                        newCamera->position = cameraHeadNode.position;
                        newCamera->target = cameraTargetNode.target;
                        newCamera->lens = io::Camera::FOV2Lens (cameraHeadNode.fov);
                        newCamera->roll = cameraHeadNode.roll;
                    }
                    return newCamera;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
