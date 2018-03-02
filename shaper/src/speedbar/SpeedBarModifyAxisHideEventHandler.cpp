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

#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/Axis.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct SpeedBarModifyAxisHideEventHandler : public core::SpeedBar::Item::ButtonEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifyAxisHideEventHandler)

                core::Module &module;

                explicit SpeedBarModifyAxisHideEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnSetFocus () {
                    core::GetIOProject ().shaper.axis.visible = false;
                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                        _3ds::opengl::View::UniquePtr view = core::GetOpenGLProject ().shaper.GetView (
                            core::GetIOProject ().shaper.viewLayout[i]);
                        _3ds::opengl::SetupView setupView (*view);
                        _3ds::opengl::Axis2 (core::GetIOProject ().shaper.axis).DrawWithColor (*view);
                    }
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
                virtual bool IsChecked () const {
                    return !core::GetIOProject ().shaper.axis.visible;
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifyAxisHideEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
