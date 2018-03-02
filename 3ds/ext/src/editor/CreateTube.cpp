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

                io::Mesh::UniquePtr Mesh::CreateTube (
                        const std::string &name,
                        util::f32 inRadius,
                        util::f32 outRadius,
                        util::f32 height,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        util::ui32 bands = segs / 2;
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize ((segs + 1) * bands * 2);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize ((segs + 1) * bands * 4);
                        {
                            util::f32 dtheta = 2.0f * util::PI / bands;
                            util::f32 dheight = height / segs;
                            height *= 0.5f;
                            for (util::ui32 i = 0, vertexIndex = 0; i <= segs; ++i, height -= dheight) {
                                util::f32 theta = 0.0f;
                                for (util::ui32 j = 0; j < bands; ++j, theta += dtheta) {
                                    util::f32 cs = cosf (theta);
                                    util::f32 sn = sinf (theta);
                                    vertices[vertexIndex++] = blas::Point3 (
                                        inRadius * cs, inRadius * sn, height);
                                    vertices[vertexIndex++] = blas::Point3 (
                                        outRadius * cs, outRadius * sn, height);
                                }
                            }
                            height += dheight;
                        }
                        util::ui32 faceIndex = 0;
                        {
                            for (util::ui32 i = 0; i < bands; ++i) {
                                util::ui32 t1 = i * 2;
                                util::ui32 t2 = (i + 1) % bands * 2;
                                faces[faceIndex++] = io::Mesh::Face (t1, t1 + 1, t2,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (t2, t1 + 1, t2 + 1,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                            }
                        }
                        {
                            for (util::ui32 j = 0; j < segs; ++j) {
                                for (util::ui32 i = 0; i < bands; ++i) {
                                    util::ui32 t1 = j * bands * 2 + i * 2;
                                    util::ui32 t2 = j * bands * 2 + (i + 1) % bands * 2;
                                    util::ui32 t3 = (j + 1) * bands * 2 + i * 2;
                                    util::ui32 t4 = (j + 1) * bands * 2 +(i + 1) % bands * 2;
                                    faces[faceIndex++] = io::Mesh::Face (t1 + 1, t3 + 1, t2 + 1,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                    faces[faceIndex++] = io::Mesh::Face (t2 + 1, t3 + 1, t4 + 1,
                                        io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                    faces[faceIndex++] = io::Mesh::Face (t2, t4, t1,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                    faces[faceIndex++] = io::Mesh::Face (t1, t4, t3,
                                        io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                }
                            }
                        }
                        {
                            for (util::ui32 i = 0; i < bands; ++i) {
                                util::ui32 t1 = segs * bands * 2 + i * 2;
                                util::ui32 t2 = segs * bands * 2 + (i + 1) % bands * 2;
                                faces[faceIndex++] = io::Mesh::Face (t1 + 1, t1, t2 + 1,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                faces[faceIndex++] = io::Mesh::Face (t2 + 1, t1, t2,
                                    io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                            }
                        }
                        if (smooth) {
                            std::vector<util::ui32> &faceSmoothGroups = mesh->faceSmoothGroups;
                            faceSmoothGroups.resize (faces.size ());
                            {
                                for (util::ui32 i = 0, count = bands * 2; i < count; ++i) {
                                    faceSmoothGroups[i] = 1;
                                }
                            }
                            {
                                for (std::size_t i = bands * 2, count = faces.size () - bands * 2; i < count; ++i) {
                                    faceSmoothGroups[i] = 2;
                                }
                            }
                            {
                                for (std::size_t i = faces.size () - bands * 2, count = faces.size (); i < count; ++i) {
                                    faceSmoothGroups[i] = 1;
                                }
                            }
                        }
                        mesh->xform = blas::Matrix3::Translate (blas::Point3 (0.0f, 0.0f, -height)) * xform;
                    }
                    return mesh;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
