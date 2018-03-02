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

                io::Mesh::UniquePtr Mesh::CreateTorus (
                        const std::string &name,
                        util::f32 outRadius,
                        util::f32 inRadius,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize (segs * bands);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize (segs * bands * 2);
                        util::f32 dtheta1 = 2.0f * util::PI / bands;
                        util::f32 dtheta2 = 2.0f * util::PI / segs;
                        {
                            // vertices
                            util::f32 theta1 = 0.0f;
                            util::f32 theta2 = 0.0f;
                            for (util::ui32 i = 0, vertexIndex = 0; i < segs; ++i, theta2 += dtheta2) {
                                for (util::ui32 j = 0; j < bands; ++j, theta1 += dtheta1) {
                                    vertices[vertexIndex++] = blas::Point3 (
                                        (outRadius + inRadius * cosf (theta1)) * cosf (theta2),
                                        (outRadius + inRadius * cosf (theta1)) * sinf (theta2),
                                        inRadius * sinf (theta1));
                                }
                            }
                        }
                        {
                            // faces
                            util::f32 theta1 = 0.0f;
                            util::f32 theta2 = 0.0f;
                            for (util::ui32 i = 0, faceIndex = 0; i < segs; ++i, theta2 += dtheta2) {
                               for (util::ui32 j = 0; j < bands; ++j, theta1 += dtheta1) {
                                    util::ui32 t1 = i * bands + j;
                                    util::ui32 t2 = i * bands + (j + 1) % bands;
                                    util::ui32 t3 = (i + 1) % segs * bands + j;
                                    util::ui32 t4 = (i + 1) % segs * bands + (j + 1) % bands;
                                    faces[faceIndex++] = io::Mesh::Face (t1, t3, t2,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                    faces[faceIndex++] = io::Mesh::Face (t2, t3, t4,
                                        io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                }
                            }
                        }
                        if (smooth) {
                            std::vector<util::ui32> &faceSmoothGroups = mesh->faceSmoothGroups;
                            faceSmoothGroups.resize (faces.size ());
                            for (std::size_t i = 0, count = faceSmoothGroups.size (); i < count; ++i) {
                                faceSmoothGroups[i] = 1;
                            }
                        }
                        mesh->xform = xform;
                    }
                    return mesh;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
