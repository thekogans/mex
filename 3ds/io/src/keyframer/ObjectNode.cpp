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

#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/3ds/io/keyframer/ObjectNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (ObjectNode)

                const char * const ObjectNode::DUMMY_NAME = "$$$DUMMY";

                ObjectNode::ObjectNode (
                        util::ui16 id,
                        const Mesh &mesh) :
                        Node (id, mesh.name),
                        pivot (blas::Point3::Zero),
                        bound (mesh.GetBound3 ()),
                        morphSmooth (24.0f) {
                    blas::Point3 translation;
                    blas::Quaternion rotation;
                    blas::Point3 scale;
                    mesh.xform.Decompose (translation, rotation, scale);
                    positionTrack.keys.push_back (Point3Track::Key (0, translation));
                    rotationTrack.keys.push_back (
                        AngleAxisTrack::Key (0, blas::AngleAxis::FromQuaternion (rotation)));
                    scaleTrack.keys.push_back (Point3Track::Key (0, scale));
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
