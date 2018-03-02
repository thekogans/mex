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

#include <vector>
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Matrix2.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Polygon2 Circle::ToPolygon (std::size_t pointCount) const {
                Matrix2 xform = Matrix2::RotateZ (util::TWOPI / pointCount) * Matrix2::Translate (center);
                std::vector<Point2> points;
                points.resize (pointCount);
                points[0] = Point2::X * radius;
                for (std::size_t i = 1; i < pointCount; ++i) {
                    points[i] = points[i - 1] * xform;
                }
                return Polygon2 (points, true);
            }

            bool Circle::InBound (const Bound2 &bound) const {
                Point2 min = bound.min - center;
                Point2 max = bound.max - center;
                return radius >= min.x && radius >= min.y && radius <= max.x && radius <= max.y;
            }

            bool Circle::InBound (const Bound3 &bound) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::CrossBound (const Bound2 &bound) const {
                return bound.CrossCircle (*this);
            }

            bool Circle::CrossBound (const Bound3 &bound) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::InCircle (const Circle &circle) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::CrossCircle (const Circle &circle) const {
                Point2 delta = circle.center - center;
                util::f32 radiusSquared = circle.radius + radius;
                return Dot (delta, delta) < radiusSquared * radiusSquared;
            }

            bool Circle::InEllipse (const Ellipse &ellipse) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::CrossEllipse (const Ellipse &ellipse) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::InPolygon (const Polygon2 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::InPolygon (const Polygon3 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::CrossPolygon (const Polygon2 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Circle::CrossPolygon (const Polygon3 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
