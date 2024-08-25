// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_keyframer.
//
// libthekogans_mex_keyframer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_keyframer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_keyframer. If not, see <http://www.gnu.org/licenses/>.

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
#include "thekogans/mex/keyframer/Module.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace keyframer {

            namespace {
                class SelectCameraDialog : public CDialog {
                private:
                    const std::vector<_3ds::io::CameraNode *> &cameraNodes;

                public:
                    CString name;

                public:
                    SelectCameraDialog (
                        const std::vector<_3ds::io::CameraNode *> &cameraNodes_,
                        CWnd *parent = 0) :
                        CDialog ("SelectCameraDialog", parent),
                        cameraNodes (cameraNodes_) {}

                protected:
                    virtual BOOL OnInitDialog () {
                        CDialog::OnInitDialog ();
                        CWnd *wnd = GetDlgItem (101);
                        for (std::size_t i = 0, count = cameraNodes.size (); i < count; ++i) {
                            ((CListBox *)wnd)->AddString (cameraNodes[i]->name.c_str ());
                        }
                        ((CListBox *)wnd)->SetCurSel (0);
                        return TRUE;
                    }

                    virtual void OnOK () {
                        CWnd *wnd = GetDlgItem (101);
                        ((CListBox *)wnd)->GetText (((CListBox *)wnd)->GetCurSel (), name);
                        CDialog::OnOK ();
                    }
                };

                class SelectSpotLightDialog : public CDialog {
                private:
                    const std::vector<_3ds::io::SpotLightNode *> &spotLightNodes;

                public:
                    CString name;

                public:
                    SelectSpotLightDialog (
                        const std::vector<_3ds::io::SpotLightNode *> &spotLightNodes_,
                        CWnd *parent = 0):
                        CDialog ("SelectSpotLightDialog", parent),
                        spotLightNodes (spotLightNodes_) {}

                protected:
                    virtual BOOL OnInitDialog () {
                        CDialog::OnInitDialog ();
                        CWnd *wnd = GetDlgItem (101);
                        for (std::size_t i = 0, count = spotLightNodes.size (); i < count; ++i) {
                            ((CListBox*)wnd)->AddString (spotLightNodes[i]->name.c_str ());
                        }
                        ((CListBox *)wnd)->SetCurSel (0);
                        return TRUE;
                    }

                    virtual void OnOK () {
                        CWnd *wnd = GetDlgItem (101);
                        ((CListBox *)wnd)->GetText (((CListBox *)wnd)->GetCurSel (), name);
                        CDialog::OnOK ();
                    }
                };

                class ViewportsDialog : public CDialog {
                private:
                    core::Module &module;
                    _3ds::io::Keyframer &keyframer;
                    util::ui16 currStyle;
                    char currView;
                    const _3ds::io::SpotLightNode *spotLightNode;
                    const _3ds::io::CameraNode *cameraNode;
                    char oldViews[4];
                    char newViews[4];
                    const _3ds::io::SpotLightNode *spotLightNodes[4];
                    const _3ds::io::CameraNode *cameraNodes[4];

                    void DrawLayout () {
                        CWnd *wnd = GetDlgItem (115);
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
                        switch (currStyle) {
                            case _3ds::io::ViewLayout::Layout::Single:
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::TwoVert:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (MX + 1, T + 1, &newViews[1], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::TwoHorz:
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (T + 1, MY + 1, &newViews[1], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::Four:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (MX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (L + 1, MY + 1, &newViews[2], 1);
                                dc->TextOut (MX + 1, MY + 1, &newViews[3], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeLeft:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->MoveTo (L, MY);
                                dc->LineTo (MX, MY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (MX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (L + 1, MY + 1, &newViews[2], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeBottom:
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->MoveTo (MX, MY);
                                dc->LineTo (MX, B);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (L + 1, MY + 1, &newViews[1], 1);
                                dc->TextOut (MX + 1, MY + 1, &newViews[2], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeRight:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->MoveTo (MX, MY);
                                dc->LineTo (R, MY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (MX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (MX + 1, MY + 1, &newViews[2], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeTop:
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, MY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (MX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (L + 1, MY + 1, &newViews[2], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeVert:
                                dc->MoveTo (TX, T);
                                dc->LineTo (TX, B);
                                dc->MoveTo (TTX, T);
                                dc->LineTo (TTX, B);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (TX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (TTX + 1, T + 1, &newViews[2], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeHorz:
                                dc->MoveTo (L, TY);
                                dc->LineTo (R, TY);
                                dc->MoveTo (L, TTY);
                                dc->LineTo (R, TTY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (L + 1, TY + 1, &newViews[1], 1);
                                dc->TextOut (L + 1, TTY + 1, &newViews[2], 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::FourLeft:
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
                                break;
                        #if EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourBottom:
                                dc->MoveTo (L, TTY);
                                dc->LineTo (R, TTY);
                                dc->MoveTo (TX, TTY);
                                dc->LineTo (TX, B);
                                dc->MoveTo (TTX, TTY);
                                dc->LineTo (TTX, B);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (L + 1, TTY + 1, &newViews[1], 1);
                                dc->TextOut (TX + 1, TTY + 1, &newViews[2], 1);
                                dc->TextOut (TTX + 1, TTY + 1, &newViews[3], 1);
                                break;
                        #endif // EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourRight:
                                dc->MoveTo (TTX, T);
                                dc->LineTo (TTX, B);
                                dc->MoveTo (TTX, TY);
                                dc->LineTo (R, TY);
                                dc->MoveTo (TTX, TTY);
                                dc->LineTo (R, TTY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (TTX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (TTX + 1, TY + 1, &newViews[2], 1);
                                dc->TextOut (TTX + 1, TTY + 1, &newViews[3], 1);
                                break;
                        #if EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourTop:
                                dc->MoveTo (L, TY);
                                dc->LineTo (R, TY);
                                dc->MoveTo (TX, T);
                                dc->LineTo (TX, TY);
                                dc->MoveTo (TTX, T);
                                dc->LineTo (TTX, TY);
                                dc->TextOut (L + 1, T + 1, &newViews[0], 1);
                                dc->TextOut (TX + 1, T + 1, &newViews[1], 1);
                                dc->TextOut (TTX + 1, T + 1, &newViews[2], 1);
                                dc->TextOut (L + 1, TY + 1, &newViews[3], 1);
                                break;
                        #endif // EXTENDED_VIEW_LAYOUT
                        }
                        wnd->ReleaseDC (dc);
                    }

                    void SetLayout (util::ui16 currStyle_) {
                        currStyle = currStyle_;
                        newViews[0] =
                            newViews[1] =
                            newViews[2] =
                            newViews[3] = 'N';
                        DrawLayout ();
                    }

                    CBitmapButton single;
                    CBitmapButton twovert;
                    CBitmapButton twohorz;
                    CBitmapButton four;
                    CBitmapButton threeleft;
                    CBitmapButton threebottom;
                    CBitmapButton threeright;
                    CBitmapButton threetop;
                    CBitmapButton threevert;
                    CBitmapButton threehorz;
                    CBitmapButton fourleft;
                #if EXTENDED_VIEW_LAYOUT
                    CBitmapButton fourbottom;
                #endif // EXTENDED_VIEW_LAYOUT
                    CBitmapButton fourright;
                #if EXTENDED_VIEW_LAYOUT
                    CBitmapButton fourtop;
                #endif // EXTENDED_VIEW_LAYOUT

                public:
                    ViewportsDialog (core::Module &module_, CWnd *parent = 0) :
                            CDialog (
                            #if EXTENDED_VIEW_LAYOUT
                                "ExtendedViewportsDialog",
                            #else // EXTENDED_VIEW_LAYOUT
                                "ViewportsDialog",
                            #endif // EXTENDED_VIEW_LAYOUT
                                parent),
                            module (module_),
                            keyframer (static_cast<_3ds::io::Keyframer &> (module.GetIOModule ())),
                            currStyle (keyframer.viewLayout.layout.currStyle),
                            currView ('N'),
                            spotLightNode (0),
                            cameraNode (0) {
                        _3ds::io::ViewLayout &viewLayout = keyframer.viewLayout;
                        bool maximized = viewLayout.layout.maximized;
                        if (maximized) {
                            command::ViewLayoutToggleMaximizedCommand (viewLayout).Execute ();
                        }
                        currStyle = viewLayout.layout.currStyle;
                        for (util::ui32 i = 0; i < 4; ++i) {
                            spotLightNodes[i] = 0;
                            cameraNodes[i] = 0;
                            switch (viewLayout[i].type) {
                                case _3ds::io::View::None:
                                    oldViews[i] = 'N';
                                    newViews[i] = 'N';
                                    break;
                                case _3ds::io::View::Top:
                                    oldViews[i] = 'T';
                                    newViews[i] = 'T';
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
                                case _3ds::io::View::SpotLight:
                                    oldViews[i] = '$';
                                    newViews[i] = '$';
                                    spotLightNodes[i] = keyframer.GetSpotLightNode (viewLayout[i].name);
                                    assert (spotLightNodes[i] != 0);
                                    break;
                                case _3ds::io::View::Camera:
                                    oldViews[i] = 'C';
                                    newViews[i] = 'C';
                                    cameraNodes[i] = keyframer.GetCameraNode (viewLayout[i].name);
                                    assert (cameraNodes[i] != 0);
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
                        VERIFY (single.AutoLoad (101, this));
                        VERIFY (twovert.AutoLoad (102, this));
                        VERIFY (twohorz.AutoLoad (103, this));
                        VERIFY (four.AutoLoad (104, this));
                        VERIFY (threeleft.AutoLoad (105, this));
                        VERIFY (threebottom.AutoLoad (106, this));
                        VERIFY (threeright.AutoLoad (107, this));
                        VERIFY (threetop.AutoLoad (108, this));
                        VERIFY (threevert.AutoLoad (109, this));
                        VERIFY (threehorz.AutoLoad (110, this));
                        VERIFY (fourleft.AutoLoad (111, this));
                    #if EXTENDED_VIEW_LAYOUT
                        VERIFY (fourbottom.AutoLoad (112, this));
                    #endif // EXTENDED_VIEW_LAYOUT
                        VERIFY (fourright.AutoLoad (113, this));
                    #if EXTENDED_VIEW_LAYOUT
                        VERIFY (fourtop.AutoLoad (114, this));
                    #endif // EXTENDED_VIEW_LAYOUT
                        if (keyframer.cameraNodes.empty ()) {
                            GetDlgItem (121)->EnableWindow (FALSE);
                        }
                        if (keyframer.spotLightNodes.empty ()) {
                            GetDlgItem (124)->EnableWindow (FALSE);
                        }
                        ((CButton *)GetDlgItem (125))->SetCheck (TRUE);
                        return TRUE;
                    }

                    struct ViewLayoutSetStyleCommand : public CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND

                        _3ds::io::Keyframer &keyframer;
                        util::ui16 currStyle;
                        const char *oldViews;
                        const char *newViews;
                        const _3ds::io::SpotLightNode **spotLightNodes;
                        const _3ds::io::CameraNode **cameraNodes;

                        ViewLayoutSetStyleCommand (
                            _3ds::io::Keyframer &editor_,
                            util::ui16 currStyle_,
                            const char *oldViews_,
                            const char *newViews_,
                            const _3ds::io::SpotLightNode **spotLightNodes_,
                            const _3ds::io::CameraNode **cameraNodes_) :
                            keyframer (editor_),
                            currStyle (currStyle_),
                            oldViews (oldViews_),
                            newViews (newViews_),
                            spotLightNodes (spotLightNodes_),
                            cameraNodes (cameraNodes_) {}

                        virtual bool Execute () {
                            _3ds::io::ViewLayout &viewLayout = keyframer.viewLayout;
                            if (viewLayout.layout.maximized) {
                                ExecuteAndAddCommand (
                                    new command::ViewLayoutToggleMaximizedCommand (viewLayout));
                            }
                            ExecuteAndAddCommand (
                                new ui16SetCommand (viewLayout.layout.currStyle, currStyle));
                            ExecuteAndAddCommand (
                                new ui16SetCommand (viewLayout.layout.currViewIndex, 0));
                            ExecuteAndAddCommand (
                                new command::ViewLayoutSetSizeCommand (viewLayout, viewLayout.size));
                            for (util::ui32 i = 0; i < 4; ++i) {
                                switch (newViews[i]) {
                                    case 'T':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::TopView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'F':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::FrontView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'L':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::LeftView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'B':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::BottomView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'R':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::RightView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'A':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::BackView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'U':
                                        if (oldViews[i] != newViews[i]) {
                                            ExecuteAndAddCommand (
                                                new stdVectorViewassignCommand (
                                                    viewLayout.views3,
                                                    i,
                                                    _3ds::io::UserView (viewLayout[i].size)));
                                            ExecuteAndAddCommand (
                                                new command::ViewZoomBoundCommand (
                                                    viewLayout[i],
                                                    _3ds::ext::Keyframer (keyframer).GetViewBound (
                                                        _3ds::ext::View (viewLayout[i]))));
                                        }
                                        break;
                                    case 'N':
                                        ExecuteAndAddCommand (
                                            new stdVectorViewassignCommand (
                                                viewLayout.views3,
                                                i,
                                                _3ds::io::NoneView (viewLayout[i].size)));
                                        break;
                                    case '$':
                                        ExecuteAndAddCommand (
                                            new stdVectorViewassignCommand (
                                                viewLayout.views3,
                                                i,
                                                _3ds::io::SpotLightView (
                                                    viewLayout[i].size,
                                                    spotLightNodes[i]->name.c_str ())));
                                        break;
                                    case 'C':
                                        ExecuteAndAddCommand (
                                            new stdVectorViewassignCommand (
                                                viewLayout.views3,
                                                i,
                                                _3ds::io::CameraView (
                                                    viewLayout[i].size,
                                                    cameraNodes[i]->name.c_str ())));
                                        break;
                                }
                            }
                            struct FinalOperation : public thekogans::mex::command::FinalOperation {
                                _3ds::io::ViewLayout &viewLayout;
                                FinalOperation (_3ds::io::ViewLayout &viewLayout_) :
                                    viewLayout (viewLayout_) {}
                                virtual bool Execute () {
                                    core::HideCursor hideCursor;
                                    assert (core::UI::Instance ()->viewLayoutWindow != 0);
                                    core::UI::Instance ()->viewLayoutWindow->Draw ();
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
                                    keyframer, currStyle, oldViews,
                                    newViews, spotLightNodes, cameraNodes)));
                    }

                    afx_msg void OnPaint () {
                        CPaintDC dc (this);
                        DrawLayout ();
                    }

                    afx_msg void IDSingle () {
                        SetLayout (_3ds::io::ViewLayout::Layout::Single);
                    }

                    afx_msg void IDTwoVert () {
                        SetLayout (_3ds::io::ViewLayout::Layout::TwoVert);
                    }

                    afx_msg void IDTwoHorz () {
                        SetLayout (_3ds::io::ViewLayout::Layout::TwoHorz);
                    }

                    afx_msg void IDFour () {
                        SetLayout (_3ds::io::ViewLayout::Layout::Four);
                    }

                    afx_msg void IDThreeLeft () {
                        SetLayout (_3ds::io::ViewLayout::Layout::ThreeLeft);
                    }

                    afx_msg void IDThreeBottom () {
                        SetLayout (_3ds::io::ViewLayout::Layout::ThreeBottom);
                    }

                    afx_msg void IDThreeRight () {
                        SetLayout (_3ds::io::ViewLayout::Layout::ThreeRight);
                    }

                    afx_msg void IDThreeTop () {
                        SetLayout (_3ds::io::ViewLayout::Layout::ThreeTop);
                    }

                    afx_msg void IDThreeVert () {
                        SetLayout (_3ds::io::ViewLayout::Layout::ThreeVert);
                    }

                    afx_msg void IDThreeHorz () {
                        SetLayout (_3ds::io::ViewLayout::Layout::ThreeHorz);
                    }

                    afx_msg void IDFourLeft () {
                        SetLayout (_3ds::io::ViewLayout::Layout::FourLeft);
                    }

#if EXTENDED_VIEW_LAYOUT
                    afx_msg void IDFourBottom () {
                        SetLayout (_3ds::io::ViewLayout::Layout::FourBottom);
                    }
#endif // EXTENDED_VIEW_LAYOUT

                    afx_msg void IDFourRight () {
                        SetLayout (_3ds::io::ViewLayout::Layout::FourRight);
                    }

#if EXTENDED_VIEW_LAYOUT
                    afx_msg void IDFourTop () {
                        SetLayout (_3ds::io::ViewLayout::Layout::FourTop);
                    }
#endif // EXTENDED_VIEW_LAYOUT

                    afx_msg void IDLayout () {
                        CWnd *wnd = GetDlgItem (115);
                        CRect rect;
                        wnd->GetClientRect (rect);
                        CPoint pt;
                        GetCursorPos (&pt);
                        wnd->ScreenToClient (&pt);
                        switch (currStyle) {
                            case _3ds::io::ViewLayout::Layout::Single:
                                newViews[0] = currView;
                                spotLightNodes[0] = spotLightNode;
                                cameraNodes[0] = cameraNode;
                                break;
                            case _3ds::io::ViewLayout::Layout::TwoVert:
                                if (pt.x < rect.right / 2) {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                else {
                                    newViews[1] = currView;
                                    spotLightNodes[1] = spotLightNode;
                                    cameraNodes[1] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::TwoHorz:
                                if (pt.y < rect.bottom / 2) {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                else {
                                    newViews[1] = currView;
                                    spotLightNodes[1] = spotLightNode;
                                    cameraNodes[1] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::Four:
                                if (pt.x < rect.right / 2 && pt.y < rect.bottom / 2) {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                else if (pt.x > rect.right / 2 && pt.y < rect.bottom / 2) {
                                    newViews[1] = currView;
                                    spotLightNodes[1] = spotLightNode;
                                    cameraNodes[1] = cameraNode;
                                }
                                else if (pt.x < rect.right / 2 && pt.y > rect.bottom / 2) {
                                    newViews[2] = currView;
                                    spotLightNodes[2] = spotLightNode;
                                    cameraNodes[2] = cameraNode;
                                }
                                else {
                                    newViews[3] = currView;
                                    spotLightNodes[3] = spotLightNode;
                                    cameraNodes[3] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeLeft:
                                if (pt.x < rect.right / 2) {
                                    if (pt.y < rect.bottom / 2) {
                                        newViews[0] = currView;
                                        spotLightNodes[0] = spotLightNode;
                                        cameraNodes[0] = cameraNode;
                                    }
                                    else {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[1] = currView;
                                    spotLightNodes[1] = spotLightNode;
                                    cameraNodes[1] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeBottom:
                                if (pt.y > rect.bottom / 2) {
                                    if (pt.x < rect.right / 2) {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                    else {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeRight:
                                if (pt.x > rect.right / 2) {
                                    if (pt.y < rect.bottom / 2) {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                    else {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeTop:
                                if (pt.y < rect.bottom / 2) {
                                    if (pt.x < rect.right / 2) {
                                        newViews[0] = currView;
                                        spotLightNodes[0] = spotLightNode;
                                        cameraNodes[0] = cameraNode;
                                    }
                                    else {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[2] = currView;
                                    spotLightNodes[2] = spotLightNode;
                                    cameraNodes[2] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeVert:
                                if (pt.x < rect.right / 3) {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                else if (pt.x < 2 * rect.right / 3) {
                                    newViews[1] = currView;
                                    spotLightNodes[1] = spotLightNode;
                                    cameraNodes[1] = cameraNode;
                                }
                                else {
                                    newViews[2] = currView;
                                    spotLightNodes[2] = spotLightNode;
                                    cameraNodes[2] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeHorz:
                                if (pt.y < rect.bottom / 3) {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                else if (pt.y < 2 * rect.bottom / 3) {
                                    newViews[1] = currView;
                                    spotLightNodes[1] = spotLightNode;
                                    cameraNodes[1] = cameraNode;
                                }
                                else {
                                    newViews[2] = currView;
                                    spotLightNodes[2] = spotLightNode;
                                    cameraNodes[2] = cameraNode;
                                }
                                break;
                            case _3ds::io::ViewLayout::Layout::FourLeft:
                                if (pt.x < rect.right / 3) {
                                    if (pt.y < rect.bottom / 3) {
                                        newViews[0] = currView;
                                        spotLightNodes[0] = spotLightNode;
                                        cameraNodes[0] = cameraNode;
                                    }
                                    else if (pt.y < 2 * rect.bottom / 3) {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                    else {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[3] = currView;
                                    spotLightNodes[3] = spotLightNode;
                                    cameraNodes[3] = cameraNode;
                                }
                                break;
#if EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourBottom:
                                if (pt.y > 2 * rect.bottom / 3) {
                                    if (pt.x < rect.right / 3) {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                    else if (pt.x < 2 * rect.right / 3) {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                    else {
                                        newViews[3] = currView;
                                        spotLightNodes[3] = spotLightNode;
                                        cameraNodes[3] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                break;
#endif // EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourRight:
                                if (pt.x > 2 * rect.right / 3) {
                                    if (pt.y < rect.bottom / 3) {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                    else if (pt.y < 2 * rect.bottom / 3) {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                    else {
                                        newViews[3] = currView;
                                        spotLightNodes[3] = spotLightNode;
                                        cameraNodes[3] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[0] = currView;
                                    spotLightNodes[0] = spotLightNode;
                                    cameraNodes[0] = cameraNode;
                                }
                                break;
#if EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourTop:
                                if (pt.y < rect.bottom / 3) {
                                    if (pt.x < rect.right / 3) {
                                        newViews[0] = currView;
                                        spotLightNodes[0] = spotLightNode;
                                        cameraNodes[0] = cameraNode;
                                    }
                                    else if (pt.x < 2 * rect.right / 3) {
                                        newViews[1] = currView;
                                        spotLightNodes[1] = spotLightNode;
                                        cameraNodes[1] = cameraNode;
                                    }
                                    else {
                                        newViews[2] = currView;
                                        spotLightNodes[2] = spotLightNode;
                                        cameraNodes[2] = cameraNode;
                                    }
                                }
                                else {
                                    newViews[3] = currView;
                                    spotLightNodes[3] = spotLightNode;
                                    cameraNodes[3] = cameraNode;
                                }
                                break;
#endif // EXTENDED_VIEW_LAYOUT
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

                    afx_msg void IDCameraView () {
                        if (keyframer.cameraNodes.size () == 1) {
                            cameraNode = keyframer.cameraNodes[0];
                            currView = 'C';
                        }
                        else {
                            SelectCameraDialog selectCameraDialog (keyframer.cameraNodes, this);
                            if (selectCameraDialog.DoModal () == IDOK) {
                                cameraNode = keyframer.GetCameraNode ((LPCTSTR)selectCameraDialog.name);
                                currView = 'C';
                            }
                        }
                    }

                    afx_msg void IDFrontView () {
                        currView = 'F';
                    }

                    afx_msg void IDBackView () {
                        currView = 'A';
                    }

                    afx_msg void IDSpotLightView () {
                        if (keyframer.spotLightNodes.size () == 1) {
                            spotLightNode = keyframer.spotLightNodes[0];
                            currView = '$';
                        }
                        else {
                            SelectSpotLightDialog selectSpotLightDialog (keyframer.spotLightNodes, this);
                            if (selectSpotLightDialog.DoModal () == IDOK) {
                                spotLightNode = keyframer.GetSpotLightNode ((LPCTSTR)selectSpotLightDialog.name);
                                currView = '$';
                            }
                        }
                    }

                    afx_msg void IDNoneView () {
                        currView = 'N';
                    }

                    DECLARE_MESSAGE_MAP ()
                };

                BEGIN_MESSAGE_MAP (ViewportsDialog, CDialog)
                ON_WM_PAINT ()
                ON_BN_CLICKED (101, IDSingle)
                ON_BN_CLICKED (102, IDTwoVert)
                ON_BN_CLICKED (103, IDTwoHorz)
                ON_BN_CLICKED (104, IDFour)
                ON_BN_CLICKED (105, IDThreeLeft)
                ON_BN_CLICKED (106, IDThreeBottom)
                ON_BN_CLICKED (107, IDThreeRight)
                ON_BN_CLICKED (108, IDThreeTop)
                ON_BN_CLICKED (109, IDThreeVert)
                ON_BN_CLICKED (110, IDThreeHorz)
                ON_BN_CLICKED (111, IDFourLeft)
#if EXTENDED_VIEW_LAYOUT
                ON_BN_CLICKED (112, IDFourBottom)
#endif // EXTENDED_VIEW_LAYOUT
                ON_BN_CLICKED (113, IDFourRight)
#if EXTENDED_VIEW_LAYOUT
                ON_BN_CLICKED (114, IDFourTop)
#endif // EXTENDED_VIEW_LAYOUT
                ON_BN_CLICKED (115, IDLayout)
                ON_BN_CLICKED (116, IDTopView)
                ON_BN_CLICKED (117, IDBottomView)
                ON_BN_CLICKED (118, IDUserView)
                ON_BN_CLICKED (119, IDLeftView)
                ON_BN_CLICKED (120, IDRightView)
                ON_BN_CLICKED (121, IDCameraView)
                ON_BN_CLICKED (122, IDFrontView)
                ON_BN_CLICKED (123, IDBackView)
                ON_BN_CLICKED (124, IDSpotLightView)
                ON_BN_CLICKED (125, IDNoneView)
                END_MESSAGE_MAP ()
            }

            struct MenuBarViewViewportsEventHandler : public core::MenuBar::Item::CommandEventHandler {
                DECLARE_MENUBAR_ITEM_EVENT_HANDLER (MenuBarViewViewportsEventHandler)

                core::Module &module;

                explicit MenuBarViewViewportsEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnSetFocus () {
                    AFX_MANAGE_STATE (AfxGetStaticModuleState ());
                    ViewportsDialog viewportsDialog (module);
                    viewportsDialog.DoModal ();
                }
            };

            IMPLEMENT_MENUBAR_ITEM_EVENT_HANDLER (MenuBarViewViewportsEventHandler, Keyframer)

        } // namespace keyframer
    } // namespace mex
} // namespace thekogans
#endif
