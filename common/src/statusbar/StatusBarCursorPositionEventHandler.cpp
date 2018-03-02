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

#include <cassert>
#include "thekogans/mex/common/statusbar/StatusBarCursorPositionEventHandler.h"

namespace thekogans {
    namespace mex {
        namespace common {

            void StatusBarCursorPositionEventHandler::OnUpdateUI (UI &ui) {
                if (module.viewLayout->lastPt.InSize (
                        module.GetIOModule ().viewLayout.GetCurrView ().size)) {
                    _3ds::ext::View::UniquePtr view (module.GetExtModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        blas::Point2 pt = view->D2P (module.viewLayout->lastPt);
                        if (view->view.IsShape ()) {
                            ui.SetText (FormatCoord2 (pt).c_str ());
                        }
                        else {
                            ui.SetText (FormatCoord3 (*view, pt).c_str ());
                        }
                    }
                }
            }

        } // namespace common
    } // namespace mex
} // namespace thekogans
