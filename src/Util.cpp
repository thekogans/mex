// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of thekogans_mex.
//
// thekogans_mex is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// thekogans_mex is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with thekogans_mex. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/Util.h"

namespace thekogans {
    namespace mex {

        util::ui32 GetControlKeyState (Qt::KeyboardModifiers modifiers) {
            util::ui32 flags = 0;
            if ((modifiers & Qt::ControlModifier) == Qt::ControlModifier) {
                flags |= core::FLAG_CTRL;
            }
            if ((modifiers & Qt::AltModifier) == Qt::AltModifier) {
                flags |= core::FLAG_ALT;
            }
            if ((modifiers & Qt::ShiftModifier) == Qt::ShiftModifier) {
                flags |= core::FLAG_SHIFT;
            }
            return flags;
        }

    } // namespace mex
} // namespace thekogans
