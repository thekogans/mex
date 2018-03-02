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
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"

namespace thekogans {
    namespace mex {
        namespace blas {

            Polygon2 Ellipse::ToPolygon (std::size_t pointCount) const {
                util::f32 angle = 0.0f;
                const util::f32 dangle = util::TWOPI / pointCount;
                const util::f32 width = bound.Extents ().x * 0.5f;
                const util::f32 height = bound.Extents ().y * 0.5f;
                const blas::Point2 center = bound.Center ();
                std::vector<Point2> points;
                for (std::size_t i = 0; i < pointCount; ++i) {
                    points.push_back (center +
                        blas::Point2 (width * cosf (angle), height * sinf (angle)));
                    angle += dangle;
                }
                return Polygon2 (points, true);
            }

            bool Ellipse::InBound (const Bound2 &bound) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::InBound (const Bound3 &bound) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::CrossBound (const Bound2 &bound) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::CrossBound (const Bound3 &bound) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::InCircle (const Circle &circle) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::CrossCircle (const Circle &circle) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::InEllipse (const Ellipse &ellipse) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::CrossEllipse (const Ellipse &ellipse) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::InPolygon (const Polygon2 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::InPolygon (const Polygon3 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::CrossPolygon (const Polygon2 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

            bool Ellipse::CrossPolygon (const Polygon3 &polygon) const {
                // FIXME: implement
                assert (0);
                return false;
            }

        } // namespace blas
    } // namespace mex
} // namespace thekogans
