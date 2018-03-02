// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_keyframer_ObjectNode_h)
#define __thekogans_mex_3ds_io_keyframer_ObjectNode_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/io/keyframer/Track.h"
#include "thekogans/mex/3ds/io/keyframer/Node.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL ObjectNode : public Node {
                    typedef std::unique_ptr<ObjectNode> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (ObjectNode)

                    static const char * const DUMMY_NAME;

                    blas::Point3 pivot;
                    std::string instanceName;
                    blas::Bound3 bound;
                    Point3Track positionTrack;
                    AngleAxisTrack rotationTrack;
                    Point3Track scaleTrack;
                    stringTrack morphTrack;
                    boolTrack hideTrack;
                    util::f32 morphSmooth;
                    std::vector<util::ui8> xdata;

                    ObjectNode () :
                        pivot (blas::Point3::Zero),
                        bound (blas::Bound3::Empty),
                        morphSmooth (24.0f) {}
                    ObjectNode (
                        util::ui16 id,
                        const Mesh &mesh);

                    virtual std::string GetName () const {
                        return !instanceName.empty () ? name + "." + instanceName : name;
                    }

                    // FIXME: move to ext?
                    virtual bool IsKeyFrame (util::ui32 frame) const {
                        return
                            positionTrack.IsKeyFrame (frame) ||
                            rotationTrack.IsKeyFrame (frame) ||
                            scaleTrack.IsKeyFrame (frame) ||
                            morphTrack.IsKeyFrame (frame) ||
                            hideTrack.IsKeyFrame (frame);
                    }
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_keyframer_ObjectNode_h)
