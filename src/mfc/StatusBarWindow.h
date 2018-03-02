#ifndef __thekogans_3ds_mex_ui_StatusBarWindow_h
#define __thekogans_3ds_mex_ui_StatusBarWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxext.h>
#include <vector>
#include "thekogans/mex/3ds/io/Progress.h"
#include "thekogans/mex/3ds/mex/core/StatusBar.h"
#include "thekogans/mex/3ds/mex/core/UI.h"
#include "FrameBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct StatusBarWindow : public CStatusBar, public core::UI::StatusBarWindow {
                private:
                    core::StatusBar *statusBar;
                    FrameBarWindow frameBarWindow;

                public:
                    StatusBarWindow () : statusBar (0) {}

                    virtual void OnIdle ();
                    virtual void SetStatusBar (core::StatusBar &statusBar_);
                    virtual std::auto_ptr<_3ds::io::Progress> CreateProgress ();

                protected:
                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnSize (UINT nType, int cx, int cy);
                    afx_msg void OnMouseMove (UINT nFlags, CPoint point);
                    afx_msg void OnUpdateCommandUI (CCmdUI *cmdUI);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_StatusBarWindow_h)
