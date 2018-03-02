#if !defined (__thekogans_3ds_mex_ui_MainFrame_h)
#define __thekogans_3ds_mex_ui_MainFrame_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include "thekogans/mex/3ds/mex/core/UI.h"
#include "MenuBarWindow.h"
#include "ToolBarWindow.h"
#include "ViewLayoutWindow.h"
#include "SpeedBarFrame.h"
#include "ConsoleFrame.h"
#include "StatusBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct MainFrame : public CFrameWnd, public core::UI::MainFrameWindow {
                public:
                    MenuBarWindow menuBarWindow;
                    ToolBarWindow toolBarWindow;
                    ViewLayoutWindow viewLayoutWindow;
                    SpeedBarFrame speedBarFrame;
                    ConsoleFrame consoleFrame;
                    StatusBarWindow statusBarWindow;

                    virtual bool IsMenuBarWindowVisible () const {
                        return menuBarWindow.IsVisible () == TRUE;
                    }
                    virtual void ShowMenuBarWindow (bool show) {
                        ShowControlBar (&menuBarWindow, show ? TRUE : FALSE, FALSE);
                    }
                    virtual bool IsToolBarWindowVisible () const {
                        return toolBarWindow.IsVisible () == TRUE;
                    }
                    virtual void ShowToolBarWindow (bool show) {
                        ShowControlBar (&toolBarWindow, show ? TRUE : FALSE, FALSE);
                    }
                    virtual bool IsSpeedBarWindowVisible () const {
                        return speedBarFrame.IsVisible () == TRUE;
                    }
                    virtual void ShowSpeedBarWindow (bool show) {
                        ShowControlBar (&speedBarFrame, show ? TRUE : FALSE, FALSE);
                    }
                    virtual bool IsConsoleWindowVisible () const {
                        return consoleFrame.IsVisible () == TRUE;
                    }
                    virtual void ShowConsoleWindow (bool show) {
                        ShowControlBar (&consoleFrame, show ? TRUE : FALSE, FALSE);
                    }
                    virtual bool IsStatusBarWindowVisible () const {
                        return statusBarWindow.IsVisible () == TRUE;
                    }
                    virtual void ShowStatusBarWindow (bool show) {
                        ShowControlBar (&statusBarWindow, show ? TRUE : FALSE, FALSE);
                    }

                protected:
                    virtual BOOL PreTranslateMessage (MSG *pMsg);
                    virtual BOOL OnCmdMsg (UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo);

                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnClose ();
                    afx_msg void OnActivate (UINT nState, CWnd *pWndOther, BOOL bMinimized);
                    afx_msg void OnSetFocus (CWnd *pOldWnd);
                    afx_msg void OnEnterMenuLoop (BOOL bIsTrackPopupMenu);
                    afx_msg void OnGetMinMaxInfo (LPMINMAXINFO lpMMI);
                    afx_msg BOOL OnEraseBkgnd (CDC *);
                    afx_msg BOOL OnToolTipText (UINT nID, NMHDR *pNMHDR, LRESULT *pResult);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_MainFrame_h)
