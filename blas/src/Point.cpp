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

#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/blas/Size.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            bool Point::InRect (const Rect &rect) const {
                return x >= rect.left && x <= rect.right &&
                    y >= rect.top && y <= rect.bottom;
            }

            bool Point::InSize (const Size &size) const {
                return x >= size.x && x <= size.Right () &&
                    y >= size.y && y <= size.Top ();
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
