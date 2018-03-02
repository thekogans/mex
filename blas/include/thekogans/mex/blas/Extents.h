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

#if !defined (__thekogans_mex_blas_Extents_h)
#define __thekogans_mex_blas_Extents_h

#include <cassert>
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Rect;
            struct Size;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Extents {
                util::ui32 width;
                util::ui32 height;

                static const Extents Empty;

                inline Extents () {}
                inline Extents (util::ui32 width_, util::ui32 height_) : width (width_), height (height_) {}

                inline Extents &operator = (const Extents &extents) {
                    width = extents.width;
                    height = extents.height;
                    return *this;
                }
            };

            inline bool operator == (const Extents &extents1, const Extents &extents2) {
                return extents1.width == extents2.width && extents1.height == extents2.height;
            }

            inline bool operator != (const Extents &extents1, const Extents &extents2) {
                return extents1.width != extents2.width || extents1.height != extents2.height;
            }

            inline util::Serializer &operator << (util::Serializer &serializer, const Extents &extentst) {
                serializer << extentst.width << extentst.height;
                return serializer;
            }

            inline util::Serializer &operator >> (util::Serializer &serializer, Extents &extentst) {
                serializer >> extentst.width >> extentst.height;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Extents_h)
