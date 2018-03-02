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

#include "thekogans/mex/opengl/BlitFramebuffer.h"
#include "thekogans/mex/opengl/Extensions.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

        #if defined (TOOLCHAIN_OS_Windows)
            PFNGLBLITFRAMEBUFFEREXTPROC BlitFramebuffer::glBlitFramebufferEXT = 0;
        #elif defined (TOOLCHAIN_OS_Linux)
            #define glBlitFramebufferEXT glBlitFramebuffer
        #endif // defined (TOOLCHAIN_OS_Windows)

            bool BlitFramebuffer::Init () {
                if (Extensions::Instance ().IsExtensionSupported ("GL_EXT_framebuffer_blit")) {
                #if defined (TOOLCHAIN_OS_Windows)
                    glBlitFramebufferEXT =
                        (PFNGLBLITFRAMEBUFFEREXTPROC)wglGetProcAddress (
                            "glBlitFramebufferEXT");
                    return glBlitFramebufferEXT != 0;
                #else // defined (TOOLCHAIN_OS_Windows)
                    return true;
                #endif // defined (TOOLCHAIN_OS_Windows)
                }
                return false;
            }

            bool BlitFramebuffer::Blit (
                    const blas::Size &size,
                    GLbitfield mask,
                    GLenum filter) const {
                if (IsSupported ()) {
                    BindFramebuffer readFramebuffer (GL_READ_FRAMEBUFFER_EXT, readId);
                    BindFramebuffer drawFramebuffer (GL_DRAW_FRAMEBUFFER_EXT, drawId);
                    glBlitFramebufferEXT (
                        size.x, size.y, size.x + size.width, size.y + size.height,
                        size.x, size.y, size.x + size.width, size.y + size.height,
                        mask, filter);
                    return true;
                }
                return false;
            }

            bool BlitFramebuffer::IsSupported () {
                static const bool supported = Init ();
                return supported;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
