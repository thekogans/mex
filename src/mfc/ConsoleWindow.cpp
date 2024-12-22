#include <cstdio>
#include "ConsoleWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                void ConsoleWindow::Print (const char *format, ...) {
                    assert (format != 0);
                    char str[512];
                    {
                        va_list argptr;
                        va_start (argptr, format);
                        vsnprintf (str, 512, format, argptr);
                        va_end (argptr);
                    }
                    int len = GetWindowTextLength ();
                    SetSel (len, len);
                    ReplaceSel (str);
                    // See if we need to scroll.
                    int nlines = GetLineCount ();
                    int first = GetFirstVisibleLine ();
                    CRect rect;
                    GetClientRect (&rect);
                    CDC *pDC = GetDC ();
                    TEXTMETRIC tm;
                    GetTextMetrics (pDC->GetSafeHdc (), &tm);
                    ReleaseDC (pDC);
                    int nvisible = rect.bottom / tm.tmHeight + 1;
                    if (nlines - first > nvisible) {
                        LineScroll (nlines - first - nvisible / 2 - 1);
                    }
                }

                int ConsoleWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CRichEditCtrl::OnCreate (lpCreateStruct) == -1) {
                        return -1;
                    }
                    assert (core::UI::Instance ()->consoleWindow == 0);
                    core::UI::Instance ()->consoleWindow = this;
                    return 0;
                }

                void ConsoleWindow::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags) {
                    switch (nChar) {
                        case VK_ESCAPE:
                            return;
                    }
                    CRichEditCtrl::OnKeyDown (nChar, nRepCnt, nFlags);
                }

                BEGIN_MESSAGE_MAP (ConsoleWindow, CRichEditCtrl)
                    ON_WM_CREATE ()
                    ON_WM_KEYDOWN ()
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
