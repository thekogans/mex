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

#if !defined (__thekogans_mex_opengl_Viewport2D_h)
#define __thekogans_mex_opengl_Viewport2D_h

#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/opengl/Config.h"
#include "thekogans/mex/opengl/ShadeModel.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Scissor.h"
#include "thekogans/mex/opengl/ModelView.h"
#include "thekogans/mex/opengl/Projection.h"
#include "thekogans/mex/opengl/Viewport.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct Viewport2D {
            private:
                ShadeModel shadeModel;
                Enable lighting;
                Enable alphaTest;
                Enable depthTest;
                Enable cullFace;
                Enable texture2D;
                Enable scissorTest;
                Scissor scissor;
                ModelView modeView;
                Projection projection;
                Viewport viewport;

            public:
                // Create an identity transformation.
                explicit Viewport2D (const blas::Size &size) :
                    shadeModel (GL_FLAT),
                    lighting (GL_LIGHTING, false),
                    alphaTest (GL_ALPHA_TEST, false),
                    depthTest (GL_DEPTH_TEST, false),
                    cullFace (GL_CULL_FACE, false),
                    texture2D (GL_TEXTURE_2D, false),
                    scissorTest (GL_SCISSOR_TEST, true),
                    scissor (size),
                    modeView (blas::Matrix4::Identity),
                    projection (
                        blas::Point2 (0.375f, 0.375f),
                        blas::Matrix4::Orthographic (
                            blas::Bound3 (
                                blas::Point3 (size.x, size.y, -1),
                                blas::Point3 (size.Right (), size.Top (), 1)))),
                    viewport (size) {}

                THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN (Viewport2D)
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Viewport2D_h)
