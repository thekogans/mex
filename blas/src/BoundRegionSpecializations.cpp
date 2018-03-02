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

#include "thekogans/mex/blas/BoundRegion.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            THEKOGANS_UTIL_IMPLEMENT_HEAP_EX_T (BoundRegion2, 32)
            THEKOGANS_UTIL_IMPLEMENT_HEAP_EX_T (BoundRegion3, 32)

        } // namespace blas
    } // namespace mex
} // namespace thekogans
