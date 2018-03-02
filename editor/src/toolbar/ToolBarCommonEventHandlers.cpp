// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_editor.
//
// libthekogans_mex_editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_editor. If not, see <http://www.gnu.org/licenses/>.

//#include "thekogans/mex/common/toolbar/ToolBarFileNewEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarFileOpenEventHandler.h"
//#include "thekogans/mex/common/toolbar/ToolBarFileSaveEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarEditUndoEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarEditRedoEventHandler.h"
//#include "thekogans/mex/common/toolbar/ToolBarAxisEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarPanEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarToggleMaximizedEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomInEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomWindowEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomExtentsEventHandler.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomOutEventHandler.h"
#include "thekogans/mex/common/toolbar/ImplementCommonToolBarItemEventHandler.h"
#include "thekogans/mex/editor/Editor.h"

namespace thekogans {
    namespace mex {
        namespace editor {

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileNewEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileOpenEventHandler, Editor)
            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFileSaveEventHandler, Editor)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarEditUndoEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarEditRedoEventHandler, Editor)

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarAxisEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPanEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarToggleMaximizedEventHandler, Editor)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomInEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomWindowEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomExtentsEventHandler, Editor)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarZoomOutEventHandler, Editor)

        } // namespace editor
    } // namespace mex
} // namespace thekogans
