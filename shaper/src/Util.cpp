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

#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommandSpecializations.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/command/DrawPolygonCommand.h"
#include "thekogans/mex/shaper/command/DrawPolygonsCommand.h"
#include "thekogans/mex/shaper/command/DrawPolygonsFinalOperation.h"
#include "thekogans/mex/shaper/command/ErasePolygonCommand.h"
#include "thekogans/mex/shaper/command/ErasePolygonsCommand.h"
#include "thekogans/mex/shaper/command/DrawSegmentCommand.h"
#include "thekogans/mex/shaper/command/EraseSegmentCommand.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            THEKOGANS_UTIL_IMPLEMENT_HEAP (BezierPolygonSegment)
            THEKOGANS_UTIL_IMPLEMENT_HEAP (BezierPolygonVertex)

            // Polygon

            void DrawPolygon (const _3ds::io::BezierPolygon2 &bezierPolygon, bool flip) {
                for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                        i < count; ++i) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new DrawPolygonCommand (
                                core::GetIOProject ().shaper.viewLayout[i], bezierPolygon)));
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void ErasePolygon (const _3ds::io::BezierPolygon2 &bezierPolygon, bool flip) {
                for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                        i < count; ++i) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new ErasePolygonCommand (
                                core::GetIOProject ().shaper.viewLayout[i], bezierPolygon)));
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void AddPolygon (_3ds::io::BezierPolygon2::UniquePtr bezierPolygon, bool flip) {
                assert (bezierPolygon.get () != 0);
                if (bezierPolygon.get () != 0) {
                    const _3ds::io::BezierPolygon2 &bezierPolygon_ = *bezierPolygon.get ();
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new command::OwnerVectorBezierPolygon2push_backCommand (
                                core::GetIOProject ().shaper.polygons2, std::move (bezierPolygon))));
                    DrawPolygon (bezierPolygon_, flip);
                }
            }

            void DeletePolygon (std::size_t polygonIndex, bool flip) {
                assert (polygonIndex < core::GetIOProject ().shaper.polygons2.size ());
                ErasePolygon (*core::GetIOProject ().shaper.polygons2[polygonIndex], flip);
                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                    command::Command::UniquePtr (
                        new command::OwnerVectorBezierPolygon2eraseCommand (
                            core::GetIOProject ().shaper.polygons2, polygonIndex)));
            }

            std::size_t GetPolygonIndex (const _3ds::io::BezierPolygon2 &bezierPolygon) {
                for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                    if (core::GetIOProject ().shaper.polygons2[i] == &bezierPolygon) {
                        return i;
                    }
                }
                return util::NIDX;
            }

            // Polygons

            void DrawPolygons2 (
                    const _3ds::opengl::View &view,
                    const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                    const blas::Matrix2 &xform,
                    const opengl::ui8Color &color) {
                core::Tool::Viewport2D viewport2D (view, color);
                for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                    _3ds::io::BezierPolygon2::UniquePtr bezierPolygon (*bezierPolygons[i] * xform);
                    assert (bezierPolygon.get () != 0);
                    if (bezierPolygon.get () != 0) {
                        _3ds::opengl::DrawPolygon (
                            _3ds::ext::BezierPolygon2 (*bezierPolygon.get ()).ToPolygon (
                                core::GetIOProject ().shaper.steps));
                    }
                }
            }

            void DrawPolygons (const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons, bool flip) {
                assert (!bezierPolygons.empty ());
                if (!bezierPolygons.empty ()) {
                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                            i < count; ++i) {
                        core::GetCommandDispatcher ().ExecuteAndAddCommand (
                            command::Command::UniquePtr (
                                new DrawPolygonsCommand (
                                    core::GetIOProject ().shaper.viewLayout[i], bezierPolygons)));
                    }
                    if (flip) {
                        core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                            command::FinalOperation::UniquePtr (
                                new core::command::FlipFramebufferFinalOperation));
                    }
                }
            }

            void ErasePolygons (const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons, bool flip) {
                assert (!bezierPolygons.empty ());
                if (!bezierPolygons.empty ()) {
                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                            i < count; ++i) {
                        core::GetCommandDispatcher ().ExecuteAndAddCommand (
                            command::Command::UniquePtr (
                                new ErasePolygonsCommand (
                                    core::GetIOProject ().shaper.viewLayout[i], bezierPolygons)));
                    }
                    if (flip) {
                        core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                            command::FinalOperation::UniquePtr (
                                new core::command::FlipFramebufferFinalOperation));
                    }
                }
            }

            void AddPolygons (const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons, bool flip) {
                assert (!bezierPolygons.empty ());
                if (!bezierPolygons.empty ()) {
                    for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                        core::GetCommandDispatcher ().ExecuteAndAddCommand (
                            command::Command::UniquePtr (
                                new command::OwnerVectorBezierPolygon2push_backCommand (
                                    core::GetIOProject ().shaper.polygons2,
                                    _3ds::io::BezierPolygon2::UniquePtr (bezierPolygons[i]))));
                    }
                    DrawPolygons (bezierPolygons, flip);
                }
            }

            void DeletePolygons (const std::vector<std::size_t> &polygonIndices, bool flip) {
                std::vector<std::size_t> sortedPolygonIndices = polygonIndices;
                std::sort (sortedPolygonIndices.begin (), sortedPolygonIndices.end ());
                {
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;
                    bezierPolygons.resize (sortedPolygonIndices.size ());
                    for (std::size_t i = 0, count = sortedPolygonIndices.size (); i < count; ++i) {
                        bezierPolygons[i] = core::GetIOProject ().shaper.polygons2[sortedPolygonIndices[i]];
                        assert (bezierPolygons[i] != 0);
                    }
                    ErasePolygons (bezierPolygons, flip);
                }
                {
                    for (std::size_t i = sortedPolygonIndices.size (); i--;) {
                        core::GetCommandDispatcher ().ExecuteAndAddCommand (
                            command::Command::UniquePtr (
                                new command::OwnerVectorBezierPolygon2eraseCommand (
                                    core::GetIOProject ().shaper.polygons2, sortedPolygonIndices[i])));
                    }
                }
            }

            void GetSelectedPolygons (
                    std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                    util::ui16 selectMask,
                    bool frozen) {
                for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                    if (!_3ds::ext::BezierPolygon2 (*core::GetIOProject ().shaper.polygons2[i]).IsHidden () &&
                            (frozen || !_3ds::ext::BezierPolygon2 (
                                *core::GetIOProject ().shaper.polygons2[i]).IsFrozen ()) &&
                            _3ds::ext::BezierPolygon2 (
                                *core::GetIOProject ().shaper.polygons2[i]).IsSelected (selectMask)) {
                        bezierPolygons.push_back (core::GetIOProject ().shaper.polygons2[i]);
                    }
                }
            }

            void ToggleSelectedPolygons (const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons) {
                assert (!bezierPolygons.empty ());
                if (!bezierPolygons.empty ()) {
                    for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                        if (_3ds::ext::BezierPolygon2 (*bezierPolygons[i]).IsSelected (
                                core::GetIOProject ().shaper.selectMask)) {
                            core::GetCommandDispatcher ().ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2ClearFlagsCommand (
                                        *bezierPolygons[i], core::GetIOProject ().shaper.selectMask)));
                        }
                        else {
                            core::GetCommandDispatcher ().ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2SetFlagsCommand (
                                        *bezierPolygons[i], core::GetIOProject ().shaper.selectMask)));
                        }
                    }
                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                            i < count; ++i) {
                        core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                            command::FinalOperation::UniquePtr (
                                new DrawPolygonsFinalOperation (
                                    core::GetIOProject ().shaper.viewLayout[i], bezierPolygons)));
                    }
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            blas::Bound2 GetPolygonsBound (const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons) {
                blas::Bound2 bound (blas::Bound2::Empty);
                for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                    bound += _3ds::ext::BezierPolygon2 (*bezierPolygons[i]).GetBound (
                        core::GetIOProject ().shaper.steps);
                }
                return bound;
            }

            void GetPolygonIndeces (
                    const std::vector<_3ds::io::BezierPolygon2 *> &bezierPolygons,
                    std::vector<std::size_t> &polygonIndices) {
                for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                    std::size_t polygonIndex = GetPolygonIndex (*bezierPolygons[i]);
                    if (polygonIndex != util::NIDX) {
                        polygonIndices.push_back (polygonIndex);
                    }
                }
            }

            // Segment

            void DrawSegment (
                    const _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2,
                    bool flip) {
                for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                        i < count; ++i) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new DrawSegmentCommand (core::GetIOProject ().shaper.viewLayout[i],
                                bezierPolygon, vertexIndex1, vertexIndex2)));
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void EraseSegment (
                    const _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2,
                    bool flip) {
                for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                        i < count; ++i) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new EraseSegmentCommand (core::GetIOProject ().shaper.viewLayout[i],
                                bezierPolygon, vertexIndex1, vertexIndex2)));
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void DrawSegmentSegments (
                    const _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2,
                    bool flip) {
                {
                    std::size_t previousVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex1);
                    if (previousVertexIndex != util::NIDX) {
                        DrawSegment (bezierPolygon, previousVertexIndex, vertexIndex1, false);
                    }
                }
                DrawSegment (bezierPolygon, vertexIndex1, vertexIndex2, false);
                {
                    std::size_t nextVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex2);
                    if (nextVertexIndex != util::NIDX) {
                        DrawSegment (bezierPolygon, vertexIndex2, nextVertexIndex, false);
                    }
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void EraseSegmentSegments (
                    const _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2,
                    bool flip) {
                {
                    std::size_t previousVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex1);
                    if (previousVertexIndex != util::NIDX) {
                        EraseSegment (bezierPolygon, previousVertexIndex, vertexIndex1, false);
                    }
                }
                EraseSegment (bezierPolygon, vertexIndex1, vertexIndex2, false);
                {
                    std::size_t nextVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex2);
                    if (nextVertexIndex != util::NIDX) {
                        EraseSegment (bezierPolygon, vertexIndex2, nextVertexIndex, false);
                    }
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void HideSegment (
                    _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2) {
                if (!bezierPolygon.vertices[vertexIndex1].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex1,
                                bezierPolygon.vertices[vertexIndex1].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (!bezierPolygon.vertices[vertexIndex2].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex2,
                                bezierPolygon.vertices[vertexIndex2].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
            }

            void UnhideSegment (
                    _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2) {
                if (bezierPolygon.vertices[vertexIndex1].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex1,
                                bezierPolygon.vertices[vertexIndex1].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (bezierPolygon.vertices[vertexIndex2].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex2,
                                bezierPolygon.vertices[vertexIndex2].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
            }

            void HideSegmentSegments (
                    _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2) {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex1);
                if (previousVertexIndex != util::NIDX &&
                        !bezierPolygon.vertices[previousVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, previousVertexIndex,
                                bezierPolygon.vertices[previousVertexIndex].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (!bezierPolygon.vertices[vertexIndex1].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex1,
                                bezierPolygon.vertices[vertexIndex1].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (!bezierPolygon.vertices[vertexIndex2].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex2,
                                bezierPolygon.vertices[vertexIndex2].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex2);
                if (nextVertexIndex != util::NIDX &&
                        !bezierPolygon.vertices[nextVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, nextVertexIndex,
                                bezierPolygon.vertices[nextVertexIndex].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
            }

            void UnhideSegmentSegments (
                    _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2) {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex1);
                if (previousVertexIndex != util::NIDX &&
                        bezierPolygon.vertices[previousVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, previousVertexIndex,
                                bezierPolygon.vertices[previousVertexIndex].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (bezierPolygon.vertices[vertexIndex1].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex1,
                                bezierPolygon.vertices[vertexIndex1].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (bezierPolygon.vertices[vertexIndex2].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex2,
                                bezierPolygon.vertices[vertexIndex2].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex2);
                if (nextVertexIndex != util::NIDX &&
                        bezierPolygon.vertices[nextVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, nextVertexIndex,
                                bezierPolygon.vertices[nextVertexIndex].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
            }

            // Segments

            void GetSelectedSegments (std::vector<BezierPolygonSegment *> &segments, util::ui16 selectMask) {
                for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                    _3ds::io::BezierPolygon2 *bezierPolygon = core::GetIOProject ().shaper.polygons2[i];
                    const std::vector<_3ds::io::BezierPolygon2::Vertex> &vertices = bezierPolygon->vertices;
                    for (std::size_t j = 0, vertexcCount = vertices.size (); j < vertexcCount; ++j) {
                        if (!vertices[j].IsHidden () && !vertices[j].IsFrozen () &&
                                vertices[j].IsSelected (selectMask)) {
                            std::size_t nextVertexIndex =
                                _3ds::ext::BezierPolygon2 (*bezierPolygon).NextVertexIndex (j);
                            if (nextVertexIndex != util::NIDX &&
                                    !vertices[nextVertexIndex].IsHidden () &&
                                    !vertices[nextVertexIndex].IsFrozen () &&
                                    vertices[nextVertexIndex].IsSelected (selectMask)) {
                                BezierPolygonSegment::UniquePtr segment (
                                    new BezierPolygonSegment (*bezierPolygon, j, nextVertexIndex));
                                assert (segment.get () != 0);
                                segments.push_back (segment.get ());
                                segment.release ();
                            }
                        }
                    }
                }
            }

            void DrawSegments (const std::vector<BezierPolygonSegment *> &segments, bool flip) {
                for (std::size_t i = 0, count = segments.size (); i < count; ++i) {
                    DrawSegment (segments[i]->bezierPolygon, segments[i]->vertexIndex1,
                        segments[i]->vertexIndex2, false);
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void EraseSegments (const std::vector<BezierPolygonSegment *> &segments, bool flip) {
                for (std::size_t i = 0, count = segments.size (); i < count; ++i) {
                    EraseSegment (segments[i]->bezierPolygon, segments[i]->vertexIndex1,
                        segments[i]->vertexIndex2, false);
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void HideSegments (const std::vector<BezierPolygonSegment *> &segments) { 
                for (std::size_t i = 0, count = segments.size (); i < count; ++i) {
                    HideSegment (segments[i]->bezierPolygon, segments[i]->vertexIndex1,
                        segments[i]->vertexIndex2);
                }
            }

            void UnhideSegments (const std::vector<BezierPolygonSegment *> &segments) {
                for (std::size_t i = 0, count = segments.size (); i < count; ++i) {
                    UnhideSegment (segments[i]->bezierPolygon, segments[i]->vertexIndex1,
                        segments[i]->vertexIndex2);
                }
            }

            // Vertex

            void DrawVertexSegments (
                    const _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex,
                    bool flip) {
                {
                    std::size_t previousVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex);
                    if (previousVertexIndex != util::NIDX) {
                        DrawSegment (bezierPolygon, previousVertexIndex, vertexIndex, false);
                    }
                }
                {
                    std::size_t nextVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex);
                    if (nextVertexIndex != util::NIDX) {
                        DrawSegment (bezierPolygon, vertexIndex, nextVertexIndex, false);
                    }
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void EraseVertexSegments (
                    const _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex,
                    bool flip) {
                {
                    std::size_t previousVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex);
                    if (previousVertexIndex != util::NIDX) {
                        EraseSegment (bezierPolygon, previousVertexIndex, vertexIndex, false);
                    }
                }
                {
                    std::size_t nextVertexIndex =
                        _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex);
                    if (nextVertexIndex != util::NIDX) {
                        EraseSegment (bezierPolygon, vertexIndex, nextVertexIndex, false);
                    }
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void HideVertexSegments (_3ds::io::BezierPolygon2 &bezierPolygon, std::size_t vertexIndex) {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex);
                if (previousVertexIndex != util::NIDX &&
                        !bezierPolygon.vertices[previousVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, previousVertexIndex,
                                bezierPolygon.vertices[previousVertexIndex].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (!bezierPolygon.vertices[vertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex,
                                bezierPolygon.vertices[vertexIndex].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex);
                if (nextVertexIndex != util::NIDX &&
                        !bezierPolygon.vertices[nextVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, nextVertexIndex,
                                bezierPolygon.vertices[nextVertexIndex].flags |
                                _3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
            }

            void UnhideVertexSegments (_3ds::io::BezierPolygon2 &bezierPolygon, std::size_t vertexIndex) {
                std::size_t previousVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).PreviousVertexIndex (vertexIndex);
                if (previousVertexIndex != util::NIDX &&
                        bezierPolygon.vertices[previousVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, previousVertexIndex,
                                bezierPolygon.vertices[previousVertexIndex].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                if (bezierPolygon.vertices[vertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, vertexIndex,
                                bezierPolygon.vertices[vertexIndex].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
                std::size_t nextVertexIndex =
                    _3ds::ext::BezierPolygon2 (bezierPolygon).NextVertexIndex (vertexIndex);
                if (nextVertexIndex != util::NIDX &&
                        bezierPolygon.vertices[nextVertexIndex].IsHidden ()) {
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                bezierPolygon, nextVertexIndex,
                                bezierPolygon.vertices[nextVertexIndex].flags &
                                ~_3ds::io::BezierPolygon2::Vertex::Hidden)));
                }
            }

            // Vertices

            void GetSelectedVertices (std::vector<BezierPolygonVertex *> &vertices, util::ui16 selectMask) {
                for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                    _3ds::io::BezierPolygon2 *bezierPolygon = core::GetIOProject ().shaper.polygons2[i];
                    const std::vector<_3ds::io::BezierPolygon2::Vertex> &vertices_ = bezierPolygon->vertices;
                    for (std::size_t j = 0, vertexcCount = vertices_.size (); j < vertexcCount; ++j) {
                        if (!vertices_[j].IsHidden () && !vertices_[j].IsFrozen () &&
                                vertices_[j].IsSelected (selectMask)) {
                            BezierPolygonVertex::UniquePtr vertex (
                                new BezierPolygonVertex (*bezierPolygon, j));
                            assert (vertex.get () != 0);
                            vertices.push_back (vertex.get ());
                            vertex.release ();
                        }
                    }
                }
            }

            std::size_t FindReferenceVertex (
                    std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                    _3ds::io::BezierPolygon2 &bezierPolygon,
                    std::size_t vertexIndex,
                    util::ui16 selectMask) {
                // See if the polygon is in the list.
                std::size_t polygonIndex = util::NIDX;
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    if (&bezierPolygonVertices[i]->bezierPolygon == &bezierPolygon) {
                        polygonIndex = i;
                        break;
                    }
                }
                if (polygonIndex != util::NIDX) {
                    // Found polygon. Now find the vertex, or
                    // appropriate insertion point.
                    while (polygonIndex < bezierPolygonVertices.size () &&
                            &bezierPolygonVertices[polygonIndex]->bezierPolygon == &bezierPolygon &&
                            bezierPolygonVertices[polygonIndex]->vertexIndex < vertexIndex) {
                        ++polygonIndex;
                    }
                    // Found vertex. Return it.
                    if (polygonIndex < bezierPolygonVertices.size () &&
                            &bezierPolygonVertices[polygonIndex]->bezierPolygon == &bezierPolygon &&
                            bezierPolygonVertices[polygonIndex]->vertexIndex == vertexIndex) {
                        return polygonIndex;
                    }
                    // Insert a new one.
                    BezierPolygonVertex::UniquePtr vertex (
                        new BezierPolygonVertex (bezierPolygon, vertexIndex));
                    assert (vertex.get () != 0);
                    bezierPolygonVertices.insert (bezierPolygonVertices.begin () + polygonIndex, vertex.get ());
                    vertex.release ();
                    return polygonIndex;
                }
                // The polygon is not in the list. Just append
                // a new one to the end.
                BezierPolygonVertex::UniquePtr vertex (new BezierPolygonVertex (bezierPolygon, vertexIndex));
                assert (vertex.get () != 0);
                bezierPolygonVertices.push_back (vertex.get ());
                vertex.release ();
                return bezierPolygonVertices.size () - 1;
            }

            void GetSelectedVertexSegments (
                    std::vector<BezierPolygonSegment *> &segments,
                    util::ui16 selectMask) {
                for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                    _3ds::io::BezierPolygon2 *bezierPolygon = core::GetIOProject ().shaper.polygons2[i];
                    const std::vector<_3ds::io::BezierPolygon2::Vertex> &vertices = bezierPolygon->vertices;
                    for (std::size_t j = 0, vertexcCount = vertices.size (); j < vertexcCount; ++j) {
                        std::size_t nextVertexIndex =
                            _3ds::ext::BezierPolygon2 (*bezierPolygon).NextVertexIndex (j);
                        if (nextVertexIndex != util::NIDX &&
                                !vertices[j].IsHidden () && !vertices[j].IsFrozen () &&
                                !vertices[nextVertexIndex].IsHidden () &&
                                !vertices[nextVertexIndex].IsFrozen () &&
                                (vertices[j].IsSelected (selectMask) ||
                                    vertices[nextVertexIndex].IsSelected (selectMask))) {
                            BezierPolygonSegment::UniquePtr segment (
                                new BezierPolygonSegment (*bezierPolygon, j, nextVertexIndex));
                            assert (segment.get () != 0);
                            segments.push_back (segment.get ());
                            segment.release ();
                        }
                    }
                }
            }

            // IMPORTANT: The next two routines assume that the
            // vertices are sorted in polygon, and ascending
            // order. If you use GetSelectedVertices (above), that
            // will be done for you. If you generate the list by
            // yourself, you need to make sure that's the case.
            void DrawVertexSegments2 (
                    const _3ds::opengl::View &view,
                    const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                    const blas::Matrix2 &xform) {
                if (!bezierPolygonVertices.empty ()) {
                #if 0
                    BezierPolygon (bezierPolygonVertices[0]->bezierPolygon).DrawVertexSegments (
                        view, bezierPolygonVertices[0]->vertexIndex, xform);
                    std::size_t firstVertexIndex = bezierPolygonVertices[0]->vertexIndex;
                    for (std::size_t i = 1, count = bezierPolygonVertices.size (); i < count; ++i) {
                        if (&bezierPolygonVertices[i - 1]->bezierPolygon !=
                                &bezierPolygonVertices[i]->bezierPolygon) {
                            firstVertexIndex = bezierPolygonVertices[i]->vertexIndex;
                            BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawVertexSegments (
                                view, bezierPolygonVertices[i]->vertexIndex, xform);
                        }
                        else {
                            std::size_t previousNextVertexIndex = _3ds::ext::BezierPolygon2 (
                                bezierPolygonVertices[i - 1]->bezierPolygon).NextVertexIndex (
                                    bezierPolygonVertices[i - 1]->vertexIndex);
                            if (previousNextVertexIndex != bezierPolygonVertices[i]->vertexIndex) {
                                std::size_t nextVertexIndex = _3ds::ext::BezierPolygon2 (
                                    bezierPolygonVertices[i]->bezierPolygon).NextVertexIndex (
                                        bezierPolygonVertices[i]->vertexIndex);
                                if (nextVertexIndex != util::NIDX && nextVertexIndex != firstVertexIndex) {
                                    BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawVertexSegments (
                                        view, bezierPolygonVertices[i]->vertexIndex, xform);
                                }
                                else {
                                    std::size_t previousVertexIndex = _3ds::ext::BezierPolygon2 (
                                        bezierPolygonVertices[i]->bezierPolygon).PreviousVertexIndex (
                                            bezierPolygonVertices[i]->vertexIndex);
                                    if (previousVertexIndex != util::NIDX) {
                                        BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawSegment (
                                            view, previousVertexIndex, blas::Matrix2::Identity,
                                            bezierPolygonVertices[i]->vertexIndex, xform);
                                    }
                                }
                            }
                            else {
                                std::size_t nextVertexIndex = _3ds::ext::BezierPolygon2 (
                                    bezierPolygonVertices[i]->bezierPolygon).NextVertexIndex (
                                        bezierPolygonVertices[i]->vertexIndex);
                                if (nextVertexIndex != util::NIDX && nextVertexIndex != firstVertexIndex) {
                                    BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawSegment (
                                        view, bezierPolygonVertices[i]->vertexIndex, xform,
                                        nextVertexIndex, blas::Matrix2::Identity);
                                }
                            }
                        }
                    }
                #else
                #endif
                }
            }

            void DrawVertexSegments2 (
                    const _3ds::opengl::View &view,
                    const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                    const blas::Matrix2 &red,
                    const blas::Matrix2 &yellow) {
                if (!bezierPolygonVertices.empty ()) {
                    BezierPolygon (bezierPolygonVertices[0]->bezierPolygon).DrawVertexSegments (
                        view, bezierPolygonVertices[0]->vertexIndex, red, yellow);
                    std::size_t firstVertexIndex = bezierPolygonVertices[0]->vertexIndex;
                    for (std::size_t i = 1, count = bezierPolygonVertices.size (); i < count; ++i) {
                        if (&bezierPolygonVertices[i - 1]->bezierPolygon !=
                                &bezierPolygonVertices[i]->bezierPolygon) {
                            firstVertexIndex = bezierPolygonVertices[i]->vertexIndex;
                            BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawVertexSegments (
                                view, bezierPolygonVertices[i]->vertexIndex, red, yellow);
                        }
                        else {
                            std::size_t previousNextVertexIndex = _3ds::ext::BezierPolygon2 (
                                bezierPolygonVertices[i - 1]->bezierPolygon).NextVertexIndex (
                                    bezierPolygonVertices[i - 1]->vertexIndex);
                            if (previousNextVertexIndex != bezierPolygonVertices[i]->vertexIndex) {
                                std::size_t nextVertexIndex = _3ds::ext::BezierPolygon2 (
                                    bezierPolygonVertices[i]->bezierPolygon).NextVertexIndex (
                                        bezierPolygonVertices[i]->vertexIndex);
                                if (nextVertexIndex != util::NIDX && nextVertexIndex != firstVertexIndex) {
                                    BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawVertexSegments (
                                        view, bezierPolygonVertices[i]->vertexIndex, red, yellow);
                                }
                                else {
                                    std::size_t previousVertexIndex = _3ds::ext::BezierPolygon2 (
                                        bezierPolygonVertices[i]->bezierPolygon).PreviousVertexIndex (
                                            bezierPolygonVertices[i]->vertexIndex);
                                    if (previousVertexIndex != util::NIDX) {
                                        BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawSegment (view,
                                            previousVertexIndex, blas::Matrix2::Identity, blas::Matrix2::Identity,
                                            bezierPolygonVertices[i]->vertexIndex, red, yellow);
                                    }
                                    BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawVertexHandles (
                                        view, bezierPolygonVertices[i]->vertexIndex, red, yellow);
                                }
                            }
                            else {
                                std::size_t nextVertexIndex = _3ds::ext::BezierPolygon2 (
                                    bezierPolygonVertices[i]->bezierPolygon).NextVertexIndex (
                                        bezierPolygonVertices[i]->vertexIndex);
                                if (nextVertexIndex != util::NIDX && nextVertexIndex != firstVertexIndex) {
                                    BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawSegment (view,
                                        bezierPolygonVertices[i]->vertexIndex, red, yellow,
                                        nextVertexIndex, blas::Matrix2::Identity, blas::Matrix2::Identity);
                                }
                                BezierPolygon (bezierPolygonVertices[i]->bezierPolygon).DrawVertexHandles (
                                    view, bezierPolygonVertices[i]->vertexIndex, red, yellow);
                            }
                        }
                    }
                }
            }

            void DrawVertexSegments (const std::vector<BezierPolygonVertex *> &bezierPolygonVertices, bool flip) {
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    DrawVertexSegments (bezierPolygonVertices[i]->bezierPolygon,
                        bezierPolygonVertices[i]->vertexIndex, false);
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void EraseVertexSegments (const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                    bool flip) {
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    EraseVertexSegments (bezierPolygonVertices[i]->bezierPolygon,
                        bezierPolygonVertices[i]->vertexIndex, false);
                }
                if (flip) {
                    core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                        command::FinalOperation::UniquePtr (
                            new core::command::FlipFramebufferFinalOperation));
                }
            }

            void HideVertexSegments (const std::vector<BezierPolygonVertex *> &bezierPolygonVertices) {
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    HideVertexSegments (bezierPolygonVertices[i]->bezierPolygon,
                        bezierPolygonVertices[i]->vertexIndex);
                }
            }

            void UnhideVertexSegments (const std::vector<BezierPolygonVertex *> &bezierPolygonVertices) {
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    UnhideVertexSegments (bezierPolygonVertices[i]->bezierPolygon,
                        bezierPolygonVertices[i]->vertexIndex);
                }
            }

            void XformVertices (
                    const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                    const blas::Matrix2 &xform) {
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    std::size_t vertexIndex = bezierPolygonVertices[i]->vertexIndex;
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetPtCommand (
                                bezierPolygonVertices[i]->bezierPolygon, vertexIndex,
                                bezierPolygonVertices[i]->bezierPolygon.vertices[vertexIndex].pt * xform)));
                }
            }

            void XformVertices (
                    const std::vector<BezierPolygonVertex *> &bezierPolygonVertices,
                    const blas::Matrix2 &red,
                    const blas::Matrix2 &yellow) {
                for (std::size_t i = 0, count = bezierPolygonVertices.size (); i < count; ++i) {
                    std::size_t vertexIndex = bezierPolygonVertices[i]->vertexIndex;
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetRedCommand (
                                bezierPolygonVertices[i]->bezierPolygon, vertexIndex,
                                bezierPolygonVertices[i]->bezierPolygon.vertices[vertexIndex].red * red)));
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        command::Command::UniquePtr (
                            new _3ds::io::command::BezierPolygon2VertexSetYellowCommand (
                                bezierPolygonVertices[i]->bezierPolygon, vertexIndex,
                                bezierPolygonVertices[i]->bezierPolygon.vertices[vertexIndex].yellow * yellow)));
                }
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
