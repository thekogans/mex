// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_shaper_BezierPolygon_h)
#define __thekogans_mex_3ds_opengl_shaper_BezierPolygon_h

#include <memory>
#include <vector>
#include "thekogans/util/Heap.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/View.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                template<typename T>
                struct BezierPolygon {
                    typedef std::unique_ptr<BezierPolygon<T> > UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    // Component color palette.
                    enum {
                        COLOR_VERTEX_NORMAL,
                        COLOR_VERTEX_SELECTED,
                        COLOR_VERTEX_FIRST,
                        COLOR_VERTEX_SELECTED_FIRST,
                        COLOR_STEP_NORMAL,
                        COLOR_STEP_SELECTED,
                        COLOR_STEP_SHAPE,
                        COLOR_STEP_SELECTED_SHAPE,
                        COLOR_SEGMENT_NORMAL,
                        COLOR_SEGMENT_SELECTED,
                        COLOR_SEGMENT_SHAPE,
                        COLOR_SEGMENT_SELECTED_SHAPE,
                        COLOR_COUNT
                    };

                    // Default color palette.
                    static const util::ui32 DefaultColors[COLOR_COUNT];

                    const io::BezierPolygon<T> &bezierPolygon;
                    enum {
                        ShowVertices = 1,
                        ShowFirst = 2,
                        ShowSteps = 4,
                        Optimize = 8
                    };
                    util::ui32 flags;
                    util::ui16 steps;
                    util::ui16 detail;
                    util::ui16 selectionSet;
                    THEKOGANS_UTIL_TYPENAME ext::BezierPolygon<T>::VertexInfo vertexInfo;
                    util::ui32 colors[COLOR_COUNT];

                    BezierPolygon (const io::BezierPolygon<T> &bezierPolygon_,
                        util::ui32 flags_, util::ui16 steps_, util::ui16 detail_,
                        util::ui16 selectionSet_, const util::ui32 colors_[] = DefaultColors) :
                        bezierPolygon (bezierPolygon_), flags (flags_), steps (steps_),
                        detail (detail_), selectionSet (selectionSet_), vertexInfo (bezierPolygon) {
                        vertexInfo.Build ();
                        memcpy (colors, colors_, COLOR_COUNT * sizeof (util::ui32));
                    }

                    void Draw (const View &view) const;
                    void DrawWithColor (const View &view,
                        const thekogans::mex::opengl::ui8Color &color = DefaultPalette[MEDIUM_GRAY]) const;

                    void DrawSegment (const View &view, std::size_t vertexIndex1, std::size_t vertexIndex2) const;
                    void DrawSegmentWithColor (const View &view, std::size_t vertexIndex1, std::size_t vertexIndex2,
                        const thekogans::mex::opengl::ui8Color &color = DefaultPalette[MEDIUM_GRAY]) const;

                    void DrawVertex (const View &view, std::size_t vertexIndex) const;
                    void DrawVertexWithColor (const View &view, std::size_t vertexIndex,
                        const thekogans::mex::opengl::ui8Color &color = DefaultPalette[MEDIUM_GRAY]) const;

                private:
                    BezierPolygon<T> (const BezierPolygon<T> &);
                    BezierPolygon<T> &operator = (const BezierPolygon<T> &);
                };

                typedef BezierPolygon<blas::Point2> BezierPolygon2;
                typedef BezierPolygon<blas::Point3> BezierPolygon3;

                template<typename T>
                const util::ui32 BezierPolygon<T>::DefaultColors[COLOR_COUNT] = {
                    WHITE, RED, BLACK, GREEN,
                    WHITE, RED, YELLOW, ORANGE,
                    WHITE, RED, YELLOW, ORANGE
                };

                template<typename T>
                void BezierPolygon<T>::Draw (const View &view) const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        for (std::size_t i = 0, count = vertices.size () - (bezierPolygon.IsClosed () ? 0 : 1); i < count; ++i) {
                            std::size_t nextVertexIndex = ext::BezierPolygon<T> (bezierPolygon).NextVertexIndex (i);
                            assert (nextVertexIndex != util::NIDX);
                            if (!vertices[i].IsHidden () && !vertices[nextVertexIndex].IsHidden ()) {
                                blas::Polygon<T> polygon = blas::BezierCubic<T> (
                                    vertices[i].pt,
                                    vertices[i].pt + vertices[i].yellow,
                                    vertices[nextVertexIndex].pt + vertices[nextVertexIndex].red,
                                    vertices[nextVertexIndex].pt).ToPolygon (steps,
                                        (THEKOGANS_UTIL_TYPENAME blas::BezierCubic<T>::Detail)detail,
                                        util::Flags32 (flags).Test (Optimize));
                                {
                                    Color color (
                                        vertexInfo.IsFrozen () ?
                                            DefaultPalette[GRAY] :
                                            vertexInfo.IsShape () ?
                                                vertexInfo.IsSelected (selectionSet) ?
                                                    DefaultPalette[colors[COLOR_SEGMENT_SELECTED_SHAPE]] :
                                                    DefaultPalette[colors[COLOR_SEGMENT_SHAPE]] :
                                                vertexInfo.IsSelected (selectionSet) ?
                                                    DefaultPalette[colors[COLOR_SEGMENT_SELECTED]] :
                                                    DefaultPalette[colors[COLOR_SEGMENT_NORMAL]]);
                                    DrawPolygon (polygon);
                                }
                                if (util::Flags32 (flags).Test (ShowSteps)) {
                                    Color color (
                                        vertexInfo.IsFrozen () ?
                                            DefaultPalette[GRAY] :
                                            vertexInfo.IsShape () ?
                                                vertexInfo.IsSelected (selectionSet) ?
                                                    DefaultPalette[colors[COLOR_STEP_SELECTED_SHAPE]] :
                                                    DefaultPalette[colors[COLOR_STEP_SHAPE]] :
                                                vertexInfo.IsSelected (selectionSet) ?
                                                    DefaultPalette[colors[COLOR_STEP_SELECTED]] :
                                                    DefaultPalette[colors[COLOR_STEP_NORMAL]]);
                                    util::ui32 icon = GetTickIcon (7);
                                    for (std::size_t j = 1, count = polygon.points.size () - 1; j < count; ++j) {
                                        DrawIcon (icon, polygon.points[j]);
                                    }
                                }
                                if (util::Flags32 (flags).Test (ShowVertices)) {
                                    if (!vertices[i].IsHidden ()) {
                                        DrawVertex (view, i);
                                    }
                                }
                            }
                        }
                        // Draw the first vertex again to make sure it stands out.
                        if (util::Flags32 (flags).Test (ShowVertices)) {
                            if (!bezierPolygon.IsClosed () && !vertices[vertices.size () - 1].IsHidden ()) {
                                DrawVertex (view, vertices.size () - 1);
                            }
                            if (!vertices[0].IsHidden ()) {
                                DrawVertex (view, 0);
                            }
                        }
                    }
                }

                template<typename T>
                void BezierPolygon<T>::DrawWithColor (const View &view, const thekogans::mex::opengl::ui8Color &color) const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        Color color_ (color);
                        for (std::size_t i = 0, count = vertices.size () - (bezierPolygon.IsClosed () ? 0 : 1); i < count; ++i) {
                            std::size_t nextVertexIndex = ext::BezierPolygon<T> (bezierPolygon).NextVertexIndex (i);
                            assert (nextVertexIndex != util::NIDX);
                            if (!vertices[i].IsHidden () && !vertices[nextVertexIndex].IsHidden ()) {
                                blas::Polygon<T> polygon = blas::BezierCubic<T> (
                                    vertices[i].pt,
                                    vertices[i].pt + vertices[i].yellow,
                                    vertices[nextVertexIndex].pt + vertices[nextVertexIndex].red,
                                    vertices[nextVertexIndex].pt).ToPolygon (steps,
                                        (THEKOGANS_UTIL_TYPENAME blas::BezierCubic<T>::Detail)detail,
                                        util::Flags32 (flags).Test (Optimize));
                                DrawPolygon (polygon);
                                if (util::Flags32 (flags).Test (ShowSteps)) {
                                    util::ui32 icon = GetTickIcon (7);
                                    for (std::size_t j = 1, count = polygon.points.size () - 1; j < count; ++j) {
                                        DrawIcon (icon, polygon.points[j]);
                                    }
                                }
                                if (util::Flags32 (flags).Test (ShowVertices)) {
                                    if (!vertices[i].IsHidden ()) {
                                        DrawIcon (GetTickIcon (7), vertices[i].pt);
                                    }
                                }
                            }
                        }
                        if (util::Flags32 (flags).Test (ShowVertices) && !bezierPolygon.IsClosed () &&
                                !vertices[vertices.size () - 1].IsHidden ()) {
                            DrawIcon (GetTickIcon (7), vertices[vertices.size () - 1].pt);
                        }
                    }
                }

                template<typename T>
                void BezierPolygon<T>::DrawSegment (const View &view,
                        std::size_t vertexIndex1, std::size_t vertexIndex2) const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    assert (vertexIndex1 < vertices.size ());
                    assert (vertexIndex2 < vertices.size ());
                    if (!vertices[vertexIndex1].IsHidden () && !vertices[vertexIndex2].IsHidden ()) {
                        blas::Polygon<T> polygon = blas::BezierCubic<T> (
                            vertices[vertexIndex1].pt,
                            vertices[vertexIndex1].pt + vertices[vertexIndex1].yellow,
                            vertices[vertexIndex2].pt + vertices[vertexIndex2].red,
                            vertices[vertexIndex2].pt).ToPolygon (
                                steps, (THEKOGANS_UTIL_TYPENAME blas::BezierCubic<T>::Detail)detail,
                                util::Flags32 (flags).Test (Optimize));
                        {
                            Color color (
                                vertexInfo.IsFrozen () ?
                                    DefaultPalette[GRAY] :
                                    vertexInfo.IsShape () ?
                                        vertexInfo.IsSelected (selectionSet) ?
                                            DefaultPalette[colors[COLOR_SEGMENT_SELECTED_SHAPE]] :
                                            DefaultPalette[colors[COLOR_SEGMENT_SHAPE]] :
                                        vertexInfo.IsSelected (selectionSet) ?
                                            DefaultPalette[colors[COLOR_SEGMENT_SELECTED]] :
                                            DefaultPalette[colors[COLOR_SEGMENT_NORMAL]]);
                            DrawPolygon (polygon);
                        }
                        if (util::Flags32 (flags).Test (ShowSteps)) {
                            Color color (
                                vertexInfo.IsFrozen () ?
                                    DefaultPalette[GRAY] :
                                    vertexInfo.IsShape () ?
                                        vertexInfo.IsSelected (selectionSet) ?
                                            DefaultPalette[colors[COLOR_STEP_SELECTED_SHAPE]] :
                                            DefaultPalette[colors[COLOR_STEP_SHAPE]] :
                                        vertexInfo.IsSelected (selectionSet) ?
                                            DefaultPalette[colors[COLOR_STEP_SELECTED]] :
                                            DefaultPalette[colors[COLOR_STEP_NORMAL]]);
                            util::ui32 icon = GetTickIcon (7);
                            for (std::size_t i = 1, count = polygon.points.size () - 1; i < count; ++i) {
                                DrawIcon (icon, polygon.points[i]);
                            }
                        }
                        if (util::Flags32 (flags).Test (ShowVertices)) {
                            if (!vertices[vertexIndex1].IsHidden ()) {
                                DrawVertex (view, vertexIndex1);
                            }
                            if (!vertices[vertexIndex2].IsHidden ()) {
                                DrawVertex (view, vertexIndex2);
                            }
                        }
                    }
                }

                template<typename T>
                void BezierPolygon<T>::DrawSegmentWithColor (const View &view,
                        std::size_t vertexIndex1, std::size_t vertexIndex2, const thekogans::mex::opengl::ui8Color &color) const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    assert (vertexIndex1 < vertices.size ());
                    assert (vertexIndex2 < vertices.size ());
                    if (!vertices[vertexIndex1].IsHidden () && !vertices[vertexIndex2].IsHidden ()) {
                        blas::Polygon<T> polygon = blas::BezierCubic<T> (
                            vertices[vertexIndex1].pt,
                            vertices[vertexIndex1].pt + vertices[vertexIndex1].yellow,
                            vertices[vertexIndex2].pt + vertices[vertexIndex2].red,
                            vertices[vertexIndex2].pt).ToPolygon (
                                steps, (THEKOGANS_UTIL_TYPENAME blas::BezierCubic<T>::Detail)detail,
                                util::Flags32 (flags).Test (Optimize));
                        Color color_ (color);
                        DrawPolygon (polygon);
                        util::ui32 icon = GetTickIcon (7);
                        if (util::Flags32 (flags).Test (ShowSteps)) {
                            for (std::size_t i = 1, count = polygon.points.size () - 1; i < count; ++i) {
                                DrawIcon (icon, polygon.points[i]);
                            }
                        }
                        if (util::Flags32 (flags).Test (ShowVertices)) {
                            if (!vertices[vertexIndex1].IsHidden ()) {
                                DrawIcon (icon, vertices[vertexIndex1].pt);
                            }
                            if (!vertices[vertexIndex2].IsHidden ()) {
                                DrawIcon (icon, vertices[vertexIndex2].pt);
                            }
                        }
                    }
                }

                template<typename T>
                void BezierPolygon<T>::DrawVertex (const View &view, std::size_t vertexIndex) const {
                    assert (vertexIndex < bezierPolygon.vertices.size ());
                    const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &vertex =
                        bezierPolygon.vertices[vertexIndex];
                    assert (!vertex.IsHidden ());
                    util::ui16 vertexSelectMask = io::BezierPolygon<T>::Vertex::GetSelectMask (
                        selectionSet);
                    Color color (
                        vertex.IsFrozen () ?
                            DefaultPalette[GRAY] :
                            vertexIndex == 0 && util::Flags32 (flags).Test (ShowFirst) ?
                                util::Flags16 (vertex.flags).Test (vertexSelectMask) ?
                                    DefaultPalette[colors[COLOR_VERTEX_SELECTED_FIRST]] :
                                    DefaultPalette[colors[COLOR_VERTEX_FIRST]] :
                                util::Flags16 (vertex.flags).Test (vertexSelectMask) ?
                                    DefaultPalette[colors[COLOR_VERTEX_SELECTED]] :
                                    DefaultPalette[colors[COLOR_VERTEX_NORMAL]]);
                    DrawIcon (GetTickIcon (7), vertex.pt);
                }

                template<typename T>
                void BezierPolygon<T>::DrawVertexWithColor (const View &view, std::size_t vertexIndex,
                        const thekogans::mex::opengl::ui8Color &color) const {
                    assert (vertexIndex < bezierPolygon.vertices.size ());
                    const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &vertex =
                        bezierPolygon.vertices[vertexIndex];
                    assert (!vertex.IsHidden ());
                    Color color_ (color);
                    DrawIcon (GetTickIcon (7), vertex.pt);
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_shaper_BezierPolygon_h)
