// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/SelectingTool.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"

namespace thekogans {
    namespace mex {
        namespace core {

            namespace {
                const char *IDS_SELECTINGTOOL_0 = "Select first corner of quad: use CTRL key to deselect\n";
                const char *IDS_SELECTINGTOOL_2 = "Select opposite corner of quad\n";
            }

            void SelectingTool::ToggleSelecting () {
                if (state == 0) {
                    if (selecting) {
                        selecting = false;
                        core::CursorMgr::Instance ()->SetCursor (cursor);
                    }
                    else {
                        selecting = true;
                        cursor = core::CursorMgr::Instance ()->GetCurrCursor ();
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTINGTOOL_0);
                    }
                }
            }

            void SelectingTool::KillFocus () {
                if (selecting) {
                    selecting = false;
                    core::CursorMgr::Instance ()->SetCursor (cursor);
                }
                Tool::KillFocus ();
            }

            void SelectingTool::LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                if (selecting) {
                    if (state == 0) {
                        UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        unselect = IsCtrl ();
                        bound = pt;
                        BeginTransaction ();
                    }
                    else if (state == 2) {
                        UpdateState (3, flags | ScrollLockOn);
                        DrawBound2 (view, bound);
                    }
                }
                else {
                    Tool::LButtonDown (view, flags, pt);
                }
            }

            void SelectingTool::LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                if (selecting) {
                    if (state == 1) {
                        UpdateState (2, flags | ScrollLockOff);
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTINGTOOL_2);
                    }
                    else if (state == 3) {
                        UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                        bound.Normalize ();
                        Select (view, bound, unselect);
                        CommitTransaction ();
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTINGTOOL_0);
                    }
                }
                else {
                    Tool::LButtonUp (view, flags, pt);
                }
            }

            void SelectingTool::RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                if (selecting) {
                    if (state == 0) {
                        UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        core::CursorMgr::Instance ()->SetCursor (cursor);
                            selecting = false;
                    }
                    else if (state == 2) {
                        UpdateState (3, flags | ScrollLockOn);
                        DrawBound2 (view, bound);
                    }
                }
                else {
                    Tool::RButtonDown (view, flags, pt);
                }
            }

            void SelectingTool::RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                if (selecting) {
                    if (IsOddState ()) {
                        UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                        AbortTransaction ();
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTINGTOOL_0);
                    }
                }
                else {
                    Tool::RButtonUp (view, flags, pt);
                }
            }

            void SelectingTool::MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                if (selecting) {
                    if (state == 2) {
                        DrawBound2 (view, bound);
                        bound.max = pt;
                        DrawBound2 (view, bound);
                    }
                }
                else {
                    Tool::MouseMove (view, flags, pt);
                }
            }

            void SelectingTool::UpdateUI (const _3ds::opengl::View &view) {
                if (selecting) {
                    if (state == 2) {
                        DrawBound2 (view, bound);
                    }
                }
                else {
                    Tool::UpdateUI (view);
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
