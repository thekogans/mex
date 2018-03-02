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

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 _LIB_THEKOGANS_MEX_BLAS_API Bound2To3 (const Bound2 &bound) {
                return Bound3 (bound.min, bound.max);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 _LIB_THEKOGANS_MEX_BLAS_API Bound3To2 (const Bound3 &bound) {
                return Bound2 (Point2 (bound.min), Point2 (bound.max));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
