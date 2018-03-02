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

#if !defined (__thekogans_mex_3ds_io_keyframer_AmbientNode_h)
#define __thekogans_mex_3ds_io_keyframer_AmbientNode_h

#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/keyframer/Track.h"
#include "thekogans/mex/3ds/io/keyframer/Node.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL AmbientNode : public Node {
                    Point3Track colorTrack;

                    static const char * const NAME;

                    AmbientNode () :
                        Node (util::NIDX16, NAME) {}
                    AmbientNode (const blas::Point3 &color) :
                            Node (util::NIDX16, NAME) {
                        colorTrack.keys.push_back (Point3Track::Key (0, color));
                    }

                    inline void Swap (AmbientNode &ambientNode) {
                        Node::Swap (ambientNode);
                        colorTrack.Swap (ambientNode.colorTrack);
                    }

                    virtual bool IsKeyFrame (util::ui32 frame) const {
                        return colorTrack.IsKeyFrame (frame);
                    }
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_keyframer_AmbientNode_h)
