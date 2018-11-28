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

#include <map>
#include <set>
#include <string>
#include <iostream>
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/ext/keyframer/CameraNode.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                namespace {
                    const io::Mesh dummyMesh (io::ObjectNode::DUMMY_NAME);

                    void DumpHierarchy (const Node &node, util::ui32 level) {
                        for (util::ui32 i = 0; i < level; ++i) {
                            std::cout << "  ";
                        }
                        std::cout << node.node.GetName () << "\n";
                        {
                            typedef std::vector<Node *>::const_iterator const_iterator;
                            for (const_iterator it = node.children.begin (), end = node.children.end (); it != end; ++it) {
                                DumpHierarchy (**it, level + 1);
                            }
                        }
                    }
                }

                // *** IMPORTANT ***
                // This ctor does NOT validate the io::Keyframer data.
                // *****************
                Keyframer::Keyframer (const io::Keyframer &keyframer) :
                        Module (keyframer),
                        ambientNode (new ext::AmbientNode (keyframer.ambientNode, keyframer.segment)) {
                    typedef std::pair<Node *, util::ui16> NodeIdPair;
                    typedef std::map<util::ui16, NodeIdPair> NodesMap;
                    NodesMap nodes;
                    // Create anim nodes.
                    {
                        typedef util::OwnerVector<io::ObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = keyframer.objectNodes.begin (), end = keyframer.objectNodes.end (); it != end; ++it) {
                            const io::Mesh *mesh = keyframer.project.editor.GetMesh ((*it)->name);
                            if (mesh == 0) {
                                assert ((*it)->name == io::ObjectNode::DUMMY_NAME);
                                mesh = &dummyMesh;
                            }
                            ObjectNode::UniquePtr objectNode (new ObjectNode (
                                *mesh, **it, keyframer.project.editor.meshes,
                                keyframer.segment));
                            assert (objectNode.get () != 0);
                            std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                NodesMap::value_type ((*it)->id,
                                NodeIdPair (objectNode.get (), (*it)->parent)));
                            assert (result.second);
                            if ((*it)->parent == util::NIDX16) {
                                objectNodeRoots.push_back (objectNode.get ());
                            }
                            objectNodes.push_back (objectNode.get ());
                            objectNode.release ();
                        }

                    }
                    {
                        typedef util::OwnerVector<io::LightNode>::const_iterator const_iterator;
                        for (const_iterator it = keyframer.lightNodes.begin (),
                                end = keyframer.lightNodes.end (); it != end; ++it) {
                            LightNode::UniquePtr lightNode (new LightNode (
                                *keyframer.project.editor.GetLight ((*it)->name),
                                **it, keyframer.segment));
                            assert (lightNode.get () != 0);
                            std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                NodesMap::value_type ((*it)->id,
                                NodeIdPair (lightNode.get (), (*it)->parent)));
                            assert (result.second);
                            if ((*it)->parent == util::NIDX16) {
                                lightNodeRoots.push_back (lightNode.get ());
                            }
                            lightNodes.push_back (lightNode.get ());
                            lightNode.release ();
                        }
                    }
                    {
                        {
                            typedef util::OwnerVector<io::TargetNode>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.spotLightTargetNodes.begin (),
                                    end = keyframer.spotLightTargetNodes.end (); it != end; ++it) {
                                SpotLightTargetNode::UniquePtr spotLightTargetNode (
                                    new SpotLightTargetNode (*keyframer.project.editor.GetLight ((*it)->name),
                                    **it, keyframer.segment));
                                assert (spotLightTargetNode.get () != 0);
                                std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                    NodesMap::value_type ((*it)->id,
                                    NodeIdPair (spotLightTargetNode.get (), (*it)->parent)));
                                assert (result.second);
                                if ((*it)->parent == util::NIDX16) {
                                    spotLightTargetNodeRoots.push_back (spotLightTargetNode.get ());
                                }
                                spotLightTargetNodes.push_back (spotLightTargetNode.get ());
                                spotLightTargetNode.release ();
                            }
                        }
                        {
                            typedef util::OwnerVector<io::SpotLightNode>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.spotLightNodes.begin (),
                                    end = keyframer.spotLightNodes.end (); it != end; ++it) {
                                SpotLightHeadNode::UniquePtr spotLightHeadNode (new SpotLightHeadNode (
                                    *keyframer.project.editor.GetLight ((*it)->name),
                                    **it, keyframer.segment));
                                assert (spotLightHeadNode.get () != 0);
                                std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                    NodesMap::value_type ((*it)->id, NodeIdPair (spotLightHeadNode.get (), (*it)->parent)));
                                assert (result.second);
                                if ((*it)->parent == util::NIDX16) {
                                    spotLightHeadNodeRoots.push_back (spotLightHeadNode.get ());
                                }
                                spotLightHeadNodes.push_back (spotLightHeadNode.get ());
                                spotLightHeadNode.release ();
                            }
                        }
                    }
                    {
                        {
                            typedef util::OwnerVector<io::TargetNode>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.cameraTargetNodes.begin (),
                                    end = keyframer.cameraTargetNodes.end (); it != end; ++it) {
                                CameraTargetNode::UniquePtr cameraTargetNode (
                                    new CameraTargetNode (*keyframer.project.editor.GetCamera ((*it)->name),
                                    **it, keyframer.segment));
                                assert (cameraTargetNode.get () != 0);
                                std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                    NodesMap::value_type ((*it)->id, NodeIdPair (cameraTargetNode.get (), (*it)->parent)));
                                assert (result.second);
                                if ((*it)->parent == util::NIDX16) {
                                    cameraTargetNodeRoots.push_back (cameraTargetNode.get ());
                                }
                                cameraTargetNodes.push_back (cameraTargetNode.get ());
                                cameraTargetNode.release ();
                            }
                        }
                        {
                            typedef util::OwnerVector<io::CameraNode>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.cameraNodes.begin (),
                                    end = keyframer.cameraNodes.end (); it != end; ++it) {
                                CameraHeadNode::UniquePtr cameraHeadNode (new CameraHeadNode (
                                    *keyframer.project.editor.GetCamera ((*it)->name),
                                    **it, keyframer.segment));
                                assert (cameraHeadNode.get () != 0);
                                std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                    NodesMap::value_type ((*it)->id, NodeIdPair (cameraHeadNode.get (), (*it)->parent)));
                                assert (result.second);
                                if ((*it)->parent == util::NIDX16) {
                                    cameraHeadNodeRoots.push_back (cameraHeadNode.get ());
                                }
                                cameraHeadNodes.push_back (cameraHeadNode.get ());
                                cameraHeadNode.release ();
                            }
                        }
                    }
                    // Link them in a hierarchy.
                    {
                        for (NodesMap::const_iterator it = nodes.begin (), end = nodes.end (); it != end; ++it) {
                            if (it->second.second != util::NIDX16) {
                                NodesMap::iterator parent = nodes.find (it->second.second);
                                assert (parent != nodes.end ());
                                parent->second.first->AddChild (it->second.first);
                            }
                        }
                    }
                #if defined (THEKOGANS_MEX_3DS_EXT_CONFIG_Debug)
                    {
                        typedef std::vector<ObjectNode *>::const_iterator const_iterator;
                        for (const_iterator it = objectNodeRoots.begin (), end = objectNodeRoots.end (); it != end; ++it) {
                            DumpHierarchy (**it, 0);
                        }
                    }
                #endif // defined (THEKOGANS_MEX_3DS_EXT_CONFIG_Debug)
                }

                View::UniquePtr Keyframer::GetView (const io::View &view) const {
                    if (view.IsNone () || view.IsOrthographic ()) {
                        return View::UniquePtr (new View (view));
                    }
                    else if (view.IsSpotLight ()) {
                        SpotLightNode::UniquePtr spotLightNode (GetSpotLightNode (view.name.value));
                        assert (spotLightNode.get () != 0);
                        if (spotLightNode.get () != 0) {
                            io::Light::UniquePtr light (spotLightNode->Snapshot ());
                            assert (light.get () != 0);
                            if (light.get () != 0) {
                                assert (light->spot.get () != 0);
                                return View::UniquePtr (new View (view, *light));
                            }
                        }
                    }
                    else if (view.IsCamera ()) {
                        CameraNode::UniquePtr cameraNode (GetCameraNode (view.name.value));
                        assert (cameraNode.get () != 0);
                        if (cameraNode.get () != 0) {
                            io::Camera::UniquePtr camera (cameraNode->Snapshot ());
                            assert (camera.get () != 0);
                            if (camera.get () != 0) {
                                return View::UniquePtr (new View (view, *camera));
                            }
                        }
                    }
                    assert (0);
                    return View::UniquePtr ();
                }

                View::UniquePtr Keyframer::GetCurrView () const {
                    return GetView (ioKeyframer ().viewLayout.GetCurrView ());
                }

                blas::Bound2 Keyframer::GetViewBound (const View &view) const {
                    blas::Bound2 bound = blas::Bound2::Empty;
                    {
                        typedef util::OwnerVector<ObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = objectNodes.begin (), end = objectNodes.end (); it != end; ++it) {
                            bound += (*it)->GetBound2 (view.xform);
                        }
                    }
                    if (ioKeyframer ().project.display.showLights) {
                        {
                            typedef util::OwnerVector<LightNode>::const_iterator const_iterator;
                            for (const_iterator it = lightNodes.begin (), end = lightNodes.end (); it != end; ++it) {
                                bound += (*it)->GetBound2 (view.xform);
                            }
                        }
                        {
                            typedef util::OwnerVector<SpotLightTargetNode>::const_iterator const_iterator;
                            for (const_iterator it = spotLightTargetNodes.begin (), end = spotLightTargetNodes.end (); it != end; ++it) {
                                bound += (*it)->GetBound2 (view.xform);
                            }
                        }
                        {
                            typedef util::OwnerVector<SpotLightHeadNode>::const_iterator const_iterator;
                            for (const_iterator it = spotLightHeadNodes.begin (), end = spotLightHeadNodes.end (); it != end; ++it) {
                                bound += (*it)->GetBound2 (view.xform);
                            }
                        }
                    }
                    if (ioKeyframer ().project.display.showCameras) {
                        {
                            typedef util::OwnerVector<CameraTargetNode>::const_iterator const_iterator;
                            for (const_iterator it = cameraTargetNodes.begin (), end = cameraTargetNodes.end (); it != end; ++it) {
                                bound += (*it)->GetBound2 (view.xform);
                            }
                        }
                        {
                            typedef util::OwnerVector<CameraHeadNode>::const_iterator const_iterator;
                            for (const_iterator it = cameraHeadNodes.begin (), end = cameraHeadNodes.end (); it != end; ++it) {
                                bound += (*it)->GetBound2 (view.xform);
                            }
                        }
                    }
                    return bound;
                }

                void Keyframer::SetCurrentFrame () {
                    {
                        typedef std::vector<ObjectNode *>::const_iterator const_iterator;
                        for (const_iterator it = objectNodeRoots.begin (), end = objectNodeRoots.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame (ioKeyframer ().segment.currFrame, blas::Matrix3::Identity);
                        }
                    }
                    {
                        typedef std::vector<LightNode *>::const_iterator const_iterator;
                        for (const_iterator it = lightNodeRoots.begin (), end = lightNodeRoots.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame (ioKeyframer ().segment.currFrame, blas::Matrix3::Identity);
                        }
                    }
                    {
                        typedef std::vector<SpotLightTargetNode *>::const_iterator const_iterator;
                        for (const_iterator it = spotLightTargetNodeRoots.begin (), end = spotLightTargetNodeRoots.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame (ioKeyframer ().segment.currFrame, blas::Matrix3::Identity);
                        }
                    }
                    {
                        typedef std::vector<SpotLightHeadNode *>::const_iterator const_iterator;
                        for (const_iterator it = spotLightHeadNodeRoots.begin (), end = spotLightHeadNodeRoots.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame (ioKeyframer ().segment.currFrame, blas::Matrix3::Identity);
                        }
                    }
                    {
                        typedef std::vector<CameraTargetNode *>::const_iterator const_iterator;
                        for (const_iterator it = cameraTargetNodeRoots.begin (), end = cameraTargetNodeRoots.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame (ioKeyframer ().segment.currFrame, blas::Matrix3::Identity);
                        }
                    }
                    {
                        typedef std::vector<CameraHeadNode *>::const_iterator const_iterator;
                        for (const_iterator it = cameraHeadNodeRoots.begin (), end = cameraHeadNodeRoots.end (); it != end; ++it) {
                            (*it)->SetCurrentFrame (ioKeyframer ().segment.currFrame, blas::Matrix3::Identity);
                        }
                    }
                    ambientNode->SetCurrentFrame (ioKeyframer ().segment.currFrame);
                }

                void Keyframer::Snapshot (io::Editor &editor) const {
                    {
                        typedef util::OwnerVector<ObjectNode>::const_iterator const_iterator;
                        for (const_iterator it = objectNodes.begin (), end = objectNodes.end (); it != end; ++it) {
                            io::Mesh::UniquePtr mesh = (*it)->Snapshot ();
                            if (mesh.get () != 0 && !mesh->hidden) {
                                editor.meshes.push_back (mesh.get ());
                                mesh.release ();
                            }
                        }
                    }
                    {
                        typedef std::vector<LightNode *>::const_iterator const_iterator;
                        for (const_iterator it = lightNodeRoots.begin (), end = lightNodeRoots.end (); it != end; ++it) {
                            io::Light::UniquePtr light = (*it)->Snapshot ();
                            assert (light.get () != 0);
                            editor.lights.push_back (light.get ());
                            light.release ();
                        }
                    }
                    {
                        for (std::size_t i = 0, count = spotLightHeadNodes.size (); i < count; ++i) {
                            io::Light::UniquePtr light = SpotLightNode (
                                *spotLightHeadNodes[i], *spotLightTargetNodes[i]).Snapshot ();
                            assert (light.get () != 0);
                            editor.lights.push_back (light.get ());
                            light.release ();
                        }
                    }
                    {
                        for (std::size_t i = 0, count = cameraHeadNodes.size (); i < count; ++i) {
                            io::Camera::UniquePtr camera = CameraNode (
                                *cameraHeadNodes[i], *cameraTargetNodes[i]).Snapshot ();
                            assert (camera.get () != 0);
                            editor.cameras.push_back (camera.get ());
                            camera.release ();
                        }
                    }
                    editor.ambientLight = opengl::f32Color (ambientNode->color.x, ambientNode->color.y, ambientNode->color.z);
                }

                SpotLightNode::UniquePtr Keyframer::GetSpotLightNode (const std::string &name) const {
                    assert (spotLightTargetNodes.size () == spotLightHeadNodes.size ());
                    for (std::size_t i = 0, count = spotLightHeadNodes.size (); i < count; ++i) {
                        if (spotLightHeadNodes[i]->light.name == name) {
                            assert (spotLightTargetNodes[i]->light.name == name);
                            return SpotLightNode::UniquePtr (
                                new SpotLightNode (*spotLightHeadNodes[i], *spotLightTargetNodes[i]));
                        }
                    }
                    assert (0);
                    return SpotLightNode::UniquePtr ();
                }

                CameraNode::UniquePtr Keyframer::GetCameraNode (const std::string &name) const {
                    assert (cameraTargetNodes.size () == cameraHeadNodes.size ());
                    for (std::size_t i = 0, count = cameraHeadNodes.size (); i < count; ++i) {
                        if (cameraHeadNodes[i]->camera.name == name) {
                            assert (cameraTargetNodes[i]->camera.name == name);
                            return CameraNode::UniquePtr (
                                new CameraNode (*cameraHeadNodes[i], *cameraTargetNodes[i]));
                        }
                    }
                    assert (0);
                    return CameraNode::UniquePtr ();
                }

                void Keyframer::Swap (Keyframer &keyframer) {
                    objectNodes.swap (keyframer.objectNodes);
                    lightNodes.swap (keyframer.lightNodes);
                    spotLightTargetNodes.swap (keyframer.spotLightTargetNodes);
                    spotLightHeadNodes.swap (keyframer.spotLightHeadNodes);
                    cameraTargetNodes.swap (keyframer.cameraTargetNodes);
                    cameraHeadNodes.swap (keyframer.cameraHeadNodes);
                    objectNodeRoots.swap (keyframer.objectNodeRoots);
                    lightNodeRoots.swap (keyframer.lightNodeRoots);
                    spotLightTargetNodeRoots.swap (keyframer.spotLightTargetNodeRoots);
                    spotLightHeadNodeRoots.swap (keyframer.spotLightHeadNodeRoots);
                    cameraTargetNodeRoots.swap (keyframer.cameraTargetNodeRoots);
                    cameraHeadNodeRoots.swap (keyframer.cameraHeadNodeRoots);
                    std::swap (ambientNode, keyframer.ambientNode);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
