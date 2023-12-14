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

#include <algorithm>
#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                void Mesh::Element::Build (util::ui16 fidx, bool hidden) {
                    std::vector<util::ui16> elements;
                    elements.insert (elements.begin (), mesh.vertices.size (), 0);
                    std::vector<util::ui16> vertices;
                    vertices.insert (vertices.begin (), mesh.vertices.size (), util::NIDX16);
                    {
                        for (std::size_t i = 0, count = mesh.faces.size (), eidx = 0; i < count; ++i) {
                            const io::Mesh::Face &face = mesh.faces[i];
                            util::ui16 a = face.a;
                            util::ui16 b = face.b;
                            util::ui16 c = face.c;
                            util::ui16 element = std::min (std::min (vertices[a], vertices[b]), vertices[c]);
                            if (element == util::NIDX16) {
                                vertices[a] = (util::ui16)eidx;
                                vertices[b] = (util::ui16)eidx;
                                vertices[c] = (util::ui16)eidx;
                                elements[eidx] = (util::ui16)eidx;
                                ++eidx;
                            }
                            else {
                                if (vertices[a] == util::NIDX16) {
                                    vertices[a] = element;
                                }
                                else {
                                    elements[vertices[a]] = elements[element];
                                }
                                if (vertices[b] == util::NIDX16) {
                                    vertices[b] = element;
                                }
                                else {
                                    elements[vertices[b]] = elements[element];
                                }
                                if (vertices[c] == util::NIDX16) {
                                    vertices[c] = element;
                                }
                                else {
                                    elements[vertices[c]] = elements[element];
                                }
                            }
                        }
                    }
                    std::vector<util::ui16> faces_;
                    {
                        for (std::size_t i = 0, count = mesh.faces.size (), element = elements[vertices[mesh.faces[fidx].a]]; i < count; ++i) {
                            const io::Mesh::Face &face = mesh.faces[i];
                            if ((hidden || !face.IsHidden ()) &&
                                    (elements[vertices[face.a]] == element ||
                                     elements[vertices[face.b]] == element ||
                                     elements[vertices[face.c]] == element)) {
                                faces_.push_back ((util::ui16)i);
                            }
                        }
                    }
                    faces.swap (faces_);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
