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

#if !defined (__thekogans_mex_3ds_ext_keyframer_Keyframer_h)
#define __thekogans_mex_3ds_ext_keyframer_Keyframer_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/Module.h"
#include "thekogans/mex/3ds/ext/keyframer/Node.h"
#include "thekogans/mex/3ds/ext/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/ext/keyframer/LightNode.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightTargetNode.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightHeadNode.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraTargetNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraHeadNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraNode.h"
#include "thekogans/mex/3ds/ext/keyframer/AmbientNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct Project;

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Keyframer : public Module {
                    // Flattened view.
                    util::OwnerVector<ObjectNode> objectNodes;
                    util::OwnerVector<LightNode> lightNodes;
                    util::OwnerVector<SpotLightTargetNode> spotLightTargetNodes;
                    util::OwnerVector<SpotLightHeadNode> spotLightHeadNodes;
                    util::OwnerVector<CameraTargetNode> cameraTargetNodes;
                    util::OwnerVector<CameraHeadNode> cameraHeadNodes;

                    // Hierarchical view.
                    std::vector<ObjectNode *> objectNodeRoots;
                    std::vector<LightNode *> lightNodeRoots;
                    std::vector<SpotLightTargetNode *> spotLightTargetNodeRoots;
                    std::vector<SpotLightHeadNode *> spotLightHeadNodeRoots;
                    std::vector<CameraTargetNode *> cameraTargetNodeRoots;
                    std::vector<CameraHeadNode *> cameraHeadNodeRoots;

                    AmbientNode::UniquePtr ambientNode;

                    explicit Keyframer (const io::Keyframer &keyframer);
                    explicit Keyframer (Project &project) :
                        Module (project) {}

                    virtual View::UniquePtr GetView (const io::View &view) const;
                    virtual View::UniquePtr GetCurrView () const;
                    virtual blas::Bound2 GetViewBound (const View &view) const;

                    // *** IMPORTANT ***
                    // SetCurrentFrame must be the first API called after ctor.
                    // *****************
                    void SetCurrentFrame ();
                    void Snapshot (io::Editor &editor) const;

                    void Swap (Keyframer &keyframer);

                private:
                    Keyframer (const Keyframer &);
                    Keyframer &operator = (const Keyframer &);

                    SpotLightNode::UniquePtr GetSpotLightNode (const std::string &name) const;
                    CameraNode::UniquePtr GetCameraNode (const std::string &name) const;
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_Keyframer_h)
