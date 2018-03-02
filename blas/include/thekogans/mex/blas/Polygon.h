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

#if !defined (__thekogans_mex_blas_Polygon_h)
#define __thekogans_mex_blas_Polygon_h

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Config.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/MathUtil.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            template<typename T>
            struct Polygon {
                typedef std::unique_ptr<Polygon<T> > UniquePtr;

                THEKOGANS_UTIL_DECLARE_HEAP (Polygon<T>)

                std::vector<T> points;
                bool closed;

                Polygon () : closed (false) {}
                Polygon (const std::vector<T> &points_, bool closed_) :
                    points (points_), closed (closed_) {}
                Polygon (const Polygon &polygon) :
                    points (polygon.points), closed (polygon.closed) {}

                inline T &operator [] (std::size_t index) {
                    assert (index < points.size ());
                    return points[index];
                }

                inline const T &operator [] (std::size_t index) const {
                    assert (index < points.size ());
                    return points[index];
                }

                inline Polygon &operator = (const Polygon<T> &polygon) {
                    if (&polygon != this) {
                        points = polygon.points;
                        closed = polygon.closed;
                    }
                    return *this;
                }

                inline Polygon &operator *= (const Matrix<T> &xform) {
                    for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                        points *= xform;
                    }
                    return *this;
                }

                bool IsClockWise () const;
                bool CrossSelf () const;

                bool InBound (const Bound<T> &bound) const;
                bool CrossBound (const Bound<T> &bound) const;

                bool InPolygon (const Polygon<T> &polygon) const;
                bool CrossPolygon (const Polygon<T> &polygon) const;

                bool InCircle (const Circle &circle) const;
                bool CrossCircle (const Circle &circle) const;

                bool InEllipse (const Ellipse &ellipse) const;
                bool CrossEllipse (const Ellipse &ellipse) const;

                T PointNormal (std::size_t pointIndex) const;
                Polygon<T> Outline (util::f32 width) const;

                Bound2 GetBound2 (const Matrix3 &xform) const;
                Bound<T> GetBound () const;

                // Description:
                //      Returns an array of incremental lengths computed
                //      at every point.
                void GetLengths (std::vector<util::f32> &lengths) const;

                // Description:
                //      Computes torsion minimizing frames at every
                //      point. When applied to an XY (2D) polygon,
                //      sweeps the polygon along the path (this polygon).
                void GetFrames (std::vector<Matrix<T> > &frames, bool contour) const;

                inline std::size_t PreviousPointIndex (std::size_t pointIndex) const {
                    assert (!points.empty ());
                    return pointIndex > 0 ? pointIndex - 1 :
                        closed ? points.size () - 1 : util::NIDX;
                }
                inline std::size_t NextPointIndex (std::size_t pointIndex) const {
                    assert (!points.empty ());
                    return pointIndex < points.size () - 1 ? pointIndex + 1 :
                        closed ? 0 : util::NIDX;
                }

                T ComputeLeftTangent (std::size_t index) const {
                    assert (index < points.size () - 1);
                    return Normalize (points[index + 1] - points[index]);
                }

                T ComputeRightTangent (std::size_t index) const {
                    assert (index > 0);
                    return Normalize (points[index - 1] - points[index]);
                }

                T ComputeCenterTangent (std::size_t index) const {
                    assert (index > 0 && index < points.size () - 1);
                    return Normalize (Mid (points[index - 1] - points[index], points[index] - points[index + 1]));
                }
            };

            typedef Polygon<Point2> Polygon2;
            typedef Polygon<Point3> Polygon3;

            _LIB_THEKOGANS_MEX_BLAS_DECL Polygon3 _LIB_THEKOGANS_MEX_BLAS_API Polygon2To3 (const Polygon2 &polygon);
            _LIB_THEKOGANS_MEX_BLAS_DECL Polygon2 _LIB_THEKOGANS_MEX_BLAS_API Polygon3To2 (const Polygon3 &polygon);

            template<typename T>
            T Polygon<T>::PointNormal (std::size_t pointIndex) const {
                assert (pointIndex < points.size ());
                T previousPoint = T::Zero;
                for (std::size_t previousPointIndex = PreviousPointIndex (pointIndex);
                     previousPointIndex != util::NIDX && previousPointIndex != pointIndex;
                     previousPointIndex = PreviousPointIndex (previousPointIndex)) {
                    previousPoint = points[previousPointIndex] - points[pointIndex];
                    if (!previousPoint.IsZero ()) {
                        break;
                    }
                }
                T nextPoint = T::Zero;
                for (std::size_t nextPointIndex = NextPointIndex (pointIndex);
                     nextPointIndex != util::NIDX && nextPointIndex != pointIndex;
                     nextPointIndex = NextPointIndex (nextPointIndex)) {
                    nextPoint = points[nextPointIndex] - points[pointIndex];
                    if (!nextPoint.IsZero ()) {
                        break;
                    }
                }
                if (previousPoint.IsZero () && nextPoint.IsZero ()) {
                    return T::Zero;
                }
                if (previousPoint.IsZero ()) {
                    previousPoint = -nextPoint;
                }
                if (nextPoint.IsZero ()) {
                    nextPoint = -previousPoint;
                }
                T normal = Normalize (Bisect (nextPoint, previousPoint));
                return normal / cosf (Angle (normal, previousPoint.Perpendicular ()));
            }

            template<typename T>
            Polygon<T> Polygon<T>::Outline (util::f32 width) const {
                Polygon<T> polygon;
                polygon.points.resize (points.size ());
                for (std::size_t i = 0; i < points.size (); ++i) {
                    polygon.points[i] = points[i] + PointNormal (i) * width;
                }
                polygon.closed = closed;
                return polygon;
            }

            template<typename T>
            Bound2 Polygon<T>::GetBound2 (const Matrix3 &xform) const {
                Bound2 bound (Bound2::Empty);
                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                    bound += blas::Point2 (points[i] * xform);
                }
                return bound;
            }

            template<typename T>
            Bound<T> Polygon<T>::GetBound () const {
                Bound<T> bound (Bound<T>::Empty);
                for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                    bound += points[i];
                }
                return bound;
            }

            template<typename T>
            void Polygon<T>::GetLengths (std::vector<util::f32> &lengths) const {
                if (!points.empty ()) {
                    std::size_t count = points.size ();
                    if (closed) {
                        ++count;
                    }
                    lengths.resize (count);
                    lengths[0] = 0.0f;
                    for (std::size_t i = 1; i < count; ++i) {
                        lengths[i] = lengths[i - 1] + (points[i % points.size ()] - points[i - 1]).Length ();
                    }
                }
            }

            template<typename T>
            void Polygon<T>::GetFrames (std::vector<Matrix<T> > &frames, bool contour) const {
                if (points.empty ()) {
                    return;
                }
                frames.resize (points.size ());
                if (contour) {
                    if (points.size () == 1) {
                        frames[0] = Matrix<T>::Identity;
                    }
                    else {
                        T t;
                        T n;
                        T b;
                        for (std::size_t i = 0, count = points.size (); i < count; ++i) {
                            if (!i) {
                                t = Normalize (Normalize (closed ? points.back () - points[0] : T::Zero) -
                                    Normalize (points[1] - points[0]));
                                Matrix<T> matrix = Matrix<T>::AimZ (t).Transpose ();
                                n = T::X * matrix;
                                b = T::Y * matrix;
                            }
                            else {
                                T p1;
                                T p2;
                                if (i == points.size () - 1) {
                                    if (closed) {
                                        p1 = Normalize (points[points.size () - 2] - points[i]);
                                        p2 = Normalize (points[0] - points[i]);
                                    }
                                    else {
                                        p1 = Normalize (points[i - 1] - points[i]);
                                        p2 = T::Zero;
                                    }
                                }
                                else {
                                    p1 = Normalize (points[i - 1] - points[i]);
                                    p2 = Normalize (points[i + 1] - points[i]);
                                }
                                T tNew = Normalize (p1 - p2);
                                T axis = Cross (tNew, t);
                                if (!axis.IsZero ()) {
                                    Matrix<T> matrix = Matrix<T>::FromQuaternion (
                                        Quaternion::FromAngleAxis (AngleAxis (Angle (tNew, t), axis)));
                                    t *= matrix;
                                    n *= matrix;
                                    b *= matrix;
                                }
                            }
                            frames[i] = Matrix<T> (n, b, t, points[i]);
                        }
                    }
                }
                else {
                    Matrix<T> matrix = Matrix<T>::RotateX (util::HALFPI);
                    for (std::size_t i = 0; i < points.size (); ++i) {
                        frames[i] = matrix * Matrix<T>::Translate (points[i]);
                    }
                }
            }

            template<typename T>
            util::Serializer &operator << (util::Serializer &serializer, const Polygon<T> &polygon) {
                util::ui32 count = polygon.points.size ();
                serializer << count;
                for (util::ui32 i = 0; i < count; ++i) {
                    serializer << polygon.points[i];
                }
                serializer << polygon.closed;
                return serializer;
            }

            template<typename T>
            util::Serializer &operator >> (util::Serializer &serializer, Polygon<T> &polygon) {
                util::ui32 count;
                serializer >> count;
                polygon.points.resize (count);
                for (util::ui32 i = 0; i < count; ++i) {
                    serializer >> polygon.points[i];
                }
                serializer >> polygon.closed;
                return serializer;
            }

            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::IsClockWise () const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossSelf () const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InBound (const Bound2 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossBound (const Bound2 &bound) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InPolygon (const Polygon2 &polygon) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossPolygon (const Polygon2 &polygon) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InCircle (const Circle &circle) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossCircle (const Circle &circle) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::InEllipse (const Ellipse &ellipse) const;
            template<>
            _LIB_THEKOGANS_MEX_BLAS_DECL bool Polygon2::CrossEllipse (const Ellipse &ellipse) const;

        } // namespace blas
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_blas_Polygon_h)
