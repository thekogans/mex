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

                namespace {
                    const blas::Point3 points[] = {
                        blas::Point3 (      0.0f,       0.0f,       1.0f),
                        blas::Point3 (-0.471404f, -0.816497f, -0.333333f),
                        blas::Point3 ( 0.942809f,       0.0f, -0.333333f),
                        blas::Point3 (-0.471404f,  0.816497f, -0.333333f),
                    };
                    const util::ui32 edgeVertices[][2] = {
                        {0, 1},
                        {0, 2},
                        {0, 3},
                        {1, 2},
                        {2, 3},
                        {3, 1}
                    };
                    const util::ui32 faceVertices[][3] = {
                        {0, 1, 2},
                        {0, 2, 3},
                        {0, 3, 1},
                        {1, 3, 2}
                    };
                    const util::ui32 faceEdges[][3] = {
                        {0, 1, 3},
                        {1, 2, 4},
                        {2, 0, 5},
                        {5, 3, 4}
                    };
                }

                io::Mesh::UniquePtr Mesh::CreateGSphere (
                        const std::string &name,
                        util::f32 radius,
                        util::ui32 nsub,
                        const blas::Matrix3 &xform,
                        bool smooth) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.resize (4 + 6 * nsub + 4 * nsub * (nsub - 1) / 2);
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.resize (4 * (nsub + 1) * (nsub + 1));
                        util::ui32 vertexIndex = 0;
                        vertices[vertexIndex++] = blas::Normalize (points[0]) * radius;
                        vertices[vertexIndex++] = blas::Normalize (points[1]) * radius;
                        vertices[vertexIndex++] = blas::Normalize (points[2]) * radius;
                        vertices[vertexIndex++] = blas::Normalize (points[3]) * radius;
                        // subdivide edges
                        util::ui32 eidxarray[] = {0, 0, 0, 0, 0, 0};
                        util::ui32 e4idx = 0;
                        util::ui32 e5idx = 0;
                        {
                            for (util::ui32 i = 0; i < 6; ++i) {
                                eidxarray[i] = vertexIndex;
                                blas::Point3 v0 = blas::Normalize (points[edgeVertices[i][0]]) * radius;
                                blas::Point3 v1 = blas::Normalize (points[edgeVertices[i][1]]) * radius;
                                blas::Matrix3 xform = blas::Matrix3::FromAngleAxis (blas::AngleAxis (
                                    -blas::Angle (v0, v1) / (nsub + 1), blas::Cross (v0, v1)));
                                for (util::ui32 j = 0; j < nsub; ++j) {
                                    vertices[vertexIndex++] = v0 *= xform;
                                }
                                if (i == 4) {
                                    e4idx = vertexIndex - 1;
                                }
                                if (i == 5) {
                                    e5idx = vertexIndex - 1;
                                }
                            }
                        }
                        std::vector<std::vector<util::ui32> > rows;
                        {
                            rows.resize (nsub + 2);
                            for (util::ui32 i = 0; i < nsub + 2; ++i) {
                                rows[i].resize (i + 1);
                            }
                        }
                        {
                            for (util::ui32 i = 0; i < 4; ++i) {
                                // subdivide triangle
                                if (nsub > 1) {
                                    blas::Point3 v0 = blas::Normalize (points[faceVertices[i][0]]) * radius;
                                    blas::Point3 v1 = blas::Normalize (points[faceVertices[i][1]]) * radius;
                                    blas::Point3 v2 = blas::Normalize (points[faceVertices[i][2]]) * radius;
                                    blas::Matrix3 m1 = blas::Matrix3::FromAngleAxis (blas::AngleAxis (
                                        -blas::Angle (v0, v1) / (nsub + 1), blas::Cross (v0, v1)));
                                    blas::Matrix3 m2 = blas::Matrix3::FromAngleAxis (blas::AngleAxis (
                                        -blas::Angle (v0, v2) / (nsub + 1), blas::Cross (v0, v2)));
                                    blas::Point3 v01 = v0 * m1;
                                    blas::Point3 v02 = v0 * m2;
                                    for (util::ui32 j = 1; j < nsub; ++j) {
                                        v01 *= m1;
                                        v02 *= m2;
                                        blas::Matrix3 m3 = blas::Matrix3::FromAngleAxis (blas::AngleAxis (
                                            -blas::Angle (v01, v02) / (j + 1), blas::Cross (v01, v02)));
                                        blas::Point3 v0102 = v01;
                                        for (util::ui32 k = 0; k < j; ++k) {
                                            rows[j + 1][k + 1] = vertexIndex;
                                            vertices[vertexIndex++] = v0102 *= m3;
                                        }
                                    }
                                }
                                {
                                    // create triangle vertex map
                                    rows[0][0] = faceVertices[i][0];
                                    rows[nsub + 1][0] = faceVertices[i][1];
                                    rows[nsub + 1][nsub + 1] = faceVertices[i][2];
                                    util::ui32 eidx1 = eidxarray[faceEdges[i][0]];
                                    util::ui32 eidx2 = eidxarray[faceEdges[i][1]];
                                    util::ui32 eidx3 = eidxarray[faceEdges[i][2]];
                                    for (util::ui32 j = 1; j < nsub + 1; ++j) {
                                        if (i == 3) {
                                            rows[j][0] = e5idx--;
                                            rows[j][j] = eidx2++;
                                            rows[nsub+1][j] = e4idx--;
                                        }
                                        else {
                                            rows[j][0] = eidx1++;
                                            rows[j][j] = eidx2++;
                                            rows[nsub+1][j] = eidx3++;
                                        }
                                    }
                                }
                                {
                                    // create faces
                                    util::ui32 faceIndex = 0;
                                    for (util::ui32 j = 1; j < nsub + 2; ++j) {
                                        for (util::ui32 k = 0; k < j - 1; ++k) {
                                            faces[faceIndex++] = io::Mesh::Face (
                                                rows[j - 1][k],
                                                rows[j][k],
                                                rows[j][k + 1],
                                                io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                            faces[faceIndex++] = io::Mesh::Face (
                                                rows[j - 1][k],
                                                rows[j][k + 1],
                                                rows[j - 1][k + 1],
                                                io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                        }
                                        faces[faceIndex++] = io::Mesh::Face (
                                            rows[j - 1][j - 1],
                                            rows[j][j - 1],
                                            rows[j][j],
                                            io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible | io::Mesh::Face::EdgeCAVisible);
                                    }
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
