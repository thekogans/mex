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

#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                void Mesh::SelectionInfo::Build () {
                    vertexCount[0] = vertexCount[1] = vertexCount[2] = 0;
                    const std::vector<util::ui16> &vertexFlags = mesh.vertexFlags;
                    if (!vertexFlags.empty ()) {
                        assert (vertexFlags.size () == mesh.vertices.size ());
                        for (std::size_t i = 0, count = vertexFlags.size (); i < count; ++i) {
                            switch (vertexFlags[i] & (io::Mesh::SelectionSetA | io::Mesh::SelectionSetB | io::Mesh::SelectionSetC)) {
                                case io::Mesh::SelectionSetA | io::Mesh::SelectionSetB | io::Mesh::SelectionSetC:
                                    ++vertexCount[0];
                                    ++vertexCount[1];
                                    ++vertexCount[2];
                                    break;
                                case io::Mesh::SelectionSetA | io::Mesh::SelectionSetB:
                                    ++vertexCount[0];
                                    ++vertexCount[1];
                                    break;
                                case io::Mesh::SelectionSetA | io::Mesh::SelectionSetC:
                                    ++vertexCount[0];
                                    ++vertexCount[2];
                                    break;
                                case io::Mesh::SelectionSetB | io::Mesh::SelectionSetC:
                                    ++vertexCount[1];
                                    ++vertexCount[2];
                                    break;
                                case io::Mesh::SelectionSetA:
                                    ++vertexCount[0];
                                    break;
                                case io::Mesh::SelectionSetB:
                                    ++vertexCount[1];
                                    break;
                                case io::Mesh::SelectionSetC:
                                    ++vertexCount[2];
                                    break;
                            }
                        }
                    }
                    faceCount[0] = faceCount[1] = faceCount[2] = 0;
                    const std::vector<io::Mesh::Face> &faces = mesh.faces;
                    for (std::size_t i = 0, count = faces.size (); i < count; ++i) {
                        switch (faces[i].flags &
                                (io::Mesh::Face::SelectionSetA |
                                    io::Mesh::Face::SelectionSetB |
                                    io::Mesh::Face::SelectionSetC)) {
                            case io::Mesh::Face::SelectionSetA | io::Mesh::Face::SelectionSetB | io::Mesh::Face::SelectionSetC:
                                ++faceCount[0];
                                ++faceCount[1];
                                ++faceCount[2];
                                break;
                            case io::Mesh::Face::SelectionSetA | io::Mesh::Face::SelectionSetB:
                                ++faceCount[0];
                                ++faceCount[1];
                                break;
                            case io::Mesh::Face::SelectionSetA | io::Mesh::Face::SelectionSetC:
                                ++faceCount[0];
                                ++faceCount[2];
                                break;
                            case io::Mesh::Face::SelectionSetB | io::Mesh::Face::SelectionSetC:
                                ++faceCount[1];
                                ++faceCount[2];
                                break;
                            case io::Mesh::Face::SelectionSetA:
                                ++faceCount[0];
                                break;
                            case io::Mesh::Face::SelectionSetB:
                                ++faceCount[1];
                                break;
                            case io::Mesh::Face::SelectionSetC:
                                ++faceCount[2];
                                break;
                        }
                    }
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
