#include "ConsoleFrame.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                int ConsoleFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (SizingControlBar::OnCreate (lpCreateStruct) == -1 ||
                        !consoleWindow.Create (WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY | 0x1044,
                            CRect (0, 0, 0, 0), this, 1001)) {
                        return -1;
                    }
                    consoleWindow.ModifyStyleEx (0, WS_EX_CLIENTEDGE);
                    return 0;
                }

                void ConsoleFrame::OnSize (UINT nType, int cx, int cy) {
                    SizingControlBar::OnSize (nType, cx, cy);
                    CRect rect;
                    GetClientRect (rect);
                    consoleWindow.SetWindowPos (NULL, rect.left, rect.top,
                        rect.Width (), rect.Height (), SWP_NOZORDER);
                }

                void ConsoleFrame::OnPaint () {
                    SizingControlBar::OnPaint ();
                    CRect rect;
                    GetClientRect (rect);
                    CDC *pDC = GetDC ();
                    CBrush brush (GetSysColor (COLOR_BTNFACE));
                    pDC->FillRect (&rect, &brush);
                    ReleaseDC (pDC);
                }

                BEGIN_MESSAGE_MAP (ConsoleFrame, win::mfc::SizingControlBar)
                    ON_WM_CREATE ()
                    ON_WM_SIZE ()
                    ON_WM_PAINT ()
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
