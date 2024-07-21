// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_core_command_DrawViewFinalOperation_h)
#define __thekogans_mex_core_command_DrawViewFinalOperation_h

#include <cassert>
#include "thekogans/mex/opengl/BindFramebuffer.h"
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/Module.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Module.h"

namespace thekogans {
    namespace mex {
        namespace core {
            namespace command {

                struct _LIB_THEKOGANS_MEX_CORE_DECL DrawViewFinalOperation :
                        public thekogans::mex::command::FinalOperation {
                    THEKOGANS_MEX_COMMAND_DECLARE_FINAL_OPERATION (DrawViewFinalOperation)

                    const Module &module;
                    const _3ds::io::View &view;

                    DrawViewFinalOperation (
                        const Module &module_,
                        const _3ds::io::View &view_) :
                        module (module_),
                        view (view_) {}

                    virtual bool Execute () {
                        UI::Instance ()->viewLayoutWindow->MakeCurrent ();
                        thekogans::mex::opengl::BindFramebuffer bindFramebuffer (
                            GL_FRAMEBUFFER_EXT, UI::Instance ()->framebuffer->id);
                        module.GetOpenGLModule ().GetView (view)->Draw (module.GetOpenGLModule ());
                        return true;
                    }
                };

            } // namespace command
        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_command_DrawViewFinalOperation_h)
