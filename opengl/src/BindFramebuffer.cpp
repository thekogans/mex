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

#include "thekogans/mex/opengl/BindFramebuffer.h"
#include "thekogans/mex/opengl/Extensions.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

        #if defined (TOOLCHAIN_OS_Windows)
            PFNGLBINDFRAMEBUFFEREXTPROC BindFramebuffer::glBindFramebufferEXT = 0;
        #endif // defined (TOOLCHAIN_OS_Windows)

            bool BindFramebuffer::Init () {
                if (Extensions::Instance ().IsExtensionSupported ("GL_EXT_framebuffer_object")) {
                #if defined (TOOLCHAIN_OS_Windows)
                    glBindFramebufferEXT =
                        (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress ("glBindFramebufferEXT");
                    return glBindFramebufferEXT != 0;
                #else // defined (TOOLCHAIN_OS_Windows)
                    return true;
                #endif // defined (TOOLCHAIN_OS_Windows)
                }
                return false;
            }

            BindFramebuffer::BindFramebuffer (GLenum target_, GLint newFramebuffer_) :
                    target (target_), newFramebuffer (newFramebuffer_) {
                if (IsSupported ()) {
                    glGetIntegerv (BindingFromTarget (target), &oldFramebuffer);
                    if (newFramebuffer != oldFramebuffer) {
                        glFlush ();
                        glBindFramebufferEXT (target, newFramebuffer);
                    }
                }
            }

            BindFramebuffer::~BindFramebuffer () {
                if (IsSupported ()) {
                    if (newFramebuffer != oldFramebuffer) {
                        glFlush ();
                        glBindFramebufferEXT (target, oldFramebuffer);
                    }
                }
            }

            bool BindFramebuffer::IsSupported () {
                static const bool supported = Init ();
                return supported;
            }

            GLenum BindFramebuffer::BindingFromTarget (GLenum target) {
                switch (target) {
                    case GL_FRAMEBUFFER_EXT:
                        return GL_FRAMEBUFFER_BINDING_EXT;
                    case GL_READ_FRAMEBUFFER_EXT:
                        return GL_READ_FRAMEBUFFER_BINDING_EXT;
                    case GL_DRAW_FRAMEBUFFER_EXT:
                        return GL_DRAW_FRAMEBUFFER_BINDING_EXT;
                }
                assert (0);
                return 0;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
