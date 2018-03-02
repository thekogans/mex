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

#if !defined (__thekogans_mex_blas_AngleAxis_h)
#define __thekogans_mex_blas_AngleAxis_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point3.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Quaternion;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL AngleAxis {
                util::f32 angle;
                Point3 axis;

                AngleAxis () {}
                AngleAxis (util::f32 angle_, const Point3 &axis_) :
                    angle (angle_), axis (axis_) {}

                static AngleAxis FromQuaternion (const Quaternion &quat);
            };

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const AngleAxis &angleAxis) {
                serializer << angleAxis.angle << angleAxis.axis;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    AngleAxis &angleAxis) {
                serializer >> angleAxis.angle >> angleAxis.axis;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_AngleAxis_h)
