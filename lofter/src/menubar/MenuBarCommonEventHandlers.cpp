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

//#include "thekogans/mex/common/menubar/MenuBarFileNewEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFileResetEventHandler.h"
#include "thekogans/mex/common/menubar/MenuBarFileOpenEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFileMergeEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFileSaveEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFileSaveAsEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFilePreferencesEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFileSceneInfoEventHandler.h"
//#include "thekogans/mex/common/menubar/MenuBarFileExitEventHandler.h"
#include "thekogans/mex/common/menubar/MenuBarWindowMenuBarEventHandler.h"
#include "thekogans/mex/common/menubar/MenuBarWindowToolBarEventHandler.h"
#include "thekogans/mex/common/menubar/MenuBarWindowSpeedBarEventHandler.h"
#include "thekogans/mex/common/menubar/MenuBarWindowConsoleEventHandler.h"
#include "thekogans/mex/common/menubar/MenuBarWindowStatusBarEventHandler.h"
#include "thekogans/mex/common/menubar/ImplementCommonMenuBarItemEventHandler.h"
#include "thekogans/mex/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace lofter {

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileNewEventHandler, Lofter)
            //THEKOGANS_MEX_CORE_IMPLEMENT_MENUBAR_ITEM_EVENT_HANDLER (MenuBarMenuBarFileResetEventHandler, Lofter)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileOpenEventHandler, Lofter)
            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileMergeEventHandler, Lofter)

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileSaveEventHandler, Lofter)
            //THEKOGANS_MEX_CORE_IMPLEMENT_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileSaveAsEventHandler, Lofter)

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFilePreferencesEventHandler, Lofter)
            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileSceneInfoEventHandler, Lofter)

            //THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarFileExitEventHandler, Lofter)

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarWindowMenuBarEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarWindowToolBarEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarWindowSpeedBarEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarWindowConsoleEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_MENUBAR_ITEM_EVENT_HANDLER (MenuBarWindowStatusBarEventHandler, Lofter)

        } // namespace lofter
    } // namespace mex
} // namespace thekogans
