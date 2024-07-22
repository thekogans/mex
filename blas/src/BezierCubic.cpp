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
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            namespace {
                inline util::f32 L (util::f32 alpha) {
                    return 4.0f / 3.0f * tanf (alpha / 4.0f);
                }
                inline util::f32 e1 (util::f32 alpha) {
                    return 4.0f / 27.0f * powf (sinf (alpha / 4.0f), 6.0f) / powf (cosf (alpha / 4.0f), 2.0f);
                }
                inline util::f32 rho (util::f32 alpha) {
                    return sqrtf (2.0f / (2.0f + e1 (alpha)));
                }

                inline BezierCubic2 UnitArc (util::f32 alpha) {
                    util::f32 l = L (alpha);
                    util::f32 ca = cosf (alpha);
                    util::f32 sa = sinf (alpha);
                    return BezierCubic2 (
                        Point2 (1.0f, 0.0f), Point2 (1.0f, l),
                        Point2 (ca + l * sa, sa - l * ca), Point2 (ca, sa));
                }
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL BezierCubic2 _LIB_THEKOGANS_MEX_BLAS_API Arc (
                    const Point2 &center,
                    util::f32 radius,
                    util::f32 startAngle,
                    util::f32 spanAngle) {
                BezierCubic2 bezierCubic = UnitArc (spanAngle);
                Matrix3 matrix =
                    Matrix3::RotateZ (startAngle) *
                    Matrix3::Scale (Point3 (radius, radius, 1.0f)) *
                    Matrix3::Translate (center);
                Point3 p1 = bezierCubic.p1 * matrix;
                Point3 p2 = bezierCubic.p2 * matrix;
                Point3 p3 = bezierCubic.p3 * matrix;
                Point3 p4 = bezierCubic.p4 * matrix;
                return BezierCubic2 (
                    Point2 (p1.x, p1.y), Point2 (p2.x, p2.y),
                    Point2 (p3.x, p3.y), Point2 (p4.x, p4.y));
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL BezierCubic2 _LIB_THEKOGANS_MEX_BLAS_API EllipticArc (
                    util::f32 a,
                    util::f32 b) {
                const util::f32 k = 0.5522847498308f;
                return BezierCubic2 (
                    Point2 (a, 0.0f), Point2 (a, b * k),
                    Point2 (a * k, b), Point2 (0.0f, b));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
