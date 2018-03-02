// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_shaper_BezierPolygon_h)
#define __thekogans_mex_shaper_BezierPolygon_h

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Util.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct BezierPolygon {
                const _3ds::io::BezierPolygon2 &bezierPolygon;

                explicit BezierPolygon (const _3ds::io::BezierPolygon2 &bezierPolygon_) :
                    bezierPolygon (bezierPolygon_) {}

                void DrawVertexHandles (const _3ds::opengl::View &view, std::size_t vertexIndex,
                    const blas::Matrix2 &red, const blas::Matrix2 &yellow) const;

                void DrawVertexSegments (const _3ds::opengl::View &view,
                    std::size_t vertexIndex, const blas::Matrix2 &xform) const;
                void DrawVertexSegments (const _3ds::opengl::View &view, std::size_t vertexIndex,
                    const blas::Matrix2 &red, const blas::Matrix2 &yellow) const;

                void DrawSegment (const _3ds::opengl::View &view,
                    std::size_t vertexIndex1, const blas::Matrix2 &xform1,
                    std::size_t vertexIndex2, const blas::Matrix2 &xform2) const;
                void DrawSegment (const _3ds::opengl::View &view,
                    std::size_t vertexIndex1, const blas::Matrix2 &red1, const blas::Matrix2 &yellow1,
                    std::size_t vertexIndex2, const blas::Matrix2 &red2, const blas::Matrix2 &yellow2) const;
                void DrawSegmentSegments (const _3ds::opengl::View &view,
                    std::size_t vertexIndex1, const blas::Matrix2 &red1, const blas::Matrix2 &yellow1,
                    std::size_t vertexIndex2, const blas::Matrix2 &red2, const blas::Matrix2 &yellow2) const;

                void Draw (const _3ds::opengl::View &view, const blas::Matrix2 &xform) const;
                void Draw (const _3ds::opengl::View &view, const blas::Matrix2 &red, const blas::Matrix2 &yellow) const;

            private:
                inline void DrawBezierCubic (const _3ds::opengl::View &view,
                    const _3ds::io::BezierPolygon2::Vertex &vertex1,
                    const _3ds::io::BezierPolygon2::Vertex &vertex2) const {
                    core::DrawBezierCubic2 (view,
                        blas::BezierCubic2 (
                            blas::Point2 (vertex1.pt), blas::Point2 (vertex1.pt + vertex1.yellow),
                            blas::Point2 (vertex2.pt + vertex2.red), blas::Point2 (vertex2.pt)),
                        core::GetIOProject ().shaper.steps, _3ds::opengl::SystemXORPalette[_3ds::opengl::GRAY]);
                }
            };

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_shaper_BezierPolygon_h)
