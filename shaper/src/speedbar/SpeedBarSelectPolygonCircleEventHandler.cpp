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

#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/CircleRegion.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/Shaper.h"
#include "thekogans/mex/shaper/command/SelectPolygonsInRegionCommand.h"
#include "thekogans/mex/shaper/command/UnselectPolygonsInRegionCommand.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_SELECTPOLYGONCIRCLE_0 = "Select center of circle: use CTRL key to deselect\n";
                const char *IDS_SELECTPOLYGONCIRCLE_1 = "Select radius of circle\n";
            }

            class SpeedBarSelectPolygonCircleEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectPolygonCircleEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarSelectPolygonCircleEventHandlerTool)

                private:
                    blas::Circle circle;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ().consoleWindow->Print (IDS_SELECTPOLYGONCIRCLE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            circle.center = pt;
                            circle.radius = 0.0f;
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTPOLYGONCIRCLE_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            core::WaitCursor waitCursor;
                            if (circle.radius != 0.0f && IsCtrl () ?
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new UnselectPolygonsInRegionCommand (
                                                blas::Region2::UniquePtr (
                                                    new blas::CircleRegion (circle))))) :
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new SelectPolygonsInRegionCommand (
                                                blas::Region2::UniquePtr (
                                                    new blas::CircleRegion (circle)))))) {
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTPOLYGONCIRCLE_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTPOLYGONCIRCLE_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                            circle.radius = (pt - circle.center).Length ();
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectPolygonCircleEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectPolygonCircleEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
