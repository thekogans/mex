// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/io/editor/Camera.h"
#include "thekogans/mex/3ds/ext/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (View)

                const util::f32 View::ShapeWidth = 512.0f;
                const util::f32 View::OrthographicWidth = 256.0f;
                const util::f32 View::PerspectiveWidth = 1.0f;

                namespace {
                    util::f32 CalcScale (
                            const io::View &view,
                            util::f32 viewWidth) {
                        return view.size.width > view.size.height ?
                            view.size.height * view.zoom / viewWidth :
                            view.size.width * view.zoom / viewWidth;
                    }

                    blas::Point2 CalcOffset (
                            const io::View &view,
                            const blas::Point2 &viewOrigin,
                            util::f32 scale) {
                        return blas::Point2 (
                            view.size.x + view.size.width * 0.5f - viewOrigin.x * scale + 0.5f,
                            view.size.y + view.size.height * 0.5f - viewOrigin.y * scale + 0.5f);
                    }
                }

                View::View (const io::View &view_) :
                    view (view_),
                    xform (blas::Matrix3::RotateX (-util::RAD (90.0f)) *
                        blas::Matrix3::RotateY (-util::RAD (GetHorizontalAngle (view))) *
                        blas::Matrix3::RotateX (util::RAD (GetVerticalAngle (view)))),
                    ixform (xform.Invert ()),
                    viewOrigin (blas::Point2 (view.worldOrigin * xform)),
                    scale (CalcScale (view, view.IsShape () ? ShapeWidth : OrthographicWidth)),
                    oneOverScale (1.0f / scale),
                    offset (CalcOffset (view, viewOrigin, scale)),
                    viewBound (D2P (blas::Point (view.size.x, view.size.y)),
                        D2P (blas::Point (view.size.Right (), view.size.Top ()))),
                    nearPlane (-1000000.0f),
                    farPlane (1000000.0f) {
                    assert (view.IsNone () || view.IsOrthographic () || view.IsShape ());
                    assert (view.size.width > 0);
                    assert (view.size.height > 0);
                    assert (view.zoom != 0.0f);
                }

                View::View (const io::View &view_, const io::Light &light) :
                    view (view_),
                    xform (blas::Matrix3::AimViewer (light.position, light.spot->target,
                        util::RAD (light.spot->fallOff), util::RAD (light.spot->roll))),
                    ixform (xform.Invert ()),
                    viewOrigin (blas::Point2 (view.worldOrigin * xform)),
                    scale (CalcScale (view, PerspectiveWidth)),
                    oneOverScale (1.0f / scale),
                    offset (CalcOffset (view, viewOrigin, scale)),
                    viewBound (D2P (blas::Point (view.size.x, view.size.y)),
                        D2P (blas::Point (view.size.Right (), view.size.Top ()))),
                    nearPlane (1.0f/*light.attenuation.inner*/),
                    farPlane (100000.0f/*light.attenuation.outer*/) {
                    assert (view.IsSpotLight ());
                    assert (view.size.width > 0);
                    assert (view.size.height > 0);
                    assert (view.zoom != 0.0f);
                }

                View::View (const io::View &view_, const io::Camera &camera) :
                    view (view_),
                    xform (blas::Matrix3::AimViewer (camera.position, camera.target,
                        util::RAD (io::Camera::Lens2FOV (camera.lens)), util::RAD (camera.roll))),
                    ixform (xform.Invert ()),
                    viewOrigin (blas::Point2 (view.worldOrigin * xform)),
                    scale (CalcScale (view, PerspectiveWidth)),
                    oneOverScale (1.0f / scale),
                    offset (CalcOffset (view, viewOrigin, scale)),
                    viewBound (D2P (blas::Point (view.size.x, view.size.y)),
                        D2P (blas::Point (view.size.Right (), view.size.Top ()))),
                    nearPlane (1.0f/*camera.ranges.hither*/),
                    farPlane (100000.0f/*camera.ranges.yon*/) {
                    assert (view.IsCamera ());
                    assert (view.size.width > 0);
                    assert (view.size.height > 0);
                    assert (view.zoom != 0.0f);
                }

                const char *View::GetName (const io::View &view) {
                    if (!view.IsEnabled ()) {
                        return "View disabled";
                    }
                    switch (view.type) {
                        case io::View::None:
                            return "No view defined";
                        case io::View::Top:
                            return view.IsShapeFlag () ? "Shape" : "Top(X/Y)";
                        case io::View::Bottom:
                            return "Bottom(X/Y)";
                        case io::View::Left:
                            return "Left(Y/Z)";
                        case io::View::Right:
                            return "Right(Y/Z)";
                        case io::View::Front:
                            return "Front(X/Z)";
                        case io::View::Back:
                            return "Back(X/Z)";
                        case io::View::User:
                            return "User";
                        case io::View::Shape:
                            return "Shape";
                        case io::View::SpotLight:
                        case io::View::Camera:
                            return view.name.value.c_str ();
                    }
                    return "Unknown view type";
                }

                util::f32 View::GetHorizontalAngle (const io::View &view) {
                    switch (view.type) {
                        case io::View::None:
                        case io::View::Top:
                        case io::View::Shape:
                            return 0.0f;
                        case io::View::Bottom:
                            return 180.0f;
                        case io::View::Left:
                            return -90.0f;
                        case io::View::Right:
                            return 90.0f;
                        case io::View::Front:
                            return 0.0f;
                        case io::View::Back:
                            return 180.0f;
                        case io::View::User:
                            return view.horizontalAngle;
                        default:
                            throw BadViewType ();
                    }
                    assert (0);
                    return 0.0f;
                }

                util::f32 View::GetVerticalAngle (const io::View &view) {
                    switch (view.type) {
                        case io::View::None:
                        case io::View::Top:
                        case io::View::Shape:
                            return 90.0f;
                        case io::View::Bottom:
                            return -90.0f;
                        case io::View::Left:
                            return 0.0f;
                        case io::View::Right:
                            return 0.0f;
                        case io::View::Front:
                            return 0.0f;
                        case io::View::Back:
                            return 0.0f;
                        case io::View::User:
                            return view.verticalAngle;
                        default:
                            throw BadViewType ();
                    }
                    assert (0);
                    return 0.0f;
                }

                bool View::W2D (const blas::Point3 &w, blas::Point &d) const {
                    blas::Point3 v = w * xform;
                    if (v.z >= nearPlane && v.z <= farPlane) {
                        blas::Point2 p = v.Project (view.IsPerspective ());
                        if (p.InBound (viewBound)) {
                            d = P2D (p);
                            return true;
                        }
                    }
                    return false;
                }

                bool View::W2D (const blas::Line3 &w, blas::Line &d) const {
                    blas::Point3 v1 = w.start * xform;
                    blas::Point3 v2 = w.end * xform;
                    if (blas::ClipNearFar (v1, v2, nearPlane, farPlane)) {
                        blas::Point2 p1 = v1.Project (view.IsPerspective ());
                        blas::Point2 p2 = v2.Project (view.IsPerspective ());
                        if (blas::Clip2 (p1, p2, viewBound)) {
                            d.start = P2D (p1);
                            d.end = P2D (p2);
                            return true;
                        }
                    }
                    return false;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
