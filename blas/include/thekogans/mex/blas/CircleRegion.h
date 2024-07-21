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

#if !defined (__thekogans_mex_blas_CircleRegion_h)
#define __thekogans_mex_blas_CircleRegion_h

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

            struct _LIB_THEKOGANS_MEX_BLAS_DECL CircleRegion : public Region2 {
                THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

            private:
                Circle circle;

            public:
                explicit CircleRegion (const Circle &circle_) : circle (circle_) {}

                virtual bool PointInRegion (const Point2 &pt) const {
                    return pt.InCircle (circle);
                }
                virtual bool LineInRegion (const Line2 &line, bool cross = true) const {
                    return cross ? line.CrossCircle (circle) : line.InCircle (circle);
                }
                virtual bool BoundInRegion (const Bound2 &bound, bool cross = true) const {
                    return cross ? bound.CrossCircle (circle) : bound.InCircle (circle);
                }
                virtual bool CircleInRegion (const Circle &circle_, bool cross = true) const {
                    return cross ? circle_.CrossCircle (circle) : circle_.InCircle (circle);
                }
                virtual bool EllipseInRegion (const Ellipse &ellipse, bool cross = true) const {
                    return cross ? ellipse.CrossCircle (circle) : ellipse.InCircle (circle);
                }
                virtual bool PolygonInRegion (const Polygon2 &polygon, bool cross = true) const {
                    return cross ? polygon.CrossCircle (circle) : polygon.InCircle (circle);
                }
                virtual bool BezierCubicInRegion (const BezierCubic2 &bezierCubic,
                        util::ui32 steps, bool cross = true) const {
                    return cross ? bezierCubic.CrossCircle (circle, steps) :
                        bezierCubic.InCircle (circle, steps);
                }
            };

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_CircleRegion_h)
