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
#include "thekogans/util/Environment.h"
#include "thekogans/mex/opengl/Renderbuffer.h"
#include "thekogans/mex/opengl/Extensions.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

        #if defined (TOOLCHAIN_OS_Windows)
            PFNGLGENRENDERBUFFERSEXTPROC Renderbuffer::glGenRenderbuffersEXT = 0;
            PFNGLDELETERENDERBUFFERSEXTPROC Renderbuffer::glDeleteRenderbuffersEXT = 0;
            PFNGLRENDERBUFFERSTORAGEEXTPROC Renderbuffer::glRenderbufferStorageEXT = 0;
            PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC Renderbuffer::glGetRenderbufferParameterivEXT = 0;
        #endif // defined (TOOLCHAIN_OS_Windows)

            bool Renderbuffer::Init () {
                if (Extensions::Instance ()->IsExtensionSupported ("GL_EXT_framebuffer_object")) {
                #if defined (TOOLCHAIN_OS_Windows)
                    glGenRenderbuffersEXT =
                        (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress (
                            "glGenRenderbuffersEXT");
                    glDeleteRenderbuffersEXT =
                        (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress (
                            "glDeleteRenderbuffersEXT");
                    glRenderbufferStorageEXT =
                        (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress (
                            "glRenderbufferStorageEXT");
                    glGetRenderbufferParameterivEXT =
                        (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress (
                            "glGetRenderbufferParameterivEXT");
                    return glGenRenderbuffersEXT != 0 &&
                        glDeleteRenderbuffersEXT != 0 &&
                        glRenderbufferStorageEXT != 0 &&
                        glGetRenderbufferParameterivEXT != 0;
                #else // defined (TOOLCHAIN_OS_Windows)
                    return true;
                #endif // defined (TOOLCHAIN_OS_Windows)
                }
                return false;
            }

            Renderbuffer::Renderbuffer (
                    GLenum internalFormat,
                    const blas::Size &size) :
                    id (-1) {
                if (IsSupported ()) {
                    glGenRenderbuffersEXT (1, &id);
                    BindRenderbuffer bindRenderbuffer (id);
                    glRenderbufferStorageEXT (GL_RENDERBUFFER_EXT,
                        internalFormat, size.width, size.height);
                }
            }

            Renderbuffer::~Renderbuffer () {
                if (IsSupported ()) {
                    glDeleteRenderbuffersEXT (1, &id);
                }
            }

            GLint Renderbuffer::GetParameter (GLenum param) const {
                GLint value = 0;
                if (IsSupported ()) {
                    BindRenderbuffer bindRenderbuffer (id);
                    glGetRenderbufferParameterivEXT (
                        GL_RENDERBUFFER_EXT, param, &value);
                }
                return value;
            }

            bool Renderbuffer::IsSupported () {
                static const bool supported = Init ();
                return supported;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
