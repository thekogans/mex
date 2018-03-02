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

#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                io::Mesh::UniquePtr Mesh::CreateBox (
                        const std::string &name,
                        util::f32 length,
                        util::f32 width,
                        util::f32 height,
                        const blas::Matrix3 &xform) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize (8);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize (12);
                        util::f32 halfLength = length * 0.5f;
                        util::f32 halfWidth = width * 0.5f;
                        util::f32 halfHeight = height * 0.5f;
                        vertices[0] = blas::Point3 (-halfLength, -halfWidth, -halfHeight);
                        vertices[1] = blas::Point3 (halfLength, -halfWidth, -halfHeight);
                        vertices[2] = blas::Point3 (halfLength, -halfWidth, halfHeight);
                        vertices[3] = blas::Point3 (-halfLength, -halfWidth, halfHeight);
                        vertices[4] = blas::Point3 (-halfLength, halfWidth, -halfHeight);
                        vertices[5] = blas::Point3 (halfLength, halfWidth, -halfHeight);
                        vertices[6] = blas::Point3 (halfLength, halfWidth, halfHeight);
                        vertices[7] = blas::Point3 (-halfLength, halfWidth, halfHeight);
                        faces[0] = io::Mesh::Face (0, 1, 2, io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                        faces[1] = io::Mesh::Face (0, 2, 3, io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                        faces[2] = io::Mesh::Face (0, 4, 5, io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                        faces[3] = io::Mesh::Face (0, 5, 1, io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                        faces[4] = io::Mesh::Face (1, 5, 6, io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                        faces[5] = io::Mesh::Face (1, 6, 2, io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                        faces[6] = io::Mesh::Face (2, 6, 7, io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                        faces[7] = io::Mesh::Face (2, 7, 3, io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                        faces[8] = io::Mesh::Face (3, 7, 4, io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                        faces[9] = io::Mesh::Face (3, 4, 0, io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                        faces[10] = io::Mesh::Face (4, 7, 6, io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                        faces[11] = io::Mesh::Face (4, 6, 5, io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                        mesh->xform = blas::Matrix3::Translate (blas::Point3 (0.0f, 0.0f, halfHeight)) * xform;
                    }
                    return mesh;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
