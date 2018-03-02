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

#include "thekogans/mex/3ds/ext/shaper/Shaper.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/Project.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                Project::Project (const io::Project &project_) :
                    project (project_),
                    shaper (*this),
                    lofter (*this),
                    editor (*this),
                    keyframer (*this)/*,
                    materialsEditor (*this),
                    videoPost (*this)*/ {}

                void Project::Swap (Project &project) {
                    shaper.Swap (project.shaper);
                    lofter.Swap (project.lofter);
                    editor.Swap (project.editor);
                    keyframer.Swap (project.keyframer);
                    //materialsEditor.Swap (project.materialsEditor);
                    //videoPost.Swap (project.videoPost);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
