// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_common.
//
// libthekogans_mex_common is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_common is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_common. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/MenuBar.h"
#include "thekogans/mex/core/Module.h"

namespace thekogans {
    namespace mex {

        struct MenuBarWindowToolBarEventHandler : public core::MenuBar::Item::ButtonEventHandler {
            THEKOGANS_MEX_CORE_DECLARE_MENUBAR_ITEM_EVENT_HANDLER (MenuBarWindowToolBarEventHandler)

            core::Module &module;

            explicit MenuBarWindowToolBarEventHandler (core::Module &module_) : module (module_) {}

            virtual void OnSetFocus () {
                core::UI::Instance ()->mainFrameWindow->ShowToolBarWindow (
                    !core::UI::Instance ()->mainFrameWindow->IsToolBarWindowVisible ());
            }

            virtual bool IsChecked () const {
                assert (core::UI::Instance ()->mainFrameWindow != 0);
                return core::UI::Instance ()->mainFrameWindow->IsToolBarWindowVisible ();
            }
        };

    } // namespace mex
} // namespace thekogans
