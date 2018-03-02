// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_T (BezierPolygon2)
                THEKOGANS_UTIL_IMPLEMENT_HEAP_T (BezierPolygon3)

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::IsClockWise () const {
                    assert (bezierPolygon.IsClosed ());
                    util::f32 normalZ = 0.0f;
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        std::size_t nextVertexIndex = (i + 1) % count;
                        normalZ +=
                            vertices[i].pt.x * vertices[nextVertexIndex].pt.y -
                            vertices[i].pt.y * vertices[nextVertexIndex].pt.x;
                    }
                    return normalZ < 0.0f;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::IsCircular (util::f32 eps) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    if (!bezierPolygon.IsClosed () || vertices.size () < 3) {
                        return false;
                    }
                    blas::Point2 center = GetCenterMass ();
                    util::f32 r = blas::Distance (center, vertices.back ().pt);
                    util::f32 d = blas::Distance (vertices.back ().pt, vertices[0].pt);
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        if (!util::IS_EQ (d / blas::Distance (vertices[i].pt, vertices[i + 1].pt), 1.0f, eps) ||
                            !util::IS_EQ (r / blas::Distance (center, vertices[i].pt), 1.0f, eps)) {
                            return false;
                        }
                    }
                    return true;
                }

                namespace {
                    blas::Polygon2 GetHullPolygon (const io::BezierPolygon2 &bezierPolygon) {
                        blas::Polygon2 polygon;
                        for (std::size_t i = 0, count = bezierPolygon.vertices.size (); i < count; ++i) {
                            polygon.points.push_back (bezierPolygon.vertices[i].pt + bezierPolygon.vertices[i].red);
                            polygon.points.push_back (bezierPolygon.vertices[i].pt);
                            polygon.points.push_back (bezierPolygon.vertices[i].pt + bezierPolygon.vertices[i].yellow);
                        }
                        polygon.closed = bezierPolygon.IsClosed ();
                        return polygon;
                    }
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL void BezierPolygon2::Outline (
                        util::f32 width,
                        io::BezierPolygon2::UniquePtr &bezierPolygon1,
                        io::BezierPolygon2::UniquePtr &bezierPolygon2) const {
                    blas::Polygon2 polygon = GetHullPolygon (bezierPolygon);
                    bezierPolygon1.reset (new io::BezierPolygon2 (bezierPolygon));
                    assert (bezierPolygon1.get () != 0);
                    bezierPolygon2.reset (new io::BezierPolygon2 (bezierPolygon));
                    assert (bezierPolygon2.get () != 0);
                    for (std::size_t i = 0, count = polygon.points.size (), vertexIndex = 0; i < count; i += 3, ++vertexIndex) {
                        blas::Point2 redNormal = polygon.PointNormal (i) * width * 0.5f;
                        blas::Point2 ptNormal = polygon.PointNormal (i + 1) * width * 0.5f;
                        blas::Point2 yellowNormal = polygon.PointNormal (i + 2) * width * 0.5f;
                        bezierPolygon1->vertices[vertexIndex].pt += ptNormal;
                        bezierPolygon1->vertices[vertexIndex].red =
                            bezierPolygon.vertices[vertexIndex].pt +
                            bezierPolygon.vertices[vertexIndex].red +
                            redNormal -
                            bezierPolygon1->vertices[vertexIndex].pt;
                        bezierPolygon1->vertices[vertexIndex].yellow =
                            bezierPolygon.vertices[vertexIndex].pt +
                            bezierPolygon.vertices[vertexIndex].yellow +
                            yellowNormal -
                            bezierPolygon1->vertices[vertexIndex].pt;
                        bezierPolygon2->vertices[vertexIndex].pt -= ptNormal;
                        bezierPolygon2->vertices[vertexIndex].red =
                            bezierPolygon.vertices[vertexIndex].pt +
                            bezierPolygon.vertices[vertexIndex].red -
                            redNormal -
                            bezierPolygon2->vertices[vertexIndex].pt;
                        bezierPolygon2->vertices[vertexIndex].yellow =
                            bezierPolygon.vertices[vertexIndex].pt +
                            bezierPolygon.vertices[vertexIndex].yellow -
                            yellowNormal -
                            bezierPolygon2->vertices[vertexIndex].pt;
                    }
                    if (!bezierPolygon.IsClosed ()) {
                        bezierPolygon1 = BezierPolygon2 (
                            *bezierPolygon1.get ()).PolyConnect (
                                *bezierPolygon2.get (), CONNECT_LAST_LAST);
                        bezierPolygon2.reset ();
                    }
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL util::f32 BezierPolygon2::GetSegmentBoundIntersection (
                        std::size_t vertexIndex1,
                        std::size_t vertexIndex2,
                        const blas::Bound2 &bound,
                        util::ui32 steps) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    blas::BezierCubic2 bezierCubic (
                        vertices[vertexIndex1].pt, vertices[vertexIndex1].pt + vertices[vertexIndex1].yellow,
                        vertices[vertexIndex2].pt + vertices[vertexIndex2].red, vertices[vertexIndex2].pt);
                    util::f32 t = 0.0f;
                    util::f32 delta = 1.0f / (steps + 1);
                    for (util::ui32 i = 0; i <= steps; ++i, t += delta) {
                        blas::Point2 p1 = bezierCubic.GetPoint (t, false);
                        blas::Point2 p2 = bezierCubic.GetPoint (t + delta, false);
                        util::f32 dx = p2.x - p1.x;
                        util::f32 dy = p2.y - p1.y;
                        if (dx == 0.0f && dy == 0.0f) {
                            return -1.0f;
                        }
                        util::f32 t1 = 0.0f;
                        util::f32 t2 = 1.0f;
                        if (blas::CLIPt (dx, bound.min.x - p1.x, t1, t2) && blas::CLIPt (-dx, p1.x - bound.max.x, t1, t2) &&
                                blas::CLIPt (dy, bound.min.y - p1.y, t1, t2) && blas::CLIPt (-dy, p1.y - bound.max.y, t1, t2)) {
                            return t + (t1 + t2) * 0.5f * delta;
                        }
                    }
                    return -1.0f;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr BezierPolygon2::Simplify (util::f32 amount) const {
                    io::BezierPolygon2::UniquePtr newBezierPolygon (new io::BezierPolygon2);
                    assert (newBezierPolygon.get () != 0);
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        amount = fabs (amount);
                        newBezierPolygon->vertices.push_back (vertices[0]);
                        for (std::size_t i = 1, count = vertices.size () - !bezierPolygon.IsClosed (); i < count; ++i) {
                            blas::Point2 t = vertices[NextVertexIndex (i)].pt - newBezierPolygon->vertices.back ().pt;
                            blas::Matrix2 xform =
                                blas::Matrix2::Translate (-newBezierPolygon->vertices.back ().pt) *
                                blas::Matrix2::RotateZ (blas::atan2f (t.x, t.y));
                            io::BezierPolygon2::Vertex vertex1 = newBezierPolygon->vertices.back () * xform;
                            io::BezierPolygon2::Vertex vertex2 = vertices[i] * xform;
                            io::BezierPolygon2::Vertex vertex3 = vertices[NextVertexIndex (i)] * xform;
                            if (!(blas::fabsf (vertex1.yellow.x) < amount &&
                                    blas::fabsf (vertex2.pt.x) < amount &&
                                    blas::fabsf (vertex2.red.x) < amount &&
                                    blas::fabsf (vertex2.yellow.x) < amount &&
                                    blas::fabsf (vertex3.red.x) < amount)) {
                                newBezierPolygon->vertices.push_back (vertices[i]);
                            }
                        }
                        if (!bezierPolygon.IsClosed ()) {
                            newBezierPolygon->vertices.push_back (vertices.back ());
                        }
                        else {
                            newBezierPolygon->vertices.back ().flags |= io::BezierPolygon2::Vertex::Last;
                        }
                    }
                    return newBezierPolygon;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr BezierPolygon2::Linear () const {
                    io::BezierPolygon2::UniquePtr newBezierPolygon (new io::BezierPolygon2);
                    assert (newBezierPolygon.get () != 0);
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    newBezierPolygon->vertices.resize (vertices.size ());
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        newBezierPolygon->vertices[i] = io::BezierPolygon2::Vertex (
                            vertices[i].pt, blas::Point2::Zero, blas::Point2::Zero, vertices[i].flags);
                    }
                    return newBezierPolygon;
                }

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

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr BezierPolygon2::Curve () const {
                    io::BezierPolygon2::UniquePtr newBezierPolygon (new io::BezierPolygon2);
                    assert (newBezierPolygon.get () != 0);
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    if (bezierPolygon.IsClosed ()) {
                        if (vertices.size () > 2) {
                            if (IsCircular (CURVE_EPS)) {
                                blas::Point2 center = GetCenterMass ();
                                blas::Point2 t = vertices[0].pt - center;
                                util::f32 radius = t.Length ();
                                util::f32 startAngle = atan2f (t.y, t.x);
                                util::f32 spanAngle = util::RAD (360.0f) / vertices.size ();
                                if (IsClockWise ()) {
                                    spanAngle *= -1;
                                }
                                blas::Point2 red = blas::Arc (
                                    center, radius, startAngle + (vertices.size () - 1) * spanAngle, spanAngle).p3;
                                for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                    blas::BezierCubic2 bezierCubic =
                                        blas::Arc (center, radius, startAngle + i * spanAngle, spanAngle);
                                    newBezierPolygon->vertices.push_back (io::BezierPolygon2::Vertex (
                                        vertices[i].pt, red, bezierCubic.p2, vertices[i].flags));
                                    red = bezierCubic.p3;
                                }
                            }
                            else {
                                const util::f32 *aj;
                                std::size_t m;
                                switch (vertices.size ()) {
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
                                for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                    blas::Point2 da (blas::Point2::Zero);
                                    for (std::size_t j = 1; j <= m; ++j) {
                                        std::size_t nextVertexIndex = (i + j) % vertices.size ();
                                        std::size_t previousVertexIndex = i < j ? vertices.size () - j + i : i - j;
                                        da += aj[j - 1] * (vertices[nextVertexIndex].pt - vertices[previousVertexIndex].pt);
                                    }
                                    newBezierPolygon->vertices.push_back (io::BezierPolygon2::Vertex (
                                        vertices[i].pt, -da, da, vertices[i].flags));
                                }
                            }
                        }
                    }
                    else if (vertices.size () > 1) {
                        const util::f32 *aj;
                        std::size_t m;
                        switch (vertices.size ()) {
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
                        for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                            blas::Point2 da (blas::Point2::Zero);
                            for (std::size_t j = 1; j <= m; ++j) {
                                std::size_t nextVertexIndex = i + j;
                                if (nextVertexIndex >= vertices.size ()) {
                                    nextVertexIndex = vertices.size () + vertices.size () - nextVertexIndex - 1;
                                }
                                std::size_t previousVertexIndex = i < j ? j - i - 1 : i - j;
                                da += aj[j - 1] * (vertices[nextVertexIndex].pt - vertices[previousVertexIndex].pt);
                            }
                            newBezierPolygon->vertices.push_back (io::BezierPolygon2::Vertex (
                                vertices[i].pt, -da, da, vertices[i].flags));
                        }
                    }
                    return newBezierPolygon;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL void BezierPolygon2::CrossHorizontal (
                        util::f32 y,
                        util::ui32 steps,
                        std::vector<util::f32> &intersections) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size () - !vertices.back ().IsClosed (); i < count; ++i) {
                        std::size_t nextVertexIndex = (i + 1) % vertices.size ();
                        blas::BezierCubic2 (
                            vertices[i].pt,
                            vertices[i].pt + vertices[i].yellow,
                            vertices[nextVertexIndex].red + vertices[nextVertexIndex].pt,
                            vertices[nextVertexIndex].pt).CrossHorizontal (y, steps, intersections);
                    }
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL void BezierPolygon2::CrossVertical (
                        util::f32 x,
                        util::ui32 steps,
                        std::vector<util::f32> &intersections) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size () - !vertices.back ().IsClosed (); i < count; ++i) {
                        std::size_t nextVertexIndex = (i + 1) % vertices.size ();
                        blas::BezierCubic2 (
                            vertices[i].pt,
                            vertices[i].pt + vertices[i].yellow,
                            vertices[nextVertexIndex].red + vertices[nextVertexIndex].pt,
                            vertices[nextVertexIndex].pt).CrossVertical (x, steps, intersections);
                    }
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::InCircle (
                        const blas::Circle &circle,
                        util::ui32 steps) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        blas::BezierCubic2 bezierCubic (
                            vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                            vertices[i + 1].pt + vertices[i + 1].red, vertices[i + 1].pt);
                        if (!bezierCubic.InCircle (circle, steps)) {
                            return false;
                        }
                    }
                    if (bezierPolygon.IsClosed ()) {
                        blas::BezierCubic2 bezierCubic (
                            vertices.back ().pt, vertices.back ().pt + vertices.back ().yellow,
                            vertices[0].pt + vertices[0].red, vertices[0].pt);
                        if (!bezierCubic.InCircle (circle, steps)) {
                            return false;
                        }
                    }
                    return true;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::CrossCircle (
                        const blas::Circle &circle,
                        util::ui32 steps) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        blas::BezierCubic2 bezierCubic (
                            vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                            vertices[i + 1].pt + vertices[i + 1].red, vertices[i + 1].pt);
                        if (bezierCubic.InCircle (circle, steps) ||
                                bezierCubic.CrossCircle (circle, steps)) {
                            return true;
                        }
                    }
                    if (bezierPolygon.IsClosed ()) {
                        blas::BezierCubic2 bezierCubic (
                            vertices.back ().pt, vertices.back ().pt + vertices.back ().yellow,
                            vertices[0].pt + vertices[0].red, vertices[0].pt);
                        if (bezierCubic.InCircle (circle, steps) ||
                                bezierCubic.CrossCircle (circle, steps)) {
                            return true;
                        }
                    }
                    return false;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::InPolygon (
                        const blas::Polygon2 &polygon,
                        util::ui32 steps) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        blas::BezierCubic2 bezierCubic (
                            vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                            vertices[i + 1].pt + vertices[i + 1].red, vertices[i + 1].pt);
                        if (!bezierCubic.InPolygon (polygon, steps)) {
                            return false;
                        }
                    }
                    if (bezierPolygon.IsClosed ()) {
                        blas::BezierCubic2 bezierCubic (
                            vertices.back ().pt, vertices.back ().pt + vertices.back ().yellow,
                            vertices[0].pt + vertices[0].red, vertices[0].pt);
                        if (!bezierCubic.InPolygon (polygon, steps)) {
                            return false;
                        }
                    }
                    return true;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::CrossPolygon (
                        const blas::Polygon2 &polygon,
                        util::ui32 steps) const {
                    const std::vector<io::BezierPolygon2::Vertex> &vertices = bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                        blas::BezierCubic2 bezierCubic (
                            vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                            vertices[i + 1].pt + vertices[i + 1].red, vertices[i + 1].pt);
                        if (bezierCubic.InPolygon (polygon, steps) ||
                                bezierCubic.CrossPolygon (polygon, steps)) {
                            return true;
                        }
                    }
                    if (bezierPolygon.IsClosed ()) {
                        blas::BezierCubic2 bezierCubic (
                            vertices.back ().pt, vertices.back ().pt + vertices.back ().yellow,
                            vertices[0].pt + vertices[0].red, vertices[0].pt);
                        if (bezierCubic.InPolygon (polygon, steps) ||
                                bezierCubic.CrossPolygon (polygon, steps)) {
                            return true;
                        }
                    }
                    return false;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
