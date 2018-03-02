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

#if !defined (__thekogans_mex_common_StatusBarCursorPositionEventHandler_h)
#define __thekogans_mex_common_StatusBarCursorPositionEventHandler_h

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/StatusBar.h"
#include "thekogans/mex/common/Config.h"

namespace thekogans {
    namespace mex {
        namespace common {

            struct _LIB_THEKOGANS_MEX_COMMON_DECL StatusBarCursorPositionEventHandler :
                    public core::StatusBar::Item::EventHandler {
                core::Module &module;

                StatusBarCursorPositionEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnUpdateUI (UI &ui);

            private:
                inline std::string FormatCoord2 (const blas::Point2 &pt) {
                    return "X: " + _3ds::ext::Units (core::GetIOProject ().units).Format (pt.x) +
                        " Y: " + _3ds::ext::Units (core::GetIOProject ().units).Format (pt.y);
                }

                inline std::string FormatCoord3 (const _3ds::ext::View &view, const blas::Point2 &pt) {
                    blas::Point3 wp (blas::Point3 (pt, (view.view.worldOrigin * view.xform).z) * view.ixform);
                    return "X: " + _3ds::ext::Units (core::GetIOProject ().units).Format (wp.x) +
                        " Y: " + _3ds::ext::Units (core::GetIOProject ().units).Format (wp.y) +
                        " Z: " + _3ds::ext::Units (core::GetIOProject ().units).Format (wp.z);
                }
            };

        } // namespace common
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_common_StatusBarCursorPositionEventHandler_h)
