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

#if !defined (__thekogans_mex_blas_Point_h)
#define __thekogans_mex_blas_Point_h

#include <cassert>
#include "thekogans/util/Constants.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Rect;
            struct Size;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Point {
                union {
                    struct {
                        util::i32 x;
                        util::i32 y;
                    };
                    struct {
                        util::i32 xy[2];
                    };
                };

                static const Point Zero;

                inline Point () {}
                inline Point (
                    util::i32 x_,
                    util::i32 y_) :
                    x (x_),
                    y (y_) {}
                inline Point (util::i32 xy[]) :
                    x (xy[0]),
                    y (xy[1]) {}

                inline util::i32 operator [] (util::ui32 index) const {
                    assert (index < 2);
                    return xy[index];
                }
                inline util::i32 &operator [] (util::ui32 index) {
                    assert (index < 2);
                    return xy[index];
                }

                inline Point &operator = (const Point &pt) {
                    x = pt.x;
                    y = pt.y;
                    return *this;
                }

                bool InRect (const Rect &rect) const;
                bool InSize (const Size &size) const;
            };

            inline Point operator * (
                    const Point &pt,
                    util::f32 scale) {
                return Point (util::ROUND (pt.x * scale), util::ROUND (pt.y * scale));
            }

            inline Point operator * (
                    util::f32 scale,
                    const Point &pt) {
                return Point (util::ROUND (pt.x * scale), util::ROUND (pt.y * scale));
            }

            inline Point operator / (
                    const Point &pt,
                    util::f32 scale) {
                return Point (util::ROUND (pt.x / scale), util::ROUND (pt.y / scale));
            }

            inline bool operator == (
                    const Point &p1,
                    const Point &p2) {
                return p1.x == p2.x && p1.y == p2.y;
            }

            inline bool operator != (
                    const Point &p1,
                    const Point &p2) {
                return p1.x != p2.x || p1.y != p2.y;
            }

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Point &pt) {
                serializer << pt.x << pt.y;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    Point &pt) {
                serializer >> pt.x >> pt.y;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Point_h)
