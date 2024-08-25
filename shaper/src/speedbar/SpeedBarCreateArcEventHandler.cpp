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

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QMessageBox>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/shaper/Util.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_CREATEARC_0 = "Place center of arc:\n";
                const char *IDS_CREATEARC_1 = "Set arc radius and start angle\n";
                const char *IDS_CREATEARC_2 = "Radius: %s Angle: %.2f\n";
                const char *IDS_CREATEARC_4 = "Radius: %s Angle: %.2f Arc: %.2f\n";
            }

            class SpeedBarCreateArcEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateArcEventHandler)

            private:
                // Create an arc. If the span angle is 360 degrees, the
                // arc automatically becomes a circle.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateArcEventHandlerTool)

                private:
                    blas::Point2 last;
                    blas::Point2 center;
                    util::f32 radius;
                    util::f32 startAngle;
                    util::f32 spanAngle;
                    bool clockWise;

                public:
                    Tool (core::Module &module) :
                        core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATEARC_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            last = pt;
                            center = pt;
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawLine2 (view, center, last);
                            blas::Point2 t = pt - center;
                            radius = t.Length ();
                            startAngle = radius == 0.0f ? 0.0f : atan2f (t.y, t.x);
                            spanAngle = 0.0f;
                            clockWise = false;
                            core::DrawArc2 (view, center, radius, startAngle,
                                util::RAD (spanAngle), clockWise, true,
                                4 * (core::GetIOProject ().shaper.steps + 1));
                        }
                        else if (state == 4) {
                            UpdateState (5, flags | ScrollLockOn);
                            core::DrawArc2 (view, center, radius, startAngle,
                                util::RAD (spanAngle), clockWise, true,
                                4 * (core::GetIOProject ().shaper.steps + 1));
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEARC_1);
                        }
                        else if (state == 3) {
                            UpdateState (4, flags | ScrollLockOff);
                        }
                        else if (state == 5) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (radius != 0.0f) {
                                bool closed = spanAngle > 359.5f;
                                if (closed && QMessageBox::question (
                                        core::UI::Instance ().mainFrameWindow->GetQWidget (), "Arc",
                                        "Connect these vertices?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
                                    closed = false;
                                }
                                core::WaitCursor waitCursor;
                                assert (IsTransactionPending ());
                                AddPolygon (
                                    _3ds::ext::Arc (
                                        center, radius, startAngle, util::RAD (spanAngle),
                                        4 - closed, clockWise, closed));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEARC_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawLine2 (view, center, last);
                        }
                        else if (state == 4) {
                            UpdateState (5, flags | ScrollLockOn);
                            core::DrawArc2 (view, center, radius, startAngle,
                                util::RAD (spanAngle), clockWise, true,
                                4 * (core::GetIOProject ().shaper.steps + 1));
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEARC_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawLine2 (view, center, last);
                            last = pt;
                            core::DrawLine2 (view, center, last);
                            blas::Point2 t = pt - center;
                            radius = t.Length ();
                            startAngle = radius == 0.0f ? 0.0f : atan2f (t.y, t.x);
                        }
                        else if (state == 4) {
                            core::DrawArc2 (view, center, radius, startAngle,
                                util::RAD (spanAngle), clockWise, true,
                                4 * (core::GetIOProject ().shaper.steps + 1));
                            blas::Point d1 = view.P2D (pt);
                            blas::Point d2 = view.P2D (last);
                            core::UI::Instance ().viewLayoutWindow->SetMousePosition (d2);
                            if (d1.x < d2.x) {
                                if (clockWise) {
                                    spanAngle -= ((util::f32)d2.x - d1.x) * 0.25f;
                                    if (spanAngle < 0.0f) {
                                        spanAngle *= -1;
                                        clockWise = false;
                                    }
                                }
                                else {
                                    spanAngle += ((util::f32)d2.x - d1.x) * 0.25f;
                                    spanAngle = fmodf (spanAngle, 360.0f);
                                }
                            }
                            else {
                                if (clockWise) {
                                    spanAngle += ((util::f32)d1.x - d2.x) * 0.25f;
                                    spanAngle = fmodf (spanAngle, 360.0f);
                                }
                                else {
                                    spanAngle -= ((util::f32)d1.x - d2.x) * 0.25f;
                                    if (spanAngle < 0.0f) {
                                        spanAngle *= -1;
                                        clockWise = true;
                                    }
                                }
                            }
                            core::DrawArc2 (view, center, radius, startAngle,
                                util::RAD (spanAngle), clockWise, true,
                                4 * (core::GetIOProject ().shaper.steps + 1));
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawLine2 (view, center, last);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_CREATEARC_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (radius).c_str (),
                                util::DEG (startAngle));
                        }
                        else if (state == 4) {
                            ui.SetText (IDS_CREATEARC_4,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (radius).c_str (),
                                util::DEG (startAngle), spanAngle);
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateArcEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateArcEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
