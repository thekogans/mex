// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_editor.
//
// libthekogans_mex_editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_editor. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_editor_Module_h)
#define __thekogans_mex_editor_Module_h

#include "thekogans/util/Flags.h"
#include "thekogans/mex/3ds/io/Module.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/3ds/opengl/editor/Editor.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/editor/Config.h"

namespace thekogans {
    namespace mex {
        namespace editor {

            struct _LIB_THEKOGANS_MEX_EDITOR_DECL Editor : public core::Module {
                THEKOGANS_MEX_CORE_DECLARE_MODULE

                enum {
                    UseLocalAxis = 1,
                    Selected = 2,
                    SelectObjectWindow = 4
                };
                util::Flags32 flags;

                Editor () : flags (SelectObjectWindow) {}

                virtual _3ds::io::Module &GetIOModule () const {return core::GetIOProject ().editor;}
                virtual _3ds::ext::Module &GetExtModule () const {return core::GetExtProject ().editor;}
                virtual _3ds::opengl::Module &GetOpenGLModule () const {return core::GetOpenGLProject ().editor;}
            };

        } // namespace editor
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_editor_Module_h)
