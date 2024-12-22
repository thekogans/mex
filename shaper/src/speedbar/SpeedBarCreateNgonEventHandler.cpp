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
            class NgonDlg : public CDialog {
            public:
                NgonDlg (CWnd *parent = 0) : CDialog ("IDD_NGONDLG", parent) {}

            protected:
                virtual BOOL OnInitDialog () {
                    CDialog::OnInitDialog ();
                    CScrollBar *pScrollBar = (CScrollBar *)GetDlgItem (101);
                    pScrollBar->SetScrollRange (3, 50, false);
                    pScrollBar->SetScrollPos (GetGlobalInfo2D ().createInfo.ngon_vertices, true);
                    char temp[20];
                    sprintf (temp, "%d", GetGlobalInfo2D ().createInfo.ngon_vertices);
                    GetDlgItem (102)->SetWindowText (temp);
                    return TRUE;
                }

                virtual void OnOK () {
                    GetGlobalInfo2D ().createInfo.ngon_vertices =
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
                    wnd->SetScrollPos (p, true);
                    char str[20];
                    sprintf (str, "%d", p);
                    GetDlgItem (102)->SetWindowText (str);
                }
                DECLARE_MESSAGE_MAP ()
            };

            BEGIN_MESSAGE_MAP (NgonDlg, CDialog)
                ON_WM_HSCROLL ()
            END_MESSAGE_MAP ()

            class CreateNgonNSidesEventHandler : public Item::EventHandler {
                DECLARE_EVENT_HANDLER_CONSTRUCTOR (CreateNgonNSidesEventHandler, Module2D)

            public:
                virtual void SetFocus () {
                    ExecuteDlg2D (NgonDlg ());
                }
            };

            IMPLEMENT_EVENT_HANDLER (CreateNgonNSidesEventHandler, Module2D)
#endif

            namespace {
                const char *IDS_CREATENGON_0 = "Place center of %d-sided N-gon:\n";
                const char *IDS_CREATENGON_1 = "Set N-gon size\n";
                const char *IDS_CREATENGON_2 = "Radius: %s Angle: %.2f\n";

                // Create an N sided regular polygon.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateNgonEventHandlerTool)

                private:
                    bool circular;
                    blas::Circle circle;
                    util::f32 angle;

                public:
                    Tool (core::Module &module, bool circular_) :
                        core::Tool (module), circular (circular_) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ()->consoleWindow->Print (IDS_CREATENGON_0,
                            core::GetIOProject ().shaper.create.ngonVertices);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            circle.center = pt;
                            circle.radius = 0.0f;
                            angle = 0.0f;
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, circle, angle,
                                core::GetIOProject ().shaper.create.ngonVertices *
                                (circular ? (core::GetIOProject ().shaper.steps + 1) : 1));
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATENGON_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (circle.radius != 0.0f) {
                                core::WaitCursor waitCursor;
                                AddPolygon (_3ds::ext::Ngon (
                                        circle.center, circle.radius, angle,
                                        core::GetIOProject ().shaper.create.ngonVertices, circular));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATENGON_0,
                                core::GetIOProject ().shaper.create.ngonVertices);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, circle, angle,
                                core::GetIOProject ().shaper.create.ngonVertices *
                                (circular ? (core::GetIOProject ().shaper.steps + 1) : 1));
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATENGON_0,
                                core::GetIOProject ().shaper.create.ngonVertices);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawCircle2 (view, circle, angle,
                                core::GetIOProject ().shaper.create.ngonVertices *
                                (circular ? (core::GetIOProject ().shaper.steps + 1) : 1));
                            _3ds::ext::DrawingAids (core::GetIOProject ().drawingAids).GetRadiusAndAngle (
                                circle.center, pt, circle.radius, angle);
                            core::DrawCircle2 (view, circle, angle,
                                core::GetIOProject ().shaper.create.ngonVertices *
                                (circular ? (core::GetIOProject ().shaper.steps + 1) : 1));
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawCircle2 (view, circle, angle,
                                core::GetIOProject ().shaper.create.ngonVertices *
                                (circular ? (core::GetIOProject ().shaper.steps + 1) : 1));
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_CREATENGON_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (circle.radius).c_str (),
                                (angle < 0.0f) ? (360.0f + util::DEG (angle)) : util::DEG (angle));
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                };
            }

            class SpeedBarCreateNgonFlatEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateNgonFlatEventHandler)

            private:
                Tool tool;

            public:
                explicit SpeedBarCreateNgonFlatEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module, false) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateNgonFlatEventHandler, Shaper)

            class SpeedBarCreateNgonCircularEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateNgonCircularEventHandler)

            private:
                Tool tool;

            public:
                explicit SpeedBarCreateNgonCircularEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module, true) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateNgonCircularEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
