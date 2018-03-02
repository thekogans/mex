// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_keyframer.
//
// libthekogans_mex_keyframer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_keyframer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_keyframer. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/core/StatusBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/common/statusbar/StatusBarCursorPositionEventHandler.h"
#include "thekogans/mex/common/statusbar/ImplementCommonStatusBarItemEventHandler.h"

namespace thekogans {
    namespace mex {
        namespace keyframer {

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_STATUSBAR_ITEM_EVENT_HANDLER (StatusBarCursorPositionEventHandler, Keyframer)

            struct StatusBarStartFrameEventHandler : public core::StatusBar::Item::EventHandler {
                THEKOGANS_MEX_CORE_DECLARE_STATUSBAR_ITEM_EVENT_HANDLER (StatusBarStartFrameEventHandler)

                explicit StatusBarStartFrameEventHandler (core::Module &) {}

                virtual void OnUpdateUI (UI &ui) {
                    ui.SetText (util::ui32Tostring (core::GetIOProject ().keyframer.segment.startFrame).c_str ());
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_STATUSBAR_ITEM_EVENT_HANDLER (StatusBarStartFrameEventHandler, Keyframer)

            struct StatusBarEndFrameEventHandler : public core::StatusBar::Item::EventHandler {
                THEKOGANS_MEX_CORE_DECLARE_STATUSBAR_ITEM_EVENT_HANDLER (StatusBarEndFrameEventHandler)

                explicit StatusBarEndFrameEventHandler (core::Module &) {}

                virtual void OnUpdateUI (UI &ui) {
                    ui.SetText (util::ui32Tostring (core::GetIOProject ().keyframer.segment.endFrame).c_str ());
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_STATUSBAR_ITEM_EVENT_HANDLER (StatusBarEndFrameEventHandler, Keyframer)

        } // namespace keyframer
    } // namespace mex
} // namespace thekogans
