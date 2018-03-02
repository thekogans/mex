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

#if !defined (__thekogans_mex_3ds_ext_keyframer_ObjectNode_h)
#define __thekogans_mex_3ds_ext_keyframer_ObjectNode_h

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/io/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/keyframer/Point3Track.h"
#include "thekogans/mex/3ds/ext/keyframer/QuaternionTrack.h"
#include "thekogans/mex/3ds/ext/keyframer/boolTrack.h"
#include "thekogans/mex/3ds/ext/keyframer/MorphTrack.h"
#include "thekogans/mex/3ds/ext/keyframer/Node.h"
#include "thekogans/mex/3ds/ext/keyframer/Util.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL ObjectNode : public Node {
                    typedef std::unique_ptr<ObjectNode> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (ObjectNode)

                    const io::Mesh &mesh;
                    const io::ObjectNode &objectNode;

                    // Track caches used in SetCurrentFrame.
                    const Point3Track positionTrack;
                    const QuaternionTrack rotationTrack;
                    const Point3Track scaleTrack;
                    const boolTrack hideTrack;
                    const MorphTrack morphTrack;

                    // Invariant code optimization.
                    const blas::Matrix3 ixform;
                    const bool isXformLeftHanded;

                    // Current frame cache (updated in SetCurrentFrame).
                    blas::Matrix3 xform;
                    std::vector<blas::Point3> vertices;
                    bool hidden;

                    ObjectNode (
                        const io::Mesh &mesh_,
                        const io::ObjectNode &objectNode_,
                        const util::OwnerVector<io::Mesh> &meshes,
                        const io::Keyframer::Segment &segment);

                    io::Mesh::UniquePtr Snapshot () const;

                    // Return bound in global coordinates.
                    virtual blas::Bound2 GetBound2 (const blas::Matrix3 &xform) const;
                    // Return bound in local coordinates.
                    blas::Bound3 GetBound3 () const;

                protected:
                    virtual void SetCurrentFrame (
                        util::ui32 frame,
                        const blas::Matrix3 &parentXform);
                    friend struct Keyframer;

                    virtual blas::Matrix3 GetFrameXform (
                            util::ui32 frame,
                            const blas::Matrix3 &parentXform) const {
                        return GetPRSFrameXform (positionTrack, rotationTrack,
                            scaleTrack, frame, parentXform);
                    }
                    virtual const Point3Track &GetPositionTrack () const {
                        return positionTrack;
                    }
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_ObjectNode_h)
