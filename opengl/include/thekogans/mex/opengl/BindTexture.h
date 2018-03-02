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

#if !defined (__thekogans_mex_opengl_BindTexture_h)
#define __thekogans_mex_opengl_BindTexture_h

#include "thekogans/mex/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL BindTexture {
            private:
                GLint newTexture;
                GLenum target;
                GLint oldTexture;

            public:
                BindTexture (
                        GLint newTexture_,
                        GLenum target_ = GL_TEXTURE_2D) :
                        newTexture (newTexture_),
                        target (target_) {
                    glGetIntegerv (GetBindingForTarget (), &oldTexture);
                    if (newTexture != oldTexture) {
                        glBindTexture (target, newTexture);
                    }
                }
                ~BindTexture () {
                    if (newTexture != oldTexture) {
                        glBindTexture (target, oldTexture);
                    }
                }

            private:
                inline GLenum GetBindingForTarget () const {
                    switch (target) {
                        case GL_TEXTURE_1D:
                            return GL_TEXTURE_BINDING_1D;
                        case GL_TEXTURE_2D:
                            return GL_TEXTURE_BINDING_2D;
                        case GL_TEXTURE_3D:
                            return GL_TEXTURE_BINDING_3D;
                        // FIXME: case GL_TEXTURE_CUBE_MAP:
                    }
                    assert (0);
                    return 0;
                }

                THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN (BindTexture)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_BindTexture_h)
