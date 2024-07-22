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

#if !defined (__thekogans_mex_blas_EulerAngles_h)
#define __thekogans_mex_blas_EulerAngles_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Quaternion;

            struct _LIB_THEKOGANS_MEX_BLAS_DECL EulerAngles {
                Point3 angles;

                EulerAngles () {}
                EulerAngles (const Point3 &angles_) :
                    angles (angles_) {}
                EulerAngles (const EulerAngles &eulerAngles) :
                    angles (eulerAngles.angles) {}

                static EulerAngles FromMatrix3 (const Matrix3 &matrix);
                static EulerAngles FromQuaternion (const Quaternion &quat);
            };

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const EulerAngles &eulerAngles) {
                serializer << eulerAngles.angles;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    EulerAngles &eulerAngles) {
                serializer >> eulerAngles.angles;
                return serializer;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_EulerAngles_h)
