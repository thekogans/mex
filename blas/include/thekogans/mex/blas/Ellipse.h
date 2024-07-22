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

#if !defined (__thekogans_mex_blas_Ellipse_h)
#define __thekogans_mex_blas_Ellipse_h

#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Polygon.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Circle;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Ellipse {
                Bound2 bound;

                Ellipse () {}
                explicit Ellipse (const Bound2 &bound_) :
                    bound (bound_) {}
                Ellipse (const Ellipse &ellipse) :
                    bound (ellipse.bound) {}

                inline Ellipse &operator = (const Ellipse &ellipse) {
                    if (&ellipse != this) {
                        bound = ellipse.bound;
                    }
                    return *this;
                }

                Polygon2 ToPolygon (std::size_t pointCount) const;

                bool InBound (const Bound2 &bound) const;
                bool InBound (const Bound3 &bound) const;
                bool CrossBound (const Bound2 &bound) const;
                bool CrossBound (const Bound3 &bound) const;

                bool InCircle (const Circle &circle) const;
                bool CrossCircle (const Circle &circle) const;

                bool InEllipse (const Ellipse &ellipse) const;
                bool CrossEllipse (const Ellipse &ellipse) const;

                bool InPolygon (const Polygon2 &polygon) const;
                bool InPolygon (const Polygon3 &polygon) const;
                bool CrossPolygon (const Polygon2 &polygon) const;
                bool CrossPolygon (const Polygon3 &polygon) const;
            };

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Ellipse &ellipse) {
                serializer << ellipse.bound;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    Ellipse &ellipse) {
                serializer >> ellipse.bound;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Ellipse_h)
