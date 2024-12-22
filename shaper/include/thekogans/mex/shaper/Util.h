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

#if !defined (__thekogans_mex_shaper_Util_h)
#define __thekogans_mex_shaper_Util_h

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/shaper/Config.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            // Polygon

            void DrawPolygon (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                bool flip = true);
            void ErasePolygon (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                bool flip = true);

            void AddPolygon (
                _3ds::io::BezierPolygon2::UniquePtr bezierPolygon,
                bool flip = true);
            void DeletePolygon (
                std::size_t polygonIndex,
                bool flip = true);

            std::size_t GetPolygonIndex (
                const _3ds::io::BezierPolygon2 &bezierPolygon);

            // Polygons

            void DrawPolygons2 (
                const _3ds::opengl::View &view,
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                const blas::Matrix2 &xform,
                const opengl::ui8Color &color = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);

            void DrawPolygons (
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                bool flip = true);
            void ErasePolygons (
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                bool flip = true);

            void AddPolygons (
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                bool flip = true);
            void DeletePolygons (
                const std::vector<std::size_t> &polygonIndices,
                bool flip = true);

            void GetSelectedPolygons (
                std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                util::ui16 selectMask,
                bool frozen = false);
            void ToggleSelectedPolygons (
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons);
            blas::Bound2 GetPolygonsBound (
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons);
            void GetPolygonIndeces (
                const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                std::vector<std::size_t> &polygonIndices);

            // Segment

            void DrawSegment (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2,
                bool flip = true);
            void EraseSegment (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2,
                bool flip = true);

            void DrawSegmentSegments (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2,
                bool flip = true);
            void EraseSegmentSegments (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2,
                bool flip = true);

            void HideSegment (
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2);
            void UnhideSegment (
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2);

            void HideSegmentSegments (
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2);
            void UnhideSegmentSegments (
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex1,
                std::size_t vertexIndex2);

            // Segments

            struct _LIB_THEKOGANS_MEX_SHAPER_DECL BezierPolygonSegment {
                typedef std::unique_ptr<BezierPolygonSegment> UniquePtr;
                THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS
                _3ds::io::BezierPolygon2 &bezierPolygon;
                std::size_t vertexIndex1;
                std::size_t vertexIndex2;
                BezierPolygonSegment (
                    _3ds::io::BezierPolygon2 &bezierPolygon_,
                    std::size_t vertexIndex1_,
                    std::size_t vertexIndex2_) :
                    bezierPolygon (bezierPolygon_),
                    vertexIndex1 (vertexIndex1_),
                    vertexIndex2 (vertexIndex2_) {}
            private:
                BezierPolygonSegment (const BezierPolygonSegment &);
                BezierPolygonSegment &operator = (const BezierPolygonSegment &);
            };
            void GetSelectedSegments (
                std::vector<BezierPolygonSegment *> &segments,
                util::ui16 selectMask);

            void DrawSegments (
                const std::vector<BezierPolygonSegment *> &segments,
                bool flip = true);
            void EraseSegments (
                const std::vector<BezierPolygonSegment *> &segments,
                bool flip = true);

            void HideSegments (
                const std::vector<BezierPolygonSegment *> &segments);
            void UnhideSegments (
                const std::vector<BezierPolygonSegment *> &segments);

            // Vertex

            void DrawVertexSegments (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex,
                bool flip = true);
            void EraseVertexSegments (
                const _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex,
                bool flip = true);

            void HideVertexSegments (
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex);
            void UnhideVertexSegments (
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex);

            // Vertices

            struct _LIB_THEKOGANS_MEX_SHAPER_DECL BezierPolygonVertex {
                typedef std::unique_ptr<BezierPolygonVertex> UniquePtr;
                THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS
                _3ds::io::BezierPolygon2 &bezierPolygon;
                std::size_t vertexIndex;
                BezierPolygonVertex (
                    _3ds::io::BezierPolygon2 &bezierPolygon_,
                    std::size_t vertexIndex_) :
                    bezierPolygon (bezierPolygon_),
                    vertexIndex (vertexIndex_) {}
            private:
                BezierPolygonVertex (const BezierPolygonVertex &);
                BezierPolygonVertex &operator = (const BezierPolygonVertex &);
            };
            void GetSelectedVertices (
                std::vector<BezierPolygonVertex *> &vertices,
                util::ui16 selectMask);
            std::size_t FindReferenceVertex (
                std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                _3ds::io::BezierPolygon2 &bezierPolygon,
                std::size_t vertexIndex,
                util::ui16 selectMask);
            void GetSelectedVertexSegments (
                std::vector<BezierPolygonSegment *> &segments,
                util::ui16 selectMask);

            void DrawVertexSegments2 (
                const _3ds::opengl::View &view,
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                const blas::Matrix2 &xform);
            void DrawVertexSegments2 (
                const _3ds::opengl::View &view,
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                const blas::Matrix2 &red,
                const blas::Matrix2 &yellow);

            void DrawVertexSegments (
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                bool flip = true);
            void EraseVertexSegments (
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                bool flip = true);

            void HideVertexSegments (
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices);
            void UnhideVertexSegments (
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices);

            void XformVertices (
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                const blas::Matrix2 &xform);
            void XformVertices (
                const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                const blas::Matrix2 &red,
                const blas::Matrix2 &yellow);

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_shaper_Util_h)
