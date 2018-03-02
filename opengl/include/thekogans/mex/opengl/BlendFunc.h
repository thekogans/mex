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

#if !defined (__thekogans_mex_opengl_BlendFunc_h)
#define __thekogans_mex_opengl_BlendFunc_h

#include "thekogans/mex/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct BlendFunc {
            private:
                GLint blendSrc;
                GLint blendDst;
                GLint oldBlendSrc;
                GLint oldBlendDst;

            public:
                BlendFunc (
                        GLint blendSrc_,
                        GLint blendDst_) :
                        blendSrc (blendSrc_),
                        blendDst (blendDst_) {
                    glGetIntegerv (GL_BLEND_SRC_ALPHA, &oldBlendSrc);
                    glGetIntegerv (GL_BLEND_DST_ALPHA, &oldBlendDst);
                    if (blendSrc != oldBlendSrc || blendDst != oldBlendDst) {
                        glBlendFunc (blendSrc, blendDst);
                    }
                }
                ~BlendFunc () {
                    if (blendSrc != oldBlendSrc || blendDst != oldBlendDst) {
                        glBlendFunc (oldBlendSrc, oldBlendDst);
                    }
                }

                THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN (BlendFunc)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_BlendFunc_h)
