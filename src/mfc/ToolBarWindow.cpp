#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxpriv.h>
#include "thekogans/win/util/Error.h"
#include "thekogans/mex/3ds/mex/core/UI.h"
#include "ToolBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                namespace {
                    const UINT ID_TOOLBAR_FIRST = 22401;
                    const UINT ID_TOOLBAR_LAST = 22500;

                    util::Image::RGBA Map (const util::Image::RGBA &rgba) {
                        struct ColorMap {
                            DWORD from;
                            int to;
                        } static const colorMap[] = {
                            {RGB (0x00, 0x00, 0x00), COLOR_BTNTEXT},       // black
                            {RGB (0x80, 0x80, 0x80), COLOR_BTNSHADOW},     // dark gray
                            {RGB (0xC0, 0xC0, 0xC0), COLOR_BTNFACE},       // bright gray
                            {RGB (0xFF, 0xFF, 0xFF), COLOR_BTNHIGHLIGHT}   // white
                        };
                        for (util::ui32 i = 0; i < 4; ++i) {
                            if (RGB (rgba.r, rgba.g, rgba.b) == colorMap[i].from) {
                                COLORREF color = GetSysColor (colorMap[i].to);
                                return util::Image::RGBA (GetBValue (color), GetGValue (color), GetRValue (color), 0);
                            }
                        }
                        return rgba;
                    }

                    HBITMAP HBITMAPFromImage (const util::Image &image) {
                        HBITMAP hbitmap = 0;
                        HDC hdcScreen = GetDC (0);
                        if (hdcScreen != 0) {
                            hbitmap = CreateCompatibleBitmap (hdcScreen, image.width, image.height);
                            if (hbitmap != 0) {
                                HDC hdcImage = CreateCompatibleDC (hdcScreen);
                                if (hdcImage != 0) {
                                    HBITMAP hbitmapOld = (HBITMAP)SelectObject (hdcImage, hbitmap);
                                    BITMAPINFOHEADER bitmapInfo;
                                    bitmapInfo.biSize = sizeof (BITMAPINFOHEADER);
                                    bitmapInfo.biWidth = image.width;
                                    bitmapInfo.biHeight = image.height;
                                    bitmapInfo.biPlanes = 1;
                                    bitmapInfo.biBitCount = 32;
                                    bitmapInfo.biCompression = BI_RGB;
                                    bitmapInfo.biSizeImage = image.bits.size () * sizeof (util::Image::RGBA);
                                    bitmapInfo.biXPelsPerMeter = 0;
                                    bitmapInfo.biYPelsPerMeter = 0;
                                    bitmapInfo.biClrUsed = 0;
                                    bitmapInfo.biClrImportant = 0;
                                    std::vector<util::Image::RGBA> bits;
                                    bits.resize (image.bits.size ());
                                    for (std::size_t i = 0, count = image.bits.size (); i < count; ++i) {
                                        bits[i] = Map (image.bits[i]);
                                    }
                                    int rc = StretchDIBits (hdcImage, 0, 0, image.width, image.height, 0, 0,
                                        image.width, image.height, &bits[0], (LPBITMAPINFO)&bitmapInfo,
                                        DIB_RGB_COLORS, SRCCOPY);
                                    if (rc == GDI_ERROR) {
                                        assert (core::UI::Instance ()->consoleWindow != 0);
                                        core::UI::Instance ()->consoleWindow->Print (
                                            "ToolBarWindow: StretchDIBits - '%s'.\n",
                                            win::util::Error (THEKOGANS_UTIL_OS_ERROR_CODE).message.c_str ());
                                    }
                                    SelectObject (hdcImage, hbitmapOld);
                                    DeleteDC (hdcImage);
                                }
                            }
                            ReleaseDC (0, hdcScreen);
                        }
                        return hbitmap;
                    }
                }

                void ToolBarWindow::SetToolBar (core::ToolBar &toolBar_) {
                    Flush ();
                    toolBar = &toolBar_;

                    CSize sizeButton (toolBar->width + 7, toolBar->height + 7);
                    CSize sizeImage (toolBar->width, toolBar->height);
                    SetSizes (sizeButton, sizeImage);

                    TBBUTTON button;
                    memset (&button, 0, sizeof (TBBUTTON));
                    button.iString = -1;

                    UINT nID = ID_TOOLBAR_FIRST;

                    std::vector<core::ToolBar::Item *> &items = toolBar->items;
                    for (std::size_t i = 0; i < items.size (); ++i, ++nID) {
                        button.fsState = TBSTATE_ENABLED;
                        if (items[i] == 0) {
                            // separator
                            button.fsStyle = TBSTYLE_SEP;
                            button.iBitmap = 8;
                            bitmaps.push_back (0);
                        }
                        else {
                            button.idCommand = nID;
                            button.fsStyle = TBSTYLE_BUTTON;
                            TBADDBITMAP addBitmap;
                            addBitmap.hInst = NULL; // makes TBADDBITMAP::nID behave like a HBITMAP
                            HBITMAP bitmap = HBITMAPFromImage (*items[i]->image);
                            addBitmap.nID = (UINT)bitmap;
                            button.iBitmap = DefWindowProc (TB_ADDBITMAP, 1, (LPARAM)&addBitmap);
                            bitmaps.push_back (bitmap);
                            if (button.iBitmap == -1) {
                                Flush ();
                                return;
                            }
                        }
                        if (!DefWindowProc (TB_ADDBUTTONS, 1, (LPARAM)&button)) {
                            Flush ();
                            return;
                        }
                    }

                    GetTopLevelFrame ()->RecalcLayout ();
                }

                const char *ToolBarWindow::GetItemText (UINT nID, int type) {
                    if (toolBar != 0) {
                        core::ToolBar::Item *item = toolBar->items[nID - ID_TOOLBAR_FIRST];
                        if (item != 0) {
                            return type == 1 ? item->toolTipText.c_str () : item->statusBarText.c_str ();
                        }
                    }
                    return 0;
                }

                void ToolBarWindow::Flush () {
                    {
                        for (util::ui32 count = (util::ui32)DefWindowProc (TB_BUTTONCOUNT, 0, 0); count--;) {
                            VERIFY (DefWindowProc (TB_DELETEBUTTON, 0, 0));
                        }
                    }
                    {
                        for (std::size_t i = 0, count = bitmaps.size (); i < count; ++i) {
                            if (bitmaps[i] != 0) {
                                DeleteObject (bitmaps[i]);
                            }
                        }
                        bitmaps.clear ();
                    }
                }

                void ToolBarWindow::DoButton (CPoint point, int type) {
                    if (toolBar != 0) {
                        int nID = OnToolHitTest (point, 0);
                        if (nID != -1) {
                            core::ToolBar::Item *item = toolBar->items[nID - ID_TOOLBAR_FIRST];
                            if (item != 0) {
                                if (type == 1) {
                                    item->eventHandler->OnClickLeft ();
                                }
                                else {
                                    item->eventHandler->OnClickRight ();
                                }
                            }
                        }
                    }
                }

                int ToolBarWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CToolBar::OnCreate (lpCreateStruct) == -1) {
                        return -1;
                    }
                    assert (core::UI::Instance ()->toolBarWindow == 0);
                    core::UI::Instance ()->toolBarWindow = this;
                    return 0;
                }

                void ToolBarWindow::OnLButtonUp (UINT nFlags, CPoint point) {
                    CToolBar::OnLButtonUp (nFlags, point);
                    if (!rightButton) {
                        DoButton (point, 1);
                    }
                }

                void ToolBarWindow::OnRButtonDown (UINT nFlags, CPoint point) {
                    nFlags &= ~MK_RBUTTON;
                    nFlags |= MK_LBUTTON;
                    SendMessage (WM_LBUTTONDOWN, nFlags, MAKELONG (point.x, point.y));
                }

                void ToolBarWindow::OnRButtonUp (UINT nFlags, CPoint point) {
                    nFlags &= ~MK_RBUTTON;
                    nFlags |= MK_LBUTTON;
                    rightButton = true;
                    SendMessage (WM_LBUTTONUP, nFlags, MAKELONG (point.x, point.y));
                    rightButton = false;
                    DoButton (point, 2);
                }

                void ToolBarWindow::OnCommand (UINT nID) {
                    // Handled by DoButton
                }

                void ToolBarWindow::OnUpdateCommandUI (CCmdUI *pCmdUI) {
                    if (toolBar != 0) {
                        core::ToolBar::Item *item = toolBar->items[pCmdUI->m_nID - ID_TOOLBAR_FIRST];
                        if (item != 0) {
                            struct UI : public core::ToolBar::Item::EventHandler::UI {
                                CCmdUI &m_pCmdUI;
                                std::vector<HBITMAP> &bitmaps;
                                std::size_t index;

                                UI (core::ToolBar::Item &item, CCmdUI &pCmdUI,
                                    std::vector<HBITMAP> &bitmaps_, std::size_t index_) :
                                    core::ToolBar::Item::EventHandler::UI (item),
                                    m_pCmdUI (pCmdUI), bitmaps (bitmaps_), index (index_) {}

                                virtual void SetEnabled (bool enabled) {m_pCmdUI.Enable (enabled);}
                                virtual void SetCheckable (bool checked) {}
                                virtual void SetChecked (bool checked) {m_pCmdUI.SetCheck (checked);}
                                virtual void SetImage () {
                                    HBITMAP bitmap = HBITMAPFromImage (*item.image);
                                    TBREPLACEBITMAP replaceBitmap;
                                    replaceBitmap.hInstOld = 0;
                                    replaceBitmap.nIDOld = (UINT)bitmaps[index];
                                    replaceBitmap.hInstNew = 0;
                                    replaceBitmap.nIDNew = (UINT)bitmap;
                                    replaceBitmap.nButtons = 1;
                                    if (m_pCmdUI.m_pOther->SendMessage (
                                        TB_REPLACEBITMAP, 0, (LPARAM)&replaceBitmap) != 0) {
                                        bitmaps.push_back (bitmaps[index]);
                                        bitmaps[index] = bitmap;
                                    }
                                    else {
                                        DeleteObject (bitmap);
                                    }
                                }
                            };
                            UI ui (*item, *pCmdUI, bitmaps, pCmdUI->m_nID - ID_TOOLBAR_FIRST);
                            item->eventHandler->OnUpdateUI (ui);
                        }
                    }
                }

                BOOL ToolBarWindow::OnToolTipText (UINT, NMHDR *pNMHDR, LRESULT *pResult) {
                    ASSERT (pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

                    // need to handle both ANSI and UNICODE versions of the message
                    TOOLTIPTEXTA *pTTTA = (TOOLTIPTEXTA *)pNMHDR;
                    TOOLTIPTEXTW *pTTTW = (TOOLTIPTEXTW *)pNMHDR;
                    UINT nID = pNMHDR->idFrom;
                    if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
                        pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND)) {
                        // idFrom is actually the HWND of the tool
                        nID = ((UINT)(WORD)::GetDlgCtrlID ((HWND)nID));
                    }

                    const char *strTipText = GetItemText (nID, 1);

                    if (!strTipText) {
                        return FALSE;
                    }

                    #define _countof(array) (sizeof (array) / sizeof (array[0]))
                    if (pNMHDR->code == TTN_NEEDTEXTA) {
                        lstrcpyn (pTTTA->szText, strTipText, _countof (pTTTA->szText));
                    }
                    else {
                        _mbstowcsz (pTTTW->szText, strTipText, _countof (pTTTW->szText));
                    }
                    #undef _countof

                    *pResult = 0;

                    // bring the tooltip window above other popup windows
                    ::SetWindowPos (pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
                        SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_NOOWNERZORDER);

                    return TRUE;    // message was handled
                }

                #define ID_TIMER_WAIT   0xE000  // timer while waiting to show status
                #define ID_TIMER_CHECK  0xE001  // timer to check for removal of status

                BOOL ToolBarWindow::SetStatusText (int nHit) {
                    CWnd *pOwner = GetOwner ();
                    _AFX_THREAD_STATE *pThreadState = AfxGetThreadState ();
                    if (nHit == -1) {
                        // handle reset case
                        pThreadState->m_pLastStatus = NULL;
                        if (m_nStateFlags & statusSet) {
                            pOwner->SendMessage (WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
                            m_nStateFlags &= ~statusSet;
                            return TRUE;
                        }
                        KillTimer (ID_TIMER_WAIT);
                    }
                    else {
                        // handle setnew case
                        if (!(m_nStateFlags & statusSet) || pThreadState->m_nLastStatus != nHit) {
                            pThreadState->m_pLastStatus = this;
                            pOwner->SendMessage (WM_SETMESSAGESTRING, 0, (LPARAM)GetItemText (nHit, 2));
                            m_nStateFlags |= statusSet;
                            ResetTimer (ID_TIMER_CHECK, 200);
                            return TRUE;
                        }
                    }
                    return FALSE;
                }

                BEGIN_MESSAGE_MAP (ToolBarWindow, CToolBar)
                    ON_WM_CREATE ()
                    ON_WM_LBUTTONUP ()
                    ON_WM_RBUTTONDOWN ()
                    ON_WM_RBUTTONUP ()
                    ON_COMMAND_RANGE (ID_TOOLBAR_FIRST, ID_TOOLBAR_LAST, OnCommand)
                    ON_UPDATE_COMMAND_UI_RANGE (ID_TOOLBAR_FIRST, ID_TOOLBAR_LAST, OnUpdateCommandUI)
                    // toolbar "tooltip" notification
                    ON_NOTIFY_EX_RANGE (TTN_NEEDTEXTW, 0, 0xffff, OnToolTipText)
                    ON_NOTIFY_EX_RANGE (TTN_NEEDTEXTA, 0, 0xffff, OnToolTipText)
                END_MESSAGE_MAP()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
