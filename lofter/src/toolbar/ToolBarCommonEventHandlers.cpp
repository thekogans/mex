// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_lofter.
//
// libthekogans_mex_lofter is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_lofter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_lofter. If not, see <http://www.gnu.org/licenses/>.

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
#include "thekogans/mex/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace lofter {

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileNewEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileOpenEventHandler, Lofter)
            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileSaveEventHandler, Lofter)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarEditUndoEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarEditRedoEventHandler, Lofter)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPanEventHandler, Lofter)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarToggleMaximizedEventHandler, Lofter)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomInEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomWindowEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomExtentsEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomOutEventHandler, Lofter)

        } // namespace lofter
    } // namespace mex
} // namespace thekogans
