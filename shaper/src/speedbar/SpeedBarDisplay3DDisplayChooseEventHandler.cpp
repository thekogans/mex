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

#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Path.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/command/OwnerVectorCommands.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/ui/3DDisplayDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            class SpeedBarDisplay3DDisplayChooseEventHandler : public core::SpeedBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplay3DDisplayChooseEventHandler)

            public:
                explicit SpeedBarDisplay3DDisplayChooseEventHandler (core::Module &) {}

                virtual void OnSetFocus () {
                    _3DDisplayDialog dialog (core::UI::Instance ().mainFrameWindow->GetQWidget ());
                    dialog.exec ();
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplay3DDisplayChooseEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
