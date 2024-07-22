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

#include "thekogans/mex/blas/Point4.h"
#include "thekogans/mex/blas/Matrix4.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Point4 &Point4::operator *= (const Matrix4 &matrix) {
                util::f32 x_ = x * matrix.x.x + y * matrix.y.x + z * matrix.z.x + w * matrix.t.x;
                util::f32 y_ = x * matrix.x.y + y * matrix.y.y + z * matrix.z.y + w * matrix.t.y;
                util::f32 z_ = x * matrix.x.z + y * matrix.y.z + z * matrix.z.z + w * matrix.t.z;
                util::f32 w_ = x * matrix.x.w + y * matrix.y.w + z * matrix.z.w + w * matrix.t.w;
                x = x_;
                y = y_;
                z = z_;
                w = w_;
                return *this;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point4 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                    const Point4 &pt,
                    const Matrix4 &matrix) {
                return Point4 (
                    pt.x * matrix.x.x + pt.y * matrix.y.x + pt.z * matrix.z.x + pt.w * matrix.t.x,
                    pt.x * matrix.x.y + pt.y * matrix.y.y + pt.z * matrix.z.y + pt.w * matrix.t.y,
                    pt.x * matrix.x.z + pt.y * matrix.y.z + pt.z * matrix.z.z + pt.w * matrix.t.z,
                    pt.x * matrix.x.w + pt.y * matrix.y.w + pt.z * matrix.z.w + pt.w * matrix.t.w);
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
