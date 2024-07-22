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

#if !defined (__thekogans_mex_blas_MathUtil_h)
#define __thekogans_mex_blas_MathUtil_h

#include <cmath>
#include <cstdlib>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            inline util::f32 atoff (const char *value) {
                return (util::f32)atof (value);
            }
            inline util::f32 fabsf (util::f32 value) {
                return (util::f32)fabs (value);
            }
            inline util::f32 sqrtf (util::f32 value) {
                return (util::f32)sqrt (value);
            }
            inline util::f32 cosf (util::f32 value) {
                return (util::f32)cos (value);
            }
            inline util::f32 sinf (util::f32 value) {
                return (util::f32)sin (value);
            }
            inline util::f32 tanf (util::f32 value) {
                return (util::f32)tan (value);
            }
            inline util::f32 atanf (util::f32 value) {
                return (util::f32)atan (value);
            }
            inline util::f32 acosf (util::f32 value) {
                // acos that does not return NaN with rounding errors
                if (value <= -1.0f) {
                    return PI;
                }
                if (value >= 1.0f) {
                    return 0.0f;
                }
                return (util::f32)acos (value);
            }
            inline util::f32 atan2f (
                    util::f32 x,
                    util::f32 y) {
                return (util::f32)atan2 (x, y);
            }
            inline util::f32 fmodf (
                    util::f32 x,
                    util::f32 y) {
                return (util::f32)fmod (x, y);
            }
            inline util::f32 powf (
                    util::f32 x,
                    util::f32 y) {
                return (util::f32)pow (x, y);
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_MathUtil_h)
