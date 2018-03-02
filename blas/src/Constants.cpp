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

#include <cfloat>
#include "thekogans/mex/blas/Extents.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Point4.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/blas/Quaternion.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            // These are all collected here to make sure they
            // get initialized in order.
            const Extents Extents::Empty (0, 0);
            const Point Point::Zero (0, 0);
            const Rect Rect::Zero (0, 0, 0, 0);

            const Point2 Point2::Zero (0.0f, 0.0f);
            const Point2 Point2::One (1.0f, 1.0f);
            const Point2 Point2::Big (FLT_MAX, FLT_MAX);
            const Point2 Point2::NegBig (-FLT_MAX, -FLT_MAX);
            const Point2 Point2::Small (FLT_MIN, FLT_MIN);
            const Point2 Point2::NegSmall (-FLT_MIN, -FLT_MIN);
            const Point2 Point2::X (1.0f, 0.0f);
            const Point2 Point2::Y (0.0f, 1.0f);

            const Point3 Point3::Zero (0.0f, 0.0f, 0.0f);
            const Point3 Point3::One (1.0f, 1.0f, 1.0f);
            const Point3 Point3::Big (FLT_MAX, FLT_MAX, FLT_MAX);
            const Point3 Point3::NegBig (-FLT_MAX, -FLT_MAX, -FLT_MAX);
            const Point3 Point3::Small (FLT_MIN, FLT_MIN, FLT_MIN);
            const Point3 Point3::NegSmall (-FLT_MIN, -FLT_MIN, -FLT_MIN);
            const Point3 Point3::X (1.0f, 0.0f, 0.0f);
            const Point3 Point3::Y (0.0f, 1.0f, 0.0f);
            const Point3 Point3::Z (0.0f, 0.0f, 1.0f);

            const Point4 Point4::Zero (0.0f, 0.0f, 0.0f, 1.0f);
            const Point4 Point4::One (1.0f, 1.0f, 1.0f, 1.0f);
            const Point4 Point4::Big (FLT_MAX, FLT_MAX, FLT_MAX, 1.0f);
            const Point4 Point4::NegBig (-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0f);
            const Point4 Point4::Small (FLT_MIN, FLT_MIN, FLT_MIN, 1.0f);
            const Point4 Point4::NegSmall (-FLT_MIN, -FLT_MIN, -FLT_MIN, 1.0f);
            const Point4 Point4::X (1.0f, 0.0f, 0.0f, 1.0f);
            const Point4 Point4::Y (0.0f, 1.0f, 0.0f, 1.0f);
            const Point4 Point4::Z (0.0f, 0.0f, 1.0f, 1.0f);

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL const Bound2 Bound2::Empty (Point2::Big, Point2::NegBig);
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL const Bound3 Bound3::Empty (Point3::Big, Point3::NegBig);

            const Matrix2 Matrix2::Zero (Point2::Zero, Point2::Zero, Point2::Zero);
            const Matrix2 Matrix2::Identity (Point2::X, Point2::Y, Point2::Zero);
            const Matrix2 Matrix2::MirrorX (-Point2::X, Point2::Y, Point2::Zero);
            const Matrix2 Matrix2::MirrorY (Point2::X, -Point2::Y, Point2::Zero);
            const Matrix2 Matrix2::MirrorXY (-Point2::X, -Point2::Y, Point2::Zero);

            const Matrix3 Matrix3::Zero (Point3::Zero, Point3::Zero, Point3::Zero, Point3::Zero);
            const Matrix3 Matrix3::Identity (Point3::X, Point3::Y, Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorX (-Point3::X, Point3::Y, Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorY (Point3::X, -Point3::Y, Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorZ (Point3::X, Point3::Y, -Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorXY (-Point3::X, -Point3::Y, Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorXZ (-Point3::X, Point3::Y, -Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorYZ (Point3::X, -Point3::Y, -Point3::Z, Point3::Zero);
            const Matrix3 Matrix3::MirrorXYZ (-Point3::X, -Point3::Y, -Point3::Z, Point3::Zero);

            const Matrix4 Matrix4::Zero (
                Point4 (Point3::Zero, 0.0f),
                Point4 (Point3::Zero, 0.0f),
                Point4 (Point3::Zero, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::Identity (
                Point4 (Point3::X, 0.0f),
                Point4 (Point3::Y, 0.0f),
                Point4 (Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorX (
                Point4 (-Point3::X, 0.0f),
                Point4 (Point3::Y, 0.0f),
                Point4 (Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorY (
                Point4 (Point3::X, 0.0f),
                Point4 (-Point3::Y, 0.0f),
                Point4 (Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorZ (
                Point4 (Point3::X, 0.0f),
                Point4 (Point3::Y, 0.0f),
                Point4 (-Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorXY (
                Point4 (-Point3::X, 0.0f),
                Point4 (-Point3::Y, 0.0f),
                Point4 (Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorXZ (
                Point4 (-Point3::X, 0.0f),
                Point4 (Point3::Y, 0.0f),
                Point4 (-Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorYZ (
                Point4 (Point3::X, 0.0f),
                Point4 (-Point3::Y, 0.0f),
                Point4 (-Point3::Z, 0.0f),
                Point4::Zero);
            const Matrix4 Matrix4::MirrorXYZ (
                Point4 (-Point3::X, 0.0f),
                Point4 (-Point3::Y, 0.0f),
                Point4 (-Point3::Z, 0.0f),
                Point4::Zero);

            const Quaternion Quaternion::Zero (0.0f, 0.0f, 0.0f, 0.0f);
            const Quaternion Quaternion::Identity (0.0f, 0.0f, 0.0f, 1.0f);

        } // namespace blas
    } // namespace mex
} // namespace thekogans
