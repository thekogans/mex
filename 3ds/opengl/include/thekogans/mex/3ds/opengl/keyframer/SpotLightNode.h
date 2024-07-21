// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_keyframer_SpotLightNode_h)
#define __thekogans_mex_3ds_opengl_keyframer_SpotLightNode_h

#include <memory>
#include "thekogans/util/Heap.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightHeadNode.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightTargetNode.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL SpotLightNode {
                    typedef std::unique_ptr<SpotLightNode> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    const ext::SpotLightHeadNode &spotLightHeadNode;
                    const ext::SpotLightTargetNode &spotLightTargetNode;
                    const io::Project &project;

                    std::vector<blas::Point3> headPath;
                    std::vector<util::ui32> headKeys;
                    std::vector<blas::Point3> targetPath;
                    std::vector<util::ui32> targetKeys;

                    SpotLightNode (
                        const ext::SpotLightHeadNode &spotLightHeadNode_,
                        const ext::SpotLightTargetNode &spotLightTargetNode_,
                        const io::Project &project_);

                    void Draw (const View &view) const;
                    void DrawWithColor (
                        const View &view,
                        const thekogans::mex::opengl::ui8Color &color = DefaultPalette[MEDIUM_GRAY]) const;

                private:
                    SpotLightNode (const SpotLightNode &);
                    SpotLightNode &operator = (const SpotLightNode &);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_keyframer_SpotLightNode_h)
