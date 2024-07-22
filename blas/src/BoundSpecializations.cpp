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

#include <cassert>
#include <algorithm>
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/Polygon.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 &Bound2::operator += (const Point2 &pt) {
                if (min.x > pt.x) {
                    min.x = pt.x;
                }
                if (max.x < pt.x) {
                    max.x = pt.x;
                }
                if (min.y > pt.y) {
                    min.y = pt.y;
                }
                if (max.y < pt.y) {
                    max.y = pt.y;
                }
                return *this;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 &Bound3::operator += (const Point3 &pt) {
                if (min.x > pt.x) {
                    min.x = pt.x;
                }
                if (max.x < pt.x) {
                    max.x = pt.x;
                }
                if (min.y > pt.y) {
                    min.y = pt.y;
                }
                if (max.y < pt.y) {
                    max.y = pt.y;
                }
                if (min.z > pt.z) {
                    min.z = pt.z;
                }
                if (max.z < pt.z) {
                    max.z = pt.z;
                }
                return *this;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 operator + (
                    const Bound2 &bound,
                    const Point2 &pt) {
                return Bound2 (
                    Point2 (std::min (bound.min.x, pt.x), std::min (bound.min.y, pt.y)),
                    Point2 (std::max (bound.max.x, pt.x), std::max (bound.max.y, pt.y)));
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 operator + (
                    const Bound3 &bound,
                    const Point3 &pt) {
                return Bound3 (
                    Point3 (std::min (bound.min.x, pt.x), std::min (bound.min.y, pt.y), std::min (bound.min.z, pt.z)),
                    Point3 (std::max (bound.max.x, pt.x), std::max (bound.max.y, pt.y), std::max (bound.max.z, pt.z)));
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 Bound2::Normalize () const {
                return Bound2 (
                    Point2 (std::min (min.x, max.x), std::min (min.y, max.y)),
                    Point2 (std::max (min.x, max.x), std::max (min.y, max.y)));
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 Bound3::Normalize () const {
                return Bound3 (
                    Point3 (std::min (min.x, max.x), std::min (min.y, max.y), std::min (min.z, max.z)),
                    Point3 (std::max (min.x, max.x), std::max (min.y, max.y), std::max (min.z, max.z)));
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 Bound2::Area () const {
                Point2 extents = Extents ();
                return extents.x * extents.y;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 Bound3::Area () const {
                Point3 extents = Extents ();
                return 2.0f * (extents.x * extents.y + extents.x * extents.z + extents.y * extents.z);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::InCircle (const Circle &circle) const {
                return
                    Point2 (min.x, max.y).InCircle (circle) &&
                    Point2 (max.x, min.y).InCircle (circle) &&
                    min.InCircle (circle) &&
                    max.InCircle (circle);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossCircle (const Circle &circle) const {
                return
                    Point2 (min.x, max.y).InCircle (circle) ||
                    Point2 (max.x, min.y).InCircle (circle) ||
                    min.InCircle (circle) ||
                    max.InCircle (circle);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::InEllipse (const Ellipse &ellipse) const {
                return
                    Point2 (min.x, max.y).InEllipse (ellipse) &&
                    Point2 (max.x, min.y).InEllipse (ellipse) &&
                    min.InEllipse (ellipse) &&
                    max.InEllipse (ellipse);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossEllipse (const Ellipse &ellipse) const {
                return
                    Point2 (min.x, max.y).InEllipse (ellipse) ||
                    Point2 (max.x, min.y).InEllipse (ellipse) ||
                    min.InEllipse (ellipse) ||
                    max.InEllipse (ellipse);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossBound (const Bound2 &bound) const {
                return !(bound.max.x < min.x || bound.min.x > max.x ||
                    bound.max.y < min.y || bound.min.y > max.y);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound3::CrossBound (const Bound3 &bound) const {
                return !(bound.max.x < min.x || bound.min.x > max.x ||
                    bound.max.y < min.y || bound.min.y > max.y ||
                    bound.max.z < min.z || bound.min.z > max.z);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::InPolygon (const Polygon2 &polygon) const {
                Point2 topLeft (min.x, max.y);
                Point2 rightBottom (max.x, min.y);
                return
                    Line2 (min, rightBottom).InPolygon (polygon) &&
                    Line2 (rightBottom, max).InPolygon (polygon) &&
                    Line2 (max, topLeft).InPolygon (polygon) &&
                    Line2 (topLeft, min).InPolygon (polygon);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossPolygon (const Polygon2 &polygon) const {
                Point2 topLeft (min.x, max.y);
                Point2 rightBottom (max.x, min.y);
                return
                    Line2 (min, rightBottom).InPolygon (polygon) ||
                    Line2 (rightBottom, max).InPolygon (polygon) ||
                    Line2 (max, topLeft).InPolygon (polygon) ||
                    Line2 (topLeft, min).InPolygon (polygon);
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
