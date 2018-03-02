#ifndef __thekogans_3ds_mex_ui_SpeedBarFrame_h
#define __thekogans_3ds_mex_ui_SpeedBarFrame_h

#include "thekogans/win/mfc/SizingControlBar.h"
#include "SpeedBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct SpeedBarFrame : public win::mfc::SizingControlBar {
                private:
                    SpeedBarWindow speedBarWindow;

                protected:
                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnSize (UINT nType, int cx, int cy);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_SpeedBarFrame_h)
