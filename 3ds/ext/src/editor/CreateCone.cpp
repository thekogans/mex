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

                io::Mesh::UniquePtr Mesh::CreateCone (
                        const std::string &name,
                        util::f32 loRadius,
                        util::f32 highRadius,
                        util::f32 height,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize ((segs + 1) * bands + 2);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize ((segs + 1) * bands * 2);
                        util::ui32 vertexIndex = 0;
                        {
                            util::f32 dtheta = 2.0f * util::PI / bands;
                            util::f32 dheight = height / segs;
                            util::f32 dradius = (loRadius - highRadius) / segs;
                            util::f32 radius = highRadius;
                            height *= 0.5f;
                            vertices[vertexIndex++] = blas::Point3 (0.0f, 0.0f, height);
                            for (util::ui32 i = 0; i <= segs; ++i, height -= dheight, radius += dradius) {
                                util::f32 theta = 0.0f;
                                for (util::ui32 j = 0; j < bands; ++j, theta += dtheta) {
                                    vertices[vertexIndex++] = blas::Point3 (
                                        radius * (util::f32)cos (theta), radius * (util::f32)sin (theta), height);
                                }
                            }
                            height += dheight;
                            vertices[vertexIndex++] = blas::Point3 (0.0f, 0.0f, height);
                        }
                        util::ui32 faceIndex = 0;
                        {
                            for (util::ui32 i = 0; i < bands; ++i) {
                                faces[faceIndex++] = io::Mesh::Face (
                                    i + 1,
                                    (i + 1) % bands + 1,
                                    0,
                                    io::Mesh::Face::EdgeABVisible);
                            }
                        }
                        {
                            for (util::ui32 j = 0; j < segs; ++j) {
                                util::ui32 topBandStartIndex = j * bands + 1;
                                util::ui32 bottomBandStartIndex = (j + 1) * bands + 1;
                                for (util::ui32 i = 0; i < bands; ++i) {
                                    faces[faceIndex++] = io::Mesh::Face (
                                        i + bottomBandStartIndex,
                                        (i + 1) % bands + bottomBandStartIndex,
                                        i + topBandStartIndex,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                    faces[faceIndex++] = io::Mesh::Face (
                                        (i + 1) % bands + bottomBandStartIndex,
                                        (i + 1) % bands + topBandStartIndex,
                                        i + topBandStartIndex,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                }
                            }
                        }
                        {
                            for (util::ui32 i = 0, lastBandStartIndex = bands * segs + 1; i < bands; ++i) {
                               faces[faceIndex++] = io::Mesh::Face (
                                   i + lastBandStartIndex,
                                   vertexIndex - 1,
                                   (i + 1) % bands + lastBandStartIndex,
                                   io::Mesh::Face::EdgeCAVisible);
                            }
                        }
                        if (smooth) {
                            std::vector<util::ui32> &faceSmoothGroups = mesh->faceSmoothGroups;
                            faceSmoothGroups.resize (faces.size ());
                            {
                                for (util::ui32 i = 0; i < bands; ++i) {
                                    faceSmoothGroups[i] = 1;
                                }
                            }
                            {
                                for (std::size_t i = bands, count = faces.size () - bands; i < count; ++i) {
                                    faceSmoothGroups[i] = 2;
                                }
                            }
                            {
                                for (std::size_t i = faces.size () - bands, count = faces.size (); i < count; ++i) {
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
