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

#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/3ds/opengl/editor/Element.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Element)

                Element::Element (
                        const io::Mesh &mesh,
                        const std::vector<util::ui16> &faces,
                        const Material &material_) :
                        ext::Mesh::Element (mesh, faces),
                        material (material_) {
                    if (!mesh.textureVertices.empty ()) {
                        textureVertices.resize (faces.size () * 3);
                        class CopyTextureVertices : public util::Vectorizer::Job {
                        private:
                            Element &element;

                        public:
                            CopyTextureVertices (Element &element_) :
                                element (element_) {}

                            virtual void Execute (
                                    std::size_t sidx,
                                    std::size_t eidx,
                                    std::size_t rank) throw () {
                                blas::Point2 *textureVerticesPtr =
                                    &element.textureVertices[sidx * 3];
                                const io::Mesh &mesh = element.mesh;
                                for (; sidx < eidx; ++sidx) {
                                    const io::Mesh::Face &face = mesh.faces[element.faces[sidx]];
                                    *textureVerticesPtr++ = mesh.textureVertices[face.a];
                                    *textureVerticesPtr++ = mesh.textureVertices[face.b];
                                    *textureVerticesPtr++ = mesh.textureVertices[face.c];
                                }
                            }
                            virtual std::size_t Size () const throw () {
                                return element.faces.size ();
                            }
                        } job (*this);
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                }

                void Element::UpdateVerticesAndNormals (
                        const std::vector<blas::Point3> &vertices,
                        const ext::Mesh::Normals &normals,
                        bool flat) {
                    class UpdateVerticesAndNormalsJob : public util::Vectorizer::Job {
                    private:
                        Element &element;
                        const std::vector<blas::Point3> &vertices;
                        const ext::Mesh::Normals &normals;
                        bool flat;

                    public:
                        UpdateVerticesAndNormalsJob (
                                Element &element_,
                                const std::vector<blas::Point3> &vertices_,
                                const ext::Mesh::Normals &normals_,
                                bool flat_) :
                                element (element_),
                                vertices (vertices_),
                                normals (normals_),
                                flat (flat_) {
                            element.vertices.resize (element.faces.size () * 3);
                            element.normals.resize (element.faces.size () * 3);
                        }

                        virtual void Execute (
                                std::size_t sidx,
                                std::size_t eidx,
                                std::size_t rank) throw () {
                            blas::Point3 *elementVertices = &element.vertices[sidx * 3];
                            blas::Point3 *elementNormals = &element.normals[sidx * 3];
                            const std::vector<io::Mesh::Face> &faces = element.mesh.faces;
                            const std::vector<util::ui16> &elementFaces = element.faces;
                            if (flat) {
                                for (; sidx < eidx; ++sidx) {
                                    const io::Mesh::Face &face = faces[elementFaces[sidx]];
                                    *elementVertices++ = vertices[face.a];
                                    *elementVertices++ = vertices[face.b];
                                    *elementVertices++ = vertices[face.c];
                                    *elementNormals++ = normals.GetFaceNormal (elementFaces[sidx]);
                                    *elementNormals++ = normals.GetFaceNormal (elementFaces[sidx]);
                                    *elementNormals++ = normals.GetFaceNormal (elementFaces[sidx]);
                                }
                            }
                            else {
                                for (; sidx < eidx; ++sidx) {
                                    const io::Mesh::Face &face = faces[elementFaces[sidx]];
                                    *elementVertices++ = vertices[face.a];
                                    *elementVertices++ = vertices[face.b];
                                    *elementVertices++ = vertices[face.c];
                                    normals.GetFaceVertexNormals (elementFaces[sidx],
                                        *(elementNormals + 0), *(elementNormals + 1), *(elementNormals + 2));
                                    elementNormals += 3;
                                }
                            }
                        }
                        virtual std::size_t Size () const throw () {
                            return element.faces.size ();
                        }
                    } job (*this, vertices, normals, flat);
                    util::GlobalVectorizer::Instance ()->Execute (job);
                }

                void Element::Draw () const {
                    material.Setup ();
                    if (!textureVertices.empty ()) {
                        assert (vertices.size () == normals.size ());
                        assert (vertices.size () == textureVertices.size ());
                        ClientState clientState (
                            ClientState::VERTEX_ARRAY |
                            ClientState::NORMAL_ARRAY |
                            ClientState::TEXTURE_COORD_ARRAY);
                        glVertexPointer (3, GL_FLOAT, 0, &vertices[0]);
                        glNormalPointer (GL_FLOAT, 0, &normals[0]);
                        glTexCoordPointer (2, GL_FLOAT, 0, &textureVertices[0]);
                        glDrawArrays (GL_TRIANGLES, 0, vertices.size ());
                    }
                    else {
                        ClientState clientState (
                            ClientState::VERTEX_ARRAY |
                            ClientState::NORMAL_ARRAY);
                        glVertexPointer (3, GL_FLOAT, 0, &vertices[0]);
                        glNormalPointer (GL_FLOAT, 0, &normals[0]);
                        glDrawArrays (GL_TRIANGLES, 0, vertices.size ());
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
