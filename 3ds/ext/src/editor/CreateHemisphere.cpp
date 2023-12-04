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

                io::Mesh::UniquePtr Mesh::CreateHemisphere (
                        const std::string &name,
                        util::f32 radius,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        util::ui32 bands = segs / 4;
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize (segs * bands + 2);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize (segs * bands * 2);
                        util::ui32 vertexIndex = 0;
                        util::f32 halfRadius = radius * 0.5f;
                        {
                            util::f32 dphi = PI / (bands * 2.0f);
                            util::f32 phi = dphi;
                            util::f32 dtheta = 2.0f * PI / segs;
                            // top
                            vertices[vertexIndex++] = blas::Point3 (0.0f, 0.0f, halfRadius);
                            // middle
                            for (util::ui32 i = 0; i < bands; ++i, phi += dphi) {
                                util::f32 theta = 0.0f;
                                for (util::ui32 j = 0; j < segs; ++j, theta += dtheta) {
                                    vertices[vertexIndex++] = blas::Point3 (
                                        radius * sinf (phi) * cosf (theta),
                                        radius * sinf (phi) * sinf (theta),
                                        radius * cosf (phi) - halfRadius);
                                }
                            }
                            // bottom
                            vertices[vertexIndex++] = blas::Point3 (0.0f, 0.0f, -halfRadius);
                        }
                        util::ui32 faceIndex = 0;
                        {
                            // top band
                            for (util::ui32 i = 0; i < segs; ++i) {
                                faces[faceIndex++] = io::Mesh::Face (
                                    i + 1,
                                    (i + 1) % segs + 1,
                                    0,
                                    io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                            }
                        }
                        {
                            // middle bands
                            for (util::ui32 j=0 ; j<bands-1 ; j++) {
                                util::ui32 topBandStartIndex = j * segs + 1;
                                util::ui32 bottomBandStartIndex = (j + 1) * segs + 1;
                                for (util::ui32 i = 0; i < segs; ++i) {
                                    faces[faceIndex++] = io::Mesh::Face (
                                        i + bottomBandStartIndex,
                                        (i + 1) % segs + bottomBandStartIndex,
                                        i + topBandStartIndex,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible);
                                    faces[faceIndex++] = io::Mesh::Face (
                                        (i + 1) % segs + bottomBandStartIndex,
                                        (i + 1) % segs + topBandStartIndex,
                                        i + topBandStartIndex,
                                        io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible);
                                }
                            }
                        }
                        {
                            // bottom band
                            util::ui32 lastBandStartIndex = segs*(bands-1)+1;
                            for (util::ui32 i = 0; i < segs; ++i) {
                                faces[faceIndex++] = io::Mesh::Face (
                                    i + lastBandStartIndex,
                                    vertexIndex - 1,
                                    (i + 1) % segs + lastBandStartIndex,
                                    io::Mesh::Face::EdgeCAVisible);
                            }
                        }
                        if (smooth) {
                            std::vector<util::ui32> &faceSmoothGroups = mesh->faceSmoothGroups;
                            faceSmoothGroups.resize (faces.size ());
                            {
                                for (std::size_t i = 0, count = faceSmoothGroups.size () - segs; i < count; ++i) {
                                    faceSmoothGroups[i] = 1;
                                }
                            }
                            {
                                for (std::size_t i = faceSmoothGroups.size () - segs,
                                        count = faceSmoothGroups.size (); i < count; ++i) {
                                    faceSmoothGroups[i] = 2;
                                }
                            }
                        }
                        mesh->xform = blas::Matrix3::Translate (blas::Point3 (0.0f, 0.0f, halfRadius)) * xform;
                    }
                    return mesh;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
