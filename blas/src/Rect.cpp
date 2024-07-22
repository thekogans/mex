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

#include <algorithm>
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Rect.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Rect::Rect (const Size &size) :
                left (size.x),
                top (size.y),
                right (size.Right ()),
                bottom (size.Top ()) {}

            Rect &Rect::operator = (const Size &size) {
                left = size.x;
                top = size.y;
                right = size.Right ();
                bottom = size.Top ();
                return *this;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Rect _LIB_THEKOGANS_MEX_BLAS_API operator & (
                    const Rect &r1,
                    const Rect &r2) {
                return Rect (std::max (r1.left, r2.left), std::max (r1.top, r2.top),
                    std::min (r1.right, r2.right), std::min (r1.bottom, r2.bottom));
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Rect _LIB_THEKOGANS_MEX_BLAS_API operator | (
                    const Rect &r1,
                    const Rect &r2) {
                return Rect (std::min (r1.left, r2.left), std::min (r1.top, r2.top),
                    std::max (r1.right, r2.right), std::max (r1.bottom, r2.bottom));
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
