// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                _LIB_THEKOGANS_MEX_3DS_IO_DECL BezierPolygon3::UniquePtr _LIB_THEKOGANS_MEX_3DS_IO_API
                        BezierPolygon2To3 (const BezierPolygon2 &bezierPolygon2) {
                    BezierPolygon3::UniquePtr bezierPolygon3 (new BezierPolygon3);
                    assert (bezierPolygon3.get () != 0);
                    bezierPolygon3->vertices.resize (bezierPolygon2.vertices.size ());
                    for (std::size_t i = 0, count = bezierPolygon2.vertices.size (); i < count; ++i) {
                        bezierPolygon3->vertices[i].pt = bezierPolygon2.vertices[i].pt;
                        bezierPolygon3->vertices[i].red = bezierPolygon2.vertices[i].red;
                        bezierPolygon3->vertices[i].yellow = bezierPolygon2.vertices[i].yellow;
                        bezierPolygon3->vertices[i].flags = bezierPolygon2.vertices[i].flags;
                    }
                    return bezierPolygon3;
                }

                _LIB_THEKOGANS_MEX_3DS_IO_DECL BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_IO_API
                        BezierPolygon3To2 (const BezierPolygon3 &bezierPolygon3) {
                    BezierPolygon2::UniquePtr bezierPolygon2 (new BezierPolygon2);
                    assert (bezierPolygon2.get () != 0);
                    bezierPolygon2->vertices.resize (bezierPolygon3.vertices.size ());
                    for (std::size_t i = 0, count = bezierPolygon3.vertices.size (); i < count; ++i) {
                        bezierPolygon2->vertices[i].pt = blas::Point2 (bezierPolygon3.vertices[i].pt);
                        bezierPolygon2->vertices[i].red = blas::Point2 (bezierPolygon3.vertices[i].red);
                        bezierPolygon2->vertices[i].yellow = blas::Point2 (bezierPolygon3.vertices[i].yellow);
                        bezierPolygon2->vertices[i].flags = bezierPolygon3.vertices[i].flags;
                    }
                    return bezierPolygon2;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
