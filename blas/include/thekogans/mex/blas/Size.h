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

#if !defined (__thekogans_mex_blas_Size_h)
#define __thekogans_mex_blas_Size_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Extents.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Rect;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Size {
                // *** IMPORTANT ***: (x, y) = lower left hand corner
                util::ui16 x;
                util::ui16 y;
                util::ui16 width;
                util::ui16 height;

                static const Size Empty;

                Size () {}
                Size (
                    util::ui16 x_,
                    util::ui16 y_,
                    util::ui16 width_,
                    util::ui16 height_) :
                    x (x_),
                    y (y_),
                    width (width_),
                    height (height_) {}
                Size (
                    const Point &origin,
                    const Extents &extents) :
                    x (origin.x),
                    y (origin.y),
                    width (extents.width),
                    height (extents.height) {}
                Size (const Size &size) :
                    x (size.x),
                    y (size.y),
                    width (size.width),
                    height (size.height) {}
                explicit Size (const Rect &rect);

                inline Size &operator = (const Size &size) {
                    if (this != &size) {
                        x = size.x;
                        y = size.y;
                        width = size.width;
                        height = size.height;
                    }
                    return *this;
                }

                Size &operator = (const Rect &rect);

                inline util::ui16 Right () const {
                    return x + width - 1;
                }
                inline util::ui16 Top () const {
                    return y + height - 1;
                }

                inline void Swap (Size &size) {
                    std::swap (x, size.x);
                    std::swap (y, size.y);
                    std::swap (width, size.width);
                    std::swap (height, size.height);
                }
            };

            _LIB_THEKOGANS_MEX_BLAS_DECL Size _LIB_THEKOGANS_MEX_BLAS_API operator & (
                const Size &s1,
                const Size &s2);
            _LIB_THEKOGANS_MEX_BLAS_DECL Size _LIB_THEKOGANS_MEX_BLAS_API operator | (
                const Size &s1,
                const Size &s2);

            inline bool operator == (
                    const Size &size1,
                    const Size &size2) {
                return size1.x == size2.x && size1.y == size2.y &&
                    size1.width == size2.width && size1.height == size2.height;
            }

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Size &size) {
                serializer << size.x << size.y << size.width << size.height;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    Size &size) {
                serializer >> size.x >> size.y >> size.width >> size.height;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Size_h)
