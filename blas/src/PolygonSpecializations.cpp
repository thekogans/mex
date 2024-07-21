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

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Polygon.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS_T (Polygon2)
            THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS_T (Polygon3)

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::IsClockWise () const {
                assert (closed);
                util::f32 normalZ = 0.0f;
                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                    std::size_t next = (i + 1) % count;
                    normalZ += points[i].x * points[next].y - points[next].x * points[i].y;
                }
                return normalZ < 0.0f;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossSelf () const {
                if (points.size () > 2) {
                    for (std::size_t i = 0, outerCount = points.size () - 2; i < outerCount; ++i) {
                        for (std::size_t j = i + 2, innerCount = points.size () - 1; j < innerCount; ++j) {
                            if (Line2 (points[i], points[i + 1]).CrossLine (Line2 (points[j], points[j + 1]))) {
                                return true;
                            }
                        }
                    }
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InBound (const Bound2 &bound) const {
                if (points.empty ()) {
                    return false;
                }
                for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                    if (!Line2 (points[i], points[i + 1]).InBound (bound)) {
                        return false;
                    }
                }
                return !closed || Line2 (points.back (), points[0]).InBound (bound);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossBound (const Bound2 &bound) const {
                if (!points.empty ()) {
                    for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                        if (Line2 (points[i], points[i + 1]).CrossBound (bound)) {
                            return true;
                        }
                    }
                    return closed && Line2 (points.back (), points[0]).CrossBound (bound);
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InPolygon (const Polygon2 &polygon) const {
                if (points.empty () || polygon.points.empty () || !polygon.closed) {
                    return false;
                }
                for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                    if (!Line2 (points[i], points[i + 1]).InPolygon (polygon)) {
                        return false;
                    }
                }
                return !closed || Line2 (points.back (), points[0]).InPolygon (polygon);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossPolygon (const Polygon2 &polygon) const {
                if (!points.empty () && !polygon.points.empty ()) {
                    for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                        if (Line2 (points[i], points[i + 1]).CrossPolygon (polygon)) {
                            return true;
                        }
                    }
                    return closed && Line2 (points.back (), points[0]).CrossPolygon (polygon);
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InCircle (const Circle &circle) const {
                if (points.empty ()) {
                    return false;
                }
                for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                    if (!Line2 (points[i], points[i + 1]).InCircle (circle)) {
                        return false;
                    }
                }
                return !closed || Line2 (points.back (), points[0]).InCircle (circle);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossCircle (const Circle &circle) const {
                if (!points.empty ()) {
                    for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                        if (Line2 (points[i], points[i + 1]).CrossCircle (circle)) {
                            return true;
                        }
                    }
                    return closed && Line2 (points.back (), points[0]).CrossCircle (circle);
                }
                return false;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InEllipse (const Ellipse &ellipse) const {
                if (points.empty ()) {
                    return false;
                }
                for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                    if (!Line2 (points[i], points[i + 1]).InEllipse (ellipse)) {
                        return false;
                    }
                }
                return !closed || Line2 (points.back (), points[0]).InEllipse (ellipse);
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossEllipse (const Ellipse &ellipse) const {
                if (!points.empty ()) {
                    for (std::size_t i = 0, count = points.size () - 1; i < count; ++i) {
                        if (Line2 (points[i], points[i + 1]).CrossEllipse (ellipse)) {
                            return true;
                        }
                    }
                    return closed && Line2 (points.back (), points[0]).CrossEllipse (ellipse);
                }
                return false;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
