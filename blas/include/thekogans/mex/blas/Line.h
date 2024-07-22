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

#if !defined (__thekogans_mex_blas_Line_h)
#define __thekogans_mex_blas_Line_h

#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Polygon.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Circle;
            struct Ellipse;
            template<typename T>
            struct BezierCubic;

            template<typename T>
            struct LineT {
                T start;
                T end;

                LineT () {}
                LineT (
                    const T &start_,
                    const T &end_) :
                    start (start_),
                    end (end_) {}
                LineT (const LineT<T> &line) :
                    start (line.start),
                    end (line.end) {}

                LineT<T> &operator = (const LineT<T> &line) {
                    if (&line != this) {
                        start = line.start;
                        end = line.end;
                    }
                    return *this;
                }

                // Description:
                //      Return true if intersects with line.
                bool CrossLine (
                    const LineT<T> &line,
                    T &pt) const;
                inline bool CrossLine (const LineT<T> &line) const {
                    T pt;
                    return CrossLine (line, pt);
                }

                bool InCircle (const Circle &circle) const;
                bool CrossCircle (const Circle &circle) const;

                bool InEllipse (const Ellipse &ellipse) const;
                bool CrossEllipse (const Ellipse &ellipse) const;

                // Description:
                //      Return true if is in bound.
                bool InBound (const Bound<T> &bound) const;

                // Description:
                //      Return true if intersects with bound.
                bool CrossBound (const Bound<T> &bound) const;

                // Description:
                //      Return true if is in polygon.
                bool InPolygon (const Polygon<T> &polygon) const;

                // Description:
                //      Return true if intersects with polygon.
                bool CrossPolygon (const Polygon<T> &polygon) const;

                // Description:
                //      Return true if intersects with BezierCubic.
                bool CrossBezierCubic (
                    const BezierCubic<T> &bezierCubic,
                    util::ui32 steps) const;
            };

            typedef LineT<Point> Line;
            typedef LineT<Point2> Line2;
            typedef LineT<Point3> Line3;

            template<typename T>
            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const LineT<T> &line) {
                serializer << line.start << line.end;
                return serializer;
            }

            template<typename T>
            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    LineT<T> &line) {
                serializer >> line.start >> line.end;
                return serializer;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InCircle (const Circle &circle) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossCircle (const Circle &circle) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InEllipse (const Ellipse &ellipse) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossEllipse (const Ellipse &ellipse) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InBound (const Bound2 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::InPolygon (const Polygon2 &polygon) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossPolygon (const Polygon2 &polygon) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossBound (const Bound2 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line3::CrossBound (const Bound3 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossLine (
                const Line2 &line,
                Point2 &pt) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Line2::CrossBezierCubic (
                const BezierCubic<Point2> &bezierCubic,
                util::ui32 steps) const;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Line_h)
