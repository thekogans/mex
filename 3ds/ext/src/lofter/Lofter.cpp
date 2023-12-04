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

#include <algorithm>
#include <cassert>
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "Triangulator.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                namespace {
                    inline bool CompareShapes (const ext::Lofter::Shape *shape1, const ext::Lofter::Shape *shape2) {
                        return shape1->shape.level < shape2->shape.level;
                    }
                }

                Lofter::Lofter (const io::Lofter &lofter) : Module (lofter),
                        scaleDeformation (new ScaleDeformation (lofter.scaleDeformation)),
                        twistDeformation (new TwistDeformation (lofter.twistDeformation)),
                        teeterDeformation (new TeeterDeformation (lofter.teeterDeformation)),
                        bevelDeformation (new BevelDeformation (lofter.bevelDeformation)),
                        fitDeformation (new FitDeformation (lofter.fitDeformation)) {
                    typedef util::OwnerVector<io::Lofter::Shape>::const_iterator const_iterator;
                    for (const_iterator it = lofter.shapes.begin (), end = lofter.shapes.end (); it != end; ++it) {
                        Shape::UniquePtr shape (new Shape (**it, lofter.masterShapeSteps,
                            lofter.modelParams.shapeDetail, lofter.modelParams.optimize));
                        shapes.push_back (shape.get ());
                        shape.release ();
                    }
                    // Shapes are not necessarily stored in level order.
                    // ext::Lofter::MakeMesh depends on them to be so.
                    std::sort (shapes.begin (), shapes.end (), CompareShapes);
                }

                View::UniquePtr Lofter::GetView (const io::View &view) const {
                    assert (view.IsNone () || view.IsOrthographic () || view.IsShape ());
                    return View::UniquePtr (new View (view));
                }

                View::UniquePtr Lofter::GetCurrView () const {
                    return GetView (ioLofter ().viewLayout.GetCurrView ());
                }

                blas::Bound2 Lofter::GetViewBound (const View &view) const {
                    blas::Bound2 bound (blas::Bound2::Empty);
                    if (view.view.IsShape ()) {
                        const Shape *shape = GetCurrShape ();
                        if (shape != 0) {
                            bound = shape->GetBound2 (view.xform);
                        }
                    }
                    else {
                        blas::Polygon3 path = BezierPolygon3 (ioLofter ().path.polygon).ToPolygon (
                            ioLofter ().path.steps);
                        bound = path.GetBound2 (view.xform);
                        std::vector<blas::Matrix3> frames;
                        path.GetFrames (frames, ioLofter ().contour);
                        for (std::size_t i = 0, shapeCount = shapes.size (); i < shapeCount; ++i) {
                            bound += shapes[i]->GetBound2 (frames[shapes[i]->shape.level] * view.xform);
                        }
                    }
                    return bound;
                }

                const Lofter::Shape *Lofter::GetCurrShape () const {
                    for (std::size_t i = 0, count = shapes.size (); i < count; ++i) {
                        if (shapes[i]->shape.level == ioLofter ().path.level) {
                            return shapes[i];
                        }
                    }
                    return 0;
                }

                namespace {
                    // A Crossection is a 3D Shape. Since Lofter::Shape is a 2D entity,
                    // and the final Mesh is 3D, Crossection is used to build
                    // the Mesh.
                    struct Crossection {
                        typedef std::unique_ptr<Crossection> UniquePtr;

                        const Lofter::Shape &shape;
                        struct Polygon {
                            typedef std::unique_ptr<Polygon> UniquePtr;

                            blas::Polygon3::UniquePtr outer;
                            util::OwnerVector<blas::Polygon3> inner;
                        };
                        util::OwnerVector<Polygon> polygons;

                        // Create a Crossection from a Shape and a matrix. The matrix
                        // represents the various deformations used to build the Model.
                        Crossection (const Lofter::Shape &shape_, const blas::Matrix3 &matrix) : shape (shape_) {
                            // vectorize
                            for (std::size_t i = 0, count = shape.polygons.size (); i < count; ++i) {
                                // The following idiom is used throughout the code:
                                // {
                                //     // This is usually represented by util::OwnerVector<TYPE>
                                //     std::vector<TYPE *> vectorOfTYPEs; // Sometimes passed in as argument
                                //     std::unique_ptr<TYPE> pointerToTYPE (new TYPE);
                                //     // do something with pointerToTYPE
                                //     vectorOfTYPEs.push_back (pointerToTYPE.get ());
                                //     pointerToTYPE.release ();
                                // }
                                // The above code provides a strong exception safety guarantee
                                // while keeping the code clean and readable. Please note that
                                // replacing:
                                //     vectorOfTYPEs.push_back (pointerToTYPE.get ());
                                //     pointerToTYPE.release ();
                                // with:
                                //     vectorOfTYPEs.push_back (pointerToTYPE.release ());
                                // breaks that promise and introduces a potential memory
                                // leak.
                                Polygon::UniquePtr polygon (new Polygon);
                                {
                                    assert (shape.polygons[i]->outer.get ());
                                    polygon->outer.reset (new blas::Polygon3 ());
                                    polygon->outer->points.resize (
                                        shape.polygons[i]->outer->points.size ());
                                    for (std::size_t j = 0,
                                            count = shape.polygons[i]->outer->points.size ();
                                            j < count; ++j) {
                                        polygon->outer->points[j] =
                                            shape.polygons[i]->outer->points[j] * matrix;
                                    }
                                }
                                {
                                    for (std::size_t j = 0,
                                            countInner = shape.polygons[i]->inner.size ();
                                            j < countInner; ++j) {
                                        blas::Polygon3::UniquePtr inner (new blas::Polygon3 ());
                                        inner->points.resize (
                                            shape.polygons[i]->inner[j]->points.size ());
                                        for (std::size_t k = 0,
                                                countInnerPoints = shape.polygons[i]->inner[j]->points.size ();
                                                k < countInnerPoints; ++k) {
                                            inner->points[k] =
                                                shape.polygons[i]->inner[j]->points[k] * matrix;
                                        }
                                        polygon->inner.push_back (inner.get ());
                                        inner.release ();
                                    }
                                }
                                polygons.push_back (polygon.get ());
                                polygon.release ();
                            }
                        }

                        inline void Triangulate (std::size_t firstVertexOffset,
                                bool flipFaceNormals, std::vector<io::Mesh::Face> &faces) const {
                            Triangulator (shape).GetFaces (
                                firstVertexOffset, flipFaceNormals, faces);
                        }

                    private:
                        Crossection (const Crossection &);
                        Crossection &operator = (const Crossection &);
                    };

                    struct Statistics {
                        std::size_t currentCrossection;
                        std::size_t crossectionCount;
                        struct CrossectionOffsets {
                            std::size_t total;
                            struct Polygon {
                                std::size_t outer;
                                std::vector<std::size_t> inner;

                                Polygon (std::size_t &total, const Crossection::Polygon &polygon) :
                                        outer (total) {
                                    total += polygon.outer->points.size ();
                                    // vectorize
                                    for (std::size_t i = 0, count = polygon.inner.size (); i < count; ++i) {
                                        inner.push_back (total);
                                        total += polygon.inner[i]->points.size ();
                                    }
                                }
                            };
                            std::vector<Polygon> polygon;

                            explicit CrossectionOffsets (const Crossection &crossection) :
                                    total (0) {
                                // vectorize
                                for (std::size_t i = 0, count = crossection.polygons.size (); i < count; ++i) {
                                    polygon.push_back (Polygon (total, *crossection.polygons[i]));
                                }
                            }
                        } crossectionOffsets;

                        Statistics (const blas::Polygon3 &pathPolygon, std::size_t pathSteps,
                                const util::OwnerVector<Lofter::Shape> &shapes) :
                                currentCrossection (0), crossectionCount (0), crossectionOffsets (
                                    *Crossection::UniquePtr (
                                        new Crossection (
                                            *Lofter::Shape::UniquePtr (new Lofter::Shape (*shapes[0])),
                                            blas::Matrix3::Identity))) {
                            for (std::size_t level = 0, levelCount = pathPolygon.points.size (),
                                    shapeIndex = 0; level < levelCount; ++level) {
                                if (shapeIndex < shapes.size () - 1 &&
                                        shapes[shapeIndex + 1]->shape.level == level) {
                                    ++shapeIndex;
                                }
                                if (level % pathSteps == 0 || shapes[shapeIndex]->shape.level == level) {
                                    ++crossectionCount;
                                }
                            }
                            assert (crossectionCount > 1);
                        }

                    private:
                        Statistics (const Statistics &);
                        Statistics &operator = (const Statistics &);
                    };

                    struct VertexInfo {
                        blas::Point3 pt;
                        util::ui16 index;
                    };

                    inline bool operator < (const VertexInfo &vertexInfo1, const VertexInfo &vertexInfo2) {
                        if (vertexInfo1.pt.x < vertexInfo2.pt.x) {
                            return true;
                        }
                        if (vertexInfo1.pt.x > vertexInfo2.pt.x) {
                            return false;
                        }
                        if (vertexInfo1.pt.y < vertexInfo2.pt.y) {
                            return true;
                        }
                        if (vertexInfo1.pt.y > vertexInfo2.pt.y) {
                            return false;
                        }
                        if (vertexInfo1.pt.z < vertexInfo2.pt.z) {
                            return true;
                        }
                        if (vertexInfo1.pt.z > vertexInfo2.pt.z) {
                            return false;
                        }
                        return vertexInfo1.index < vertexInfo2.index;
                    }

                    // Weld mesh vertices that are within weldThreshold of each other.
                    void WeldMeshVertices (io::Mesh &mesh, util::f32 weldThreshold = EPSILON) {
                        std::size_t vertexCount = mesh.vertices.size ();
                        std::vector<util::ui16> vertexMap;
                        vertexMap.resize (vertexCount);
                        std::vector<VertexInfo> vertexInfos;
                        vertexInfos.resize (vertexCount);
                        {
                            // Initialize maps.
                            // vectorize
                            for (std::size_t i = 0; i < vertexCount; ++i) {
                                vertexMap[i] = util::NIDX16;
                                vertexInfos[i].pt = mesh.vertices[i];
                                vertexInfos[i].index = (util::ui16)i;
                            }
                        }
                        // Sort vertices to cluster same ones together.
                        std::sort (vertexInfos.begin (), vertexInfos.end ());
                        {
                            // Loop over sorted vertices and mark in the map the
                            // same index for all similar vertices.
                            // vectorize
                            for (std::size_t i = 1; i < vertexCount; ++i) {
                                if (util::IS_EQ (vertexInfos[i].pt.x, vertexInfos[i - 1].pt.x, weldThreshold) &&
                                        util::IS_EQ (vertexInfos[i].pt.y, vertexInfos[i - 1].pt.y, weldThreshold) &&
                                        util::IS_EQ (vertexInfos[i].pt.z, vertexInfos[i - 1].pt.z, weldThreshold)) {
                                    vertexInfos[i].index =
                                    vertexMap[vertexInfos[i].index] = vertexInfos[i - 1].index;
                                }
                            }
                        }
                        {
                            // Loop over faces adjusting vertex indeces to point
                            // to first of a cluster of similar vertices.
                            for (std::size_t i = 0, count = mesh.faces.size (); i < count; ++i) {
                                io::Mesh::Face &face = mesh.faces[i];
                                if (vertexMap[face.a] != util::NIDX16) {
                                    face.a = vertexMap[face.a];
                                }
                                if (vertexMap[face.b] != util::NIDX16) {
                                    face.b = vertexMap[face.b];
                                }
                                if (vertexMap[face.c] != util::NIDX16) {
                                    face.c = vertexMap[face.c];
                                }
                            }
                        }
                        {
                            // This loop runs over all indeces in the map,
                            // compacting unused spaces. At the end it creates
                            // a new map representing the deltas the new indeces
                            // are from the old.
                            std::size_t i = 0;   // Loop index
                            std::size_t j = 0;   // Index in to next unused entry.
                                                 // At the end will hold the new vector size
                            std::size_t k = 0;   // Running delta representing number
                                                 // of previously removed vertices at
                                                 // a particular vertex index.
                            for (; i < vertexCount; ++i) {
                                if (vertexMap[i] != util::NIDX16) {
                                    ++k;
                                }
                                else {
                                    vertexMap[i] = (util::ui16)k;
                                    if (i != j) {
                                        mesh.vertices[j] = mesh.vertices[i];
                                        if (!mesh.vertexFlags.empty ()) {
                                            mesh.vertexFlags[j] = mesh.vertexFlags[i];
                                        }
                                        if (!mesh.textureVertices.empty ()) {
                                            mesh.textureVertices[j] = mesh.textureVertices[i];
                                        }
                                    }
                                    ++j;
                                }
                            }
                            assert (j <= i);
                            // Trim vertex array to remove unused entries.
                            if (j < i) {
                                // These guarantee not to throw, because they
                                // make arrays smaller.
                                mesh.vertices.resize (j);
                                if (!mesh.vertexFlags.empty ()) {
                                    mesh.vertexFlags.resize (j);
                                }
                                if (!mesh.textureVertices.empty ()) {
                                    mesh.textureVertices.resize (j);
                                }
                            }
                        }
                        {
                            // Loop over the faces to adjust vertex indeces based
                            // on new map deltas.
                            // vectorize
                            for (std::size_t i = 0, count = mesh.faces.size (); i < count; ++i) {
                                io::Mesh::Face &face = mesh.faces[i];
                                face.a -= vertexMap[face.a];
                                face.b -= vertexMap[face.b];
                                face.c -= vertexMap[face.c];
                            }
                        }
                    }
                }

                io::Mesh::UniquePtr Lofter::MakeMesh (const std::string &name) const {
                    bool pathClosed = ioLofter ().path.polygon.IsClosed ();
                    // Validate the model.
                    {
                        if (shapes.empty ()) {
                            throw NoShapes ();
                        }

                        if (shapes.size () > 1) {
                            Shape::UniquePtr shape0 (new Shape (*shapes[0]));
                            if (!shape0->IsValid ()) {
                                throw InvalidShape ();
                            }
                            for (std::size_t i = 1, count = shapes.size (); i < count; ++i) {
                                Shape::UniquePtr shapei (new Shape (*shapes[i]));
                                if (!shapei->IsValid ()) {
                                    throw InvalidShape ();
                                }
                                if (!shapei->IsSimilar (*shape0)) {
                                    throw DissimilarShape ();
                                }
                            }
                        }

                        if (ioLofter ().path.polygon.vertices.size () < 2) {
                            throw InvalidPath ();
                        }

                        if (pathClosed && (ioLofter ().modelParams.capStart ||
                            ioLofter ().modelParams.capEnd)) {
                            throw InvalidParams ();
                        }
                    }
                    // Prepare the path.
                    blas::Polygon3 pathPolygon =
                        BezierPolygon3 (ioLofter ().path.polygon).ToPolygon (ioLofter ().path.steps);

                    std::vector<util::f32> pathLengths;
                    pathPolygon.GetLengths (pathLengths);

                    std::vector<blas::Matrix3> pathFrames;
                    pathPolygon.GetFrames (pathFrames, ioLofter ().contour);

                    std::size_t pathSteps;
                    if (ioLofter ().tween) {
                        if (ioLofter ().modelParams.pathDetail == io::Lofter::ModelParams::Low) {
                            pathSteps = ioLofter ().path.steps + 1;
                        }
                        else if (ioLofter ().modelParams.pathDetail == io::Lofter::ModelParams::Medium) {
                            pathSteps = 2;
                        }
                        else {
                            pathSteps = 1;
                        }
                    }
                    else {
                        pathSteps = ioLofter ().path.steps + 1;
                    }
                    // Setup smoothing groups.
                    util::ui32 smoothWidth[2] = {0, 0};
                    util::ui32 smoothLength[2] = {0, 0};
                    bool doSmoothing = true;
                    if (ioLofter ().modelParams.smoothWidth &&
                        ioLofter ().modelParams.smoothLength) {
                        smoothWidth[0] = smoothWidth[1] = 1;
                        smoothLength[0] = smoothLength[1] = 1;
                    }
                    else if (ioLofter ().modelParams.smoothWidth) {
                        smoothWidth[0] = smoothWidth[1] = 1;
                        smoothLength[0] = smoothLength[1] = 2;
                    }
                    else if (ioLofter ().modelParams.smoothLength) {
                        smoothWidth[0] = smoothLength[0] = 1;
                        smoothWidth[1] = smoothLength[1] = 2;
                    }
                    else {
                        doSmoothing = false;
                    }
                    // Collect model statistics to be used by cross-section builder.
                    Statistics statistics (pathPolygon, pathSteps, shapes);
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    // Build cross-sections.
                    for (std::size_t level = 0, levelCount = pathPolygon.points.size (),
                            shapeIndex = 0, smoothIndex = 0;
                            level < levelCount; ++level, smoothIndex = 0) {
                        if (shapeIndex < shapes.size () - 1 &&
                                shapes[shapeIndex + 1]->shape.level == level) {
                            ++shapeIndex;
                        }
                        if (level % pathSteps == 0 || shapes[shapeIndex]->shape.level == level) {
                            // Lerp between neighboring shapes.
                            Shape::UniquePtr shape (new Shape (*shapes[shapeIndex]));
                            if (shapeIndex < shapes.size () - 1) {
                                std::size_t nextLevel = shapes[shapeIndex + 1]->shape.level;
                                if (level < nextLevel) {
                                    Shape::UniquePtr nextShape (new Shape (*shapes[shapeIndex + 1]));
                                    shape->Lerp (*nextShape,
                                        (util::f32)(level - shape->shape.level) /
                                        (util::f32)(nextLevel - shape->shape.level));
                                }
                            }
                            blas::Matrix3 deform = blas::Matrix3::Identity;
                            util::f32 t = pathLengths[level] / pathLengths[pathPolygon.points.size () - 1];
                            // Apply deformations.
                            if (ioLofter ().scale) {
                                deform *= blas::Matrix3::Scale (blas::Point3 (
                                    scaleDeformation->GetXZValue (t), scaleDeformation->GetYZValue (t), 1.0f));
                            }
                            if (ioLofter ().twist) {
                                deform *= blas::Matrix3::RotateZ (twistDeformation->GetValue (t));
                            }
                            if (ioLofter ().teeter) {
                                deform *= blas::Matrix3::RotateX (
                                    teeterDeformation->GetXZValue (t)) *
                                    blas::Matrix3::RotateY (teeterDeformation->GetYZValue (t));
                            }
                            if (ioLofter ().bevel) {
                                shape->Outline (bevelDeformation->GetValue (t));
                            }
                            if (ioLofter ().fit) {
                                std::vector<util::f32> xIntersections;
                                fitDeformation->GetXZValues (t, xIntersections);
                                if (xIntersections.empty ()) {
                                    throw InvalidFitX ();
                                }
                                util::f32 minx = xIntersections[0];
                                util::f32 maxx = xIntersections[0];
                                {
                                    for (std::size_t i = 1, count = xIntersections.size (); i < count; ++i) {
                                        if (minx > xIntersections[i]) {
                                            minx = xIntersections[i];
                                        }
                                        else if (maxx < xIntersections[i]) {
                                            maxx = xIntersections[i];
                                        }
                                    }
                                }
                                std::vector<util::f32> yIntersections;
                                fitDeformation->GetYZValues (t, yIntersections);
                                if (yIntersections.empty ()) {
                                    throw InvalidFitY ();
                                }
                                util::f32 miny = yIntersections[0];
                                util::f32 maxy = yIntersections[0];
                                {
                                    for (std::size_t i = 1, count = yIntersections.size (); i < count; ++i) {
                                        if (miny > yIntersections[i]) {
                                            miny = yIntersections[i];
                                        }
                                        else if (maxy < yIntersections[i]) {
                                            maxy = yIntersections[i];
                                        }
                                    }
                                }
                                blas::Bound2 bound = shape->GetBound2 (blas::Matrix3::Identity);
                                blas::Point2 extents = bound.Extents ();
                                deform *= blas::Matrix3::Translate (-bound.Center ()) *
                                    blas::Matrix3::Scale (
                                        blas::Point3 ((maxx - minx) / extents.x,
                                            (maxy - miny) / extents.y, 1.0f)) *
                                    blas::Matrix3::Translate (
                                        blas::Point3 ((minx + maxx) * 0.5f,
                                            (miny + maxy) * 0.5f, 0.0f));
                            }

                            Crossection::UniquePtr crossection (
                                new Crossection (*shape, deform * pathFrames[level]));
                            if (level == 0 && ioLofter ().modelParams.capStart) {
                                crossection->Triangulate (0, true, mesh->faces);
                                if (doSmoothing) {
                                    mesh->faceSmoothGroups.insert (mesh->faceSmoothGroups.end (),
                                        mesh->faces.size () - mesh->faceSmoothGroups.size (), 0);
                                }
                            }
                            std::size_t currentCrossectionStartVertexIndex =
                                statistics.currentCrossection * statistics.crossectionOffsets.total;
                            std::size_t nextCrossectionStartVertexIndex =
                                ((statistics.currentCrossection + 1) % statistics.crossectionCount) *
                                statistics.crossectionOffsets.total;
                            util::f32 v =
                                ioLofter ().modelParams.mapping.length * (ioLofter ().modelParams.mapping.normalizeLength ?
                                    pathLengths[level] / pathLengths.back () :
                                    (util::f32)level / pathPolygon.points.size ());
                            for (std::size_t polygonIndex = 0,
                                    polygonCount = crossection->polygons.size ();
                                    polygonIndex < polygonCount; ++polygonIndex) {
                                mesh->vertices.insert (mesh->vertices.end (),
                                    crossection->polygons[polygonIndex]->outer->points.begin (),
                                    crossection->polygons[polygonIndex]->outer->points.end ());
                                if (ioLofter ().modelParams.mapping.use) {
                                    std::vector<util::f32> outerLengths;
                                    crossection->polygons[polygonIndex]->outer->GetLengths (outerLengths);
                                    assert (outerLengths.size () ==
                                        crossection->polygons[polygonIndex]->outer->points.size ());
                                    for (std::size_t vertexIndex = 0,
                                            vertexCount = outerLengths.size ();
                                            vertexIndex < vertexCount; ++vertexIndex) {
                                        mesh->textureVertices.push_back (blas::Point2 (
                                            ioLofter ().modelParams.mapping.perimeter * outerLengths[vertexIndex] / outerLengths.back (), v));
                                    }
                                }
                                if (statistics.currentCrossection < statistics.crossectionCount - 1 || pathClosed) {
                                    std::size_t outerPolygonStartVertexIndex =
                                        statistics.crossectionOffsets.polygon[polygonIndex].outer;
                                    // Three part vertex offset creation algorithm:
                                    // 1 - select crossection
                                    // 2 - select polygon in crossection
                                    // 3 - select vertex in polygon in crossection
                                    for (std::size_t vertexIndex = 0,
                                            vertexCount = crossection->polygons[polygonIndex]->outer->points.size ();
                                            vertexIndex < vertexCount; ++vertexIndex) {
                                        std::size_t nextVertexIndex = (vertexIndex + 1) % vertexCount;
                                        mesh->faces.push_back (io::Mesh::Face (
                                            currentCrossectionStartVertexIndex + outerPolygonStartVertexIndex + vertexIndex,
                                            nextCrossectionStartVertexIndex + outerPolygonStartVertexIndex + vertexIndex,
                                            nextCrossectionStartVertexIndex + outerPolygonStartVertexIndex + nextVertexIndex,
                                            io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible));
                                        mesh->faces.push_back (io::Mesh::Face (
                                            currentCrossectionStartVertexIndex + outerPolygonStartVertexIndex + vertexIndex,
                                            nextCrossectionStartVertexIndex + outerPolygonStartVertexIndex + nextVertexIndex,
                                            currentCrossectionStartVertexIndex + outerPolygonStartVertexIndex + nextVertexIndex,
                                            io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible));
                                        // FIXME: implement io::Mesh::Face::UWrap and io::Mesh::Face::VWrap
                                        if (doSmoothing) {
                                            mesh->faceSmoothGroups.push_back (smoothWidth[smoothIndex]);
                                            mesh->faceSmoothGroups.push_back (smoothWidth[smoothIndex]);
                                            smoothIndex ^= 1;
                                        }
                                    }
                                }
                                for (std::size_t innerIndex = 0,
                                        innerCount = crossection->polygons[polygonIndex]->inner.size ();
                                        innerIndex < innerCount; ++innerIndex) {
                                    mesh->vertices.insert (mesh->vertices.end (),
                                        crossection->polygons[polygonIndex]->inner[innerIndex]->points.begin (),
                                        crossection->polygons[polygonIndex]->inner[innerIndex]->points.end ());
                                    if (ioLofter ().modelParams.mapping.use) {
                                        std::vector<util::f32> innerLengths;
                                        crossection->polygons[polygonIndex]->inner[innerIndex]->GetLengths (innerLengths);
                                        assert (innerLengths.size () == crossection->polygons[polygonIndex]->inner[innerIndex]->points.size ());
                                        for (std::size_t vertexIndex = 0, vertexCount = innerLengths.size (); vertexIndex < vertexCount; ++vertexIndex) {
                                            mesh->textureVertices.push_back (blas::Point2 (
                                                ioLofter ().modelParams.mapping.perimeter * innerLengths[vertexIndex] / innerLengths.back (), v));
                                        }
                                    }
                                    if (statistics.currentCrossection < statistics.crossectionCount - 1 || pathClosed) {
                                        std::size_t innerPolygonStartVertexIndex = statistics.crossectionOffsets.polygon[polygonIndex].inner[innerIndex];
                                        for (std::size_t vertexIndex = 0,
                                                vertexCount = crossection->polygons[polygonIndex]->inner[innerIndex]->points.size ();
                                                vertexIndex < vertexCount; ++vertexIndex) {
                                            std::size_t nextVertexIndex = (vertexIndex + 1) % vertexCount;
                                            mesh->faces.push_back (io::Mesh::Face (
                                                currentCrossectionStartVertexIndex + innerPolygonStartVertexIndex + vertexIndex,
                                                nextCrossectionStartVertexIndex + innerPolygonStartVertexIndex + vertexIndex,
                                                nextCrossectionStartVertexIndex + innerPolygonStartVertexIndex + nextVertexIndex,
                                                io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible));
                                            mesh->faces.push_back (io::Mesh::Face (
                                                currentCrossectionStartVertexIndex + innerPolygonStartVertexIndex + vertexIndex,
                                                nextCrossectionStartVertexIndex + innerPolygonStartVertexIndex + nextVertexIndex,
                                                currentCrossectionStartVertexIndex + innerPolygonStartVertexIndex + nextVertexIndex,
                                                io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible));
                                            // FIXME: implement io::Mesh::Face::UWrap and io::Mesh::Face::VWrap
                                            if (doSmoothing) {
                                                mesh->faceSmoothGroups.push_back (smoothWidth[smoothIndex]);
                                                mesh->faceSmoothGroups.push_back (smoothWidth[smoothIndex]);
                                                smoothIndex ^= 1;
                                            }
                                        }
                                    }
                                }
                            }
                            ++statistics.currentCrossection;
                            if (statistics.currentCrossection == statistics.crossectionCount && ioLofter ().modelParams.capEnd) {
                                assert (statistics.currentCrossection > 1);
                                crossection->Triangulate ((statistics.currentCrossection - 1) * statistics.crossectionOffsets.total, false, mesh->faces);
                                if (doSmoothing) {
                                    mesh->faceSmoothGroups.insert (mesh->faceSmoothGroups.end (),
                                        mesh->faces.size () - mesh->faceSmoothGroups.size (), 0);
                                }
                            }
                            if (doSmoothing) {
                                std::swap (smoothWidth[0], smoothLength[0]);
                                std::swap (smoothWidth[1], smoothLength[1]);
                            }
                        }
                    }

                    if (ioLofter ().modelParams.weldVertices) {
                        WeldMeshVertices (*mesh);
                    }

                    return mesh;
                }

                void Lofter::Swap (Lofter &lofter) {
                    shapes.swap (lofter.shapes);
                    std::swap (scaleDeformation, lofter.scaleDeformation);
                    std::swap (twistDeformation, lofter.twistDeformation);
                    std::swap (teeterDeformation, lofter.teeterDeformation);
                    std::swap (bevelDeformation, lofter.bevelDeformation);
                    std::swap (fitDeformation, lofter.fitDeformation);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
