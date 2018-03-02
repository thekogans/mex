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

#if !defined (__thekogans_mex_opengl_ClearColor_h)
#define __thekogans_mex_opengl_ClearColor_h

#include "thekogans/mex/opengl/Config.h"
#include "thekogans/mex/opengl/ui8Color.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct ClearColor {
            private:
                GLfloat oldClearColor[4];

            public:
                explicit ClearColor (const ui8Color &clearColor) {
                    glGetFloatv (GL_COLOR_CLEAR_VALUE, oldClearColor);
                    glClearColor (
                        clearColor.r / 255.0f,
                        clearColor.g / 255.0f,
                        clearColor.b / 255.0f, 1.0f);
                }
                ~ClearColor () {
                    glClearColor (
                        oldClearColor[0], oldClearColor[1],
                        oldClearColor[2], oldClearColor[3]);
                }

                THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN (ClearColor)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_ClearColor_h)
