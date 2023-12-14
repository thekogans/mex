// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonSegmentCommands.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2SegmentLinearCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3SegmentLinearCommand)

                    namespace {
                        const util::f32 a3[] = {0.3333f};
                        const util::f32 a4[] = {0.2500f};
                        const util::f32 a5[] = {0.2727f, -0.0909f};
                        const util::f32 a6[] = {0.2677f, -0.0667f};
                        const util::f32 a7[] = {0.2683f, -0.0732f, 0.0244f};
                        const util::f32 a8[] = {0.2679f, -0.0714f, 0.0179f};
                        const util::f32 a9[] = {0.2680f, -0.0719f, 0.0196f, -0.0065f};
                        const util::f32 a10[] = {0.2679f, -0.0718f, 0.0191f, -0.0048f};
                        const util::f32 a11[] = {0.2680f, -0.0718f, 0.0193f, -0.0053f, 0.0018f};
                        const util::f32 a12[] = {0.2679f, -0.0718f, 0.0192f, -0.0051f, 0.0013f};
                        const util::f32 a13[] = {0.2679f, -0.0718f, 0.0192f, -0.0052f, 0.0014f, -0.0005f};
                        const util::f32 a14[] = {0.2679f, -0.0718f, 0.0192f, -0.0052f, 0.0014f, -0.0003f};
                        const util::f32 a15[] = {0.2679f, -0.0718f, 0.0192f, -0.0052f, 0.0014f, -0.0004f, 0.0001f};

                        const util::f32 CURVE_EPS = 0.01f;
                    }

                    bool BezierPolygon2SegmentCurveCommand::Execute () {
                        if (bezierPolygon.IsClosed ()) {
                            if (bezierPolygon.vertices.size () < 3) {
                                return false;
                            }
                            if (ext::BezierPolygon2 (bezierPolygon).IsCircular (CURVE_EPS)) {
                                blas::Point2 center =
                                    ext::BezierPolygon2 (bezierPolygon).GetCenterMass ();
                                blas::Point2 t = bezierPolygon.vertices[vertexIndex1].pt - center;
                                util::f32 radius = t.Length ();
                                util::f32 startAngle = atan2f (t.y, t.x);
                                util::f32 spanAngle =
                                    util::RAD (360.0f) / bezierPolygon.vertices.size ();
                                if (ext::BezierPolygon2 (bezierPolygon).IsClockWise ()) {
                                    spanAngle *= -1;
                                }
                                blas::BezierCubic2 bezierCubic = blas::Arc (
                                    center, radius, startAngle, spanAngle);
                                ExecuteAndAddCommand (
                                    thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2VertexSetYellowCommand (
                                            bezierPolygon,
                                            vertexIndex1,
                                            bezierCubic.p2 - bezierCubic.p1)));
                                ExecuteAndAddCommand (
                                    thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2VertexSetRedCommand (
                                            bezierPolygon,
                                            vertexIndex2,
                                            bezierCubic.p3 - bezierCubic.p4)));
                            }
                            else {
                                const util::f32 *aj;
                                util::ui32 m;
                                switch (bezierPolygon.vertices.size ()) {
                                    case 3:
                                        aj = a3;
                                        m = 1;
                                        break;
                                    case 4:
                                        aj = a4;
                                        m = 1;
                                        break;
                                    case 5:
                                        aj = a5;
                                        m = 2;
                                        break;
                                    case 6:
                                        aj = a6;
                                        m = 2;
                                        break;
                                    case 7:
                                        aj = a7;
                                        m = 3;
                                        break;
                                    case 8:
                                        aj = a8;
                                        m = 3;
                                        break;
                                    case 9:
                                        aj = a9;
                                        m = 4;
                                        break;
                                    case 10:
                                        aj = a10;
                                        m = 4;
                                        break;
                                    case 11:
                                        aj = a11;
                                        m = 5;
                                        break;
                                    case 12:
                                        aj = a12;
                                        m = 5;
                                        break;
                                    case 13:
                                        aj = a13;
                                        m = 6;
                                        break;
                                    case 14:
                                        aj = a14;
                                        m = 6;
                                        break;
                                    default:
                                        aj = a15;
                                        m = 7;
                                        break;
                                }
                                {
                                    blas::Point2 da (blas::Point2::Zero);
                                    for (util::ui32 i = 1; i <= m; ++i) {
                                        std::size_t previousVertexIndex = vertexIndex1 < i ?
                                            bezierPolygon.vertices.size () - i + vertexIndex1 :
                                            vertexIndex1 - i;
                                        std::size_t nextVertexIndex =
                                            (vertexIndex1 + i) % bezierPolygon.vertices.size ();
                                        da += aj[i - 1] * (bezierPolygon.vertices[nextVertexIndex].pt -
                                            bezierPolygon.vertices[previousVertexIndex].pt);
                                    }
                                    ExecuteAndAddCommand (thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2VertexSetYellowCommand (
                                            bezierPolygon, vertexIndex1, da)));
                                }
                                {
                                    blas::Point2 da (blas::Point2::Zero);
                                    for (util::ui32 i = 1; i <= m; ++i) {
                                        std::size_t previousVertexIndex = vertexIndex2 < i ?
                                            bezierPolygon.vertices.size () - i + vertexIndex2 :
                                            vertexIndex2 - i;
                                        std::size_t nextVertexIndex =
                                            (vertexIndex2 + i) % bezierPolygon.vertices.size ();
                                        da += aj[i - 1] * (bezierPolygon.vertices[nextVertexIndex].pt -
                                            bezierPolygon.vertices[previousVertexIndex].pt);
                                    }
                                    ExecuteAndAddCommand (thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2VertexSetRedCommand (
                                            bezierPolygon, vertexIndex2, da)));
                                }
                            }
                        }
                        else {
                            if (bezierPolygon.vertices.size () < 2) {
                                return false;
                            }
                            const util::f32 *aj;
                            util::ui32 m;
                            switch (bezierPolygon.vertices.size ()) {
                                case 2:
                                    aj = a4;
                                    m = 1;
                                    break;
                                case 3:
                                    aj = a6;
                                    m = 2;
                                    break;
                                case 4:
                                    aj = a8;
                                    m = 3;
                                    break;
                                case 5:
                                    aj = a10;
                                    m = 4;
                                    break;
                                case 6:
                                    aj = a12;
                                    m = 5;
                                    break;
                                case 7:
                                    aj = a15;
                                    m = 6;
                                    break;
                                default:
                                    aj = a15;
                                    m = 7;
                                    break;
                            }
                            {
                                blas::Point2 da (blas::Point2::Zero);
                                for (util::ui32 i = 1; i <= m; ++i) {
                                    std::size_t nextVertexIndex = vertexIndex1 + i;
                                    if (nextVertexIndex >= bezierPolygon.vertices.size ()) {
                                        nextVertexIndex = bezierPolygon.vertices.size () +
                                            bezierPolygon.vertices.size () - nextVertexIndex - 1;
                                    }
                                    std::size_t previousVertexIndex = vertexIndex1 < i ?
                                        i - vertexIndex1 - 1 : vertexIndex1 - i;
                                    da += aj[i - 1] * (bezierPolygon.vertices[nextVertexIndex].pt -
                                        bezierPolygon.vertices[previousVertexIndex].pt);
                                }
                                ExecuteAndAddCommand (thekogans::mex::command::Command::SharedPtr (
                                    new BezierPolygon2VertexSetYellowCommand (
                                        bezierPolygon, vertexIndex1, da)));
                            }
                            {
                                blas::Point2 da (blas::Point2::Zero);
                                for (util::ui32 i = 1; i <= m; ++i) {
                                    std::size_t nextVertexIndex = vertexIndex2 + i;
                                    if (nextVertexIndex >= bezierPolygon.vertices.size ()) {
                                        nextVertexIndex = bezierPolygon.vertices.size () +
                                            bezierPolygon.vertices.size () - nextVertexIndex - 1;
                                    }
                                    std::size_t previousVertexIndex = vertexIndex2 < i ?
                                        i - vertexIndex2 - 1 : vertexIndex2 - i;
                                    da += aj[i - 1] * (bezierPolygon.vertices[nextVertexIndex].pt -
                                        bezierPolygon.vertices[previousVertexIndex].pt);
                                }
                                ExecuteAndAddCommand (thekogans::mex::command::Command::SharedPtr (
                                    new BezierPolygon2VertexSetRedCommand (
                                        bezierPolygon, vertexIndex2, da)));
                            }
                        }
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (BezierPolygon2SegmentCurveCommand)

                    bool BezierPolygon2SegmentRefineCommand::Execute () {
                        std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                        blas::BezierCubic2 bezierCubic (
                            vertices[vertexIndex1].pt,
                            vertices[vertexIndex1].pt + vertices[vertexIndex1].yellow,
                            vertices[vertexIndex2].pt + vertices[vertexIndex2].red,
                            vertices[vertexIndex2].pt);
                        util::f32 t = 0.0f;
                        util::f32 delta = 1.0f / (steps + 1);
                        for (util::ui32 i = 0; i <= steps; ++i, t += delta) {
                            blas::Point2 p1 = bezierCubic.GetPoint (t, false);
                            blas::Point2 p2 = bezierCubic.GetPoint (t + delta, false);
                            util::f32 dx = p2.x - p1.x;
                            util::f32 dy = p2.y - p1.y;
                            if (dx == 0.0f && dy == 0.0f) {
                                return false;
                            }
                            util::f32 t1 = 0.0f;
                            util::f32 t2 = 1.0f;
                            if (blas::CLIPt (dx, bound.min.x - p1.x, t1, t2) &&
                                    blas::CLIPt (-dx, p1.x - bound.max.x, t1, t2) &&
                                    blas::CLIPt (dy, bound.min.y - p1.y, t1, t2) &&
                                    blas::CLIPt (-dy, p1.y - bound.max.y, t1, t2)) {
                                blas::BezierCubic2 left;
                                blas::BezierCubic2 right;
                                bezierCubic.Split (t + (t1 + t2) * 0.5f * delta, left, right, false);
                                ExecuteAndAddCommand (
                                    thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2VertexSetYellowCommand (
                                            bezierPolygon,
                                            vertexIndex1,
                                            left.p2 - left.p1)));
                                ExecuteAndAddCommand (
                                    thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2VertexSetRedCommand (
                                            bezierPolygon,
                                            vertexIndex2,
                                            right.p3 - right.p4)));
                                ExecuteAndAddCommand (
                                    thekogans::mex::command::Command::SharedPtr (
                                        new BezierPolygon2InsertVertexCommand (
                                            bezierPolygon,
                                            vertexIndex1 + 1,
                                            io::BezierPolygon2::Vertex (
                                                left.p4,
                                                left.p3 - left.p4,
                                                right.p2 - right.p1))));
                                return true;
                            }
                        }
                        return false;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (BezierPolygon2SegmentRefineCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
