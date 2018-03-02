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

#include <cassert>
#include "thekogans/mex/opengl/BindRenderbuffer.h"
#include "thekogans/mex/opengl/Extensions.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

        #if defined (TOOLCHAIN_OS_Windows)
            PFNGLBINDRENDERBUFFEREXTPROC BindRenderbuffer::glBindRenderbufferEXT = 0;
        #endif // defined (TOOLCHAIN_OS_Windows)

            bool BindRenderbuffer::Init () {
                if (Extensions::Instance ().IsExtensionSupported ("GL_EXT_framebuffer_object")) {
                #if defined (TOOLCHAIN_OS_Windows)
                    glBindRenderbufferEXT =
                        (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress (
                            "glBindRenderbufferEXT");
                    return glBindRenderbufferEXT != 0;
                #else // defined (TOOLCHAIN_OS_Windows)
                    return true;
                #endif // defined (TOOLCHAIN_OS_Windows)
                }
                return false;
            }

            BindRenderbuffer::BindRenderbuffer (GLint newRenderbuffer_) :
                    newRenderbuffer (newRenderbuffer_) {
                if (IsSupported ()) {
                    glGetIntegerv (GL_RENDERBUFFER_BINDING_EXT, &oldRenderbuffer);
                    if (newRenderbuffer != oldRenderbuffer) {
                        glBindRenderbufferEXT (GL_RENDERBUFFER_EXT, newRenderbuffer);
                    }
                }
            }

            BindRenderbuffer::~BindRenderbuffer () {
                if (IsSupported ()) {
                    if (newRenderbuffer != oldRenderbuffer) {
                        glBindRenderbufferEXT (GL_RENDERBUFFER_EXT, oldRenderbuffer);
                    }
                }
            }

            bool BindRenderbuffer::IsSupported () {
                static const bool supported = Init ();
                return supported;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
