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

#if !defined (__thekogans_mex_3ds_opengl_keyframer_ObjectNode_h)
#define __thekogans_mex_3ds_opengl_keyframer_ObjectNode_h

#include "thekogans/mex/3ds/ext/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/opengl/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct ObjectNode {
                    const ext::ObjectNode &objectNode;
                    const io::Project &project;

                    ObjectNode (const ext::ObjectNode &objectNode_, const io::Project &project_) :
                        objectNode (objectNode_), project (project_) {}
                    virtual ~ObjectNode () {}

                    virtual void SetCurrentFrame () = 0;
                    virtual void Draw (const View &view) const = 0;
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_keyframer_ObjectNode_h)
