// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_Viewport3D_h)
#define __thekogans_mex_3ds_opengl_Viewport3D_h

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Scissor.h"
#include "thekogans/mex/opengl/ModelView.h"
#include "thekogans/mex/opengl/Projection.h"
#include "thekogans/mex/opengl/Viewport.h"
#include "thekogans/mex/opengl/DrawBuffer.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct Viewport3D {
                private:
                    thekogans::mex::opengl::Enable scissorTest;
                    thekogans::mex::opengl::Scissor scissor;
                    thekogans::mex::opengl::ModelView modelView;
                    thekogans::mex::opengl::Projection projection;
                    thekogans::mex::opengl::Viewport viewport;
                    thekogans::mex::opengl::DrawBuffer drawBuffer;

                public:
                    Viewport3D (const View &view) :
                        scissorTest (GL_SCISSOR_TEST, true),
                        scissor (view.view.size),
                        modelView (view.modelViewXform),
                        projection (blas::Point2::Zero, view.projectionXform),
                        viewport (view.view.size),
                        drawBuffer (GL_BACK) {}

                private:
                    Viewport3D (const Viewport3D &);
                    Viewport3D &operator = (const Viewport3D &);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Viewport3D_h)
