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

#if !defined (__thekogans_mex_3ds_opengl_keyframer_SolidObjectNode_h)
#define __thekogans_mex_3ds_opengl_keyframer_SolidObjectNode_h

#include <memory>
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/OwnerMap.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/editor/Element.h"
#include "thekogans/mex/3ds/opengl/keyframer/ObjectNode.h"

// FIXME: todo:
// - test texture mapping

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL SolidObjectNode : public ObjectNode {
                    typedef std::unique_ptr<SolidObjectNode> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    util::OwnerVector<Element> elements;

                    SolidObjectNode (
                        const ext::ObjectNode &objectNode,
                        const io::Project &project,
                        const util::OwnerMap<std::string, Material> &materials);

                    virtual void SetCurrentFrame ();
                    virtual void Draw (const View &view) const;

                private:
                    SolidObjectNode (const SolidObjectNode &);
                    SolidObjectNode &operator = (const SolidObjectNode &);

                    void BuildElements (const util::OwnerMap<std::string, Material> &materials);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_keyframer_SolidObjectNode_h)
