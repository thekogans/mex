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

#include <cmath>
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Polygon.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Point3::Point3 (
                const Point2 &pt,
                util::f32 z_) :
                x (pt.x),
                y (pt.y),
                z (z_) {}

            Point3 &Point3::operator = (const Point2 &pt) {
                x = pt.x;
                y = pt.y;
                z = 0.0f;
                return *this;
            }

            Point3 &Point3::operator *= (const Matrix3 &matrix) {
                util::f32 x_ = x * matrix.x.x + y * matrix.y.x + z * matrix.z.x + matrix.t.x;
                util::f32 y_ = x * matrix.x.y + y * matrix.y.y + z * matrix.z.y + matrix.t.y;
                util::f32 z_ = x * matrix.x.z + y * matrix.y.z + z * matrix.z.z + matrix.t.z;
                x = x_;
                y = y_;
                z = z_;
                return *this;
            }

            util::f32 Point3::Normalize (util::f32 eps) {
                util::f32 length = Length ();
                if (!util::IS_ZERO (length, eps)) {
                    util::f32 s = 1.0f / length;
                    x *= s;
                    y *= s;
                    z *= s;
                }
                return length;
            }

            bool Point3::InBound (const Bound2 &bound) const {
                return !(x < bound.min.x || x > bound.max.x ||
                    y < bound.min.y || y > bound.max.y || z != 0.0f);
            }

            bool Point3::InBound (const Bound3 &bound) const {
                return !(x < bound.min.x || x > bound.max.x ||
                    y < bound.min.y || y > bound.max.y ||
                    z < bound.min.z || z > bound.max.z);
            }

            bool Point3::InPolygon (const Polygon3 &polygon) const {
                if (!polygon.closed) {
                    return false;
                }
                util::i32 nc = 0;
                util::f32 va = polygon.points[0].y - y;
                util::i32 sva = util::SIGN2 (va);
                for (std::size_t i = 0, count = polygon.points.size (); i < count; ++i) {
                    util::f32 vb = polygon.points[(i + 1) % polygon.points.size ()].y - y;
                    util::i32 svb = util::SIGN2 (vb);
                    if (sva != svb) {
                        util::f32 ua = polygon.points[i].x - x;
                        util::f32 ub = polygon.points[(i + 1) % polygon.points.size ()].x - x;
                        if ((ua > 0.0f && ub > 0.0f) || ((ua > 0.0f || ub > 0.0f) && (ua - va * (ub - ua) / (vb - va) > 0.0f))) {
                            ++nc;
                        }
                        sva = svb;
                    }
                    va = vb;
                }
                return (nc & 1) == 1;
            }

            Point2 Point3::Project (
                    bool perspective,
                    util::f32 eps) const {
                if (!perspective) {
                    return Point2 (x, y);
                }
                if (!util::IS_ZERO (z)) {
                    util::f32 oneOverZ = 1.0f / z;
                    return Point2 (x * oneOverZ, y * oneOverZ);
                }
                return Point2::Zero;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point3 _LIB_THEKOGANS_MEX_BLAS_API operator * (
                    const Point3 &pt,
                    const Matrix3 &matrix) {
                return Point3 (
                    pt.x * matrix.x.x + pt.y * matrix.y.x + pt.z * matrix.z.x + matrix.t.x,
                    pt.x * matrix.x.y + pt.y * matrix.y.y + pt.z * matrix.z.y + matrix.t.y,
                    pt.x * matrix.x.z + pt.y * matrix.y.z + pt.z * matrix.z.z + matrix.t.z);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 _LIB_THEKOGANS_MEX_BLAS_API Angle (
                    const Point3 &p1,
                    const Point3 &p2,
                    util::f32 eps) {
                util::f32 m = p1.Length () * p2.Length ();
                if (util::IS_ZERO (m, eps)) {
                    return 0.0f;
                }
                m = Dot (p1, p2) / m;
                util::CLAMP (m, -1.0f, 1.0f);
                return acosf (m);
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL Point3 _LIB_THEKOGANS_MEX_BLAS_API Bisect (
                    const Point3 &p1,
                    const Point3 &p2,
                    util::f32 eps) {
                // FIXME: implement
                assert (0);
                return Point3::Zero;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API CLIPt (
                    util::f32 denom,
                    util::f32 num,
                    util::f32 &t1,
                    util::f32 &t2) {
                if (denom > 0.0f) {
                    util::f32 t = num / denom;
                    if (t > t2) {
                        return false;
                    }
                    else if (t > t1) {
                        t1 = t;
                    }
                }
                else if (denom < 0.0f) {
                    util::f32 t = num / denom;
                    if (t < t1) {
                        return false;
                    }
                    else if (t < t2) {
                        t2 = t;
                    }
                }
                else if (num > 0.0f) {
                    return false;
                }
                return true;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API ClipNearFar (
                    Point3 &p1,
                    Point3 &p2,
                    util::f32 nearPlane,
                    util::f32 farPlane) {
                util::f32 dz = p2.z - p1.z;
                util::f32 t1 = 0.0f;
                util::f32 t2 = 1.0f;
                if (CLIPt (dz, nearPlane - p1.z, t1, t2) && CLIPt (dz, p1.z - farPlane, t1, t2)) {
                    util::f32 dx = p2.x - p1.x;
                    util::f32 dy = p2.y - p1.y;
                    if (t2 < 1.0f) {
                        p2.x = p1.x + t2 * dx;
                        p2.y = p1.y + t2 * dy;
                        p2.z = p1.z + t2 * dz;
                    }
                    if (t1 > 0.0f) {
                        p1.x = p1.x + t1 * dx;
                        p1.y = p1.y + t1 * dy;
                        p1.z = p1.z + t1 * dz;
                    }
                    return true;
                }
                return false;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip3W (
                    Point3 &p1,
                    Point3 &p2,
                    util::f32 aspect) {
                util::f32 t1 = 0.0f;
                util::f32 t2 = 1.0f;
                util::f32 dx = p2.x - p1.x;
                util::f32 dy = p2.y - p1.y;
                util::f32 dz = p2.z - p1.z;
                if (CLIPt (dz              ,  1.0f - p1.z         , t1, t2) &&
                    CLIPt (dz - dy         ,  p1.y - p1.z         , t1, t2) &&
                    CLIPt (dz + dy         , -p1.y - p1.z         , t1, t2) &&
                    CLIPt (dz * aspect - dx,  p1.x - p1.z * aspect, t1, t2) &&
                    CLIPt (dz * aspect + dx, -p1.x - p1.z * aspect, t1, t2)) {
                    if (t2 < 1.0f) {
                        p2.x = p1.x + t2 * dx;
                        p2.y = p1.y + t2 * dy;
                        p2.z = p1.z + t2 * dz;
                    }
                    if (t1 > 0.0f) {
                        p1.x = p1.x + t1 * dx;
                        p1.y = p1.y + t1 * dy;
                        p1.z = p1.z + t1 * dz;
                    }
                    return true;
                }
                return false;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip3DH (
                    Point3 &p1,
                    Point3 &p2,
                    util::f32 aspect) {
                util::f32 t1 = 0.0f;
                util::f32 t2 = 1.0f;
                util::f32 dx = p2.x - p1.x;
                util::f32 dy = p2.y - p1.y;
                util::f32 dz = p2.z - p1.z;
                if (CLIPt (dz              ,  1.0f - p1.z         , t1, t2) &&
                    CLIPt (dz * aspect - dy,  p1.y - p1.z * aspect, t1, t2) &&
                    CLIPt (dz * aspect + dy, -p1.y - p1.z * aspect, t1, t2) &&
                    CLIPt (dz - dx         ,  p1.x - p1.z         , t1, t2) &&
                    CLIPt (dz + dx         , -p1.x - p1.z         , t1, t2)) {
                    if (t2 < 1.0f) {
                        p2.x = p1.x + t2 * dx;
                        p2.y = p1.y + t2 * dy;
                        p2.z = p1.z + t2 * dz;
                    }
                    if (t1 > 0.0f) {
                        p1.x = p1.x + t1 * dx;
                        p1.y = p1.y + t1 * dy;
                        p1.z = p1.z + t1 * dz;
                    }
                    return true;
                }
                return false;
            }

            _LIB_THEKOGANS_MEX_BLAS_DECL bool _LIB_THEKOGANS_MEX_BLAS_API Clip3D (
                    Point3 &p1,
                    Point3 &p2) {
                util::f32 t1 = 0.0f;
                util::f32 t2 = 1.0f;
                util::f32 dx = p2.x - p1.x;
                util::f32 dy = p2.y - p1.y;
                util::f32 dz = p2.z - p1.z;
                if (CLIPt (     dz,  1.0f - p1.z, t1, t2) &&
                    CLIPt (dz - dy,  p1.y - p1.z, t1, t2) &&
                    CLIPt (dy + dz, -p1.y - p1.z, t1, t2) &&
                    CLIPt (dz - dx,  p1.x - p1.z, t1, t2) &&
                    CLIPt (dx + dz, -p1.x - p1.z, t1, t2)) {
                    if (t2 < 1.0f) {
                        p2.x = p1.x + t2 * dx;
                        p2.y = p1.y + t2 * dy;
                        p2.z = p1.z + t2 * dz;
                    }
                    if (t1 > 0.0f) {
                        p1.x = p1.x + t1 * dx;
                        p1.y = p1.y + t1 * dy;
                        p1.z = p1.z + t1 * dz;
                    }
                    return true;
                }
                return false;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
