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

#if !defined (__thekogans_mex_blas_PolygonRegion_h)
#define __thekogans_mex_blas_PolygonRegion_h

#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/blas/Region.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<typename T>
            struct PolygonRegion : public Region<T> {
                THEKOGANS_UTIL_DECLARE_HEAP (PolygonRegion<T>)

            private:
                Polygon<T> polygon;

            public:
                explicit PolygonRegion (const Polygon<T> &polygon_) : polygon (polygon_) {}

                virtual bool PointInRegion (const T &pt) const {
                    return pt.InPolygon (polygon);
                }
                virtual bool LineInRegion (const LineT<T> &line, bool cross = true) const {
                    return cross ? line.CrossPolygon (polygon) : line.InPolygon (polygon);
                }
                virtual bool BoundInRegion (const Bound<T> &bound, bool cross = true) const {
                    return cross ? bound.CrossPolygon (polygon) : bound.InPolygon (polygon);
                }
                virtual bool CircleInRegion (const Circle &circle, bool cross = true) const {
                    return cross ? circle.CrossPolygon (polygon) : circle.InPolygon (polygon);
                }
                virtual bool EllipseInRegion (const Ellipse &ellipse, bool cross = true) const {
                    return cross ? ellipse.CrossPolygon (polygon) : ellipse.InPolygon (polygon);
                }
                virtual bool PolygonInRegion (const Polygon<T> &polygon_, bool cross = true) const {
                    return cross ? polygon_.CrossPolygon (polygon) : polygon_.InPolygon (polygon);
                }
                virtual bool BezierCubicInRegion (const BezierCubic<T> &bezierCubic,
                        util::ui32 steps, bool cross = true) const {
                    return cross ? bezierCubic.CrossPolygon (polygon, steps) :
                        bezierCubic.InPolygon (polygon, steps);
                }
            };

            typedef PolygonRegion<Point2> PolygonRegion2;
            typedef PolygonRegion<Point3> PolygonRegion3;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_PolygonRegion_h)
