// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_editor.
//
// libthekogans_mex_editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_editor. If not, see <http://www.gnu.org/licenses/>.

#if 0
#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <afxext.h>
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/core/ToolBar.h"
#include "thekogans/mex/editor/Module.h"

namespace thekogans {
    namespace mex {
        namespace editor {

            namespace {
                const UINT PBN_COLORCHANGE = 1;
                const UINT PBN_DBLCLK = 2;

                struct NMPALETTEBUTTON {
                    NMHDR hdr;
                    util::ui32 index;
                };

                const util::ui32 ROWS = 4;
                const util::ui32 COLS = 16;
                const util::ui32 WIDTH = 20;
                const util::ui32 HEIGHT = 20;
                const util::ui32 SPACER = 1;
                const util::ui32 TRUE_WIDTH = SPACER + WIDTH + SPACER;
                const util::ui32 TRUE_HEIGHT = SPACER + HEIGHT + SPACER;

                class PaletteButton : public CButton {
                private:
                    util::ui32 row;
                    util::ui32 col;

                public:
                    PaletteButton (util::ui32 index) : row (index % ROWS), col (index / ROWS) {}

                    inline util::ui32 GetIndex () const {
                        return col * ROWS + row;
                    }

                private:
                    void OutlineCurrColor (HDC hdc, bool outline) {
                        HPEN hpen = (HPEN)SelectObject (hdc, outline ?
                            (HPEN)GetStockObject (WHITE_PEN) :
                            CreatePen (PS_SOLID, 1, GetSysColor (COLOR_3DFACE)));
                        SelectObject (hdc, GetStockObject (NULL_BRUSH));
                        util::ui32 left = col * TRUE_WIDTH;
                        util::ui32 top = row * TRUE_HEIGHT;
                        Rectangle (hdc, left, top, left + TRUE_WIDTH, top + TRUE_HEIGHT);
                        DeleteObject (SelectObject (hdc, hpen));
                    }

                    void NotifyParent (util::ui32 code) {
                        NMPALETTEBUTTON nm;
                        nm.hdr.hwndFrom = GetSafeHwnd ();
                        nm.hdr.idFrom = GetDlgCtrlID ();
                        nm.hdr.code = code;
                        nm.index = GetIndex ();
                        GetParent ()->SendMessage (WM_NOTIFY, GetDlgCtrlID (), (LPARAM)&nm);
                    }

                    virtual void DrawItem (LPDRAWITEMSTRUCT lpDIS) {
                        assert (lpDIS != 0);
                        CRect rect;
                        GetClientRect (&rect);
                        HBRUSH hbrush = (HBRUSH)SelectObject (lpDIS->hDC,
                            CreateSolidBrush (GetSysColor (COLOR_3DFACE)));
                        PatBlt (lpDIS->hDC, rect.left, rect.top, rect.Width (), rect.Height (), PATCOPY);
                        DeleteObject (SelectObject (lpDIS->hDC, hbrush));
                        util::ui32 index = _3ds::opengl::OBJECT_PALETTE_START_INDEX;
                        for (util::ui32 i = 0; i < COLS; ++i) {
                            for (util::ui32 j = 0; j < ROWS; ++j) {
                                HBRUSH hbrush = (HBRUSH)SelectObject (lpDIS->hDC,
                                    CreateSolidBrush (
                                        RGB (
                                            _3ds::opengl::DefaultPalette[index].r,
                                            _3ds::opengl::DefaultPalette[index].g,
                                            _3ds::opengl::DefaultPalette[index].b)));
                                ++index;
                                PatBlt (lpDIS->hDC,
                                    SPACER + i * TRUE_WIDTH,
                                    SPACER + j * TRUE_HEIGHT,
                                    WIDTH, HEIGHT, PATCOPY);
                                DeleteObject (SelectObject (lpDIS->hDC, hbrush));
                            }
                        }
                        OutlineCurrColor (lpDIS->hDC, true);
                    }

