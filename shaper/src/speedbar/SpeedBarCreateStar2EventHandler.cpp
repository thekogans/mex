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

#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/3ds/ext/DrawingAids.h"
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

#if 0
            class Star2Dlg : public CDialog {
            public:
                Star2Dlg (CWnd *parent = 0) :
                    CDialog ("IDD_STARDLG", parent) {}

            protected:
                virtual BOOL OnInitDialog () {
                    CDialog::OnInitDialog ();
                    CScrollBar *pScrollBar = (CScrollBar *)GetDlgItem (101);
                    pScrollBar->SetScrollRange (3, 50, false);
                    pScrollBar->SetScrollPos (GetGlobalInfo2D ().createInfo.star_vertices, TRUE);
                    char temp[20];
                    sprintf (temp, "%d", GetGlobalInfo2D ().createInfo.star_vertices);
                    GetDlgItem (102)->SetWindowText (temp);
                    return TRUE;
                }

                virtual void OnOK () {
                    GetGlobalInfo2D ().createInfo.star_vertices =
                        ((CScrollBar *)GetDlgItem (101))->GetScrollPos ();
                    CDialog::OnOK ();
                }

                afx_msg void OnHScroll (ui32 code, ui32 pos, CScrollBar *wnd) {
                    int p;
                    switch (code) {
                        case SB_LINEDOWN:
                            p = wnd->GetScrollPos () + 1;
                            break;
                        case SB_LINEUP:
                            p = wnd->GetScrollPos () - 1;
                            break;
                        case SB_PAGEDOWN:
                            p = wnd->GetScrollPos () + 10;
                            break;
                        case SB_PAGEUP:
                            p = wnd->GetScrollPos () - 10;
                            break;
                        case SB_BOTTOM:
                            p = 50;
                            break;
                        case SB_TOP:
                            p = 2;
                            break;
                        case SB_THUMBTRACK:
                            p = pos;
                            break;
                        default:
                            p = wnd->GetScrollPos ();
                            break;
                    }
                    CLAMP (p, 3, 50);
                    wnd->SetScrollPos (p, TRUE);
                    char str[20];
                    sprintf (str, "%d", p);
                    GetDlgItem (102)->SetWindowText (str);
                }

                DECLARE_MESSAGE_MAP ()
            };

            BEGIN_MESSAGE_MAP (Star2Dlg, CDialog)
                ON_WM_HSCROLL ()
            END_MESSAGE_MAP ()

            class CreateStar2NSidesEventHandler : public Item::EventHandler {
                DECLARE_EVENT_HANDLER_CONSTRUCTOR (CreateStar2NSidesEventHandler, Module2D)

            public:
                virtual void SetFocus () {
                    ExecuteDlg2D (Star2Dlg ());
                }
            };

            IMPLEMENT_EVENT_HANDLER (CreateStar2NSidesEventHandler, Module2D)
#endif

            namespace {
                const char *IDS_CREATESTAR2_0 = "Place center of %d-pointed star:\n";
                const char *IDS_CREATESTAR2_1 = "Set radius 1\n";
                const char *IDS_CREATESTAR2_3 = "Set radius 2\n";
                const char *IDS_CREATESTAR2_2 = "Radius1: %s Angle: %.2f";
                const char *IDS_CREATESTAR2_4 = "Radius2: %s Angle: %.2f";

                // Create a star.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateStar2EventHandlerTool)

                private:
                    bool circular;
                    blas::Point2 center;
                    util::f32 radius1;
                    util::f32 radius2;
                    util::f32 angle;
                    util::f32 skew;

                public:
                    Tool (core::Module &module, bool circular_) :
                        core::Tool (module), circular (circular_) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ()->consoleWindow->Print (IDS_CREATESTAR2_0,
                            core::GetIOProject ().shaper.create.star2Vertices);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            center = pt;
                            radius1 = radius2 = angle = skew = 0.0f;
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                        }
                        else if (state == 4) {
                            UpdateState (5, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATESTAR2_1);
                        }
                        else if (state == 3) {
                            UpdateState (4, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATESTAR2_3);
                        }
                        else if (state == 5) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            util::f32 rmin;
                            util::f32 rmax;
                            if (radius1 > radius2) {
                                rmax = radius1;
                                rmin = radius2;
                            }
                            else {
                                rmax = radius2;
                                rmin = radius1;
                            }
                            if (rmin != rmax) {
                                core::WaitCursor waitCursor;
                                AddPolygon (
                                    _3ds::ext::Star2 (
                                        center, rmax, rmin, angle, skew,
                                        core::GetIOProject ().shaper.create.star2Vertices, circular));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATESTAR2_0,
                                core::GetIOProject ().shaper.create.star2Vertices);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                        }
                        else if (state == 4) {
                            UpdateState (5, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                            core::DrawCircle2 (view, blas::Circle (center, radius2), angle, 32);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATESTAR2_0,
                                core::GetIOProject ().shaper.create.star2Vertices);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                            _3ds::ext::DrawingAids (core::GetIOProject ().drawingAids).GetRadiusAndAngle (
                                center, pt, radius1, angle);
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                        }
                        else if (state == 4) {
                            core::DrawCircle2 (view, blas::Circle (center, radius2), angle, 32);
                            radius2 = (pt - center).Length ();
                            core::DrawCircle2 (view, blas::Circle (center, radius2), angle, 32);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                        }
                        else if (state == 4) {
                            core::DrawCircle2 (view, blas::Circle (center, radius1), angle, 32);
                            core::DrawCircle2 (view, blas::Circle (center, radius2), angle, 32);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_CREATESTAR2_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (radius1).c_str (),
                                (angle < 0.0f) ? (360.0f + util::DEG (angle)) : util::DEG (angle));
                        }
                        else if (state == 4) {
                            ui.SetText (IDS_CREATESTAR2_4,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (radius2).c_str (),
                                (angle < 0.0f) ? (360.0f + util::DEG (angle)) : util::DEG (angle));
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                };
            }

            class SpeedBarCreateStar2FlatEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateStar2FlatEventHandler)

            private:
                Tool tool;

            public:
                explicit SpeedBarCreateStar2FlatEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module, false) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateStar2FlatEventHandler, Shaper)

            class SpeedBarCreateStar2CircularEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateStar2CircularEventHandler)

            private:
                Tool tool;

            public:
                explicit SpeedBarCreateStar2CircularEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module, true) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateStar2CircularEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
