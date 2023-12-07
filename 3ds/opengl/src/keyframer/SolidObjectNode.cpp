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

#include <numeric>
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ShadeModel.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/DepthFunc.h"
#include "thekogans/mex/opengl/CullFace.h"
#include "thekogans/mex/opengl/LineStipple.h"
#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/keyframer/SolidObjectNode.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (SolidObjectNode)

                SolidObjectNode::SolidObjectNode (
                        const ext::ObjectNode &objectNode,
                        const io::Project &project,
                        const util::OwnerMap<std::string, Material> &materials) :
                        ObjectNode (objectNode, project) {
                    BuildElements (materials);
                }

                void SolidObjectNode::SetCurrentFrame () {
                    if (!objectNode.hidden) {
                        ext::Mesh::Normals normals (objectNode.mesh, objectNode.vertices);
                        normals.Build ();
                        typedef util::OwnerVector<Element>::const_iterator const_iterator;
                        for (const_iterator it = elements.begin (),
                                end = elements.end (); it != end; ++it) {
                            (*it)->UpdateVerticesAndNormals (objectNode.vertices, normals);
                        }
                    }
                }

                void SolidObjectNode::Draw (const View &) const {
                    if (!objectNode.hidden) {
                        ShadeModel shadeModel (GL_SMOOTH);
                        Enable lighting (GL_LIGHTING, true);
                        Enable depthTest (GL_DEPTH_TEST, true);
                        DepthFunc depthFunc (GL_LEQUAL);
                        Enable enableCullFace (GL_CULL_FACE, !project.display.kfSeeThrough);
                        CullFace cullFace (GL_BACK);
                        Enable texture2D (GL_TEXTURE_2D, true);
                        typedef util::OwnerVector<Element>::const_iterator const_iterator;
                        for (const_iterator it = elements.begin (),
                                end = elements.end (); it != end; ++it) {
                            (*it)->Draw ();
                        }
                    }
                }

                void SolidObjectNode::BuildElements (
                        const util::OwnerMap<std::string, Material> &materials) {
                    util::OwnerVector<Element> tempElements;
                    const io::Mesh &mesh = objectNode.mesh;
                    std::vector<std::size_t> facesWithMaterialsMap;
                    facesWithMaterialsMap.insert (
                        facesWithMaterialsMap.begin (),
                        mesh.faces.size (),
                        0);
                    {
                        const std::vector<io::Mesh::Material> &meshMaterials = mesh.materials;
                        for (std::size_t i = 0, meshMaterialCount = meshMaterials.size ();
                                i < meshMaterialCount; ++i) {
                            util::OwnerMap<std::string, Material>::const_iterator material =
                                materials.find (meshMaterials[i].name);
                            if (material != materials.end ()) {
                                assert (material->second != 0);
                                const std::vector<util::ui16> &meshMaterialFaces =
                                    meshMaterials[i].faces;
                                Element::UniquePtr element (
                                    new Element (mesh, meshMaterialFaces, *material->second));
                                if (!element->faces.empty ()) {
                                    assert (element.get () != 0);
                                    tempElements.push_back (element.get ());
                                    element.release ();
                                    for (util::ui16 j = 0, count = meshMaterialFaces.size ();
                                            j < count; ++j) {
                                        facesWithMaterialsMap[meshMaterialFaces[j]] = 1;
                                    }
                                }
                            }
                        }
                    }
                    {
                        // Unassigned faces will all be lumped in to default material.
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
