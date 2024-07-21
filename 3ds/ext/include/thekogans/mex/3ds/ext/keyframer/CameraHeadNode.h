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

#if !defined (__thekogans_mex_3ds_ext_keyframer_CameraHeadNode_h)
#define __thekogans_mex_3ds_ext_keyframer_CameraHeadNode_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/io/editor/Camera.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/CameraNode.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/keyframer/Point3Track.h"
#include "thekogans/mex/3ds/ext/keyframer/f32Track.h"
#include "thekogans/mex/3ds/ext/keyframer/Node.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL CameraHeadNode : public Node {
                    typedef std::unique_ptr<CameraHeadNode> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    const io::Camera &camera;
                    const io::CameraNode &cameraNode;

                    const Point3Track positionTrack;
                    const f32Track fovTrack;
                    const f32Track rollTrack;

                    // Current frame cache (updated in SetCurrentFrame).
                    blas::Point3 position;
                    util::f32 fov;
                    util::f32 roll;

                    CameraHeadNode (const io::Camera &camera_, const io::CameraNode &cameraNode_,
                        const io::Keyframer::Segment &segment);

                    virtual blas::Bound2 GetBound2 (const blas::Matrix3 &xform) const;

                protected:
                    virtual void SetCurrentFrame (util::ui32 frame, const blas::Matrix3 &parentXform);
                    friend struct Keyframer;

                    virtual blas::Matrix3 GetFrameXform (util::ui32 frame, const blas::Matrix3 &parentXform) const {
                        return blas::Matrix3::Translate (positionTrack.GetTween (frame)) * parentXform;
                    }
                    virtual const Point3Track &GetPositionTrack () const {return positionTrack;}
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_keyframer_CameraHeadNode_h)
