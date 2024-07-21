// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/ext/editor/Camera.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Camera)

                blas::Bound2 Camera::GetBound2 (const blas::Matrix3 &xform) const {
                    blas::Bound2 bound = blas::Bound2::Empty;
                    bound += blas::Point2 (camera.position * xform);
                    bound += blas::Point2 (camera.target * xform);
                    return bound;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
