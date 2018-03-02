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

#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/io/editor/Camera.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/opengl/editor/WireMesh.h"
#include "thekogans/mex/3ds/opengl/editor/SolidMesh.h"
#include "thekogans/mex/3ds/opengl/editor/Light.h"
#include "thekogans/mex/3ds/opengl/editor/Camera.h"
#include "thekogans/mex/3ds/opengl/editor/Editor.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Editor)

                Editor::Editor (const ext::Editor &editor) :
                    Module (editor) {
                    {
                        // FIXME: get this from io::Project
                        std::vector<std::string> materialMapPaths;
                        // Build materials list to be used during rendering.
                        typedef util::OwnerVector<io::Material>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().materials.begin (),
                                end = ioEditor ().materials.end (); it != end; ++it) {
                            Material::UniquePtr material (new Material (**it, materialMapPaths));
                            std::pair<util::OwnerMap<std::string, Material>::iterator, bool> result =
                                materials.insert (
                                    util::OwnerMap<std::string, Material>::value_type (
                                        (*it)->name, material.get ()));
                            if (result.second) {
                                material.release ();
                            }
                            // Silently ignore duplicate material names.
                        }
                    }
                    {
                        typedef util::OwnerVector<io::Mesh>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().meshes.begin (),
                                end = ioEditor ().meshes.end (); it != end; ++it) {
                            {
                                WireMesh::UniquePtr wireMesh (
                                    new WireMesh (**it, ioProject ()));
                                assert (wireMesh.get () != 0);
                                wireMeshes.push_back (wireMesh.get ());
                                wireMesh.release ();
                            }
                            {
                                SolidMesh::UniquePtr solidMesh (
                                    new SolidMesh (**it, ioProject (), materials));
                                assert (solidMesh.get () != 0);
                                solidMeshes.push_back (solidMesh.get ());
                                solidMesh.release ();
                            }
                        }
                    }
                    {
                        typedef util::OwnerVector<io::Light>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().lights.begin (),
                                end = ioEditor ().lights.end (); it != end; ++it) {
                            Light::UniquePtr light (new Light (**it));
                            assert (light.get () != 0);
                            lights.push_back (light.get ());
                            light.release ();
                        }
                    }
                    {
                        typedef util::OwnerVector<io::Camera>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().cameras.begin (),
                                end = ioEditor ().cameras.end (); it != end; ++it) {
                            Camera::UniquePtr camera (new Camera (**it));
                            assert (camera.get () != 0);
                            cameras.push_back (camera.get ());
                            camera.release ();
                        }
                    }
                }

                View::UniquePtr Editor::GetView (const io::View &view) const {
                    if (view.IsNone () || view.IsOrthographic ()) {
                        return View::UniquePtr (new View (view));
                    }
                    else if (view.IsSpotLight ()) {
                        const io::Light *light = ioEditor ().GetLight (view.name.value);
                        if (light != 0) {
                            assert (light->spot.get () != 0);
                            return View::UniquePtr (new View (view, *light));
                        }
                    }
                    else if (view.IsCamera ()) {
                        const io::Camera *camera = ioEditor ().GetCamera (view.name.value);
                        if (camera != 0) {
                            return View::UniquePtr (new View (view, *camera));
                        }
                    }
                    assert (0);
                    return View::UniquePtr ();
                }

                View::UniquePtr Editor::GetCurrView () const {
                    return GetView (ioEditor ().viewLayout.GetCurrView ());
                }

                void Editor::Draw (const View &view) const {
                    // FIXME:
                    /*
                    if (context.drawType != Context::Wire) {
                        if (ioEditor ().lights.empty () && context.IsUseDefaultLight ()) {
                            // FIXME: create a default light
                            assert (0);
                        }
                        else {
                            SetupLights (ioEditor ().ambientLight, ioEditor ().lights);
                        }
                    }
                    */
                    // FIXME: add decorations (tape, grids...)
                    if (view.view.IsSolid ()) {
                        typedef util::OwnerVector<SolidMesh>::const_iterator const_iterator;
                        for (const_iterator it = solidMeshes.begin (), end = solidMeshes.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                    else {
                        typedef util::OwnerVector<WireMesh>::const_iterator const_iterator;
                        for (const_iterator it = wireMeshes.begin (), end = wireMeshes.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                    if (ioProject ().display.showLights) {
                        typedef util::OwnerVector<Light>::const_iterator const_iterator;
                        for (const_iterator it = lights.begin (), end = lights.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                    if (ioProject ().display.showCameras) {
                        typedef util::OwnerVector<Camera>::const_iterator const_iterator;
                        for (const_iterator it = cameras.begin (), end = cameras.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                }

                void Editor::Swap (Editor &editor) {
                    materials.swap (editor.materials);
                    wireMeshes.swap (editor.wireMeshes);
                    solidMeshes.swap (editor.solidMeshes);
                    lights.swap (editor.lights);
                    cameras.swap (editor.cameras);
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
