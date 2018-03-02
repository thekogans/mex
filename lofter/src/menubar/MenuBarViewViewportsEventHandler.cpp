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

#if 0
#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <afxext.h>
#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/3ds/command/ViewCommands.h"
#include "thekogans/mex/3ds/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/command/CommandSpecializations.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/lofter/Module.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace lofter {

            namespace {
                class ViewportsDialog : public CDialog {
                private:
                    core::Module &module;
                    _3ds::io::Lofter &lofter;
                    char currView;
                    char oldViews[4];
                    char newViews[4];

                    void DrawLayout () {
                        CWnd *wnd = GetDlgItem (101);
                        wnd->Invalidate ();
                        wnd->UpdateWindow ();

                        CRect rect;
                        wnd->GetClientRect (rect);

                        CDC *dc = wnd->GetDC ();
                        dc->Rectangle (rect);

                        #define L   rect.left
                        #define T   rect.top
                        #define R   rect.right
                        #define B   rect.bottom
                        #define W   (R - L)
                        #define H   (B - T)
                        #define TX  (L + W / 3)
                        #define TY  (T + H / 3)
                        #define MX  (L + W / 2)
                        #define MY  (T + H / 2)
                        #define TTX (L + 2 * W / 3)
                        #define TTY (T + 2 * H / 3)

                        dc->MoveTo (TX, T);
                        dc->LineTo (TX, B);
                        dc->MoveTo (L, TY);
                        dc->LineTo (TX, TY);
                        dc->MoveTo (L, TTY);
                        dc->LineTo (TX, TTY);
                        dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                        dc->TextOut (L + 1, TY + 1, &newViews[1], 1);
                        dc->TextOut (L + 1, TTY + 1, &newViews[2], 1);
                        dc->TextOut (TX + 1, T + 1, &newViews[3], 1);

                        wnd->ReleaseDC (dc);
                    }

                public:
                    ViewportsDialog (core::Module &module_, CWnd *parent = 0) :
                        CDialog ("ViewportsDialog", parent), module (module_),
                        lofter (static_cast<_3ds::io::Lofter &> (module.GetIOModule ())),
                        currView ('N') {
                        _3ds::io::ViewLayout &viewLayout = lofter.viewLayout;
                        bool maximized = viewLayout.layout.maximized;
                        if (maximized) {
                            command::ViewLayoutToggleMaximizedCommand (viewLayout).Execute ();
                        }
                        for (util::ui32 i = 0; i < 4; ++i) {
                            switch (viewLayout[i].type) {
                                case _3ds::io::View::None:
                                    oldViews[i] = 'N';
                                    newViews[i] = 'N';
                                    break;
                                case _3ds::io::View::Top:
                                    if (viewLayout[i].IsShape ()) {
                                        oldViews[i] = '*';
                                        newViews[i] = '*';
                                    }
                                    else {
                                        oldViews[i] = 'T';
                                        newViews[i] = 'T';
                                    }
                                    break;
                                case _3ds::io::View::Bottom:
                                    oldViews[i] = 'B';
                                    newViews[i] = 'B';
                                    break;
                                case _3ds::io::View::Left:
                                    oldViews[i] = 'L';
                                    newViews[i] = 'L';
                                    break;
                                case _3ds::io::View::Right:
                                    oldViews[i] = 'R';
                                    newViews[i] = 'R';
                                    break;
                                case _3ds::io::View::Front:
                                    oldViews[i] = 'F';
                                    newViews[i] = 'F';
                                    break;
                                case _3ds::io::View::Back:
                                    oldViews[i] = 'A';
                                    newViews[i] = 'A';
                                    break;
                                case _3ds::io::View::User:
                                    oldViews[i] = 'U';
                                    newViews[i] = 'U';
                                    break;
                                case _3ds::io::View::Shape:
                                    oldViews[i] = '*';
                                    newViews[i] = '*';
                                    break;
                            }
                        }
                        if (maximized) {
                            command::ViewLayoutToggleMaximizedCommand (viewLayout).Execute ();
                        }
                    }

                protected:
                    virtual BOOL OnInitDialog () {
                        CDialog::OnInitDialog ();
                        ((CButton *)GetDlgItem (107))->SetCheck (TRUE);
                        return TRUE;
                    }

                    struct ViewLayoutSetStyleCommand : public CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND

                        _3ds::io::Lofter &lofter;
                        const char *oldViews;
                        const char *newViews;

                        ViewLayoutSetStyleCommand (_3ds::io::Lofter &editor_,
                            const char *oldViews_, const char *newViews_) :
                            lofter (editor_), oldViews (oldViews_),
                            newViews (newViews_) {}

                        virtual bool Execute () {
                            _3ds::io::ViewLayout &viewLayout = lofter.viewLayout;
                            if (viewLayout.layout.maximized) {
                                ExecuteAndAddCommand (new command::ViewLayoutToggleMaximizedCommand (viewLayout));
                            }
                            ExecuteAndAddCommand (new ui16SetCommand (viewLayout.layout.currViewIndex, 0));
                            ExecuteAndAddCommand (new command::ViewLayoutSetSizeCommand (viewLayout, viewLayout.size));
                            for (util::ui32 i = 0; i < 4; ++i) {
                                switch (newViews[i]) {
                                    case 'T':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::TopView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'F':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::FrontView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'L':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::LeftView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'B':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::BottomView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'R':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::RightView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'A':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::BackView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'U':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (new stdVectorViewassignCommand (
                                                    viewLayout.views3, i, _3ds::io::UserView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (new command::ViewZoomBoundCommand (viewLayout[i],
                                                    _3ds::ext::Lofter (lofter).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'N':
                                        ExecuteAndAddCommand (new stdVectorViewassignCommand (
                                                viewLayout.views3, i, _3ds::io::NoneView (viewLayout[i].size)));
                                        break;
                                }
                            }
                            struct FinalOperation : public thekogans::mex::command::FinalOperation {
                                _3ds::io::ViewLayout &viewLayout;
                                FinalOperation (_3ds::io::ViewLayout &viewLayout_) : viewLayout (viewLayout_) {}
                                virtual bool Execute () {
                                    core::HideCursor hideCursor;
                                    assert (core::UI::Instance ().viewLayoutWindow != 0);
                                    core::UI::Instance ().viewLayoutWindow->Draw ();
                                    return true;
                                }
                            };
                            ExecuteAndSetFinalOperation (new FinalOperation (viewLayout));
                            return true;
                        }
                    };

                    virtual void OnOK () {
                        CDialog::OnOK ();
                        module.commandDispatcher.Dispatch (
                            ViewLayoutSetStyleCommand::UniquePtr (
                                new ViewLayoutSetStyleCommand (
                                    lofter, oldViews, newViews)));
                    }

                    afx_msg void OnPaint () {
                        CPaintDC dc (this);
                        DrawLayout ();
                    }

                    afx_msg void IDLayout () {
                        CWnd *wnd = GetDlgItem (115);
                        CRect rect;
                        wnd->GetClientRect (rect);
                        CPoint pt;
                        GetCursorPos (&pt);
                        wnd->ScreenToClient (&pt);
                        if (pt.x < rect.right / 3) {
                            if (pt.y < rect.bottom / 3) {
                                if (oldViews[0] != '*') {
                                    newViews[0] = currView;
                                }
                            }
                            else if (pt.y < 2 * rect.bottom / 3) {
                                if (oldViews[1] != '*') {
                                    newViews[1] = currView;
                                }
                            }
                            else {
                                if (oldViews[2] != '*') {
                                    newViews[2] = currView;
                                }
                            }
                        }
                        else {
                            if (oldViews[3] != '*') {
                                newViews[3] = currView;
                            }
                        }
                        DrawLayout ();
                    }

                    afx_msg void IDTopView () {
                        currView = 'T';
                    }

                    afx_msg void IDBottomView () {
                        currView = 'B';
                    }

                    afx_msg void IDUserView () {
                        currView = 'U';
                    }

                    afx_msg void IDLeftView () {
                        currView = 'L';
                    }

                    afx_msg void IDRightView () {
                        currView = 'R';
                    }

                    afx_msg void IDFrontView () {
                        currView = 'F';
                    }

                    afx_msg void IDBackView () {
                        currView = 'A';
                    }

                    afx_msg void IDNoneView () {
                        currView = 'N';
                    }

                    DECLARE_MESSAGE_MAP ()
                };

                BEGIN_MESSAGE_MAP (ViewportsDialog, CDialog)
                    ON_WM_PAINT ()
                    ON_BN_CLICKED (101, IDLayout)
                    ON_BN_CLICKED (102, IDTopView)
                    ON_BN_CLICKED (103, IDBottomView)
                    ON_BN_CLICKED (104, IDUserView)
                    ON_BN_CLICKED (105, IDLeftView)
                    ON_BN_CLICKED (106, IDRightView)
                    ON_BN_CLICKED (107, IDNoneView)
                    ON_BN_CLICKED (108, IDFrontView)
                    ON_BN_CLICKED (109, IDBackView)
                END_MESSAGE_MAP ()
            }

            struct MenuBarViewViewportsEventHandler : public core::MenuBar::Item::CommandEventHandler {
                DECLARE_MENUBAR_ITEM_EVENT_HANDLER (MenuBarViewViewportsEventHandler)

                core::Module &module;

                explicit MenuBarViewViewportsEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnSetFocus () {
                    AFX_MANAGE_STATE (AfxGetStaticModuleState ());
                    ViewportsDialog viewportsDialog (module);
                    viewportsDialog.DoModal ();
                }
            };

            IMPLEMENT_MENUBAR_ITEM_EVENT_HANDLER (MenuBarViewViewportsEventHandler, Lofter)

        } // namespace lofter
    } // namespace mex
} // namespace thekogans
#endif
