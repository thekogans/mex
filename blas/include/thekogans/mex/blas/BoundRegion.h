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

#if !defined (__thekogans_mex_blas_BoundRegion_h)
#define __thekogans_mex_blas_BoundRegion_h

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
            struct BoundRegion : public Region<T> {
                THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

            private:
                Bound<T> bound;

            public:
                explicit BoundRegion (const Bound<T> &bound_) :
                    bound (bound_) {}

                virtual bool PointInRegion (const T &pt) const {
                    return pt.InBound (bound);
                }
                virtual bool LineInRegion (
                        const LineT<T> &line,
                        bool cross = true) const {
                    return cross ? line.CrossBound (bound) : line.InBound (bound);
                }
                virtual bool BoundInRegion (
                        const Bound<T> &bound_,
                        bool cross = true) const {
                    return cross ? bound_.CrossBound (bound) : bound_.InBound (bound);
                }
                virtual bool CircleInRegion (
                        const Circle &circle,
                        bool cross = true) const {
                    return cross ? circle.CrossBound (bound) : circle.InBound (bound);
                }
                virtual bool EllipseInRegion (
                        const Ellipse &ellipse,
                        bool cross = true) const {
                    return cross ? ellipse.CrossBound (bound) : ellipse.InBound (bound);
                }
                virtual bool PolygonInRegion (
                        const Polygon<T> &polygon,
                        bool cross = true) const {
                    return cross ? polygon.CrossBound (bound) : polygon.InBound (bound);
                }
                virtual bool BezierCubicInRegion (
                        const BezierCubic<T> &bezierCubic,
                        util::ui32 steps,
                        bool cross = true) const {
                    return cross ? bezierCubic.CrossBound (bound, steps) :
                        bezierCubic.InBound (bound, steps);
                }
            };

            typedef BoundRegion<Point2> BoundRegion2;
            typedef BoundRegion<Point3> BoundRegion3;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_BoundRegion_h)
