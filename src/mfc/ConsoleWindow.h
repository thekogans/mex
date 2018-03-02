#if !defined (__thekogans_3ds_mex_ui_ConsoleWindow_h)
#define __thekogans_3ds_mex_ui_ConsoleWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxcmn.h>
#include "thekogans/mex/3ds/mex/core/UI.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct ConsoleWindow : public CRichEditCtrl, public core::UI::ConsoleWindow {
                public:
                    virtual void Print (const char *format, ...);

                protected:
                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_ConsoleWindow_h)
