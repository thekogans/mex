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

#include "thekogans/util/Constants.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            void BezierPolygon::DrawVertexHandles (const _3ds::opengl::View &view, std::size_t vertexIndex,
                    const blas::Matrix2 &red, const blas::Matrix2 &yellow) const {
                const _3ds::io::BezierPolygon2::Vertex &vertex = bezierPolygon.vertices[vertexIndex];
                core::DrawArrow2 (view, blas::Point2 (vertex.pt * view.xform),
                    blas::Point2 ((vertex.pt + vertex.red * red) * view.xform),
                    _3ds::opengl::SystemXORPalette[_3ds::opengl::RED]);
                core::DrawArrow2 (view, blas::Point2 (vertex.pt * view.xform),
                    blas::Point2 ((vertex.pt + vertex.yellow * yellow) * view.xform),
                    _3ds::opengl::SystemXORPalette[_3ds::opengl::YELLOW]);
            }

            void BezierPolygon::DrawVertexSegments (const _3ds::opengl::View &view,
                    std::size_t vertexIndex, const blas::Matrix2 &xform) const {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex);
                if (previousVertexIndex != util::NIDX) {
                    DrawBezierCubic (view, bezierPolygon.vertices[previousVertexIndex],
                        bezierPolygon.vertices[vertexIndex] * xform);
                }
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex);
                if (nextVertexIndex != util::NIDX) {
                    DrawBezierCubic (view, bezierPolygon.vertices[vertexIndex] * xform,
                        bezierPolygon.vertices[nextVertexIndex]);
                }
            }

            void BezierPolygon::DrawVertexSegments (const _3ds::opengl::View &view, std::size_t vertexIndex,
                    const blas::Matrix2 &red, const blas::Matrix2 &yellow) const {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex);
                if (previousVertexIndex != util::NIDX) {
                    DrawBezierCubic (view,
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[previousVertexIndex].pt,
                            bezierPolygon.vertices[previousVertexIndex].red,
                            bezierPolygon.vertices[previousVertexIndex].yellow),
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[vertexIndex].pt,
                            bezierPolygon.vertices[vertexIndex].red * red,
                            bezierPolygon.vertices[vertexIndex].yellow * yellow));
                }
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex);
                if (nextVertexIndex != util::NIDX) {
                    DrawBezierCubic (view,
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[vertexIndex].pt,
                            bezierPolygon.vertices[vertexIndex].red * red,
                            bezierPolygon.vertices[vertexIndex].yellow * yellow),
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[nextVertexIndex].pt,
                            bezierPolygon.vertices[nextVertexIndex].red,
                            bezierPolygon.vertices[nextVertexIndex].yellow));
                }
                DrawVertexHandles (view, vertexIndex, red, yellow);
            }

            void BezierPolygon::DrawSegment (const _3ds::opengl::View &view,
                    std::size_t vertexIndex1, const blas::Matrix2 &xform1,
                    std::size_t vertexIndex2, const blas::Matrix2 &xform2) const {
                DrawBezierCubic (view, bezierPolygon.vertices[vertexIndex1] * xform1,
                    bezierPolygon.vertices[vertexIndex2] * xform2);
            }

            void BezierPolygon::DrawSegment (const _3ds::opengl::View &view,
                    std::size_t vertexIndex1, const blas::Matrix2 &red1, const blas::Matrix2 &yellow1,
                    std::size_t vertexIndex2, const blas::Matrix2 &red2, const blas::Matrix2 &yellow2) const {
                DrawBezierCubic (view,
                    _3ds::io::BezierPolygon2::Vertex (
                        bezierPolygon.vertices[vertexIndex1].pt,
                        bezierPolygon.vertices[vertexIndex1].red * red1,
                        bezierPolygon.vertices[vertexIndex1].yellow * yellow1),
                    _3ds::io::BezierPolygon2::Vertex (
                        bezierPolygon.vertices[vertexIndex2].pt,
                        bezierPolygon.vertices[vertexIndex2].red * red2,
                        bezierPolygon.vertices[vertexIndex2].yellow * yellow2));
            }

            void BezierPolygon::DrawSegmentSegments (const _3ds::opengl::View &view,
                    std::size_t vertexIndex1, const blas::Matrix2 &red1, const blas::Matrix2 &yellow1,
                    std::size_t vertexIndex2, const blas::Matrix2 &red2, const blas::Matrix2 &yellow2) const {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex1);
                if (previousVertexIndex != util::NIDX && previousVertexIndex != vertexIndex2) {
                    DrawBezierCubic (view,
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[previousVertexIndex].pt,
                            bezierPolygon.vertices[previousVertexIndex].red,
                            bezierPolygon.vertices[previousVertexIndex].yellow),
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[vertexIndex1].pt,
                            bezierPolygon.vertices[vertexIndex1].red * red1,
                            bezierPolygon.vertices[vertexIndex1].yellow * yellow1));
                }
                DrawBezierCubic (view,
                    _3ds::io::BezierPolygon2::Vertex (
                        bezierPolygon.vertices[vertexIndex1].pt,
                        bezierPolygon.vertices[vertexIndex1].red * red1,
                        bezierPolygon.vertices[vertexIndex1].yellow * yellow1),
                    _3ds::io::BezierPolygon2::Vertex (
                        bezierPolygon.vertices[vertexIndex2].pt,
                        bezierPolygon.vertices[vertexIndex2].red * red2,
                        bezierPolygon.vertices[vertexIndex2].yellow * yellow2));
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex2);
                if (nextVertexIndex != util::NIDX && nextVertexIndex != vertexIndex1) {
                    DrawBezierCubic (view,
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[vertexIndex2].pt,
                            bezierPolygon.vertices[vertexIndex2].red * red2,
                            bezierPolygon.vertices[vertexIndex2].yellow * yellow2),
                        _3ds::io::BezierPolygon2::Vertex (
                            bezierPolygon.vertices[nextVertexIndex].pt,
                            bezierPolygon.vertices[nextVertexIndex].red,
                            bezierPolygon.vertices[nextVertexIndex].yellow));
                }
                DrawVertexHandles (view, vertexIndex1, red1, yellow1);
                DrawVertexHandles (view, vertexIndex2, red2, yellow2);
            }

            void BezierPolygon::Draw (const _3ds::opengl::View &view, const blas::Matrix2 &xform) const {
                const std::vector<_3ds::io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                if (!vertices.empty ()) {
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        DrawBezierCubic (view, bezierPolygon.vertices[i] * xform,
                            bezierPolygon.vertices[i + 1] * xform);
                    }
                    if (bezierPolygon.IsClosed ()) {
                        DrawBezierCubic (view, bezierPolygon.vertices.back () * xform,
                            bezierPolygon.vertices[0] * xform);
                    }
                }
            }

            void BezierPolygon::Draw (const _3ds::opengl::View &view, const blas::Matrix2 &red, const blas::Matrix2 &yellow) const {
                const std::vector<_3ds::io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                if (!vertices.empty ()) {
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        DrawBezierCubic (view,
                            _3ds::io::BezierPolygon2::Vertex (
                                bezierPolygon.vertices[i].pt,
                                bezierPolygon.vertices[i].red * red,
                                bezierPolygon.vertices[i].yellow * yellow),
                            _3ds::io::BezierPolygon2::Vertex (
                                bezierPolygon.vertices[i + 1].pt,
                                bezierPolygon.vertices[i + 1].red * red,
                                bezierPolygon.vertices[i + 1].yellow * yellow));
                    }
                    if (bezierPolygon.IsClosed ()) {
                        DrawBezierCubic (view,
                            _3ds::io::BezierPolygon2::Vertex (
                                bezierPolygon.vertices.back ().pt,
                                bezierPolygon.vertices.back ().red * red,
                                bezierPolygon.vertices.back ().yellow * yellow),
                            _3ds::io::BezierPolygon2::Vertex (
                                bezierPolygon.vertices[0].pt,
                                bezierPolygon.vertices[0].red * red,
                                bezierPolygon.vertices[0].yellow * yellow));
                    }
                }
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
