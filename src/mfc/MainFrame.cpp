#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <memory>
#include "thekogans/win/mfc/WindowPlacement.h"
#include "Util.h"
#include "MainFrame.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                BOOL MainFrame::PreTranslateMessage (MSG *pMsg) {
                    if (menuBarWindow.TranslateFrameMessage (pMsg)) {
                        return TRUE;
                    }
                    if (pMsg->message == WM_KEYDOWN) {
                        core::Module *module = core::Module::GetCurrModule ();
                        if (module && module->TranslateAccelerator (tolower (pMsg->wParam), GetControlKeyState ())) {
                            return TRUE;
                        }
                    }
                    return CFrameWnd::PreTranslateMessage (pMsg);
                }

                BOOL MainFrame::OnCmdMsg (UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo) {
                    return
                        // Route command messages to their proper destination.
                        menuBarWindow.OnCmdMsg (nID, nCode, pExtra, pHandlerInfo) ||
                        toolBarWindow.OnCmdMsg (nID, nCode, pExtra, pHandlerInfo) ||
                        speedBarFrame.OnCmdMsg (nID, nCode, pExtra, pHandlerInfo) ||
                        statusBarWindow.OnCmdMsg (nID, nCode, pExtra, pHandlerInfo) ||
                        CFrameWnd::OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
                }

                int MainFrame::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CFrameWnd::OnCreate (lpCreateStruct) == -1) {
                        return -1;
                    }

                    if (!menuBarWindow.CreateEx (this, WS_CHILD | WS_VISIBLE | CBRS_TOP |
                        CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_RAISEDBORDER)) {
                        TRACE0 ("Failed to create menuBarWindow\n");
                        return -1;
                    }

                    if (!toolBarWindow.Create (this, WS_CHILD | WS_VISIBLE | CBRS_TOP |
                        CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_RAISEDBORDER |
                        CBRS_TOOLTIPS | CBRS_FLYBY,
                        AFX_IDW_CONTROLBAR_FIRST + 32)) {
                        TRACE0 ("Failed to create toolBarWindow\n");
                        return -1;
                    }

                    if (!viewLayoutWindow.Create (NULL, NULL, AFX_WS_DEFAULT_VIEW,
                        CRect (0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
                        TRACE0 ("Failed to create viewLayoutWindow\n");
                        return -1;
                    }

                    if (!speedBarFrame.Create ("Speed Bar", this, CSize (80, 80), TRUE, 10)) {
                        TRACE0("Failed to create speedBarFrame\n");
                        return -1;
                    }
                    speedBarFrame.SetBarStyle (speedBarFrame.GetBarStyle () |
                        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

                    if (!consoleFrame.Create ("Console", this, CSize (80, 80), TRUE, 20)) {
                        TRACE0("Failed to create consoleFrame\n");
                        return -1;
                    }
                    consoleFrame.SetBarStyle (consoleFrame.GetBarStyle () |
                        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

                    if (!statusBarWindow.Create (this)) {
                        TRACE0("Failed to create statusBarWindow\n");
                        return -1;
                    }

                    // Each one of the above created UI components
                    // initializes its coresponding member in core::UI.
                    // This is just a sanity check.
                    assert (core::UI::Instance ().menuBarWindow != 0);
                    assert (core::UI::Instance ().toolBarWindow != 0);
                    assert (core::UI::Instance ().viewLayoutWindow != 0);
                    assert (core::UI::Instance ().speedBarWindow != 0);
                    assert (core::UI::Instance ().consoleWindow != 0);
                    assert (core::UI::Instance ().statusBarWindow != 0);
                    assert (core::UI::Instance ().frameBarWindow != 0);

                    EnableDocking (CBRS_ALIGN_ANY);

                    menuBarWindow.EnableDockingEx (CBRS_ALIGN_ANY);
                    toolBarWindow.EnableDocking (CBRS_ALIGN_ANY);
                    speedBarFrame.EnableDocking (CBRS_ALIGN_ANY);
                    consoleFrame.EnableDocking (CBRS_ALIGN_ANY);

                    DockControlBar (&menuBarWindow, AFX_IDW_DOCKBAR_TOP);
                    DockControlBar (&toolBarWindow, AFX_IDW_DOCKBAR_TOP);
                    DockControlBar (&speedBarFrame, AFX_IDW_DOCKBAR_RIGHT);
                    DockControlBar (&consoleFrame, AFX_IDW_DOCKBAR_BOTTOM);

                    win::mfc::WindowPlacement ().Restore ("Settings", this);
                    LoadBarState ("BarState");
                    win::mfc::SizingControlBar::GlobalLoadState ("BarState");
                    core::UI::Instance ().menuBarWindow->Restore ("MenuBarWindow");
                    core::UI::Instance ().toolBarWindow->Restore ("ToolBarWindow");
                    core::UI::Instance ().viewLayoutWindow->Restore ("ViewLayoutWindow");
                    core::UI::Instance ().speedBarWindow->Restore ("SpeedBarWindow");
                    core::UI::Instance ().consoleWindow->Restore ("ConsoleWindow");
                    core::UI::Instance ().statusBarWindow->Restore ("StatusBarWindow");
                    core::UI::Instance ().frameBarWindow->Restore ("FrameBarWindow");

                    assert (core::UI::Instance ().mainFrameWindow == 0);
                    core::UI::Instance ().mainFrameWindow = this;

                    return 0;
                }

                void MainFrame::OnClose () {
                    win::mfc::WindowPlacement ().Save ("Settings", this);
                    SaveBarState ("BarState");
                    win::mfc::SizingControlBar::GlobalSaveState ("BarState");
                    core::UI::Instance ().menuBarWindow->Save ("MenuBarWindow");
                    core::UI::Instance ().toolBarWindow->Save ("ToolBarWindow");
                    core::UI::Instance ().viewLayoutWindow->Save ("ViewLayoutWindow");
                    core::UI::Instance ().speedBarWindow->Save ("SpeedBarWindow");
                    core::UI::Instance ().consoleWindow->Save ("ConsoleWindow");
                    core::UI::Instance ().statusBarWindow->Save ("StatusBarWindow");
                    core::UI::Instance ().frameBarWindow->Save ("FrameBarWindow");
                    CFrameWnd::OnClose ();
                }

                void MainFrame::OnActivate (UINT nState, CWnd *pWndOther, BOOL bMinimized) {
                    if (IsWindow (viewLayoutWindow.GetSafeHwnd ())) {
                        viewLayoutWindow.SendMessage (WM_ACTIVATE, WPARAM (MAKELONG (nState, bMinimized)),
                            LPARAM (pWndOther->GetSafeHwnd ()));
                    }
                    CFrameWnd::OnActivate (nState, pWndOther, bMinimized);
                }

                void MainFrame::OnSetFocus (CWnd *pOldWnd) {
                    viewLayoutWindow.SetFocus ();
                }

                void MainFrame::OnEnterMenuLoop (BOOL bIsTrackPopupMenu) {
                    core::CursorMgr::Instance ().OnMouseMove (blas::Point (-1, -1));
                }

                void MainFrame::OnGetMinMaxInfo (LPMINMAXINFO lpMMI) {
                    CFrameWnd::OnGetMinMaxInfo (lpMMI);
                    lpMMI->ptMinTrackSize.x = 800;
                    lpMMI->ptMinTrackSize.y = 600;
                }

                BOOL MainFrame::OnEraseBkgnd (CDC *) {
                    return TRUE;
                }

                BOOL MainFrame::OnToolTipText (UINT nID, NMHDR *pNMHDR, LRESULT *pResult) {
                    return FALSE;
                }

                BEGIN_MESSAGE_MAP (MainFrame, CFrameWnd)
                    ON_WM_CREATE ()
                    ON_WM_CLOSE ()
                    ON_WM_ACTIVATE ()
                    ON_WM_SETFOCUS ()
                    ON_WM_ENTERMENULOOP ()
                    ON_WM_GETMINMAXINFO ()
                    ON_WM_ERASEBKGND ()
                    ON_NOTIFY_EX_RANGE (TTN_NEEDTEXTW, 0, 0xffff, OnToolTipText)
                    ON_NOTIFY_EX_RANGE (TTN_NEEDTEXTA, 0, 0xffff, OnToolTipText)
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
