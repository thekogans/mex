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

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct SpeedBarDisplayFreezePolygonCrossingEventHandler :
                    public core::SpeedBar::Item::ButtonEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (
                    SpeedBarDisplayFreezePolygonCrossingEventHandler)

                core::Module &module;

                explicit SpeedBarDisplayFreezePolygonCrossingEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnSetFocus () {
                    Shaper::Instance ().flags.Set (Shaper::DisplayFreezePolygonWindow, false);
                }
                virtual bool IsChecked () const {
                    return !Shaper::Instance ().flags.Test (Shaper::DisplayFreezePolygonWindow);
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplayFreezePolygonCrossingEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
