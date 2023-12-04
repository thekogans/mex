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

#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/command/DrawViewFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomWindowEventHandler.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace common {

            namespace {
                const char *CONSOLE_1 = "Zoom Window: Select one corner of window\n";
                const char *CONSOLE_2 = "Zoom Window: Select opposite corner of window\n";
            }

            void ToolBarZoomWindowEventHandler::Tool::SetFocus () {
                core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                assert (core::UI::Instance ().consoleWindow != 0);
                core::UI::Instance ().consoleWindow->Print (CONSOLE_1);
            }

            void ToolBarZoomWindowEventHandler::Tool::LButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {
                if (state == 0) {
                    UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                    bound = pt;
                    BeginTransaction ();
                }
                else if (state == 2) {
                    UpdateState (3, flags | ScrollLockOn);
                    core::DrawBound2 (view, bound);
                }
            }

            void ToolBarZoomWindowEventHandler::Tool::LButtonUp (
                    const _3ds::opengl::View &,
                    util::ui32,
                    const blas::Point2 &) {
                if (state == 1) {
                    UpdateState (2, flags | ScrollLockOff);
                    core::UI::Instance ().consoleWindow->Print (CONSOLE_2);
                }
                else if (state == 3) {
                    UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                    if (bound.min != bound.max) {
                        _3ds::io::ViewLayout &viewLayout = module.GetIOModule ().viewLayout;
                        _3ds::io::View &view = viewLayout[viewLayout.layout.currViewIndex];
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new _3ds::io::command::ViewZoomBoundCommand (
                                    view, bound.Normalize ())));
                        ExecuteAndAddFinalOperation (
                            FinalOperation::SharedPtr (
                                new core::command::DrawViewFinalOperation (
                                    module, view)));
                        ExecuteAndAddFinalOperation (
                            FinalOperation::SharedPtr (
                                new core::command::FlipFramebufferFinalOperation));
                        CommitTransaction ();
                    }
                    else {
                        AbortTransaction ();
                    }
                    core::UI::Instance ().consoleWindow->Print (CONSOLE_1);
                }
            }

            void ToolBarZoomWindowEventHandler::Tool::RButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &) {
                if (state == 0) {
                    UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                }
                else if (state == 2) {
                    UpdateState (3, flags | ScrollLockOn);
                    core::DrawBound2 (view, bound);
                }
            }

            void ToolBarZoomWindowEventHandler::Tool::RButtonUp (
                    const _3ds::opengl::View &,
                    util::ui32,
                    const blas::Point2 &) {
                if (IsOddState ()) {
                    UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                    AbortTransaction ();
                    core::UI::Instance ().consoleWindow->Print (CONSOLE_1);
                }
            }

            void ToolBarZoomWindowEventHandler::Tool::MouseMove (
                    const _3ds::opengl::View &view,
                    util::ui32,
                    const blas::Point2 &pt) {
                if (state == 2) {
                    core::DrawBound2 (view, bound);
                    bound.max = pt;
                    core::DrawBound2 (view, bound);
                }
            }

            void ToolBarZoomWindowEventHandler::Tool::UpdateUI (
                    const _3ds::opengl::View &view) {
                if (state == 2) {
                    core::DrawBound2 (view, bound);
                }
            }

        } // namespace common
    } // namespace mex
} // namespace thekogans
