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

#if !defined (__thekogans_mex_opengl_Framebuffer_h)
#define __thekogans_mex_opengl_Framebuffer_h

#include <memory>
#include "thekogans/util/Environment.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/blas/Extents.h"
#include "thekogans/mex/opengl/Config.h"
#include "thekogans/mex/opengl/Texture.h"
#include "thekogans/mex/opengl/Renderbuffer.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL Framebuffer {
                typedef std::unique_ptr<Framebuffer> UniquePtr;

            private:
            #if defined (TOOLCHAIN_OS_Windows)
                static PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
                static PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
                static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
                static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
                static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
            #endif // defined (TOOLCHAIN_OS_Windows)
                static bool Init ();

            public:
                GLuint id;
                util::OwnerVector<Texture> textures;
                util::OwnerVector<Renderbuffer> renderbuffers;

                Framebuffer ();
                ~Framebuffer ();

                void AttachTexture (
                    GLenum attachmentPoint,
                    Texture::UniquePtr texture,
                    GLint level = 0);
                void AttachRenderbuffer (
                    GLenum attachmentPoint,
                    Renderbuffer::UniquePtr renderbuffer);

                blas::Extents GetExtents () const;

                bool IsValid () const;

                static bool IsSupported ();

                THEKOGANS_UTIL_DISALLOW_COPY_AND_ASSIGN (Framebuffer)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Framebuffer_h)
