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

#if !defined (__thekogans_mex_opengl_Texture_h)
#define __thekogans_mex_opengl_Texture_h

#include <memory>
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/opengl/Config.h"
#include "thekogans/mex/opengl/BindTexture.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL Texture {
                typedef std::unique_ptr<Texture> UniquePtr;

                GLenum target;
                GLuint id;

                Texture (GLenum target_ = GL_TEXTURE_2D) :
                        target (target_),
                        id (-1) {
                    glGenTextures (1, &id);
                }
                ~Texture () {
                    glDeleteTextures (1, &id);
                }

                inline GLint GetIntParameter (GLenum param) const {
                    GLint value = 0;
                    BindTexture bindTexture (id, target);
                    glGetTexParameteriv (target, param, &value);
                    return value;
                }

                inline GLfloat GetFloatParameter (GLenum param) const {
                    GLfloat value = 0.0f;
                    BindTexture bindTexture (id, target);
                    glGetTexParameterfv (target, param, &value);
                    return value;
                }

                inline GLint GetLevelIntParameter (
                        GLenum param,
                        GLint level = 0) const {
                    GLint value = 0;
                    BindTexture bindTexture (id, target);
                    glGetTexLevelParameteriv (target, level, param, &value);
                    return value;
                }

                inline GLfloat GetLevelFloatParameter (
                        GLenum param,
                        GLint level = 0) const {
                    GLfloat value = 0.0f;
                    BindTexture bindTexture (id, target);
                    glGetTexLevelParameterfv (target, level, param, &value);
                    return value;
                }

                THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN (Texture)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Texture_h)
