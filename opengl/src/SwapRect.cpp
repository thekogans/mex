// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_OSX)
    #include <OpenGL/OpenGL.h>
#endif // defined (TOOLCHAIN_OS_OSX)
#include <cassert>
#include "thekogans/mex/opengl/SwapRect.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

        #if defined (TOOLCHAIN_OS_Windows)
            PFNGLADDSWAPHINTRECTWINPROC SwapRect::glAddSwapHintRectWIN = 0;
        #endif // defined (TOOLCHAIN_OS_Windows)

            SwapRect::SwapRect (const blas::Size &size) {
            #if defined (TOOLCHAIN_OS_Windows)
                // It is assumed that SetCurrent is called before
                // wglGetProcAddress, otherwise it will return 0?!?
                // Go figure!
                if (glAddSwapHintRectWIN == 0) {
                    glAddSwapHintRectWIN = (PFNGLADDSWAPHINTRECTWINPROC)
                        wglGetProcAddress ("glAddSwapHintRectWIN");
                }
                assert (glAddSwapHintRectWIN != 0);
                if (glAddSwapHintRectWIN != 0) {
                    glAddSwapHintRectWIN (size.x, size.y, size.width, size.height);
                }
            #elif defined (TOOLCHAIN_OS_Linux)
                // FIXME: implement
                assert (0);
            #elif defined (TOOLCHAIN_OS_OSX)
                CGLIsEnabled (CGLGetCurrentContext (), kCGLCESwapRectangle, &enabled);
                if (!enabled) {
                    CGLEnable (CGLGetCurrentContext (), kCGLCESwapRectangle);
                }
                CGLGetParameter (CGLGetCurrentContext (), kCGLCPSwapRectangle, oldRect);
                GLint newRect[4] = {size.x, size.y, size.width, size.height};
                CGLSetParameter (CGLGetCurrentContext (), kCGLCPSwapRectangle, newRect);
            #endif // defined (TOOLCHAIN_OS_Windows)
            }

            SwapRect::~SwapRect () {
            #if defined (TOOLCHAIN_OS_Windows)
                // Nothing to do. Swap hint will be reset after every SwapBuffers call.
            #elif defined (TOOLCHAIN_OS_Linux)
                // FIXME: implement
                assert (0);
            #elif defined (TOOLCHAIN_OS_OSX)
                CGLSetParameter (CGLGetCurrentContext (), kCGLCPSwapRectangle, oldRect);
                if (!enabled) {
                    CGLDisable (CGLGetCurrentContext (), kCGLCESwapRectangle);
                }
            #endif // defined (TOOLCHAIN_OS_Windows)
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
