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
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/common/statusbar/StatusBarToolStatusEventHandler.h"

namespace thekogans {
    namespace mex {
        namespace common {

            void StatusBarToolStatusEventHandler::OnUpdateUI (UI &ui) {
                if (module.viewLayout->tool != 0) {
                    module.viewLayout->tool->UpdateStatus (ui);
                }
            }

        } // namespace common
    } // namespace mex
} // namespace thekogans
