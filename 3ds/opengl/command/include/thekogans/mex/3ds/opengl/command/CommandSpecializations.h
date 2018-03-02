// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl_command.
//
// libthekogans_mex_3ds_opengl_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_command_CommandSpecializations_h)
#define __thekogans_mex_3ds_opengl_command_CommandSpecializations_h

#include "thekogans/mex/command/ComplexTypeCommands.h"
#include "thekogans/mex/3ds/opengl/Project.h"
#include "thekogans/mex/3ds/opengl/shaper/Shaper.h"
#include "thekogans/mex/3ds/opengl/lofter/Lofter.h"
#include "thekogans/mex/3ds/opengl/editor/Editor.h"
#include "thekogans/mex/3ds/opengl/keyframer/Keyframer.h"
//#include "thekogans/mex/3ds/opengl/materialseditor/MaterialsEditor.h"
//#include "thekogans/mex/3ds/opengl/videopost/VideoPost.h"

namespace thekogans {
    namespace mex {
        namespace command {

            typedef ComplexTypeSwapCommand<_3ds::opengl::Project> openglProjectComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<_3ds::opengl::Shaper> openglShaperComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<_3ds::opengl::Lofter> openglLofterComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<_3ds::opengl::Editor> openglEditorComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<_3ds::opengl::Keyframer> openglKeyframerComplexTypeSwapCommand;
            //typedef ComplexTypeSwapCommand<_3ds::opengl::MaterialsEditor> openglMaterialsEditorComplexTypeSwapCommand;
            //typedef ComplexTypeSwapCommand<_3ds::opengl::VideoPost> openglVideoPostComplexTypeSwapCommand;

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_command_CommandSpecializations_h)
