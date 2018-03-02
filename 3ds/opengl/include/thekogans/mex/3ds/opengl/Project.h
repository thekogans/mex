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

#if !defined (__thekogans_mex_3ds_opengl_Project_h)
#define __thekogans_mex_3ds_opengl_Project_h

#include "thekogans/mex/3ds/ext/Project.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/shaper/Shaper.h"
#include "thekogans/mex/3ds/opengl/lofter/Lofter.h"
#include "thekogans/mex/3ds/opengl/editor/Editor.h"
#include "thekogans/mex/3ds/opengl/keyframer/Keyframer.h"
//#include "thekogans/mex/3ds/opengl/MaterialsEditor.h"
//#include "thekogans/mex/3ds/opengl/VideoPost.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Project {
                    const ext::Project &project;
                    Shaper shaper;
                    Lofter lofter;
                    Editor editor;
                    Keyframer keyframer;
                    //MaterialsEditor materialsEditor;
                    //VideoPost videoPost;

                    explicit Project (const ext::Project &project_);

                    // Swap follows the semantics of std::swap. It guarantees not
                    // to throw. All major types defined by lib3ds implement Swap
                    // allowing for creation of software which guarantees transactional
                    // semantics.
                    void Swap (Project &project);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Project_h)
