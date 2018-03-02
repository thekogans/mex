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

#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraNode.h"
#include "thekogans/mex/3ds/opengl/Tape.h"
#include "thekogans/mex/3ds/opengl/editor/Light.h"
#include "thekogans/mex/3ds/opengl/keyframer/WireObjectNode.h"
#include "thekogans/mex/3ds/opengl/keyframer/SolidObjectNode.h"
#include "thekogans/mex/3ds/opengl/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Keyframer)

                Keyframer::Keyframer (const ext::Keyframer &keyframer) : Module (keyframer) {
                    {
                        // FIXME: get this from io::Project
                        std::vector<std::string> materialMapPaths;
                        // Build materials list to be used during rendering.
                        typedef util::OwnerVector<io::Material>::const_iterator const_iterator;
                        for (const_iterator it = ioProject ().editor.materials.begin (),
                            end = ioProject ().editor.materials.end (); it != end; ++it) {
                            Material::UniquePtr material (new Material (**it, materialMapPaths));
                            std::pair<util::OwnerMap<std::string, Material>::iterator, bool> result =
                                materials.insert (util::OwnerMap<std::string, Material>::value_type (
                                    (*it)->name, material.get ()));
                            if (result.second) {
                                material.release ();
                            }
                            // Silently ignore duplicate material names.
                        }
                    }
                    {
                        typedef util::OwnerVector<ext::ObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = extKeyframer ().objectNodes.begin (),
                                end = extKeyframer ().objectNodes.end (); it != end; ++it) {
                            {
                                WireObjectNode::UniquePtr wireObjectNode (
                                    new WireObjectNode (**it, ioProject ()));
                                assert (wireObjectNode.get ());
                                wireObjectNodes.push_back (wireObjectNode.get ());
                                wireObjectNode.release ();
                            }
                            {
                                SolidObjectNode::UniquePtr solidObjectNode (new SolidObjectNode (
                                    **it, ioProject (), materials));
                                assert (solidObjectNode.get ());
                                solidObjectNodes.push_back (solidObjectNode.get ());
                                solidObjectNode.release ();
                            }
                        }
                    }
                    {
                        typedef util::OwnerVector<ext::LightNode>::const_iterator const_iterator;
                        for (const_iterator it = extKeyframer ().lightNodes.begin (),
                                end = extKeyframer ().lightNodes.end (); it != end; ++it) {
                            LightNode::UniquePtr lightNode (
                                new LightNode (**it, ioProject ()));
                            assert (lightNode.get () != 0);
                            lightNodes.push_back (lightNode.get ());
                            lightNode.release ();
                        }
                    }
                    {
                        assert (extKeyframer ().spotLightHeadNodes.size () ==
                            extKeyframer ().spotLightTargetNodes.size ());
                        for (std::size_t i = 0, count = extKeyframer ().spotLightHeadNodes.size ();
                                i < count; ++i) {
                            assert (extKeyframer ().spotLightHeadNodes[i]->light.name ==
                                extKeyframer ().spotLightTargetNodes[i]->light.name);
                            SpotLightNode::UniquePtr spotLightNode (
                                new SpotLightNode (
                                    *extKeyframer ().spotLightHeadNodes[i],
                                    *extKeyframer ().spotLightTargetNodes[i],
                                    ioProject ()));
                            assert (spotLightNode.get () != 0);
                            spotLightNodes.push_back (spotLightNode.get ());
                            spotLightNode.release ();
                        }
                    }
                    {
                        assert (extKeyframer ().cameraHeadNodes.size () ==
                            extKeyframer ().cameraTargetNodes.size ());
                        for (std::size_t i = 0, count = extKeyframer ().cameraHeadNodes.size ();
                                i < count; ++i) {
                            assert (extKeyframer ().cameraHeadNodes[i]->camera.name ==
                                extKeyframer ().cameraTargetNodes[i]->camera.name);
                            CameraNode::UniquePtr cameraNode (
                                new CameraNode (
                                    *extKeyframer ().cameraHeadNodes[i],
                                    *extKeyframer ().cameraTargetNodes[i],
                                    ioProject ()));
                            assert (cameraNode.get () != 0);
                            cameraNodes.push_back (cameraNode.get ());
                            cameraNode.release ();
                        }
                    }
                }

                View::UniquePtr Keyframer::GetView (const io::View &view) const {
                    if (view.IsNone () || view.IsOrthographic ()) {
                        return View::UniquePtr (new View (view));
                    }
                    else if (view.IsSpotLight ()) {
                        io::Light::UniquePtr light (GetLight (view.name.value));
                        if (light.get () != 0) {
                            assert (light->spot.get () != 0);
                            return View::UniquePtr (new View (view, *light));
                        }
                    }
                    else if (view.IsCamera ()) {
                        io::Camera::UniquePtr camera (GetCamera (view.name.value));
                        if (camera.get () != 0) {
                            return View::UniquePtr (new View (view, *camera));
                        }
                    }
                    assert (0);
                    return View::UniquePtr ();
                }

                View::UniquePtr Keyframer::GetCurrView () const {
                    return GetView (ioKeyframer ().viewLayout.GetCurrView ());
                }

                void Keyframer::Draw (const View &view) const {
                    // FIXME:
                    /*
                    if (context.drawType != Context::Wire) {
                        io::Color ambientLight;
                        util::OwnerVector<io::Light> lights;
                        GetLights (ambientLight, lights);
                        if (lights.empty () && context.IsUseDefaultLight ()) {
                            // FIXME: create a default light
                            assert (0);
                        }
                        SetupLights (ambientLight, lights);
                    }
                    */
                    /*
                    if (constructionPlane.IsVisible ()) {
                        ConstructionPlane (constructionPlane).Draw (view);
                    }
                    if (tape.IsVisible ()) {
                        Tape3 (tape).Draw (view);
                    }
                    */
                    if (view.view.IsSolid ()) {
                        typedef util::OwnerVector<SolidObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = solidObjectNodes.begin (),
                                end = solidObjectNodes.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                    else {
                        typedef util::OwnerVector<WireObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = wireObjectNodes.begin (),
                                end = wireObjectNodes.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                    if (ioProject ().display.showLights) {
                        {
                            typedef util::OwnerVector<LightNode>::const_iterator const_iterator;
                            for (const_iterator it = lightNodes.begin (),
                                    end = lightNodes.end (); it != end; ++it) {
                                (*it)->Draw (view);
                            }
                        }
                        {
                            typedef util::OwnerVector<SpotLightNode>::const_iterator const_iterator;
                            for (const_iterator it = spotLightNodes.begin (),
                                    end = spotLightNodes.end (); it != end; ++it) {
                                (*it)->Draw (view);
                            }
                        }
                    }
                    if (ioProject ().display.showCameras) {
                        typedef util::OwnerVector<CameraNode>::const_iterator const_iterator;
                        for (const_iterator it = cameraNodes.begin (),
                                end = cameraNodes.end (); it != end; ++it) {
                            (*it)->Draw (view);
                        }
                    }
                }

                void Keyframer::SetCurrentFrame () {
                    {
                        typedef util::OwnerVector<WireObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = wireObjectNodes.begin (),
                                end = wireObjectNodes.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame ();
                        }
                    }
                    {
                        typedef util::OwnerVector<SolidObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = solidObjectNodes.begin (),
                                end = solidObjectNodes.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame ();
                        }
                    }
                }

                io::Light::UniquePtr Keyframer::GetLight (const std::string &name) const {
                    typedef util::OwnerVector<SpotLightNode>::const_iterator const_iterator;
                    for (const_iterator it = spotLightNodes.begin (),
                            end = spotLightNodes.end (); it != end; ++it) {
                        if ((*it)->spotLightHeadNode.light.name == name) {
                            return ext::SpotLightNode (
                                (*it)->spotLightHeadNode,
                                (*it)->spotLightTargetNode).Snapshot ();
                        }
                    }
                    return io::Light::UniquePtr ();
                }

                void Keyframer::GetLights (
                        io::Color &ambientLight,
                        util::OwnerVector<io::Light> &lights) const {
                    ambientLight = extKeyframer ().ambientNode->Snapshot ();
                    {
                        typedef util::OwnerVector<LightNode>::const_iterator const_iterator;
                        for (const_iterator it = lightNodes.begin (),
                                end = lightNodes.end (); it != end; ++it) {
                            io::Light::UniquePtr light = (*it)->lightNode.Snapshot ();
                            assert (light.get () != 0);
                            lights.push_back (light.get ());
                            light.release ();
                        }
                    }
                    {
                        typedef util::OwnerVector<SpotLightNode>::const_iterator const_iterator;
                        for (const_iterator it = spotLightNodes.begin (),
                                end = spotLightNodes.end (); it != end; ++it) {
                            io::Light::UniquePtr light = ext::SpotLightNode (
                                (*it)->spotLightHeadNode,
                                (*it)->spotLightTargetNode).Snapshot ();
                            assert (light.get () != 0);
                            lights.push_back (light.get ());
                            light.release ();
                        }
                    }
                }

                io::Camera::UniquePtr Keyframer::GetCamera (const std::string &name) const {
                    typedef util::OwnerVector<CameraNode>::const_iterator const_iterator;
                    for (const_iterator it = cameraNodes.begin (),
                            end = cameraNodes.end (); it != end; ++it) {
                        if ((*it)->cameraHeadNode.camera.name == name) {
                            return ext::CameraNode (
                                (*it)->cameraHeadNode,
                                (*it)->cameraTargetNode).Snapshot ();
                        }
                    }
                    return io::Camera::UniquePtr ();
                }

                void Keyframer::Swap (Keyframer &keyframer) {
                    materials.swap (keyframer.materials);
                    wireObjectNodes.swap (keyframer.wireObjectNodes);
                    solidObjectNodes.swap (keyframer.solidObjectNodes);
                    lightNodes.swap (keyframer.lightNodes);
                    spotLightNodes.swap (keyframer.spotLightNodes);
                    cameraNodes.swap (keyframer.cameraNodes);
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
