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

#if !defined (__thekogans_mex_3ds_ext_keyframer_NodeVisitor_h)
#define __thekogans_mex_3ds_ext_keyframer_NodeVisitor_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/keyframer/Node.h"
#include "thekogans/mex/3ds/ext/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/ext/keyframer/LightNode.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightTargetNode.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightHeadNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraTargetNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraHeadNode.h"
#include "thekogans/mex/3ds/ext/keyframer/AmbientNode.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                template<
                    typename Node,
                    typename ObjectNode,
                    typename LightNode,
                    typename SpotLightTargetNode,
                    typename SpotLightHeadNode,
                    typename CameraTargetNode,
                    typename CameraHeadNode,
                    typename AmbientNode,
                    typename Keyframer>
                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL NodeVisitorT {
                    virtual ~NodeVisitorT () {}

                    // Return true to visit the branch, false to short circuit the hierarchy.
                    virtual bool StartObjectNode (const ObjectNode &objectNode) {return true;}
                    virtual void EndObjectNode (const ObjectNode &objectNode) {}

                    virtual void VisitLightNode (const LightNode &lightNode) {}
                    virtual void VisitSpotLightTargetNode (const SpotLightTargetNode &spotLightTargetNode) {}
                    virtual void VisitSpotLightHeadNode (const SpotLightHeadNode &spotLightHeadNode) {}

                    virtual void VisitCameraTargetNode (const CameraTargetNode &cameraTargetNode) {}
                    virtual bool StartCameraHeadNode (const CameraHeadNode &cameraHeadNode) {return true;}
                    virtual void EndCameraHeadNode (const CameraHeadNode &cameraHeadNode) {}

                    virtual void VisitAmbientNode (const AmbientNode &ambientNode) {}

                    enum {
                        VISIT_OBJECT_NODES = 0x00000001,
                        VISIT_LIGHT_NODES = 0x00000002,
                        VISIT_SPOT_LIGHT_TARGET_NODES = 0x00000004,
                        VISIT_SPOT_LIGHT_HEAD_NODES = 0x00000008,
                        VISIT_CAMERA_TARGET_NODES = 0x00000010,
                        VISIT_CAMERA_HEAD_NODES = 0x00000020,
                        VISIT_AMBIENT_NODE = 0x00000040,
                        VISIT_ALL_NODES =
                            VISIT_OBJECT_NODES |
                            VISIT_LIGHT_NODES |
                            VISIT_SPOT_LIGHT_TARGET_NODES |
                            VISIT_SPOT_LIGHT_HEAD_NODES |
                            VISIT_CAMERA_TARGET_NODES |
                            VISIT_CAMERA_HEAD_NODES |
                            VISIT_AMBIENT_NODE,
                        OBJECT_NODE_CHILD =
                            VISIT_OBJECT_NODES |
                            VISIT_LIGHT_NODES |
                            VISIT_SPOT_LIGHT_TARGET_NODES |
                            VISIT_SPOT_LIGHT_HEAD_NODES |
                            VISIT_CAMERA_TARGET_NODES |
                            VISIT_CAMERA_HEAD_NODES,
                        CAMERA_HEAD_NODE_CHILD =
                            VISIT_LIGHT_NODES |
                            VISIT_SPOT_LIGHT_HEAD_NODES
                    };

                    virtual void VisitHierarchy (
                            const Keyframer &keyframer,
                            util::ui32 nodeTypes = VISIT_ALL_NODES) {
                        if (nodeTypes & OBJECT_NODE_CHILD) {
                            typedef typename util::OwnerVector<ObjectNode>::const_iterator ObjecNodeIterator;
                            for (ObjecNodeIterator it = keyframer.objectNodes.begin (),
                                end = keyframer.objectNodes.end (); it != end; ++it) {
                                VisitNode (**it);
                            }
                        }
                        if (util::Flags32 (nodeTypes).Test (VISIT_LIGHT_NODES)) {
                            typedef typename util::OwnerVector<LightNode>::const_iterator LightNodeIterator;
                            for (LightNodeIterator it = keyframer.lightNodes.begin (),
                                end = keyframer.lightNodes.end (); it != end; ++it) {
                                VisitNode (**it);
                            }
                        }
                        if (util::Flags32 (nodeTypes).Test (VISIT_SPOT_LIGHT_TARGET_NODES)) {
                            typedef typename util::OwnerVector<SpotLightTargetNode>::const_iterator SpotLightTargetNodeIterator;
                            for (SpotLightTargetNodeIterator it = keyframer.spotLightTargetNodes.begin (),
                                end = keyframer.spotLightTargetNodes.end (); it != end; ++it) {
                                VisitNode (**it);
                            }
                        }
                        if (util::Flags32 (nodeTypes).Test (VISIT_SPOT_LIGHT_HEAD_NODES)) {
                            typedef typename util::OwnerVector<SpotLightHeadNode>::const_iterator SpotLightHeadNodeIterator;
                            for (SpotLightHeadNodeIterator it = keyframer.spotLightHeadNodes.begin (),
                                end = keyframer.spotLightHeadNodes.end (); it != end; ++it) {
                                VisitNode (**it);
                            }
                        }
                        if (util::Flags32 (nodeTypes).Test (VISIT_CAMERA_TARGET_NODES)) {
                            typedef typename util::OwnerVector<CameraTargetNode>::const_iterator CameraTargetNodeIterator;
                            for (CameraTargetNodeIterator it = keyframer.cameraTargetNodes.begin (),
                                end = keyframer.cameraTargetNodes.end (); it != end; ++it) {
                                VisitNode (**it);
                            }
                        }
                        if (nodeTypes & CAMERA_HEAD_NODE_CHILD) {
                            typedef typename util::OwnerVector<CameraHeadNode>::const_iterator CameraHeadNodeIterator;
                            for (CameraHeadNodeIterator it = keyframer.cameraHeadNodes.begin (),
                                end = keyframer.cameraHeadNodes.end (); it != end; ++it) {
                                VisitNode (**it);
                            }
                        }
                        if (util::Flags32 (nodeTypes).Test (VISIT_AMBIENT_NODE)) {
                            VisitAmbientNode (keyframer.ambientNode);
                        }
                    }

                private:
                    inline void VisitNode (const Node &node) {
                        if (dynamic_cast<const ObjectNode *> (&node)) {
                            if (StartObjectNode (static_cast<const ObjectNode &> (node))) {
                                for (typename util::OwnerVector<Node>::const_iterator
                                        it = node.children.begin (),
                                        end = node.children.end (); it != end; ++it) {
                                    VisitNode (**it);
                                }
                            }
                            EndObjectNode (static_cast<const ObjectNode &> (node));
                        }
                        else if (dynamic_cast<const LightNode *> (&node)) {
                            VisitLightNode (static_cast<const LightNode &> (node));
                        }
                        else if (dynamic_cast<const SpotLightTargetNode *> (&node)) {
                            VisitSpotLightTargetNode (static_cast<const SpotLightTargetNode &> (node));
                        }
                        else if (dynamic_cast<const SpotLightHeadNode *> (&node)) {
                            VisitSpotLightHeadNode (static_cast<const SpotLightHeadNode &> (node));
                        }
                        else if (dynamic_cast<const CameraTargetNode *> (&node)) {
                            VisitCameraTargetNode (static_cast<const CameraTargetNode &> (node));
                        }
                        else if (dynamic_cast<const CameraHeadNode *> (&node)) {
                            if (StartCameraHeadNode (static_cast<const CameraHeadNode &> (node))) {
                                for (typename util::OwnerVector<Node>::const_iterator
                                        it = node.children.begin (),
                                        end = node.children.end (); it != end; ++it) {
                                    VisitNode (**it);
                                }
                            }
                            EndCameraHeadNode (static_cast<const CameraHeadNode &> (node));
                        }
                    }
                };

                typedef NodeVisitorT<Node, ObjectNode, LightNode, SpotLightTargetNode, SpotLightHeadNode,
                    CameraTargetNode, CameraHeadNode, AmbientNode, Keyframer> NodeVisitor;

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_NodeVisitor_h)