                    afx_msg void OnLButtonDown (UINT flags, CPoint dp) {
                        HDC hdc = ::GetDC (m_hWnd);
                        OutlineCurrColor (hdc, false);
                        row = dp.y / TRUE_HEIGHT;
                        col = dp.x / TRUE_WIDTH;
                        OutlineCurrColor (hdc, true);
                        ::ReleaseDC (m_hWnd, hdc);
                        NotifyParent (PBN_COLORCHANGE);
                    }

                    afx_msg void OnLButtonDblClk (UINT flags, CPoint dp) {
                        NotifyParent (PBN_DBLCLK);
                    }

                    DECLARE_MESSAGE_MAP ()
                };

                BEGIN_MESSAGE_MAP (PaletteButton, CButton)
                    ON_WM_LBUTTONDOWN ()
                    ON_WM_LBUTTONDBLCLK ()
                END_MESSAGE_MAP()

                class CreateColorDialog : public CDialog {
                private:
                    util::ui32 &index;
                    PaletteButton paletteButton;

                public:
                    CreateColorDialog (util::ui32 &index_, CWnd *parent = 0) :
                        CDialog ("CreateColorDialog", parent), index (index_),
                        paletteButton (index) {}

                protected:
                    inline void SetColor (util::ui32 index) {
                        GetDlgItem (102)->SetWindowText (
                            util::ui322string (index, "Color %u selected").c_str ());
                    }

                    virtual BOOL OnInitDialog () {
                        CDialog::OnInitDialog ();
                        VERIFY (paletteButton.SubclassDlgItem (101, this));
                        VERIFY (paletteButton.SetWindowPos (NULL, -1, -1,
                                COLS * TRUE_WIDTH, ROWS * TRUE_HEIGHT,
                                SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE));
                        SetColor (index);
                        return TRUE;
                    }

                    virtual void OnOK () {
                        index = paletteButton.GetIndex ();
                        CDialog::OnOK ();
                    }

                    afx_msg void OnColorChange (NMHDR *pNMHDR, LRESULT *pResult) {
                        SetColor (((NMPALETTEBUTTON *)pNMHDR)->index);
                        *pResult = 0;
                    }

                    afx_msg void OnDblClk (NMHDR *pNMHDR, LRESULT *pResult) {
                        OnOK ();
                        *pResult = 0;
                    }

                    DECLARE_MESSAGE_MAP ()
                };

                BEGIN_MESSAGE_MAP (CreateColorDialog, CDialog)
                    ON_NOTIFY (PBN_COLORCHANGE, 101, OnColorChange)
                    ON_NOTIFY (PBN_DBLCLK, 101, OnDblClk)
                END_MESSAGE_MAP ()
            }

            struct ToolBarCreateColorEventHandler : public core::ToolBar::Item::EventHandler {
                DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarCreateColorEventHandler)

                util::ui32 index;
                bool indexChanged;

                explicit ToolBarCreateColorEventHandler (core::Module &) :
                    index (0), indexChanged (false) {}

                virtual void OnClickLeft () {
                    AFX_MANAGE_STATE (AfxGetStaticModuleState ());
                    CreateColorDialog createColorDialog (index);
                    if (createColorDialog.DoModal () == IDOK) {
                        indexChanged = true;
                    }
                }

                virtual void OnUpdateUI (UI &ui) {
                    ui.SetEnabled (true);
                    ui.SetChecked (false);
                    if (indexChanged) {
                        std::vector<util::Image::RGBA> &bits = ui.item.image->bits;
                        for (std::size_t i = 0, count = bits.size (); i < count; ++i) {
                            // NOTE: r/b are reversed.
                            bits[i].b = _3ds::opengl::DefaultPalette[_3ds::opengl::OBJECT_PALETTE_START_INDEX + index].r;
                            bits[i].g = _3ds::opengl::DefaultPalette[_3ds::opengl::OBJECT_PALETTE_START_INDEX + index].g;
                            bits[i].r = _3ds::opengl::DefaultPalette[_3ds::opengl::OBJECT_PALETTE_START_INDEX + index].b;
                        }
                        ui.SetImage ();
                        indexChanged = false;
                    }
                }
            };

            IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarCreateColorEventHandler, Editor)

        } // namespace editor
    } // namespace mex
} // namespace thekogans
#endif
