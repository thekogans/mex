#include "thekogans/mex/3ds/mex/core/UI.h"
#include "SpeedBarFrame.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                int SpeedBarFrame::OnCreate (LPCREATESTRUCT lpCreateStruct)  {
                    if (SizingControlBar::OnCreate (lpCreateStruct) == -1 ||
                        !speedBarWindow.CreateEx (WS_EX_CLIENTEDGE, NULL, "",
                            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
                            CRect (0, 0, 0, 0), this, 1001)) {
                        return -1;
                    }
                    return 0;
                }

                void SpeedBarFrame::OnSize (UINT nType, int cx, int cy) {
                    SizingControlBar::OnSize (nType, cx, cy);
                    CRect rect;
                    GetClientRect (rect);
                    speedBarWindow.SetWindowPos (NULL, rect.left, rect.top,
                        rect.Width (), rect.Height (), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
                }

                BEGIN_MESSAGE_MAP (SpeedBarFrame, win::mfc::SizingControlBar)
                    ON_WM_CREATE ()
                    ON_WM_SIZE ()
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
