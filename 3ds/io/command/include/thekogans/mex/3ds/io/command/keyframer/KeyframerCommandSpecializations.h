// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_command_keyframer_KeyframerCommandSpecializations_h)
#define __thekogans_mex_3ds_io_command_keyframer_KeyframerCommandSpecializations_h

#include "thekogans/mex/command/OwnerVectorCommands.h"
#include "thekogans/mex/3ds/io/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/io/keyframer/LightNode.h"
#include "thekogans/mex/3ds/io/keyframer/TargetNode.h"
#include "thekogans/mex/3ds/io/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/io/keyframer/CameraNode.h"

namespace thekogans {
    namespace mex {
        namespace command {

            typedef OwnerVectorpush_backCommand<_3ds::io::ObjectNode> OwnerVectorObjectNodepush_backCommand;
            typedef OwnerVectorpush_backCommand<_3ds::io::LightNode> OwnerVectorLightNodepush_backCommand;
            typedef OwnerVectorpush_backCommand<_3ds::io::TargetNode> OwnerVectorTargetNodepush_backCommand;
            typedef OwnerVectorpush_backCommand<_3ds::io::SpotLightNode> OwnerVectorSpotLightNodepush_backCommand;
            typedef OwnerVectorpush_backCommand<_3ds::io::CameraNode> OwnerVectorCameraNodepush_backCommand;

            template<typename T>
            struct CompareNodes {
                inline bool operator () (const T *node1, const T *node2) {
                    return node1->name < node2->name;
                }
            };
            typedef CompareNodes<_3ds::io::TargetNode> CompareTargetNodes;
            typedef OwnerVectorsortCommand<_3ds::io::TargetNode, CompareTargetNodes> OwnerVectorTargetNodesortCommand;
            typedef CompareNodes<_3ds::io::SpotLightNode> CompareSpotLightNodes;
            typedef OwnerVectorsortCommand<_3ds::io::SpotLightNode, CompareSpotLightNodes> OwnerVectorSpotLightNodesortCommand;
            typedef CompareNodes<_3ds::io::CameraNode> CompareCameraNodes;
            typedef OwnerVectorsortCommand<_3ds::io::CameraNode, CompareCameraNodes> OwnerVectorCameraNodesortCommand;

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_keyframer_KeyframerCommandSpecializations_h)
