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

#if !defined (__thekogans_mex_blas_BezierCubic_h)
#define __thekogans_mex_blas_BezierCubic_h

#include <vector>
#include <functional>
#include <math.h>
#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            // Bezier basis
            inline util::f32 B0 (util::f32 t) {
                util::f32 u = 1.0f - t;
                return u * u * u;
            }

            inline util::f32 B1 (util::f32 t) {
                util::f32 u = 1.0f - t;
                return 3.0f * t * u * u;
            }

            inline util::f32 B2 (util::f32 t) {
                return 3.0f * t * t * (1.0f - t);
            }

            inline util::f32 B3 (util::f32 t) {
                return t * t * t;
            }

            template<typename T>
            struct BezierCubic {
                T p1;
                T p2;
                T p3;
                T p4;

                BezierCubic () {}
                BezierCubic (const T &p1_, const T &p2_, const T &p3_, const T &p4_) :
                    p1 (p1_), p2 (p2_), p3 (p3_), p4 (p4_) {}
                BezierCubic (const BezierCubic &bezierCubic) : p1 (bezierCubic.p1),
                    p2 (bezierCubic.p2), p3 (bezierCubic.p3), p4 (bezierCubic.p4) {}

                inline BezierCubic &operator = (const BezierCubic &bezierCubic) {
                    if (&bezierCubic != this) {
                        p1 = bezierCubic.p1;
                        p2 = bezierCubic.p2;
                        p3 = bezierCubic.p3;
                        p4 = bezierCubic.p4;
                    }
                    return *this;
                }

                inline bool IsLinear (util::f32 eps = EPSILON) const {
                    return (p1 - p2).IsZero (eps) && (p3 - p4).IsZero (eps);
                }

                // Return approximate length of the spline by summing up segment lengths.
                util::f32 GetLength (util::ui32 steps) const;

                // Calculate point on the curve at parameter t.
                inline T GetPoint (
                        util::f32 t,
                        bool optimize = true,
                        util::f32 eps = EPSILON) const {
                    return optimize && IsLinear (eps) ?
                        util::LERP (t, p1, p4) :
                        B0 (t) * p1 + B1 (t) * p2 + B2 (t) * p3 + B3 (t) * p4;
                }

                // Given parameter 0.0 < t < 1.0, splits the spline in two left and
                // right pieces (keeping curvature intact).
                void Split (
                    util::f32 t,
                    BezierCubic &left,
                    BezierCubic &right,
                    bool optimize = true,
                    util::f32 eps = EPSILON) const;

                struct DefaultFlatnessTest :
                        public std::unary_function<const BezierCubic<T> &, bool> {
                    util::f32 flatness;
                    DefaultFlatnessTest (util::f32 flatness_ = EPSILON) :
                        flatness (flatness_) {}
                    bool operator () (const BezierCubic<T> &cubic) {
                        // This test was lifted directly from:
                        // http://www.antigrain.com/research/adaptive_bezier/index.html
                        return
                            fabsf (cubic.p1.x + cubic.p3.x - cubic.p2.x - cubic.p2.x) +
                            fabsf (cubic.p1.y + cubic.p3.y - cubic.p2.y - cubic.p2.y) +
                            fabsf (cubic.p2.x + cubic.p4.x - cubic.p3.x - cubic.p3.x) +
                            fabsf (cubic.p2.y + cubic.p4.y - cubic.p3.y - cubic.p3.y) <= flatness;
                    }
                };

                enum Detail {
                    Low,
                    Medium,
                    High
                };
                blas::Polygon<T> ToPolygon (
                    util::ui32 steps,
                    Detail detail = High,
                    bool optimize = false) const;

                void Rasterize (
                    std::vector<T> &points,
                    const std::unary_function<const BezierCubic<T> &, bool> &testFlatness =
                        DefaultFlatnessTest ()) const;

                bool InBound (const Bound<T> &bound, util::ui32 steps) const;
                bool CrossBound (const Bound<T> &bound, util::ui32 steps) const;

                bool InPolygon (const Polygon<T> &polygon, util::ui32 steps) const;
                bool CrossPolygon (const Polygon<T> &polygon, util::ui32 steps) const;

                bool InCircle (const Circle &circle, util::ui32 steps) const;
                bool CrossCircle (const Circle &circle, util::ui32 steps) const;

                bool InEllipse (const Ellipse &ellipse, util::ui32 steps) const;
                bool CrossEllipse (const Ellipse &ellipse, util::ui32 steps) const;

                void CrossHorizontal (
                    util::f32 y,
                    util::ui32 steps,
                    std::vector<util::f32> &intersections) const;
                void CrossVertical (
                    util::f32 x,
                    util::ui32 steps,
                    std::vector<util::f32> &intersections) const;

            private:
                void RasterizeHelper (
                    std::vector<T> &points,
                    const std::unary_function<const BezierCubic<T> &, bool> &testFlatness) const;
            };

            typedef BezierCubic<Point2> BezierCubic2;
            typedef BezierCubic<Point3> BezierCubic3;

            template<typename T>
            util::f32 BezierCubic<T>::GetLength (util::ui32 steps) const {
                util::f32 length = 0.0f;
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                T p1_ = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    T p2_ = GetPoint (t);
                    length += (p2_ - p1_).Length ();
                    p1_ = p2_;
                }
                return length;
            }

            template<typename T>
            void BezierCubic<T>::Split (
                    util::f32 t,
                    BezierCubic<T> &left,
                    BezierCubic<T> &right,
                    bool optimize,
                    util::f32 eps) const {
                if (optimize && IsLinear (eps)) {
                    left.p1 = left.p2 = p1;
                    right.p3 = right.p4 = p4;
                    left.p3 = left.p4 =
                        right.p1 = right.p2 = util::LERP (t, p1, p4);
                }
                else {
                    // first level
                    left.p1 = p1;
                    right.p4 = p4;
                    // second level
                    left.p2 = util::LERP (t, p1, p2);
                    T pt = util::LERP (t, p2, p3);
                    right.p3 = util::LERP (t, p3, p4);
                    // third level
                    left.p3 = util::LERP (t, left.p2, pt);
                    right.p2 = util::LERP (t, pt, right.p3);
                    // fourth level
                    left.p4 = util::LERP (t, left.p3, right.p2);
                    right.p1 = left.p4;
                }
            }

            template<typename T>
            blas::Polygon<T> BezierCubic<T>::ToPolygon (
                    util::ui32 steps,
                    Detail detail,
                    bool optimize) const {
                blas::Polygon<T> polygon;
                if (detail == Low/* || optimize && IsLinear ()*/) {
                    polygon.points.push_back (p1);
                    polygon.points.push_back (p4);
                }
                else {
                    polygon.points.push_back (p1);
                    util::f32 delta = 0.0f;
                    util::ui32 increment = 0;
                    if (detail == Medium) {
                        delta = 2.0f / (steps + 1);
                        increment = 2;
                    }
                    else if (detail == High) {
                        delta = 1.0f / (steps + 1);
                        increment = 1;
                    }
                    util::f32 t = delta;
                    for (util::ui32 j = 1; j < steps + 1; j += increment, t += delta) {
                        polygon.points.push_back (GetPoint (t, optimize));
                    }
                    polygon.points.push_back (p4);
                }
                polygon.closed = false;
                return polygon;
            }

            template<typename T>
            void BezierCubic<T>::Rasterize (
                    std::vector<T> &points,
                    const std::unary_function<const BezierCubic<T> &, bool> &testFlatness) const {
                points.push_back (p1);
                RasterizeHelper (points, testFlatness);
            }

            template<typename T>
            bool BezierCubic<T>::InBound (
                    const Bound<T> &bound,
                    util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = 0.0f;
                for (util::ui32 i = 0; i <= steps + 1; ++i, t += delta) {
                    if (!GetPoint (t).InBound (bound)) {
                        return false;
                    }
                }
                return true;
            }

            template<typename T>
            bool BezierCubic<T>::CrossBound (const Bound<T> &bound, util::ui32 steps) const {
                util::f32 delta = 1.0f / (steps + 1);
                util::f32 t = delta;
                T pt1 = p1;
                for (util::ui32 i = 1; i <= steps + 1; ++i, t += delta) {
                    T pt2 = GetPoint (t);
                    if (LineT<T> (pt1, pt2).CrossBound (bound)) {
                        return true;
                    }
                    pt1 = pt2;
                }
                return false;
            }

            template<typename T>
            void BezierCubic<T>::RasterizeHelper (
                    std::vector<T> &points,
                    const std::unary_function<const BezierCubic<T> &, bool> &testFlatness) const {
                if (testFlatness (*this)) {
                    points.push_back (p4);
                }
                else {
                    BezierCubic<T> left;
                    BezierCubic<T> right;
                    Split (0.5f, left, right);
                    left.RasterizeHelper (points, testFlatness);
                    right.RasterizeHelper (points, testFlatness);
                }
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL BezierCubic2 _LIB_THEKOGANS_MEX_BLAS_API Arc (
                const Point2 &center,
                util::f32 radius,
                util::f32 startAngle,
                util::f32 spanAngle);
            _LIB_THEKOGANS_MEX_BLAS_DECL BezierCubic2 _LIB_THEKOGANS_MEX_BLAS_API EllipticArc (
                util::f32 a,
                util::f32 b);

            template<typename T>
            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const BezierCubic<T> &bezierCubic) {
                serializer << bezierCubic.p1 << bezierCubic.p2 << bezierCubic.p3 << bezierCubic.p4;
                return serializer;
            }

            template<typename T>
            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    BezierCubic<T> &bezierCubic) {
                serializer >> bezierCubic.p1 >> bezierCubic.p2 >> bezierCubic.p3 >> bezierCubic.p4;
                return serializer;
            }

             template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::InPolygon (
                const Polygon2 &polygon,
                util::ui32 steps) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::CrossPolygon (
                const Polygon2 &polygon,
                util::ui32 steps) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::InCircle (
                const Circle &circle,
                util::ui32 steps) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::CrossCircle (
                const Circle &circle,
                util::ui32 steps) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::InEllipse (
                const Ellipse &ellipse,
                util::ui32 steps) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool BezierCubic2::CrossEllipse (
                const Ellipse &ellipse,
                util::ui32 steps) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL void BezierCubic2::CrossHorizontal (
                util::f32 y,
                util::ui32 steps,
                std::vector<util::f32> &intersections) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL void BezierCubic2::CrossVertical (
                util::f32 x,
                util::ui32 steps,
                std::vector<util::f32> &intersections) const;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_BezierCubic_h)
