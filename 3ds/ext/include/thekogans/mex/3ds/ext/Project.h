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

#if !defined (__thekogans_mex_3ds_ext_Project_h)
#define __thekogans_mex_3ds_ext_Project_h

#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/shaper/Shaper.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
//#include "thekogans/mex/3ds/ext/MaterialsEditor.h"
//#include "thekogans/mex/3ds/ext/VideoPost.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Project {
                    const io::Project &project;
                    Shaper shaper;
                    Lofter lofter;
                    Editor editor;
                    Keyframer keyframer;
                    //MaterialsEditor materialsEditor;
                    //VideoPost videoPost;

                    explicit Project (const io::Project &project_);

                    // Swap follows the semantics of std::swap. It guarantees not
                    // to throw. All major types defined by lib3ds implement Swap
                    // allowing for creation of software which guarantees transactional
                    // semantics.
                    void Swap (Project &project);
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_Project_h)
