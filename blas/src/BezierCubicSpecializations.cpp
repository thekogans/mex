// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_blas.
//
// libthekogans_mex_blas is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_blas is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_blas. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::InPolygon (
                    const Polygon2 &polygon,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 pt2 = GetPoint (t);
                    if (!pt1.InPolygon (polygon) || !pt2.InPolygon (polygon) ||
                            Line2 (pt1, pt2).CrossPolygon (polygon)) {
                        return false;
                    }
                    pt1 = pt2;
                }
                return true;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::CrossPolygon (
                    const Polygon2 &polygon,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 pt2 = GetPoint (t);
                    if (Line2 (pt1, pt2).CrossPolygon (polygon)) {
                        return true;
                    }
                    pt1 = pt2;
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::InCircle (
                    const Circle &circle,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = 0.0f;
                for (util::ui32 i = 0; i <= steps + 1; ++i, t += delta) {
                    if (!GetPoint (t).InCircle (circle)) {
                        return false;
                    }
                }
                return true;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::CrossCircle (
                    const Circle &circle,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 pt2 = GetPoint (t);
                    if (Line2 (pt1, pt2).CrossCircle (circle)) {
                        return true;
                    }
                    pt1 = pt2;
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::InEllipse (
                    const Ellipse &ellipse,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = 0.0f;
                for (util::ui32 i = 0; i <= steps + 1; ++i, t += delta) {
                    if (!GetPoint (t).InEllipse (ellipse)) {
                        return false;
                    }
                }
                return true;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::CrossEllipse (
                    const Ellipse &ellipse,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 pt2 = GetPoint (t);
                    if (Line2 (pt1, pt2).CrossEllipse (ellipse)) {
                        return true;
                    }
                    pt1 = pt2;
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL void BezierCubic2::CrossHorizontal (
                    util::f32 y,
                    util::ui32 steps,
                    std::vector<util::f32> &intersections) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 pt2 = GetPoint (t);
                    if (util::IS_BETWEEN_EQ (y, pt1.y, pt2.y)) {
                        if (util::IS_EQ (pt1.y, pt2.y)) {
                            intersections.push_back (pt1.x);
                        }
                        else {
                            intersections.push_back (
                                util::LERP ((y - pt1.y) / (pt2.y - pt1.y), pt1.x, pt2.x));
                        }
                    }
                    pt1 = pt2;
                }
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL void BezierCubic2::CrossVertical (
                    util::f32 x,
                    util::ui32 steps,
                    std::vector<util::f32> &intersections) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                Point2 pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    Point2 pt2 = GetPoint (t);
                    if (util::IS_BETWEEN_EQ (x, pt1.x, pt2.x)) {
                        if (util::IS_EQ (pt1.x, pt2.x)) {
                            intersections.push_back (pt1.y);
                        }
                        else {
                            intersections.push_back (
                                util::LERP ((x - pt1.x) / (pt2.x - pt1.x), pt1.y, pt2.y));
                        }
                    }
                    pt1 = pt2;
                }
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
