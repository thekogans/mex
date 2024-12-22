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
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                namespace {
                    struct CompareEdges {
                        inline bool operator () (
                                const Mesh::Edge &edge1,
                                const Mesh::Edge &edge2) {
                            std::size_t v1 = std::min (edge1.v1, edge1.v2);
                            std::size_t v2 = std::min (edge2.v1, edge2.v2);
                            return v1 < v2 || (v1 == v2 &&
                                std::max (edge1.v1, edge1.v2) < std::max (edge2.v1, edge2.v2));
                        }
                    };

                    inline bool operator == (
                            const Mesh::Edge &edge1,
                            const Mesh::Edge &edge2) {
                        return
                            std::min (edge1.v1, edge1.v2) == std::min (edge2.v1, edge2.v2) &&
                            std::max (edge1.v1, edge1.v2) == std::max (edge2.v1, edge2.v2);
                    }
                }

                void Mesh::BuildEdges (std::vector<Edge> &edges) const {
                    std::vector<Edge> tempEdges1;
                    {
                        // Step 1, Create edges.
                        class CreateEdgesJob : public util::Vectorizer::Job {
                        private:
                            std::vector<Edge> &edges;
                            const std::vector<io::Mesh::Face> &faces;
                        public:
                            CreateEdgesJob (
                                    std::vector<Edge> &edges_,
                                    const std::vector<io::Mesh::Face> &faces_) :
                                    edges (edges_),
                                    faces (faces_) {
                                edges.resize (faces.size () * 3);
                            }
                            virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
                                for (std::size_t i = sidx * 3; sidx < eidx; ++sidx) {
                                    const io::Mesh::Face &face = faces[sidx];
                                    // FIXME: A good optimizing compiler should elide
                                    // the assignment operator. Make sure.
                                    edges[i++] = Mesh::Edge (face.a, face.b, sidx, util::NIDX16, face.IsEdgeABVisible () ? Mesh::Edge::Visible : 0);
                                    edges[i++] = Mesh::Edge (face.b, face.c, sidx, util::NIDX16, face.IsEdgeBCVisible () ? Mesh::Edge::Visible : 0);
                                    edges[i++] = Mesh::Edge (face.c, face.a, sidx, util::NIDX16, face.IsEdgeCAVisible () ? Mesh::Edge::Visible : 0);
                                }
                            }
                            virtual std::size_t Size () const throw () {
                                return faces.size ();
                            }
                        };
                        CreateEdgesJob job (tempEdges1, mesh.faces);
                        util::Vectorizer::Instance ()->Execute (job);
                    }
                    if (!tempEdges1.empty ()) {
                        std::vector<Edge> tempEdges2;
                        tempEdges2.reserve (tempEdges1.size ());
                        // Step 2, Sort edges.
                        // FIXME: Investigate vectorized sort.
                        std::sort (tempEdges1.begin (), tempEdges1.end (), CompareEdges ());
                        // Step 3, Merge like edges.
                        std::size_t i, count;
                        for (i = 0, count = tempEdges1.size () - 1; i < count; ++i) {
                            if (tempEdges1[i] == tempEdges1[i + 1]) {
                                // Adjacent edges are alike. Merge them.
                                tempEdges1[i].f2 = tempEdges1[i + 1].f1;
                                if (!tempEdges1[i].IsVisible ()) {
                                    tempEdges1[i].SetVisible (tempEdges1[i + 1].IsVisible ());
                                }
                                tempEdges2.push_back (tempEdges1[i]);
                                // Skip over the merged edge.
                                ++i;
                            }
                            else {
                                tempEdges2.push_back (tempEdges1[i]);
                            }
                        }
                        // Add the last unpaired edge.
                        if (i < tempEdges1.size ()) {
                            tempEdges2.push_back (tempEdges1[i]);
                        }
                        tempEdges2.resize (tempEdges2.size ());
                        edges.swap (tempEdges2);
                    }
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
