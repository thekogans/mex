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

#if !defined (__thekogans_mex_blas_Ray_h)
#define __thekogans_mex_blas_Ray_h

#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Triangle.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct _LIB_THEKOGANS_MEX_BLAS_DECL Ray {
                Point3 origin;
                Point3 direction;
                Point3 oneOverDirection;
                struct Point3Sign {
                    util::ui32 x;
                    util::ui32 y;
                    util::ui32 z;
                    Point3Sign () {}
                    Point3Sign (const Point3 &pt) :
                        x (pt.x < 0.0f ? 1 : 0), y (pt.y < 0.0f ? 1 : 0), z (pt.z < 0.0f ? 1 : 0) {}
                } oneOverDirectionSign;

                Ray () {}
                Ray (const Point3 &origin_, const Point3 &direction_) :
                    origin (origin_), direction (direction_),
                    oneOverDirection (1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z),
                    oneOverDirectionSign (oneOverDirection) {}

                bool IntersectTriangleCull (const Triangle &triangle, Point3 &coords) const;
                bool IntersectTriangle (const Triangle &triangle, Point3 &coords) const;

                bool Intersect (const Bound<Point3> &bound, util::f32 &tmin, util::f32 &tmax) const;
            };

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Ray_h)
