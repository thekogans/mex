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

#if !defined (__thekogans_mex_blas_Region_h)
#define __thekogans_mex_blas_Region_h

#include <memory>
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/BezierCubic.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<typename T>
            struct Region {
                typedef std::unique_ptr<Region<T> > UniquePtr;

                virtual ~Region () {}

                virtual bool PointInRegion (const T &pt) const = 0;
                virtual bool LineInRegion (const LineT<T> &line, bool cross = true) const = 0;
                virtual bool BoundInRegion (const Bound<T> &bound, bool cross = true) const = 0;
                virtual bool CircleInRegion (const Circle &circle, bool cross = true) const = 0;
                virtual bool EllipseInRegion (const Ellipse &ellipse, bool cross = true) const = 0;
                virtual bool PolygonInRegion (const Polygon<T> &polygon, bool cross = true) const = 0;
                virtual bool BezierCubicInRegion (const BezierCubic<T> &bezierCubic,
                    util::ui32 steps, bool cross = true) const = 0;
            };

            typedef Region<Point2> Region2;
            typedef Region<Point3> Region3;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Region_h)
