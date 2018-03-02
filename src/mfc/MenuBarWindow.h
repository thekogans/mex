#if !defined (__thekogans_3ds_mex_ui_MenuBarWindow_h
#define __thekogans_3ds_mex_ui_MenuBarWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <vector>
#include "thekogans/win/mfc/MenuBar.h"
#include "thekogans/mex/3ds/mex/core/MenuBar.h"
#include "thekogans/mex/3ds/mex/core/UI.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct MenuBarWindow : public thekogans::win::mfc::CMenuBar, public core::UI::MenuBarWindow {
                private:
                    core::MenuBar::Item modules;
                    core::MenuBar *menuBar;
                    std::vector<core::MenuBar::Item *> items;

                public:
                    MenuBarWindow () : menuBar (0) {}

                    virtual void SetMenuBar (core::MenuBar &menuBar_);

                protected:
                    void AddMenuItem (CMenu &menu, core::MenuBar::Item *item, UINT &id);

                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnCommand (UINT nID);
                    afx_msg void OnUpdateCommandUI (CCmdUI *pCmdUI);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_MenuBarWindow_h)
