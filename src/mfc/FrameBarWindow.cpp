#include "thekogans/util/Constants.h"
#include "FrameBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                void FrameBarWindow::Draw () {
                    CRect clientRect;
                    GetClientRect (&clientRect);
                    CDC *dc = GetDC ();
                    CDC memDC;
                    memDC.CreateCompatibleDC (dc);
                    CBitmap bitmap;
                    bitmap.CreateCompatibleBitmap (dc, clientRect.Width (), clientRect.Height ());
                    CGdiObject *pBitmap = memDC.SelectObject (&bitmap);
                    memDC.DrawEdge (&clientRect, EDGE_SUNKEN, BF_RECT);
                    clientRect.InflateRect (-1, -1);
                    CBrush brush;
                    brush.CreateSolidBrush (GetSysColor (COLOR_BTNFACE));
                    CGdiObject *pBrush = memDC.SelectObject (&brush);
                    memDC.PatBlt (clientRect.left, clientRect.top,
                        clientRect.Width (), clientRect.Height (), PATCOPY);
                    memDC.SelectObject (pBrush);
                    CFont font;
                    CGdiObject *pFont;
                    if (hfont) {
                        font.Attach (hfont);
                        pFont = memDC.SelectObject (&font);
                    }
                    memDC.SetTextColor (RGB (255, 255, 255));
                    memDC.SetBkColor (GetSysColor (COLOR_APPWORKSPACE));
                    CString str;
                    str.Format ("%d", GetCurrFrame ());
                    CRect posRect;
                    posRect.left = clientRect.left + GetInsideWidth (clientRect, &memDC) *
                        (util::f32)GetCurrFrame () / (util::f32)GetEndFrame ();
                    posRect.top = clientRect.top;
                    posRect.right = posRect.left + GetCurrFrameWidth (&memDC) + 1;
                    posRect.bottom = clientRect.bottom;
                    memDC.ExtTextOut (posRect.left, posRect.top, ETO_OPAQUE, &posRect, str, 0);
                    if (hfont) {
                        memDC.SelectObject (pFont);
                        font.Detach ();
                    }
                    clientRect.InflateRect (1, 1);
                    dc->BitBlt (clientRect.left, clientRect.top,
                        clientRect.Width (), clientRect.Height (),
                        &memDC, clientRect.left, clientRect.top, SRCCOPY);
                    memDC.SelectObject (pBitmap);
                    ReleaseDC (dc);
                }

                bool FrameBarWindow::GoToFrame (CPoint point) {
                    CRect rect;
                    GetClientRect (&rect);
                    rect.InflateRect (-1, -1);
                    int width = GetInsideWidth (rect);
                    if (point.x < rect.left) {
                        point.x = rect.left;
                    }
                    else if (point.x > rect.left + width) {
                        point.x = rect.left + width;
                    }
                    util::ui32 frame = util::LERP ((util::f32)point.x / width, GetStartFrame (), GetEndFrame ());
                    if (GetCurrFrame () != frame) {
                        Invalidate ();
                        if (eventSink != 0) {
                            eventSink->GoToFrame (frame);
                        }
                        return true;
                    }
                    return false;
                }

                int FrameBarWindow::GetInsideWidth (CRect &rect, CDC *dc) {
                    bool release = dc == 0;
                    if (release) {
                        dc = GetDC ();
                    }
                    CString str;
                    str.Format ("%d", GetEndFrame ());
                    CSize sz = dc->GetTextExtent (str);
                    int width = rect.Width () - sz.cx - 1;
                    if (release) {
                        ReleaseDC (dc);
                    }
                    return width;
                }

                int FrameBarWindow::GetCurrFrameWidth (CDC *dc) {
                    CString str;
                    str.Format ("%d", GetCurrFrame ());
                    CSize sz = dc->GetTextExtent (str);
                    return sz.cx;
                }

                int FrameBarWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CWnd::OnCreate (lpCreateStruct) == -1) {
                        return -1;
                    }
                    core::UI::Instance ().frameBarWindow = this;
                    return 0;
                }

                BOOL FrameBarWindow::OnEraseBkgnd (CDC *pDC) {
                    return TRUE;
                }

                void FrameBarWindow::OnPaint () {
                    CPaintDC dc (this);
                    Draw ();
                }

                void FrameBarWindow::OnLButtonDown (UINT nFlags, CPoint point) {
                    if (!dragging) {
                        SetCapture ();
                        dragging = true;
                        boxMode = IsBoxMode ();
                        SetBoxMode (true);
                        if (!GoToFrame (point)) {
                            core::UI::Instance ().viewLayoutWindow->Draw ();
                        }
                    }
                }

                void FrameBarWindow::OnLButtonUp (UINT nFlags, CPoint point) {
                    if (dragging) {
                        ReleaseCapture ();
                        dragging = false;
                        if (!boxMode) {
                            SetBoxMode (false);
                            core::UI::Instance ().viewLayoutWindow->Draw ();
                        }
                    }
                }

                void FrameBarWindow::OnMouseMove (UINT nFlags, CPoint point) {
                    if (dragging) {
                        GoToFrame (point);
                    }
                }

                BEGIN_MESSAGE_MAP (FrameBarWindow, CWnd)
                    ON_WM_CREATE ()
                    ON_WM_LBUTTONDOWN ()
                    ON_WM_LBUTTONUP ()
                    ON_WM_MOUSEMOVE ()
                    ON_WM_ERASEBKGND ()
                    ON_WM_PAINT ()
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
