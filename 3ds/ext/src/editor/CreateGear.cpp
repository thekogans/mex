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

                io::Mesh::UniquePtr Mesh::CreateGear (
                        const std::string &name,
                        util::f32 radius,
                        util::f32 thickness,
                        util::f32 height,
                        util::ui32 teeth,
                        util::f32 taper,
                        const blas::Matrix3 &xform) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize (teeth * 8);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize (teeth * 12);
                        blas::Point3 box[8];
                        util::f32 dtheta = TWOPI / teeth;
                        util::f32 hheight = height * 0.5f;
                        {
                            util::f32 qdtheta = dtheta * 0.25f;
                            {
                                box[0] = blas::Point3 (radius * cosf (qdtheta), radius * sinf (qdtheta), hheight);
                                box[1] = blas::Point3 (radius * cosf (qdtheta), radius * sinf (qdtheta), -hheight);
                                box[2] = blas::Point3 (radius * cosf (-qdtheta), radius * sinf (-qdtheta), -hheight);
                                box[3] = blas::Point3 (radius * cosf (-qdtheta), radius * sinf (-qdtheta), hheight);
                                blas::Matrix3 m1 = blas::Matrix3::Scale (blas::Point3 (1.0f, 2.0f - taper, 1.0f));
                                for (util::ui32 i = 0; i < 4; ++i) {
                                    box[i] *= m1;
                                }
                            }
                            {
                                box[4] = blas::Point3 (radius * cosf (qdtheta) + thickness, radius * sinf (qdtheta), hheight);
                                box[5] = blas::Point3 (radius * cosf (qdtheta) + thickness, radius * sinf (qdtheta), -hheight);
                                box[6] = blas::Point3 (radius * cosf (-qdtheta) + thickness, radius * sinf (-qdtheta), -hheight);
                                box[7] = blas::Point3 (radius * cosf (-qdtheta) + thickness, radius * sinf (-qdtheta), hheight);
                                blas::Matrix3 m1 = blas::Matrix3::Scale (blas::Point3 (1.0f, taper, taper));
                                for (util::ui32 i = 4; i < 8; ++i) {
                                    box[i] *= m1;
                                }
                            }
                        }
                        {
                            util::f32 theta = 0.0f;
                            for (util::ui32 i = 0, vertexIndex = 0, faceIndex = 0; i < teeth; ++i, theta += dtheta) {
                                blas::Matrix3 m1 = blas::Matrix3::RotateZ (theta);
                                for (util::ui32 j = 0; j < 8; ++j) {
                                    vertices[vertexIndex + j] = box[j] * m1;
                                }
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 0, vertexIndex + 5, vertexIndex + 1,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 0, vertexIndex + 4, vertexIndex + 5,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 1, vertexIndex + 6, vertexIndex + 2,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 1, vertexIndex + 5, vertexIndex + 6,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 2, vertexIndex + 7, vertexIndex + 3,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 2, vertexIndex + 6, vertexIndex + 7,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 3, vertexIndex + 4, vertexIndex + 0,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 3, vertexIndex + 7, vertexIndex + 4,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 0, vertexIndex + 2, vertexIndex + 3,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 4, vertexIndex + 6, vertexIndex + 5,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (vertexIndex + 4, vertexIndex + 7, vertexIndex + 6,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                vertexIndex += 8;
                            }
                        }
                        mesh->xform = blas::Matrix3::Translate (blas::Point3 (0.0f, 0.0f, hheight)) * xform;
                    }
                    return mesh;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
