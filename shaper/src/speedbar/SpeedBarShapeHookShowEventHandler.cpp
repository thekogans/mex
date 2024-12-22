// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/opengl/BindFramebuffer.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Scissor.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/shaper/Hook.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct SpeedBarShapeHookShowEventHandler : public core::SpeedBar::Item::ButtonEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarShapeHookShowEventHandler)

                core::Module &module;

                explicit SpeedBarShapeHookShowEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnSetFocus () {
                    if (!core::GetIOProject ().shaper.hook.visible) {
                        core::GetIOProject ().shaper.hook.visible = true;
                        for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                            _3ds::opengl::View::UniquePtr openglView = core::GetOpenGLProject ().shaper.GetView (
                                core::GetIOProject ().shaper.viewLayout[i]);
                            assert (openglView.get () != 0);
                            if (openglView.get () != 0) {
                                core::UI::Instance ()->viewLayoutWindow->MakeCurrent ();
                                opengl::BindFramebuffer bindFramebuffer (
                                    GL_FRAMEBUFFER_EXT, core::UI::Instance ()->framebuffer->id);
                                opengl::Enable scissorTest (GL_SCISSOR_TEST, true);
                                opengl::Scissor scissor (core::GetIOProject ().shaper.viewLayout[i].size);
                                // FIXME: jitter?
                                _3ds::opengl::SetupView setupView (*openglView);
                                _3ds::opengl::Hook (core::GetIOProject ().shaper.hook).Draw (*openglView);
                            }
                        }
                        core::command::FlipFramebufferFinalOperation ().Execute ();
                    }
                }

                virtual bool IsChecked () const {
                    return core::GetIOProject ().shaper.hook.visible;
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarShapeHookShowEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
