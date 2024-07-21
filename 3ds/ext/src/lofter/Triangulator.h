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

#if !defined (__thekogans_mex_3ds_ext_Triangulator_h)
#define __thekogans_mex_3ds_ext_Triangulator_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct Triangulator {
                    struct Vertex;
                    struct Loop;
                    struct Face;

                    std::vector<blas::Point2> points;

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Vertex {
                        typedef std::unique_ptr<Vertex> UniquePtr;

                        THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                        std::size_t index;
                        Loop *loop;
                        Vertex *prev;
                        Vertex *next;
                        Vertex *neighbour;

                        Vertex (std::size_t index_, Loop *loop_) :
                                index (index_), loop (loop_), neighbour (loop->vertex) {
                            if (!loop->vertex) {
                                next = prev = this;
                            }
                            else {
                                prev = loop->vertex;
                                next = loop->vertex->next;
                                loop->vertex->next = loop->vertex->next->prev = this;
                            }
                            loop->vertex = this;
                        }

                        Vertex (std::size_t index_, Vertex *next_) : index (index_),
                            loop (next_->loop), prev (next_->prev), next (next_), neighbour (0) {
                            next->prev = next->prev->next = this;
                        }

                        inline bool IsNeighbour (Vertex *vertex) {
                            return neighbour == vertex || vertex->neighbour == this;
                        }
                    };
                    util::OwnerVector<Vertex> newVertices;

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Loop {
                        typedef std::unique_ptr<Loop> UniquePtr;

                        THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                        Face *face;
                        Loop *prev;
                        Loop *next;
                        Vertex *vertex;

                        Loop (Face *face_) : face (face_), prev (0), next (face->inner), vertex (0) {
                            if (face->inner) {
                                face->inner->prev = this;
                            }
                            face->inner = this;
                            if (!face->outer) {
                                face->outer = this;
                            }
                        }
                    };
                    util::OwnerVector<Loop> newLoops;

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Face {
                        typedef std::unique_ptr<Face> UniquePtr;

                        THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                        Face *prev;
                        Face *next;
                        Loop *outer;
                        Loop *inner;

                        Face (Face *&next_) : prev (0), next (next_), outer (0), inner (0) {
                            if (next) {
                                next->prev = this;
                            }
                            next_ = this;
                        }

                        void RemoveLoop (Loop *loop) {
                            if (outer == loop) {
                                outer = 0;
                            }
                            if (inner == loop) {
                                inner = loop->next;
                            }
                            if (loop->next) {
                                loop->next->prev = loop->prev;
                            }
                            if (loop->prev) {
                                loop->prev->next = loop->next;
                            }
                        }
                    } *faces;
                    util::OwnerVector<Face> newFaces;

                    explicit Triangulator (const Lofter::Shape &shape);

                    void GetFaces (
                        std::size_t offset,
                        bool flipNormal,
                        std::vector<io::Mesh::Face> &faces) const;

                private:
                    inline Vertex *NewVertex (std::size_t index, Loop *loop) {
                        Vertex::UniquePtr vertex (new Vertex (index, loop));
                        newVertices.push_back (vertex.get ());
                        return vertex.release ();
                    }
                    inline Vertex *NewVertex (std::size_t index, Vertex *next) {
                        Vertex::UniquePtr vertex (new Vertex (index, next));
                        newVertices.push_back (vertex.get ());
                        return vertex.release ();
                    }
                    inline Loop *NewLoop (Face *face) {
                        Loop::UniquePtr loop (new Loop (face));
                        newLoops.push_back (loop.get ());
                        return loop.release ();
                    }
                    inline Face *NewFace (Face *&next) {
                        Face::UniquePtr face (new Face (next));
                        newFaces.push_back (face.get ());
                        return face.release ();
                    }
                    void RemoveHoles ();
                    void MakeConvex ();
                    void TriangulateConvex ();
                    Vertex *GetCompatibleVertex (Vertex *vertex);
                    bool LineCrossFace (Vertex *vertex1, Vertex *vertex2);
                    blas::Point2 Inside (Vertex *vertex);
                    blas::Point2 Bisector (Vertex *vertex);
                    bool Obtuse (Vertex *vertex, const blas::Point2 &pt);
                    bool IsPointInFace (Vertex *vertex, const blas::Point2 &pt);
                    void lmekr (Vertex *vertex1, Vertex *vertex2);
                    Face *lmef (Vertex *vertex1, Vertex *vertex2);
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_Triangulator_h)
