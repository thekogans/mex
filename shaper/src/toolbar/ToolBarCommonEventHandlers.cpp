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

//#include "thekogans/mex/common/toolbar/ToolBarFileNewEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarFileOpenEventHandler.h"
//#include "thekogans/mex/common/toolbar/ToolBarFileSaveEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarEditUndoEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarEditRedoEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarPanEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarToggleMaximizedEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomInEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomWindowEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomExtentsEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomOutEventHandler.h"
#include "thekogans/mex/common/toolbar/ImplementCommonToolBarItemEventHandler.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            //IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileNewEventHandler, Shaper)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileOpenEventHandler, Shaper)
            //IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileSaveEventHandler, Shaper)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarEditUndoEventHandler, Shaper)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarEditRedoEventHandler, Shaper)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPanEventHandler, Shaper)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarToggleMaximizedEventHandler, Shaper)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomInEventHandler, Shaper)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomWindowEventHandler, Shaper)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomExtentsEventHandler, Shaper)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomOutEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
