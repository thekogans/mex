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

#if !defined (__thekogans_mex_opengl_Viewport_h)
#define __thekogans_mex_opengl_Viewport_h

#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct Viewport {
            private:
                GLint viewport[4];

            public:
                Viewport (const blas::Size &size) {
                    glGetIntegerv (GL_VIEWPORT, viewport);
                    glViewport (size.x, size.y, size.width, size.height);
                }
                ~Viewport () {
                    glViewport (viewport[0], viewport[1], viewport[2], viewport[3]);
                }

                THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN (Viewport)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Viewport_h)
