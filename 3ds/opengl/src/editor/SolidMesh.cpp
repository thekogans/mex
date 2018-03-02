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

#include <cassert>
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ShadeModel.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/DepthFunc.h"
#include "thekogans/mex/opengl/CullFace.h"
#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/opengl/LineStipple.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/editor/SolidMesh.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (SolidMesh)

                SolidMesh::SolidMesh (const io::Mesh &mesh, const io::Project &project,
                    const util::OwnerMap<std::string, Material> &materials) : Mesh (mesh, project) {
                    BuildElements (materials);
                }

                void SolidMesh::Draw (const View &) const {
                    if (!mesh.IsHidden ()) {
                        ShadeModel shadeModel (GL_SMOOTH);
                        Enable lighting (GL_LIGHTING, true);
                        Enable depthTest(GL_DEPTH_TEST, true);
                        DepthFunc depthFunc (GL_LEQUAL);
                        Enable enableCullFace (GL_CULL_FACE, !project.display.seeThrough);
                        CullFace cullFace (GL_BACK);
                        Enable texture2D (GL_TEXTURE_2D, true);
                        typedef util::OwnerVector<Element>::const_iterator const_iterator;
                        for (const_iterator it = elements.begin (), end = elements.end (); it != end; ++it) {
                            (*it)->Draw ();
                        }
                    }
                }

                void SolidMesh::BuildElements (const util::OwnerMap<std::string, Material> &materials) {
                    util::OwnerVector<Element> tempElements;
                    ext::Mesh::Normals normals (mesh);
                    normals.Build ();
                    std::vector<std::size_t> facesWithMaterialsMap;
                    facesWithMaterialsMap.insert (facesWithMaterialsMap.begin (), mesh.faces.size (), 0);
                    {
                        const util::OwnerVector<io::Mesh::Material> &meshMaterials = mesh.materials;
                        for (std::size_t i = 0, meshMaterialCount = meshMaterials.size ();
                                i < meshMaterialCount; ++i) {
                            util::OwnerMap<std::string, Material>::const_iterator material =
                                materials.find (meshMaterials[i]->name);
                            if (material != materials.end ()) {
                                assert (material->second != 0);
                                const std::vector<util::ui16> &meshMaterialFaces = meshMaterials[i]->faces;
                                Element::UniquePtr element (new Element (mesh, meshMaterialFaces, *material->second));
                                if (!element->faces.empty ()) {
                                    assert (element.get () != 0);
                                    element->UpdateVerticesAndNormals (mesh.vertices, normals);
                                    tempElements.push_back (element.get ());
                                    element.release ();
                                    for (util::ui16 j = 0, count = meshMaterialFaces.size (); j < count; ++j) {
                                        facesWithMaterialsMap[meshMaterialFaces[j]] = 1;
                                    }
                                }
                            }
                        }
                    }
                    {
                        // Unassigned faces will all be lumped in with default material.
                        std::vector<util::ui16> defaultMaterialFaces;
                        for (util::ui16 i = 0, count = facesWithMaterialsMap.size (); i < count; ++i) {
                            if (facesWithMaterialsMap[i] == 0) {
                                defaultMaterialFaces.push_back (i);
                            }
                        }
                        if (!defaultMaterialFaces.empty ()) {
                            Element::UniquePtr element (
                                new Element (mesh, defaultMaterialFaces, defaultMaterial));
                            assert (element.get () != 0);
                            element->UpdateVerticesAndNormals (mesh.vertices, normals);
                            tempElements.push_back (element.get ());
                            element.release ();
                        }
                    }
                    tempElements.swap (elements);
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
