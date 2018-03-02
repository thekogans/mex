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

#if !defined (__thekogans_mex_3ds_ext_keyframer_Node_h)
#define __thekogans_mex_3ds_ext_keyframer_Node_h

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/keyframer/Node.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/keyframer/Point3Track.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct Keyframer;

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Node {
                    const io::Node &node;
                    Node *parent;
                    std::vector<Node *> children;

                    explicit Node (const io::Node &node_) : node (node_), parent (0) {}
                    virtual ~Node () {}

                    void AddChild (Node *child);
                    bool RemoveChild (Node *child);

                    blas::Matrix3 GetHierarchyXform (util::ui32 frame) const;
                    void GetPath (const io::Keyframer::Segment &segment,
                        std::vector<blas::Point3> &path, std::vector<util::ui32> &keys) const;

                    virtual blas::Bound2 GetBound2 (const blas::Matrix3 &xform) const = 0;

                protected:
                    virtual void SetCurrentFrame (util::ui32 frame, const blas::Matrix3 &parentXform) = 0;
                    friend struct Keyframer;
                    friend struct ObjectNode;
                    friend struct CameraHeadNode;

                    // Helper used in GetHierarchyXform.
                    virtual blas::Matrix3 GetFrameXform (util::ui32 frame, const blas::Matrix3 &parentXform) const = 0;
                    // Helper used in GetPath.
                    virtual const Point3Track &GetPositionTrack () const = 0;
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_Node_h)
