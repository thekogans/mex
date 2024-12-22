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
#include "thekogans/mex/shaper/Module.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                class ViewportsDialog : public CDialog {
                private:
                    core::Module &module;
                    util::ui16 currStyle;

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
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::TwoVert:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (MX + 1, T + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::TwoHorz:
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (T + 1, MY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::Four:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (MX + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, MY + 1, "*", 1);
                                dc->TextOut (MX + 1, MY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeLeft:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->MoveTo (L, MY);
                                dc->LineTo (MX, MY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (MX + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, MY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeBottom:
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->MoveTo (MX, MY);
                                dc->LineTo (MX, B);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, MY + 1, "*", 1);
                                dc->TextOut (MX + 1, MY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeRight:
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, B);
                                dc->MoveTo (MX, MY);
                                dc->LineTo (R, MY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (MX + 1, T + 1, "*", 1);
                                dc->TextOut (MX + 1, MY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeTop:
                                dc->MoveTo (L, MY);
                                dc->LineTo (R, MY);
                                dc->MoveTo (MX, T);
                                dc->LineTo (MX, MY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (MX + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, MY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeVert:
                                dc->MoveTo (TX, T);
                                dc->LineTo (TX, B);
                                dc->MoveTo (TTX, T);
                                dc->LineTo (TTX, B);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (TX + 1, T + 1, "*", 1);
                                dc->TextOut (TTX + 1, T + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::ThreeHorz:
                                dc->MoveTo (L, TY);
                                dc->LineTo (R, TY);
                                dc->MoveTo (L, TTY);
                                dc->LineTo (R, TTY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, TY + 1, "*", 1);
                                dc->TextOut (L + 1, TTY + 1, "*", 1);
                                break;
                            case _3ds::io::ViewLayout::Layout::FourLeft:
                                dc->MoveTo (TX, T);
                                dc->LineTo (TX, B);
                                dc->MoveTo (L, TY);
                                dc->LineTo (TX, TY);
                                dc->MoveTo (L, TTY);
                                dc->LineTo (TX, TTY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, TY + 1, "*", 1);
                                dc->TextOut (L + 1, TTY + 1, "*", 1);
                                dc->TextOut (TX + 1, T + 1, "*", 1);
                                break;
                        #if EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourBottom:
                                dc->MoveTo (L, TTY);
                                dc->LineTo (R, TTY);
                                dc->MoveTo (TX, TTY);
                                dc->LineTo (TX, B);
                                dc->MoveTo (TTX, TTY);
                                dc->LineTo (TTX, B);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, TTY + 1, "*", 1);
                                dc->TextOut (TX + 1, TTY + 1, "*", 1);
                                dc->TextOut (TTX + 1, TTY + 1, "*", 1);
                                break;
                        #endif // EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourRight:
                                dc->MoveTo (TTX, T);
                                dc->LineTo (TTX, B);
                                dc->MoveTo (TTX, TY);
                                dc->LineTo (R, TY);
                                dc->MoveTo (TTX, TTY);
                                dc->LineTo (R, TTY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (TTX + 1, T + 1, "*", 1);
                                dc->TextOut (TTX + 1, TY + 1, "*", 1);
                                dc->TextOut (TTX + 1, TTY + 1, "*", 1);
                                break;
                        #if EXTENDED_VIEW_LAYOUT
                            case _3ds::io::ViewLayout::Layout::FourTop:
                                dc->MoveTo (L, TY);
                                dc->LineTo (R, TY);
                                dc->MoveTo (TX, T);
                                dc->LineTo (TX, TY);
                                dc->MoveTo (TTX, T);
                                dc->LineTo (TTX, TY);
                                dc->TextOut (L + 1, T + 1, "*", 1);
                                dc->TextOut (TX + 1, T + 1, "*", 1);
                                dc->TextOut (TTX + 1, T + 1, "*", 1);
                                dc->TextOut (L + 1, TY + 1, "*", 1);
                                break;
                        #endif // EXTENDED_VIEW_LAYOUT
                        }
                        wnd->ReleaseDC (dc);
                    }

                    void SetLayout (util::ui16 currStyle_) {
                        currStyle = currStyle_;
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
                            parent), module (module_),
                        currStyle (module.GetIOModule ().viewLayout.layout.currStyle) {}

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
                        return TRUE;
                    }

                    struct ViewLayoutSetStyleCommand : public CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND

                        _3ds::io::Shaper &shaper;
                        util::ui16 currStyle;

                        ViewLayoutSetStyleCommand (_3ds::io::Shaper &shaper_, util::ui16 currStyle_) :
                            shaper (shaper_), currStyle (currStyle_) {}

                        virtual bool Execute () {
                            _3ds::io::ViewLayout &viewLayout = shaper.viewLayout;
                            if (viewLayout.layout.maximized) {
                                ExecuteAndAddCommand (new command::ViewLayoutToggleMaximizedCommand (viewLayout));
                            }
                            ExecuteAndAddCommand (new ui16SetCommand (viewLayout.layout.currStyle, currStyle));
                            ExecuteAndAddCommand (new ui16SetCommand (viewLayout.layout.currViewIndex, 0));
                            ExecuteAndAddCommand (new command::ViewLayoutSetSizeCommand (viewLayout, viewLayout.size));
                            for (std::size_t i = 0, count = viewLayout.GetViewCount (); i < count; ++i) {
                                if (!viewLayout[i].IsValid () || !viewLayout[i].IsShape ()) {
                                    ExecuteAndAddCommand (new stdVectorViewassignCommand (
                                            viewLayout.views3, i, _3ds::io::ShapeView (viewLayout[i].size)));
                                    ExecuteAndAddCommand (new command::ViewZoomBoundCommand (viewLayout[i],
                                            ext::Shaper (shaper).GetViewBound (_3ds::ext::View (viewLayout[i]))));
                                }
                            }
                            struct FinalOperation : public command::FinalOperation {
                                _3ds::io::ViewLayout &viewLayout;
                                explicit FinalOperation (_3ds::io::ViewLayout &viewLayout_) : viewLayout (viewLayout_) {}
                                virtual bool Execute () {
                                    core::HideCursor hideCursor;
                                    assert (core::UI::Instance ()->viewLayoutWindow != 0);
                                    core::UI::Instance ()->viewLayoutWindow->Draw ();
                                    return true;
                                }
                            };
                            ExecuteAndAddFinalOperation (new FinalOperation (viewLayout));
                            return true;
                        }
                    };

                    virtual void OnOK () {
                        CDialog::OnOK ();
                        module.commandDispatcher.Dispatch (
                            ViewLayoutSetStyleCommand::UniquePtr (
                                new ViewLayoutSetStyleCommand (
                                    static_cast<_3ds::io::Shaper &> (module.GetIOModule ()), currStyle)));
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

            IMPLEMENT_MENUBAR_ITEM_EVENT_HANDLER (MenuBarViewViewportsEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
#endif
