#if !defined (_WINDOWS_)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
    #include <windows.h>
#endif // !defined (_WINDOWS_)
#include "thekogans/mex/3ds/mex/core/Util.h"
#include "Util.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                util::ui32 GetControlKeyState () {
                    util::ui32 state = 0;
                    if (GetAsyncKeyState (VK_CONTROL) < 0) {
                        state |= core::FLAG_CTRL;
                    }
                    if (GetAsyncKeyState (VK_MENU) < 0) {
                        state |= core::FLAG_ALT;
                    }
                    if (GetAsyncKeyState (VK_SHIFT) < 0) {
                        state |= core::FLAG_SHIFT;
                    }
                    return state;
                }

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
