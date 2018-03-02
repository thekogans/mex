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

#if !defined (__thekogans_mex_blas_Rect_h)
#define __thekogans_mex_blas_Rect_h

#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Size;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Rect {
                util::i32 left;
                util::i32 top;
                util::i32 right;
                util::i32 bottom;

                static const Rect Zero;

                Rect () {}
                Rect (util::i32 left_, util::i32 top_, util::i32 right_, util::i32 bottom_) :
                    left (left_), top (top_), right (right_), bottom (bottom_) {}
                explicit Rect (const Size &size);

                inline Rect &operator = (const Rect &rect) {
                    if (this != &rect) {
                        left = rect.left;
                        top = rect.top;
                        right = rect.right;
                        bottom = rect.bottom;
                    }
                    return *this;
                }

                Rect &operator = (const Size &size);

                inline Rect &operator += (const Point &pt) {
                    left += pt.x;
                    top += pt.y;
                    right += pt.x;
                    bottom += pt.y;
                    return *this;
                }

                inline Rect &operator -= (const Point &pt) {
                    left -= pt.x;
                    top -= pt.y;
                    right -= pt.x;
                    bottom -= pt.y;
                    return *this;
                }

                inline Point Center () const {
                    return Point ((left + right) / 2, (top + bottom) / 2);
                }
                inline bool IsEmpty () const {
                    return Width () <= 0 || Height () <= 0;
                }

                inline util::i32 Width () const {
                    return right - left + 1;
                }
                inline util::i32 Height () const {
                    return bottom - top + 1;
                }
            };

            _LIB_THEKOGANS_MEX_BLAS_DECL Rect _LIB_THEKOGANS_MEX_BLAS_API operator & (const Rect &r1, const Rect &r2);
            _LIB_THEKOGANS_MEX_BLAS_DECL Rect _LIB_THEKOGANS_MEX_BLAS_API operator | (const Rect &r1, const Rect &r2);

            inline util::Serializer &operator << (util::Serializer &serializer, const Rect &rect) {
                serializer << rect.left << rect.top << rect.right << rect.bottom;
                return serializer;
            }

            inline util::Serializer &operator >> (util::Serializer &serializer, Rect &rect) {
                serializer >> rect.left >> rect.top >> rect.right >> rect.bottom;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Rect_h)
