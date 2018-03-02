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
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/3ds/ext/DrawingAids.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/shaper/PolygonSelectingTool.h"
#include "thekogans/mex/shaper/Util.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_CREATESTAR1_0 = "Place center of star:\n";
                const char *IDS_CREATESTAR1_1 = "Set star size\n";
                const char *IDS_CREATESTAR1_2 = "Radius: %s Angle: %.2f\n";
            }

            class SpeedBarCreateStar1EventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateStar1EventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateStar1EventHandlerTool)

                private:
                    bool curved;
                    blas::Point2 center;
                    util::f32 radius;
                    util::f32 angle;

                public:
                    Tool (core::Module &module, bool curved_) :
                        core::Tool (module), curved (curved_) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATESTAR1_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            center = pt;
                            radius = 0.0f;
                            angle = 0.0f;
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            core::DrawStar2 (view, center, radius, angle);
                            UpdateState (3, flags | ScrollLockOn | CursorVisible);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATESTAR1_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (radius != 0.0f) {
                                core::WaitCursor waitCursor;
                                AddPolygon (_3ds::ext::Star1 (center, radius, angle, curved));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATESTAR1_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawStar2 (view, center, radius, angle);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATESTAR1_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawStar2 (view, center, radius, angle);
                            _3ds::ext::DrawingAids (core::GetIOProject ().drawingAids).GetRadiusAndAngle (
                                center, pt, radius, angle);
                            core::DrawStar2 (view, center, radius, angle);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawStar2 (view, center, radius, angle);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_CREATESTAR1_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (radius).c_str (),
                                (angle < 0.0f) ? (360.0f + util::DEG (angle)) : util::DEG (angle));
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateStar1EventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module, false) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateStar1EventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
