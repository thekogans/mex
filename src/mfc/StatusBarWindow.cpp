#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxcmn.h>
#include "StatusBarWindow.h"

#define SBPF_UPDATE 0x0001  // pending update of text

struct AFX_STATUSPANE {
    UINT nID;           // IDC of indicator: 0 => normal text area
    int cxText;         // width of string area in pixels
                        //   on both sides there is a 3 pixel gap and
                        //   a one pixel border, making a pane 6 pixels wider
    UINT nStyle;        // style flags (SBPS_*)
    UINT nFlags;        // state flags (SBPF_*)
    CString strText;    // text in the pane
};

inline AFX_STATUSPANE *CStatusBar::_GetPanePtr (int nIndex) const {
    ASSERT ((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
    return ((AFX_STATUSPANE *)m_pData) + nIndex;
}

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                namespace {
                    const UINT ID_STATUSBAR_FIRST = 21501;
                    const UINT ID_STATUSBAR_LAST = 21600;
                }

                void StatusBarWindow::OnIdle () {
                    if (frameBarWindow.IsVisible () && frameBarWindow.IsWindowVisible () &&
                        GetCapture ()->GetSafeHwnd () != frameBarWindow.GetSafeHwnd ()) {
                        CPoint pt;
                        GetCursorPos (&pt);
                        CRect rect;
                        GetWindowRect (&rect);
                        if (!rect.PtInRect (pt)) {
                            frameBarWindow.ShowWindow (SW_HIDE);
                        }
                    }
                }

                void StatusBarWindow::SetStatusBar (core::StatusBar &statusBar_) {
                    statusBar = &statusBar_;
                    AllocElements (1 + statusBar->items.size (), sizeof (AFX_STATUSPANE));
                    HFONT hFont = (HFONT)SendMessage (WM_GETFONT);
                    CClientDC dcScreen (NULL);
                    HGDIOBJ hOldFont = NULL;
                    if (hFont != NULL) {
                        hOldFont = dcScreen.SelectObject(hFont);
                    }
                    AFX_STATUSPANE *pSBP = _GetPanePtr (0);
                    pSBP->nID = 0;
                    pSBP->cxText = 0;
                    pSBP->nStyle |= SBPS_STRETCH | SBPS_NOBORDERS;
                    pSBP->nFlags |= SBPF_UPDATE;
                    ++pSBP;
                    UINT nID = ID_STATUSBAR_FIRST;
                    std::vector<core::StatusBar::Item *> &items = statusBar->items;
                    for (std::size_t i = 0, count = items.size (); i < count; ++i) {
                        pSBP->nID = nID++;
                        pSBP->cxText = items[i]->GetWidth () * dcScreen.GetTextExtent (" ").cx;
                        pSBP->nFlags |= SBPF_UPDATE;
                        ++pSBP;
                    }
                    if (hOldFont) {
                        dcScreen.SelectObject (hOldFont);
                    }
                    UpdateAllPanes (TRUE, TRUE);
                }

                _3ds::io::Progress::UniquePtr StatusBarWindow::CreateProgress () {
                    class Progress : public _3ds::io::Progress {
                    protected:
                        CProgressCtrl progress;
                        util::f32 scale;

                    public:
                        Progress (DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, util::ui32 nID) : scale (1.0f) {
                            progress.Create (dwStyle, rect, pParentWnd, nID);
                        }

                        virtual void SetTotal (util::ui32 total) {
                            scale = 100.0f / total;
                        }

                        virtual void SetPosition (util::ui32 position) {
                            progress.SetPos ((util::i32)(position * scale));
                        }
                    };
                    RECT rect;
                    GetItemRect (0, &rect);
                    return _3ds::io::Progress::UniquePtr (new Progress (WS_VISIBLE | WS_CHILD, rect, this, 1));
                }

                int StatusBarWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CStatusBar::OnCreate (lpCreateStruct) == -1 ||
                        frameBarWindow.Create (AfxRegisterWndClass (CS_HREDRAW | CS_VREDRAW,
                            LoadCursor (0, IDC_ARROW)), "", WS_CHILD, CRect (0, 0, 0, 0), this, 1001) == FALSE) {
                        return -1;
                    }
                    frameBarWindow.SetFont ((HFONT)SendMessage (WM_GETFONT));
                    assert (core::UI::Instance ()->statusBarWindow == 0);
                    core::UI::Instance ()->statusBarWindow = this;
                    return 0;
                }

                void StatusBarWindow::OnSize (UINT nType, int cx, int cy) {
                    CStatusBar::OnSize (nType, cx, cy);
                    CRect rect;
                    GetClientRect (&rect);
                    CControlBar::CalcInsideRect (rect, TRUE);
                    frameBarWindow.SetWindowPos (0, rect.left, rect.top,
                        rect.Width (), rect.Height (), SWP_NOZORDER);
                }

                void StatusBarWindow::OnMouseMove (UINT nFlags, CPoint point) {
                    if (frameBarWindow.IsVisible () && ::GetActiveWindow () == GetParent ()->GetSafeHwnd ()) {
                        frameBarWindow.ShowWindow (SW_SHOW);
                    }
                }

                void StatusBarWindow::OnUpdateCommandUI (CCmdUI *cmdUI) {
                    if (statusBar != 0) {
                        class UI : public core::StatusBar::Item::EventHandler::UI {
                        private:
                            CCmdUI &cmdUI;

                        public:
                            UI (core::StatusBar::Item &item, CCmdUI &cmdUI_) :
                                core::StatusBar::Item::EventHandler::UI (item), cmdUI (cmdUI_) {}
                            virtual void SetText (const char *text) {cmdUI.SetText (text);}
                        };
                        std::vector<core::StatusBar::Item *> &items = statusBar->items;
                        items[cmdUI->m_nID - ID_STATUSBAR_FIRST]->GetEventHandler ()->OnUpdateUI (
                            UI (*items[cmdUI->m_nID - ID_STATUSBAR_FIRST], *cmdUI));
                    }
                }

                BEGIN_MESSAGE_MAP (StatusBarWindow, CStatusBar)
                    ON_WM_CREATE ()
                    ON_WM_SIZE ()
                    ON_WM_MOUSEMOVE ()
                    ON_UPDATE_COMMAND_UI_RANGE (ID_STATUSBAR_FIRST, ID_STATUSBAR_LAST, OnUpdateCommandUI)
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
