#ifndef __thekogans_3ds_mex_ui_ToolBarWindow_h
#define __thekogans_3ds_mex_ui_ToolBarWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxext.h>
#include <vector>
#include "thekogans/mex/3ds/mex/core/ToolBar.h"
#include "thekogans/mex/3ds/mex/core/UI.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct ToolBarWindow : public CToolBar, public core::UI::ToolBarWindow {
                private:
                    core::ToolBar *toolBar;
                    std::vector<HBITMAP> bitmaps;
                    bool rightButton;

                public:
                    ToolBarWindow () : toolBar (0), rightButton (false) {}

                    virtual void SetToolBar (core::ToolBar &toolBar_);

                private:
                    void Flush ();
                    const char *GetItemText (UINT nID, int type);
                    virtual BOOL SetStatusText (int nHit);
                    void DoButton (CPoint point, int type);

                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
                    afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
                    afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
                    afx_msg void OnCommand (UINT nID);
                    afx_msg void OnUpdateCommandUI (CCmdUI *pCmdUI);
                    afx_msg BOOL OnToolTipText (UINT, NMHDR *pNMHDR, LRESULT *pResult);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_ToolBarWindow_h)
