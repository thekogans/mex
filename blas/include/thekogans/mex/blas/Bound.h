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

#if !defined (__thekogans_mex_blas_Bound_h)
#define __thekogans_mex_blas_Bound_h

#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            struct Circle;
            struct Ellipse;
            template<typename T>
            struct Polygon;

            template<typename T>
            struct Bound {
                T min;
                T max;

                static _LIB_THEKOGANS_MEX_BLAS_DECL const Bound<T> Empty;

                Bound () {}
                Bound (
                    const T &min_,
                    const T &max_) :
                    min (min_),
                    max (max_) {}
                Bound (const Bound<T> &bound) :
                    min (bound.min),
                    max (bound.max) {}
                explicit Bound (const T &pt) :
                    min (pt),
                    max (pt) {}

                inline const T &operator [] (util::ui32 index) const {
                    assert (index < 2);
                    return ((const T *)this)[index];
                }
                inline T &operator [] (util::ui32 index) {
                    assert (index < 2);
                    return ((T *)this)[index];
                }

                inline Bound<T> &operator = (const Bound<T> &bound) {
                    if (&bound != this) {
                        min = bound.min;
                        max = bound.max;
                    }
                    return *this;
                }

                inline Bound<T> &operator = (const T &pt) {
                    min = pt;
                    max = pt;
                    return *this;
                }

                Bound<T> &operator += (const T &pt);

                inline Bound<T> &operator += (const Bound<T> &bound) {
                    if (!bound.IsEmpty ()) {
                        operator += (bound.min);
                        operator += (bound.max);
                    }
                    return *this;
                }

                inline bool IsEmpty () const {
                    return min == Empty.min && max == Empty.max;
                }

                inline T Extents () const {
                    return max - min + T::One;
                }
                inline T Center () const {
                    return (min + max) * 0.5f;
                }

                Bound<T> Normalize () const;

                util::f32 Area () const;

                inline void Split (
                        util::ui32 axis,
                        util::f32 splitPosition,
                        Bound &left,
                        Bound &right) const {
                    left = right = *this;
                    left.max[axis] = right.min[axis] = splitPosition;
                }

                bool InCircle (const Circle &circle) const;
                bool CrossCircle (const Circle &circle) const;

                bool InEllipse (const Ellipse &ellipse) const;
                bool CrossEllipse (const Ellipse &ellipse) const;

                inline bool InBound (const Bound<T> &bound) const {
                    return min.InBound (bound) && max.InBound (bound);
                }
                bool CrossBound (const Bound<T> &bound) const;

                bool InPolygon (const Polygon<T> &polygon) const;
                bool CrossPolygon (const Polygon<T> &polygon) const;
            };

            struct Point2;
            typedef Bound<Point2> Bound2;

            struct Point3;
            typedef Bound<Point3> Bound3;

            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 _LIB_THEKOGANS_MEX_BLAS_API Bound2To3 (const Bound2 &bound);
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 _LIB_THEKOGANS_MEX_BLAS_API Bound3To2 (const Bound3 &bound);

            template<typename T>
            Bound<T> operator + (const Bound<T> &bound, const T &pt);

            template<typename T>
            inline Bound<T> operator == (
                    const Bound<T> &bound1,
                    const Bound<T> &bound2) {
                return bound1.min == bound2.min && bound1.max == bound2.max;
            }

            template<typename T>
            inline Bound<T> operator != (
                    const Bound<T> &bound1,
                    const Bound<T> &bound2) {
                return bound1.min != bound2.min || bound1.max != bound2.max;
            }

            template<typename T>
            inline Bound<T> operator + (
                    const Bound<T> &bound1,
                    const Bound<T> &bound2) {
                Bound<T> result (bound1);
                result += bound2;
                return result;
            }

            template<typename T>
            inline Bound<T> operator * (
                    const Bound<T> &bound,
                    const Matrix<T> &xform) {
                return Bound<T> (bound.min * xform, bound.max * xform);
            }

            template<typename T>
            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const Bound<T> &bound) {
                serializer << bound.min << bound.max;
                return serializer;
            }

            template<typename T>
            inline util::Serializer &operator >> (util::Serializer &serializer, Bound<T> &bound) {
                serializer >> bound.min >> bound.max;
                return serializer;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 &Bound2::operator += (const Point2 &pt);
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 &Bound3::operator += (const Point3 &pt);
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 operator + (
                const Bound2 &bound,
                const Point2 &pt);
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 operator + (
                const Bound3 &bound,
                const Point3 &pt);
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound2 Bound2::Normalize () const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL Bound3 Bound3::Normalize () const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 Bound2::Area () const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL util::f32 Bound3::Area () const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::InCircle (const Circle &circle) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossCircle (const Circle &circle) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::InEllipse (const Ellipse &ellipse) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossEllipse (const Ellipse &ellipse) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossBound (const Bound2 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound3::CrossBound (const Bound3 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::InPolygon (const Polygon<Point2> &polygon) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Bound2::CrossPolygon (const Polygon<Point2> &polygon) const;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Bound_h)
