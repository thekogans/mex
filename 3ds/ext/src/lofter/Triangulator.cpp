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

#include <iostream>
#include <algorithm>
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Line.h"
#include "Triangulator.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Triangulator::Vertex)
                THEKOGANS_UTIL_IMPLEMENT_HEAP (Triangulator::Loop)
                THEKOGANS_UTIL_IMPLEMENT_HEAP (Triangulator::Face)

                Triangulator::Triangulator (const Lofter::Shape &shape) : faces (0) {
                    for (std::size_t i = 0, index = 0, polygonCount = shape.polygons.size (); i < polygonCount; ++i) {
                        Face *face = NewFace (faces);
                        {
                            assert (shape.polygons[i]->outer.get ());
                            points.insert (points.end (), shape.polygons[i]->outer->points.begin (),
                                shape.polygons[i]->outer->points.end ());
                            Loop *loop = NewLoop (face);
                            for (std::size_t j = 0, outerVertexCount = shape.polygons[i]->outer->points.size (); j < outerVertexCount; ++j) {
                                NewVertex (index++, loop);
                            }
                        }
                        {
                            for (std::size_t j = 0, innerCount = shape.polygons[i]->inner.size (); j < innerCount; ++j) {
                                points.insert (points.end (),
                                    shape.polygons[i]->inner[j]->points.begin (),
                                    shape.polygons[i]->inner[j]->points.end ());
                                Loop *loop = NewLoop (face);
                                for (std::size_t k = 0, innerVertexCount = shape.polygons[i]->inner[j]->points.size (); k < innerVertexCount; ++k) {
                                    NewVertex (index++, loop);
                                }
                            }
                        }
                    }
                    RemoveHoles ();
                    MakeConvex ();
                    TriangulateConvex ();
                }

                void Triangulator::GetFaces (
                        std::size_t offset,
                        bool flipNormal,
                        std::vector<io::Mesh::Face> &newFaces) const {
                    for (Face *face = faces; face; face = face->next) {
                        Vertex *vertex = face->outer->vertex;
                        if (flipNormal) {
                            newFaces.push_back (
                                io::Mesh::Face (
                                    offset + vertex->index,
                                    offset + vertex->prev->prev->index,
                                    offset + vertex->prev->index,
                                    0));
                            if (vertex->IsNeighbour (vertex->prev->prev)) {
                                newFaces.back ().flags |= io::Mesh::Face::EdgeABVisible;
                            }
                            if (vertex->prev->prev->IsNeighbour (vertex->prev)) {
                                newFaces.back ().flags |= io::Mesh::Face::EdgeBCVisible;
                            }
                            if (vertex->prev->IsNeighbour (vertex)) {
                                newFaces.back ().flags |= io::Mesh::Face::EdgeCAVisible;
                            }
                        }
                        else {
                            newFaces.push_back (
                                io::Mesh::Face (
                                    offset + vertex->index,
                                    offset + vertex->prev->index,
                                    offset + vertex->prev->prev->index,
                                    0));
                            if (vertex->IsNeighbour (vertex->prev)) {
                                newFaces.back ().flags |= io::Mesh::Face::EdgeABVisible;
                            }
                            if (vertex->prev->IsNeighbour (vertex->prev->prev)) {
                                newFaces.back ().flags |= io::Mesh::Face::EdgeBCVisible;
                            }
                            if (vertex->prev->prev->IsNeighbour (vertex)) {
                                newFaces.back ().flags |= io::Mesh::Face::EdgeCAVisible;
                            }
                        }
                    }
                }

                struct CompareVertices {
                    const std::vector<blas::Point2> &points;
                    explicit CompareVertices (const std::vector<blas::Point2> &points_) :
                        points (points_) {}
                    inline bool operator () (
                            const Triangulator::Vertex *vertex1,
                            const Triangulator::Vertex *vertex2) const {
                        util::i32 dx = util::COMPARE (
                            points[vertex1->index].x,
                            points[vertex2->index].x);
                        util::i32 dy = util::COMPARE (
                            points[vertex1->index].y,
                            points[vertex2->index].y);
                        return dx == 1 || (dx == 0 && dy == 1);
                    }
                };

                void Triangulator::RemoveHoles () {
                    for (Face *face = faces; face; face = face->next) {
                        if (face->inner != face->outer) {
                            std::vector<Vertex *> vertices;
                            for (Loop *loop = face->inner; loop; loop = loop->next) {
                                Vertex *vertex = loop->vertex;
                                do {
                                    vertices.push_back (vertex);
                                    vertex = vertex->next;
                                } while (vertex != loop->vertex);
                            }
                            std::sort (vertices.begin (), vertices.end (), CompareVertices (points));
                            // Connect loops
                            for (std::size_t i = 0, count = vertices.size () - 1; i < count; ++i) {
                                if (vertices[i]->loop != vertices[i + 1]->loop && !LineCrossFace (vertices[i], vertices[i + 1])) {
                                    lmekr (vertices[i], vertices[i + 1]);
                                }
                            }
                        }
                    }
                }

                void Triangulator::MakeConvex () {
                    std::vector<Face *> stack;
                    for (Face *face = faces; face; face = face->next) {
                        stack.push_back (face);
                    }
                    // Process all faces on the stack
                    while (!stack.empty ()) {
                        // Loop through all vertices of face
                        Vertex *start = stack.back ()->outer->vertex;
                        stack.pop_back ();
                        Vertex *vertex = start;
                        do {
                            if (Obtuse (vertex, points[vertex->prev->index])) {
                                Vertex *compatibleVertex = GetCompatibleVertex (vertex);
                                if (compatibleVertex) {
                                    stack.push_back (lmef (compatibleVertex, vertex));
                                    start = vertex;
                                }
                            }
                            vertex = vertex->next;
                        } while (vertex != start);
                    }
                }

                void Triangulator::TriangulateConvex () {
                    for (Face *face = faces; face; face = face->next) {
                        for (Vertex *vertex = face->outer->vertex; vertex != vertex->next->next->next;) {
                            lmef (vertex->prev->prev, vertex);
                        }
                    }
                }

                Triangulator::Vertex *Triangulator::GetCompatibleVertex (Vertex *vertex) {
                    // Get list of all points in face
                    std::vector<Vertex *> vertices;
                    for (Vertex *nextVertex = vertex->next; nextVertex != vertex; nextVertex = nextVertex->next) {
                        if (nextVertex != vertex->prev) {
                            vertices.push_back (nextVertex);
                        }
                    }
                    // Go through list until we find one that works
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        if (!Obtuse (vertex, points[vertices[i]->index]) && IsPointInFace (vertex, points[vertices[i]->index]) &&
                            IsPointInFace (vertices[i], points[vertex->index]) && !LineCrossFace (vertex, vertices[i])) {
                            return vertices[i];
                        }
                    }
                    return 0;
                }

                bool Triangulator::LineCrossFace (Vertex *vertex1, Vertex *vertex2) {
                    for (Loop *loop = vertex1->loop->face->inner; loop; loop = loop->next) {
                        Vertex *vertex = loop->vertex;
                        do {
                            if (vertex != vertex1 && vertex != vertex2 &&
                                vertex->prev != vertex1 && vertex->prev != vertex2 &&
                                blas::Line2 (points[vertex1->index], points[vertex2->index]).CrossLine (
                                    blas::Line2 (points[vertex->index], points[vertex->prev->index]))) {
                                return true;
                            }
                        } while ((vertex = vertex->prev) != loop->vertex);
                    }
                    return false;
                }

                blas::Point2 Triangulator::Inside (Vertex *vertex) {
                    blas::Point2 pt = blas::Normalize (points[vertex->prev->index] - points[vertex->index]);
                    return blas::Point2 (pt.y, -pt.x);
                }

                blas::Point2 Triangulator::Bisector (Vertex *vertex) {
                    blas::Point2 p1 = points[vertex->prev->index] - points[vertex->index];
                    blas::Point2 p2 = points[vertex->next->index] - points[vertex->index];
                    util::f32 z = blas::Cross (p1, p2).z;
                    if (!util::IS_ZERO (z)) {
                        blas::Point2 bisector = p1 + p2;
                        return util::COMPARE (z, 0.0f) > 0 ? -bisector : bisector;
                    }
                    return Inside (vertex);
                }

                inline util::f32 Cross (const blas::Point2 &p1, const blas::Point2 &p2) {
                    return p1.x * p2.y - p1.y * p2.x;
                }

                bool Triangulator::Obtuse (Vertex *vertex, const blas::Point2 &pt) {
                    blas::Point2 p1 = pt - points[vertex->index];
                    blas::Point2 p2 = points[vertex->next->index] - points[vertex->index];
                    return Cross (p1, p2) >= 0.0f;
                }

                bool Triangulator::IsPointInFace (Vertex *vertex, const blas::Point2 &pt) {
                    blas::Point2 p1 = blas::Normalize (points[vertex->prev->index] - points[vertex->index]);
                    blas::Point2 p2 = blas::Normalize (points[vertex->next->index] - points[vertex->index]);
                    blas::Point2 p3 = blas::Normalize (pt - points[vertex->index]);

                    blas::Point2 bisector = blas::Normalize (Bisector (vertex));

                    util::f32 c1 = Cross (p1, p3);
                    if (util::IS_ZERO (c1) && util::COMPARE (blas::Dot (p1, p3), 0.0f) > 0) {
                        return false;
                    }

                    util::f32 c2 = Cross (p3, bisector);
                    if (util::IS_ZERO (c2) && util::COMPARE (blas::Dot (p3, bisector), 0.0f) > 0) {
                        return true;
                    }

                    util::f32 c3 = Cross (p1, bisector);
                    if (util::COMPARE (c1 * c2, 0.0f) > 0 && util::COMPARE (c1 * c3, 0.0f) > 0) {
                        return true;
                    }

                    // p3 against Bisector-p2
                    c1 = Cross (bisector, p3);
                    if (util::IS_ZERO (c1) && util::COMPARE (blas::Dot (bisector, p3), 0.0f) > 0) {
                        return true;
                    }

                    c2 = Cross (p3, p2);
                    if (util::IS_ZERO (c2) && util::COMPARE (blas::Dot (p3, p2), 0.0f) > 0) {
                        return false;
                    }

                    c3 = Cross (bisector, p2);
                    if (util::COMPARE (c1 * c2, 0.0f) > 0 && util::COMPARE (c1 * c3, 0.0f) > 0) {
                        return true;
                    }

                    return false;
                }

                // Removes a ring from a face by combining two loops
                void Triangulator::lmekr (Vertex *vertex1, Vertex *vertex2) {
                    if (vertex2->loop == vertex2->loop->face->outer) {
                        vertex2->loop->face->outer = vertex1->loop;
                    }

                    Loop *loop = vertex2->loop;
                    Vertex *vertex = vertex2;
                    do {
                        vertex->loop = vertex1->loop;
                        vertex = vertex->next;
                    } while (vertex != vertex2);

                    loop->face->RemoveLoop (loop);

                    NewVertex (vertex1->index, vertex1);
                    NewVertex (vertex2->index, vertex2);

                    Vertex *newVertex1 = vertex1->prev;
                    Vertex *newVertex2 = vertex2->prev;

                    newVertex1->next = vertex2;
                    newVertex2->next = vertex1;
                    vertex1->prev = newVertex2;
                    vertex2->prev = newVertex1;
                }

                // Divide a face into two, creating a new face.
                Triangulator::Face *Triangulator::lmef (Vertex *vertex1, Vertex *vertex2) {
                    Face *face = NewFace (faces);
                    Loop *loop = NewLoop (face);

                    for (Vertex *vertex = vertex1; vertex != vertex2; vertex = vertex->next) {
                        vertex->loop = loop;
                    }

                    Vertex *newVertex1 = NewVertex (vertex2->index, vertex1);
                    Vertex *newVertex2 = NewVertex (vertex1->index, vertex2);

                    newVertex1->prev->next = newVertex2;
                    newVertex2->prev->next = newVertex1;
                    Vertex *temp = newVertex1->prev;
                    newVertex1->prev = newVertex2->prev;
                    newVertex2->prev = temp;

                    loop->vertex = newVertex1;
                    newVertex2->loop->vertex = newVertex2;
                    newVertex1->loop = loop;

                    return face;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
