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

#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS_EX (
                    Mesh::Normals::VertexNormal,
                    util::SpinLock,
                    100000,
                    util::DefaultAllocator::Instance ().Get ())

                // Add a normal to the list if the smoothing group bits overlap,
                // otherwise create a new vertex normal in the list
                void Mesh::Normals::VertexNormal::AddNormal (
                        const blas::Point3 &normal_,
                        util::ui32 smooth_) {
                    assert (smooth_ != 0);
                    for (VertexNormal *vertexNormal = this;
                            vertexNormal != 0; vertexNormal = vertexNormal->next) {
                        if (vertexNormal->smooth & smooth_) {
                            vertexNormal->smooth |= smooth_;
                            vertexNormal->normal += normal_;
                            return;
                        }
                        else if (vertexNormal->next == 0) {
                            vertexNormal->next = new VertexNormal (normal_, smooth_);
                            return;
                        }
                    }
                }

                // Retrieves a normal if the smoothing groups overlap.
                const blas::Point3 &Mesh::Normals::VertexNormal::GetNormal (util::ui32 smooth_) const {
                    assert (smooth_ != 0);
                    for (const VertexNormal *vertexNormal = this;
                            vertexNormal != 0; vertexNormal = vertexNormal->next) {
                        if (vertexNormal->smooth & smooth_) {
                            return normal;
                        }
                    }
                    // FIXME: throw something meaningful.
                    assert (0);
                    return blas::Point3::Zero;
                }

                // Normalize each normal in the list
                void Mesh::Normals::VertexNormal::Normalize () {
                    for (VertexNormal *prev = this, *curr = next; curr; curr = prev->next) {
                        if (smooth & curr->smooth) {
                            normal += curr->normal;
                            smooth |= curr->smooth;
                            prev->next = curr->next;
                            curr->next = 0;
                            delete curr;
                        }
                        else {
                            prev = curr;
                        }
                    }
                    normal.Normalize ();
                    if (next) {
                        next->Normalize ();
                    }
                }

                void Mesh::Normals::Build () {
                    vertexNormals.deleteAndClear ();
                    faceNormals.resize (mesh.faces.size ());
                    {
                        class CreateFaceNormalsJob : public util::Vectorizer::Job {
                        private:
                            std::vector<blas::Point3> &faceNormals;
                            const std::vector<blas::Point3> &vertices;
                            const std::vector<io::Mesh::Face> &faces;

                        public:
                            CreateFaceNormalsJob (
                                std::vector<blas::Point3> &faceNormals_,
                                const std::vector<blas::Point3> &vertices_,
                                const std::vector<io::Mesh::Face> &faces_) :
                                faceNormals (faceNormals_),
                                vertices (vertices_),
                                faces (faces_) {}

                            virtual void Execute (
                                    std::size_t sidx,
                                    std::size_t eidx,
                                    std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    const io::Mesh::Face &face = faces[sidx];
                                    faceNormals[sidx] = blas::Normalize (
                                        blas::Cross (
                                            vertices[face.b] - vertices[face.a],
                                            vertices[face.c] - vertices[face.a]));
                                }
                            }
                            virtual std::size_t Size () const throw () {
                                return faces.size ();
                            }
                        } job (faceNormals, vertices, mesh.faces);
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                    if (!mesh.faceSmoothGroups.empty ()) {
                        assert (mesh.faceSmoothGroups.size () == mesh.faces.size ());
                        vertexNormals.insert (vertexNormals.begin (), vertices.size (), 0);
                        {
                            for (std::size_t i = 0, count = mesh.faces.size (); i < count; ++i) {
                                util::ui32 smooth = mesh.faceSmoothGroups[i];
                                if (smooth) {
                                    const io::Mesh::Face &face = mesh.faces[i];
                                    if (vertexNormals[face.a] != 0) {
                                        vertexNormals[face.a]->AddNormal (faceNormals[i], smooth);
                                    }
                                    else {
                                        vertexNormals[face.a] = new VertexNormal (faceNormals[i], smooth);
                                    }
                                    if (vertexNormals[face.b] != 0) {
                                        vertexNormals[face.b]->AddNormal (faceNormals[i], smooth);
                                    }
                                    else {
                                        vertexNormals[face.b] = new VertexNormal (faceNormals[i], smooth);
                                    }
                                    if (vertexNormals[face.c] != 0) {
                                        vertexNormals[face.c]->AddNormal (faceNormals[i], smooth);
                                    }
                                    else {
                                        vertexNormals[face.c] = new VertexNormal (faceNormals[i], smooth);
                                    }
                                }
                            }
                        }
                        {
                            bool oneNormalPerVertex = true;
                            {
                                class NormalizeVertexNormalsJob : public util::Vectorizer::Job {
                                private:
                                    util::OwnerVector<VertexNormal> &vertexNormals;
                                    bool &oneNormalPerVertex;

                                public:
                                    NormalizeVertexNormalsJob (
                                        util::OwnerVector<VertexNormal> &vertexNormals_,
                                        bool &oneNormalPerVertex_) :
                                        vertexNormals (vertexNormals_),
                                        oneNormalPerVertex (oneNormalPerVertex_) {}

                                    virtual void Execute (
                                            std::size_t sidx,
                                            std::size_t eidx,
                                            std::size_t rank) throw () {
                                        for (; sidx < eidx; ++sidx) {
                                            if (vertexNormals[sidx] != 0) {
                                                vertexNormals[sidx]->Normalize ();
                                                if (vertexNormals[sidx]->next != 0) {
                                                    oneNormalPerVertex = false;
                                                }
                                            }
                                            else {
                                                oneNormalPerVertex = false;
                                            }
                                        }
                                    }
                                    virtual std::size_t Size () const throw () {
                                        return vertexNormals.size ();
                                    }
                                } job (vertexNormals, oneNormalPerVertex);
                                util::GlobalVectorizer::Instance ()->Execute (job);
                            }
                            if (!vertexNormals.empty () && oneNormalPerVertex) {
                                vertexNormals2.resize (vertexNormals.size ());
                                class CopyVertexNormalsJob : public util::Vectorizer::Job {
                                private:
                                    std::vector<blas::Point3> &vertexNormals2;
                                    const util::OwnerVector<VertexNormal> &vertexNormals;

                                public:
                                    CopyVertexNormalsJob (
                                        std::vector<blas::Point3> &vertexNormals2_,
                                        const util::OwnerVector<VertexNormal> &vertexNormals_) :
                                        vertexNormals2 (vertexNormals2_),
                                        vertexNormals (vertexNormals_) {}

                                    virtual void Execute (
                                            std::size_t sidx,
                                            std::size_t eidx,
                                            std::size_t rank) throw () {
                                        for (; sidx < eidx; ++sidx) {
                                            vertexNormals2[sidx] = vertexNormals[sidx]->normal;
                                        }
                                    }
                                    virtual std::size_t Size () const throw () {
                                        return vertexNormals.size ();
                                    }
                                } job (vertexNormals2, vertexNormals);
                                util::GlobalVectorizer::Instance ()->Execute (job);
                                vertexNormals.deleteAndClear ();
                            }
                        }
                    }
                }

                void Mesh::Normals::GetFaceVertexNormals (
                        std::size_t faceIndex,
                        blas::Point3 &a,
                        blas::Point3 &b,
                        blas::Point3 &c) const {
                    if (!vertexNormals2.empty ()) {
                        assert (faceIndex < mesh.faces.size ());
                        const io::Mesh::Face &face = mesh.faces[faceIndex];
                        assert (face.a < vertexNormals2.size ());
                        assert (face.b < vertexNormals2.size ());
                        assert (face.c < vertexNormals2.size ());
                        a = vertexNormals2[face.a];
                        b = vertexNormals2[face.b];
                        c = vertexNormals2[face.c];
                    }
                    else {
                        util::ui32 smooth = 0;
                        if (!mesh.faceSmoothGroups.empty ()) {
                            assert (faceIndex < mesh.faceSmoothGroups.size ());
                            smooth = mesh.faceSmoothGroups[faceIndex];
                        }
                        if (smooth != 0) {
                            assert (faceIndex < mesh.faces.size ());
                            const io::Mesh::Face &face = mesh.faces[faceIndex];
                            assert (vertexNormals[face.a]);
                            assert (vertexNormals[face.b]);
                            assert (vertexNormals[face.c]);
                            a = vertexNormals[face.a]->GetNormal (smooth);
                            b = vertexNormals[face.b]->GetNormal (smooth);
                            c = vertexNormals[face.c]->GetNormal (smooth);
                        }
                        else {
                            assert (faceIndex < faceNormals.size ());
                            a = faceNormals[faceIndex];
                            b = faceNormals[faceIndex];
                            c = faceNormals[faceIndex];
                        }
                    }
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
