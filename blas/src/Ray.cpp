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

#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Ray.h"
#include "thekogans/mex/blas/Triangle.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            bool Ray::IntersectTriangleCull (const Triangle &triangle, Point3 &coords) const {
                // find vectors for two edges sharing triangle.v0
                Point3 edge1 = triangle.v1 - triangle.v0;
                Point3 edge2 = triangle.v2 - triangle.v0;
                // begin calculating determinant - also used to calculate U parameter
                Point3 p = Cross (direction, edge2);
                // if determinant is near zero, ray lies in plane of triangle
                util::f32 det = Dot (edge1, p);
                if (det < EPSILON) {
                    return false;
                }
                // calculate distance from triangle.v0 to ray origin
                Point3 t = origin - triangle.v0;
                // calculate coords.y parameter and test bounds
                coords.y = Dot (t, p);
                if (coords.y < 0.0f || coords.y > det) {
                    return false;
                }
                // prepare to test coords.z parameter
                Point3 q = Cross (t, edge1);
                // calculate coords.z parameter and test bounds
                coords.z = Dot (direction, q);
                if (coords.z < 0.0f || coords.y + coords.z > det) {
                    return false;
                }
                // calculate coords.x, scale parameters, ray intersects triangle
                coords.x = Dot (edge2, q);
                coords /= det;
                return true;
            }

            bool Ray::IntersectTriangle (const Triangle &triangle, Point3 &coords) const {
                // find vectors for two edges sharing triangle.v0
                Point3 edge1 = triangle.v1 - triangle.v0;
                Point3 edge2 = triangle.v2 - triangle.v0;
                // begin calculating determinant - also used to calculate coords.y parameter
                Point3 p = Cross (direction, edge2);
                // if determinant is near zero, ray lies in plane of triangle
                util::f32 det = Dot (edge1, p);
                if (util::IS_ZERO (det)) {
                    return false;
                }
                det = 1.0f / det;
                // calculate distance from triangle.v0 to ray origin
                Point3 t = origin - triangle.v0;
                // calculate coords.y parameter and test bounds
                coords.y = Dot (t, p) * det;
                if (coords.y < 0.0f || coords.y > 1.0f) {
                    return false;
                }
                // prepare to test coords.z parameter
                Point3 q = Cross (t, edge1);
                // calculate coords.z parameter and test bounds
                coords.z = Dot (direction, q) * det;
                if (coords.z < 0.0f || coords.y + coords.z > 1.0f) {
                    return false;
                }
                // calculate coords.x, ray intersects triangle
                coords.x = Dot (edge2, q) * det;
                return true;
            }

            bool Ray::Intersect (const Bound3 &bound, util::f32 &tmin, util::f32 &tmax) const {
                tmin = (bound[oneOverDirectionSign.x].x - origin.x) * oneOverDirection.x;
                tmax = (bound[1 - oneOverDirectionSign.x].x - origin.x) * oneOverDirection.x;
                util::f32 tymin = (bound[oneOverDirectionSign.y].y - origin.y) * oneOverDirection.y;
                util::f32 tymax = (bound[1 - oneOverDirectionSign.y].y - origin.y) * oneOverDirection.y;
                if (tmin > tymax || tymin > tmax) {
                    return false;
                }
                if (tmin < tymin) {
                    tmin = tymin;
                }
                if (tmax > tymax) {
                    tmax = tymax;
                }
                util::f32 tzmin = (bound[oneOverDirectionSign.z].z - origin.z) * oneOverDirection.z;
                util::f32 tzmax = (bound[1 - oneOverDirectionSign.z].z - origin.z) * oneOverDirection.z;
                if (tmin > tzmax || tzmin > tmax) {
                    return false;
                }
                if (tmin < tzmin) {
                    tmin = tzmin;
                }
                if (tmax > tzmax) {
                    tmax = tzmax;
                }
                return true;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
