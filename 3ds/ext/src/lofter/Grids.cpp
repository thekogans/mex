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

#include "thekogans/mex/3ds/ext/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                const blas::Bound2 gridBound (blas::Point2 (-200.0f, -200.0f), blas::Point2 (200.0f, 200.0f));

                Lofter::ScaleDeformation::ScaleDeformation (
                        const io::Lofter::ScaleDeformation &deformation_) :
                        deformation (deformation_) {
                    typedef io::BezierPolygon2::Vertex Vertex2;
                    typedef std::vector<Vertex2>::const_iterator const_iterator;
                    // Because we use gridBound to lerp the parameter,
                    // we need to scale the deformation splines so that
                    // proper units are used throughout.
                    {
                        for (const_iterator it = deformation.xzCurve.bezierPolygon.vertices.begin (),
                            end = deformation.xzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                            xzCurve.vertices.push_back (Vertex2 (
                                blas::Point2 (
                                    util::LERP (it->pt.x / deformation.limit,
                                        gridBound.min.x, gridBound.max.x),
                                    util::LERP (it->pt.y / deformation.limit,
                                        gridBound.min.y, gridBound.max.y)),
                                it->red, it->yellow));
                        }
                    }
                    {
                        for (const_iterator it = deformation.yzCurve.bezierPolygon.vertices.begin (),
                            end = deformation.yzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                            yzCurve.vertices.push_back (Vertex2 (
                                blas::Point2 (
                                    util::LERP (it->pt.x / deformation.limit,
                                        gridBound.min.x, gridBound.max.x),
                                    util::LERP (it->pt.y / deformation.limit,
                                        gridBound.min.y, gridBound.max.y)),
                                it->red, it->yellow));
                        }
                    }
                }

                util::f32 Lofter::ScaleDeformation::GetValue (util::f32 t,
                        const io::BezierPolygon2 &curve, util::ui16 steps) const {
                    std::vector<util::f32> intersections;
                    BezierPolygon2 (curve).CrossHorizontal (
                        util::LERP (t, gridBound.min.y, gridBound.max.y), steps, intersections);
                    return intersections.empty () ? 1.0f :
                        0.01f * deformation.limit * (intersections[0] - gridBound.min.x) / gridBound.Extents ().x;
                }

                Lofter::TwistDeformation::TwistDeformation (
                        const io::Lofter::TwistDeformation &deformation_) :
                        deformation (deformation_) {
                    typedef io::BezierPolygon2::Vertex Vertex2;
                    typedef std::vector<Vertex2>::const_iterator const_iterator;
                    // Because we use gridBound to lerp the parameter,
                    // we need to scale the deformation splines so that
                    // proper units are used throughout.
                    for (const_iterator it = deformation.xzCurve.bezierPolygon.vertices.begin (),
                        end = deformation.xzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                        xzCurve.vertices.push_back (Vertex2 (
                            blas::Point2 (
                                util::LERP (it->pt.x / 2.0f * deformation.limit, gridBound.min.x, gridBound.max.x),
                                util::LERP (it->pt.y / 2.0f * deformation.limit, gridBound.min.y, gridBound.max.y)),
                            it->red, it->yellow));
                    }
                }

                util::f32 Lofter::TwistDeformation::GetValue (util::f32 t) const {
                    std::vector<util::f32> intersections;
                    BezierPolygon2 (xzCurve).CrossHorizontal (
                        util::LERP (t, gridBound.min.y, gridBound.max.y),
                        deformation.xzCurve.steps, intersections);
                    return intersections.empty () ? 0.0f : util::RAD (
                        deformation.limit * (intersections[0] -
                            gridBound.Center ().x) / (gridBound.Extents ().x * 0.5f));
                }

                Lofter::TeeterDeformation::TeeterDeformation (
                        const io::Lofter::TeeterDeformation &deformation_) :
                        deformation (deformation_) {
                    typedef io::BezierPolygon2::Vertex Vertex2;
                    typedef std::vector<Vertex2>::const_iterator const_iterator;
                    // Because we use gridBound to lerp the parameter,
                    // we need to scale the deformation splines so that
                    // proper units are used throughout.
                    {
                        for (const_iterator it = deformation.xzCurve.bezierPolygon.vertices.begin (),
                            end = deformation.xzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                            xzCurve.vertices.push_back (Vertex2 (
                                blas::Point2 (
                                    util::LERP (it->pt.x / 2.0f * deformation.limit,
                                        gridBound.min.x, gridBound.max.x),
                                    util::LERP (it->pt.y / 2.0f * deformation.limit,
                                        gridBound.min.y, gridBound.max.y)),
                                it->red, it->yellow));
                        }
                    }
                    {
                        for (const_iterator it = deformation.yzCurve.bezierPolygon.vertices.begin (),
                            end = deformation.yzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                            yzCurve.vertices.push_back (Vertex2 (
                                blas::Point2 (
                                    util::LERP (it->pt.x / 2.0f * deformation.limit,
                                        gridBound.min.x, gridBound.max.x),
                                    util::LERP (it->pt.y / 2.0f * deformation.limit,
                                        gridBound.min.y, gridBound.max.y)),
                                it->red, it->yellow));
                        }
                    }
                }

                util::f32 Lofter::TeeterDeformation::GetValue (util::f32 t,
                        const io::BezierPolygon2 &curve, util::ui16 steps) const {
                    std::vector<util::f32> intersections;
                    BezierPolygon2 (curve).CrossHorizontal (
                        util::LERP (t, gridBound.min.y, gridBound.max.y), steps, intersections);
                    return intersections.empty () ? 0.0f : util::RAD (
                        deformation.limit * (intersections[0] -
                            gridBound.Center ().x) / (gridBound.Extents ().x * 0.5f));
                }

                Lofter::BevelDeformation::BevelDeformation (
                        const io::Lofter::BevelDeformation &deformation_) :
                        deformation (deformation_) {
                    typedef io::BezierPolygon2::Vertex Vertex2;
                    typedef std::vector<Vertex2>::const_iterator const_iterator;
                    // Because we use gridBound to lerp the parameter,
                    // we need to scale the deformation splines so that
                    // proper units are used throughout.
                    for (const_iterator it = deformation.xzCurve.bezierPolygon.vertices.begin (),
                        end = deformation.xzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                        xzCurve.vertices.push_back (Vertex2 (
                            blas::Point2 (
                                util::LERP (it->pt.x / 2.0f * deformation.limit,
                                    gridBound.min.x, gridBound.max.x),
                                util::LERP (it->pt.y / 2.0f * deformation.limit,
                                    gridBound.min.y, gridBound.max.y)),
                            it->red, it->yellow));
                    }
                }

                util::f32 Lofter::BevelDeformation::GetValue (util::f32 t) const {
                    std::vector<util::f32> intersections;
                    BezierPolygon2 (xzCurve).CrossHorizontal (
                        util::LERP (t, gridBound.min.y, gridBound.max.y),
                        deformation.xzCurve.steps, intersections);
                    return intersections.empty () ? 0.0f :
                        deformation.limit * (intersections[0] -
                            gridBound.Center ().x) / (gridBound.Extents ().x * 0.5f);
                }

                Lofter::FitDeformation::FitDeformation (
                        const io::Lofter::FitDeformation &deformation_) :
                        deformation (deformation_) {
                    xzCurve = deformation.xzCurve.bezierPolygon;
                    // For reasons unknown to me, 3ds does not
                    // store flags with vertices of deformation
                    // splines. Because some algorithms rely on First,
                    // Last and Closed flags for proper operation,
                    // we force them here.
                    if (!xzCurve.vertices.empty ()) {
                        xzCurve.vertices.front ().flags |=
                            io::BezierPolygon2::Vertex::First;
                        xzCurve.vertices.back ().flags |=
                            io::BezierPolygon2::Vertex::Last |
                            io::BezierPolygon2::Vertex::Closed;
                    }
                    xzBound = BezierPolygon2 (xzCurve).GetBound (deformation.xzCurve.steps);
                    typedef io::BezierPolygon2::Vertex Vertex2;
                    typedef std::vector<Vertex2>::const_iterator const_iterator;
                    for (const_iterator it = deformation.yzCurve.bezierPolygon.vertices.begin (),
                            end = deformation.yzCurve.bezierPolygon.vertices.end (); it != end; ++it) {
                        // In order to treat both xz and yz splines
                        // homogeneously, we need to flip x and y on
                        // yzCurve.
                        yzCurve.vertices.push_back (Vertex2 (
                            blas::Point2 (it->pt.y, it->pt.x),
                            blas::Point2 (it->red.y, it->red.x),
                            blas::Point2 (it->yellow.y, it->yellow.x)));
                    }
                    if (!yzCurve.vertices.empty ()) {
                        yzCurve.vertices.front ().flags |=
                            io::BezierPolygon2::Vertex::First;
                        yzCurve.vertices.back ().flags |=
                            io::BezierPolygon2::Vertex::Last |
                            io::BezierPolygon2::Vertex::Closed;
                    }
                    yzBound = BezierPolygon2 (yzCurve).GetBound (deformation.yzCurve.steps);
                }

                void Lofter::FitDeformation::GetXZValues (util::f32 t,
                        std::vector<util::f32> &intersections) const {
                    BezierPolygon2 (xzCurve).CrossHorizontal (
                        util::LERP (t, xzBound.min.y, xzBound.max.y),
                        deformation.xzCurve.steps, intersections);
                }

                void Lofter::FitDeformation::GetYZValues (util::f32 t,
                        std::vector<util::f32> &intersections) const {
                    BezierPolygon2 (yzCurve).CrossHorizontal (
                        util::LERP (t, yzBound.min.y, yzBound.max.y),
                        deformation.yzCurve.steps, intersections);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
