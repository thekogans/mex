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

#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/shaper/Shaper.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct SpeedBarDisplay3DDisplayOffEventHandler : public core::SpeedBar::Item::ButtonEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplay3DDisplayOffEventHandler)

                core::Module &module;

                explicit SpeedBarDisplay3DDisplayOffEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnSetFocus () {
                    if (!IsChecked ()) {
                        core::GetIOProject ().shaper.show3D = false;
                        core::command::DrawViewLayoutFinalOperation (module).Execute ();
                        core::command::FlipFramebufferFinalOperation ().Execute ();
                    }
                }
                virtual bool IsChecked () const {
                    return !core::GetIOProject ().shaper.show3D;
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplay3DDisplayOffEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
