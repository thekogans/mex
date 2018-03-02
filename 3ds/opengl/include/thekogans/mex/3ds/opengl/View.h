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

#if !defined (__thekogans_mex_3ds_opengl_View_h)
#define __thekogans_mex_3ds_opengl_View_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/opengl/ModelView.h"
#include "thekogans/mex/opengl/Projection.h"
#include "thekogans/mex/opengl/Viewport.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/io/editor/Camera.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct Module;

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL View : public ext::View {
                    typedef std::unique_ptr<View> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (View)

                    const blas::Matrix4 modelViewXform;
                    const blas::Matrix4 projectionXform;

                    explicit View (
                        const io::View &view) :
                        ext::View (view),
                        modelViewXform (
                            blas::Matrix4::RotateX (-util::RAD (90.0f)) *
                            blas::Matrix4::RotateY (-util::RAD (GetHorizontalAngle (view))) *
                            blas::Matrix4::RotateX (util::RAD (GetVerticalAngle (view)))),
                        projectionXform (
                            blas::Matrix4::Orthographic (
                                blas::Bound3 (
                                    blas::Point3 (viewBound.min, nearPlane),
                                    blas::Point3 (viewBound.max, farPlane)))) {}
                    View (
                        const io::View &view,
                        const io::Light &light) :
                        ext::View (view, light),
                        modelViewXform (
                            blas::Matrix4::LookAt (
                                light.position,
                                light.spot->target,
                                blas::Point3::Z,
                                util::RAD (light.spot->roll))),
                        projectionXform (
                            blas::Matrix4::Perspective (
                                util::RAD (light.spot->fallOff),
                                (util::f32)view.size.width / view.size.height,
                                nearPlane,
                                farPlane)) {}
                    View (
                        const io::View &view,
                        const io::Camera &camera) :
                        ext::View (view, camera),
                        modelViewXform (
                            blas::Matrix4::LookAt (
                                camera.position,
                                camera.target,
                                blas::Point3::Z,
                                util::RAD (camera.roll))),
                        projectionXform (
                            blas::Matrix4::Perspective (
                                util::RAD (io::Camera::Lens2FOV (camera.lens)),
                                (util::f32)view.size.width / view.size.height,
                                nearPlane,
                                farPlane)) {}

                    void Draw (
                        const Module &module,
                        const std::vector<blas::Point2> &jitterTable = std::vector<blas::Point2> ()) const;

                private:
                    void DrawHelper (
                        const Module &module,
                        const blas::Point2 &jitter = blas::Point2::Zero) const;
                };

                struct SetupView {
                    thekogans::mex::opengl::ModelView modelView;
                    thekogans::mex::opengl::Projection projection;
                    thekogans::mex::opengl::Viewport viewport;

                    SetupView (const View &view, const blas::Point2 &jitter = blas::Point2::Zero) :
                        modelView (view.modelViewXform),
                        projection (blas::Point2 (
                            jitter.x * view.viewBound.Extents ().x / view.view.size.width,
                            jitter.y * view.viewBound.Extents ().y / view.view.size.height), view.projectionXform),
                        viewport (view.view.size) {}

                private:
                    SetupView (const SetupView &);
                    SetupView &operator = (const SetupView &);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_View_h)
