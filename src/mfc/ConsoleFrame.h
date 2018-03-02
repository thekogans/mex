#if !defined (__thekogans_3ds_mex_ui_ConsoleFrame_h)
#define __thekogans_3ds_mex_ui_ConsoleFrame_h

#include "thekogans/win/mfc/SizingControlBar.h"
#include "ConsoleWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct ConsoleFrame : public win::mfc::SizingControlBar {
                private:
                    ConsoleWindow consoleWindow;

                protected:
                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnSize (UINT nType, int cx, int cy);
                    afx_msg void OnPaint ();

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_ConsoleFrame_h)
