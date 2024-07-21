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
#include "thekogans/mex/opengl/BindFramebuffer.h"
#include "thekogans/mex/opengl/Framebuffer.h"
#include "thekogans/mex/opengl/Extensions.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

        #if defined (TOOLCHAIN_OS_Windows)
            PFNGLGENFRAMEBUFFERSEXTPROC Framebuffer::glGenFramebuffersEXT = 0;
            PFNGLDELETEFRAMEBUFFERSEXTPROC Framebuffer::glDeleteFramebuffersEXT = 0;
            PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC Framebuffer::glCheckFramebufferStatusEXT = 0;
            PFNGLFRAMEBUFFERTEXTURE2DEXTPROC Framebuffer::glFramebufferTexture2DEXT = 0;
            PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC Framebuffer::glFramebufferRenderbufferEXT = 0;
        #endif // defined (TOOLCHAIN_OS_Windows)

            bool Framebuffer::Init () {
                if (Extensions::Instance ()->IsExtensionSupported ("GL_EXT_framebuffer_object")) {
                #if defined (TOOLCHAIN_OS_Windows)
                    glGenFramebuffersEXT =
                        (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress (
                            "glGenFramebuffersEXT");
                    glDeleteFramebuffersEXT =
                        (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress (
                            "glDeleteFramebuffersEXT");
                    glCheckFramebufferStatusEXT =
                        (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress (
                            "glCheckFramebufferStatusEXT");
                    glFramebufferTexture2DEXT =
                        (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress (
                            "glFramebufferTexture2DEXT");
                    glFramebufferRenderbufferEXT =
                        (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress (
                            "glFramebufferRenderbufferEXT");
                    return glGenFramebuffersEXT != 0 &&
                        glDeleteFramebuffersEXT != 0 &&
                        glCheckFramebufferStatusEXT != 0 &&
                        glFramebufferRenderbufferEXT != 0 &&
                        glFramebufferTexture2DEXT != 0;
                #else // defined (TOOLCHAIN_OS_Windows)
                    return true;
                #endif // defined (TOOLCHAIN_OS_Windows)
                }
                return false;
            }

            Framebuffer::Framebuffer () :
                    id (-1) {
                if (IsSupported ()) {
                    glGenFramebuffersEXT (1, &id);
                }
            }

            Framebuffer::~Framebuffer () {
                if (IsSupported ()) {
                    glDeleteFramebuffersEXT (1, &id);
                }
            }

            void Framebuffer::AttachTexture (
                    GLenum attachmentPoint,
                    Texture::UniquePtr texture,
                    GLint level) {
                assert (texture.get () != 0);
                assert (texture->target == GL_TEXTURE_2D);
                assert (texture->id != -1);
                if (IsSupported ()) {
                    BindFramebuffer bindFramebuffer (GL_FRAMEBUFFER_EXT, id);
                    glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, attachmentPoint,
                        texture->target, texture->id, level);
                    textures.push_back (texture.get ());
                    texture.release ();
                }
            }

            void Framebuffer::AttachRenderbuffer (
                    GLenum attachmentPoint,
                    Renderbuffer::UniquePtr renderbuffer) {
                assert (renderbuffer.get () != 0);
                assert (renderbuffer->id != -1);
                if (IsSupported ()) {
                    BindFramebuffer bindFramebuffer (GL_FRAMEBUFFER_EXT, id);
                    glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, attachmentPoint,
                        GL_RENDERBUFFER_EXT, renderbuffer->id);
                    renderbuffers.push_back (renderbuffer.get ());
                    renderbuffer.release ();
                }
            }

            blas::Extents Framebuffer::GetExtents () const {
                GLint width = 0;
                GLint height = 0;
                if (!textures.empty ()) {
                    width = textures[0]->GetLevelIntParameter (GL_TEXTURE_WIDTH);
                    height = textures[0]->GetLevelIntParameter (GL_TEXTURE_HEIGHT);
                }
                else if (!renderbuffers.empty ()) {
                    width = renderbuffers[0]->GetParameter (GL_RENDERBUFFER_WIDTH_EXT);
                    height = renderbuffers[0]->GetParameter (GL_RENDERBUFFER_HEIGHT_EXT);
                }
                return blas::Extents (width, height);
            }

            bool Framebuffer::IsValid () const {
                return IsSupported () &&
                    glCheckFramebufferStatusEXT (GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT;
            }

            bool Framebuffer::IsSupported () {
                static const bool supported = Init ();
                return supported;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
