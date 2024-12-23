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

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/PolygonRegion.h"
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
                const char *IDS_SELECTPOLYGONFENCE_0 = "Select first point of fence: CTRL to deselect, SPACE to close\n";
                const char *IDS_SELECTPOLYGONFENCE_1 = "Select points of fenced area. Hit SPACE to close\n";
            }

            class SpeedBarSelectPolygonFenceEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectPolygonFenceEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarSelectPolygonFenceEventHandlerTool)

                private:
                    blas::Point2 last;
                    blas::Polygon2 polygon;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTPOLYGONFENCE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsEvenState ()) {
                            if (state == 0) {
                                UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                                polygon.points.clear ();
                                polygon.closed = false;
                                BeginTransaction ();
                            }
                            else if (state == 2) {
                                UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            }
                            polygon.points.push_back (pt);
                            last = pt;
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ()->consoleWindow->Print (IDS_SELECTPOLYGONFENCE_1);
                        }
                        else if (state == 3) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawPolygon2 (view, polygon);
                            core::DrawLine2 (view, polygon.points.back (), last);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ()->consoleWindow->Print (IDS_SELECTPOLYGONFENCE_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawLine2 (view, polygon.points.back (), last);
                            last = pt;
                            core::DrawLine2 (view, polygon.points.back (), last);
                        }
                    }

                    virtual void KeyDown (const _3ds::opengl::View &view, util::ui32 key, util::ui32 repeatCount, util::ui32 flags) {
                        if (key == Qt::Key_Space && state == 2) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            core::DrawPolygon2 (view, polygon);
                            core::DrawLine2 (view, polygon.points.back (), last);
                            if (polygon.points.size () > 2) {
                                polygon.closed = true;
                                core::WaitCursor waitCursor;
                                if (IsCtrl () ?
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new UnselectPolygonsInRegionCommand (
                                                blas::Region2::UniquePtr (
                                                    new blas::PolygonRegion2 (polygon))))) :
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new SelectPolygonsInRegionCommand (
                                                blas::Region2::UniquePtr (
                                                    new blas::PolygonRegion2 (polygon)))))) {
                                    CommitTransaction ();
                                }
                                else {
                                    AbortTransaction ();
                                }
                            }
                            core::UI::Instance ()->consoleWindow->Print (IDS_SELECTPOLYGONFENCE_0);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawPolygon2 (view, polygon);
                            core::DrawLine2 (view, polygon.points.back (), last);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectPolygonFenceEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectPolygonFenceEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
