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
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/blas/Size.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            const Size Size::Empty (0, 0, 0, 0);

            Size::Size (const Rect &rect) :
                x (rect.left),
                y (rect.bottom),
                width (rect.Width ()),
                height (rect.Height ()) {}

            Size &Size::operator = (const Rect &rect) {
                x = rect.left;
                y = rect.bottom;
                width = rect.Width ();
                height = rect.Height ();
                return *this;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Size _LIB_THEKOGANS_MEX_BLAS_API operator & (
                    const Size &s1,
                    const Size &s2) {
                util::ui16 x = std::max (s1.x, s2.x);
                util::ui16 y = std::max (s1.y, s2.y);
                util::ui16 width = std::min (s1.Right (), s2.Right ()) - x;
                util::ui16 height = std::min (s1.Top (), s2.Top ()) - y;
                return Size (x, y, width, height);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Size _LIB_THEKOGANS_MEX_BLAS_API operator | (
                    const Size &s1,
                    const Size &s2) {
                util::ui16 x = std::min (s1.x, s2.x);
                util::ui16 y = std::min (s1.y, s2.y);
                util::ui16 width = std::max (s1.Right (), s2.Right ()) - x;
                util::ui16 height = std::max (s1.Top (), s2.Top ()) - y;
                return Size (x, y, width, height);
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
