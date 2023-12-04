// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#include <set>
#include <map>
#include <string>
#include <cassert>
#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/command/ComplexTypeCommands.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/io/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/io/keyframer/LightNode.h"
#include "thekogans/mex/3ds/io/keyframer/TargetNode.h"
#include "thekogans/mex/3ds/io/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/io/keyframer/CameraNode.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/io/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/keyframer/KeyframerCommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/keyframer/KeyframerCommands.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool KeyframerInitCommand::Execute () {
                        // Create default keyframer data.
                        util::ui16 id = 0;
                        {
                            for (util::OwnerVector<io::Mesh>::const_iterator
                                    it = editor.meshes.begin (),
                                    end = editor.meshes.end (); it != end; ++it) {
                                io::ObjectNode::UniquePtr objectNode (new io::ObjectNode (id++, **it));
                                newKeyframer.objectNodes.push_back (objectNode.get ());
                                objectNode.release ();
                            }
                        }
                        {
                            for (util::OwnerVector<io::Light>::const_iterator
                                    it = editor.lights.begin (),
                                     end = editor.lights.end (); it != end; ++it) {
                                if ((*it)->spot.get () != 0) {
                                    io::TargetNode::UniquePtr targetNode (
                                        new io::TargetNode (id++, **it));
                                    newKeyframer.spotLightTargetNodes.push_back (targetNode.get ());
                                    targetNode.release ();
                                    io::SpotLightNode::UniquePtr spotLightNode (
                                        new io::SpotLightNode (id++, **it));
                                    newKeyframer.spotLightNodes.push_back (spotLightNode.get ());
                                    spotLightNode.release ();
                                }
                                else {
                                    io::LightNode::UniquePtr lightNode (new io::LightNode (id++, **it));
                                    newKeyframer.lightNodes.push_back (lightNode.get ());
                                    lightNode.release ();
                                }
                            }
                        }
                        {
                            for (util::OwnerVector<io::Camera>::const_iterator
                                    it = editor.cameras.begin (),
                                    end = editor.cameras.end (); it != end; ++it) {
                                io::TargetNode::UniquePtr targetNode (
                                    new io::TargetNode (id++, **it));
                                newKeyframer.cameraTargetNodes.push_back (targetNode.get ());
                                targetNode.release ();
                                io::CameraNode::UniquePtr cameraNode (
                                    new io::CameraNode (id++, **it));
                                newKeyframer.cameraNodes.push_back (cameraNode.get ());
                                cameraNode.release ();
                            }
                        }
                        opengl::f32Color color = editor.ambientLight.Getf32Color (
                            opengl::f32Color (0.1f, 0.1f, 0.1f));
                        newKeyframer.ambientNode.colorTrack.keys.push_back (
                            io::Point3Track::Key (0, blas::Point3 (color.r, color.g, color.b)));
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new ComplexTypeSwapCommand<io::Keyframer> (keyframer, newKeyframer)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (KeyframerInitCommand)

                    namespace {
                        util::ui16 GetFirstUniqueId (const io::Keyframer &keyframer) {
                            util::ui16 id = 0;
                            {
                                typedef util::OwnerVector<io::ObjectNode>::const_iterator const_iterator;
                                for (const_iterator it = keyframer.objectNodes.begin (),
                                        end = keyframer.objectNodes.end (); it != end; ++it) {
                                    if ((*it)->id != util::NIDX16 && id <= (*it)->id) {
                                        id = (*it)->id + 1;
                                    }
                                }
                            }
                            {
                                typedef util::OwnerVector<io::LightNode>::const_iterator const_iterator;
                                for (const_iterator it = keyframer.lightNodes.begin (),
                                        end = keyframer.lightNodes.end (); it != end; ++it) {
                                    if ((*it)->id != util::NIDX16 && id <= (*it)->id) {
                                        id = (*it)->id + 1;
                                    }
                                }
                            }
                            {
                                {
                                    typedef util::OwnerVector<io::TargetNode>::const_iterator const_iterator;
                                    for (const_iterator it = keyframer.spotLightTargetNodes.begin (),
                                            end = keyframer.spotLightTargetNodes.end (); it != end; ++it) {
                                        if ((*it)->id != util::NIDX16 && id <= (*it)->id) {
                                            id = (*it)->id + 1;
                                        }
                                    }
                                }
                                {
                                    typedef util::OwnerVector<io::SpotLightNode>::const_iterator const_iterator;
                                    for (const_iterator it = keyframer.spotLightNodes.begin (),
                                            end = keyframer.spotLightNodes.end (); it != end; ++it) {
                                        if ((*it)->id != util::NIDX16 && id <= (*it)->id) {
                                            id = (*it)->id + 1;
                                        }
                                    }
                                }
                            }
                            {
                                {
                                    typedef util::OwnerVector<io::TargetNode>::const_iterator const_iterator;
                                    for (const_iterator it = keyframer.cameraTargetNodes.begin (),
                                            end = keyframer.cameraTargetNodes.end (); it != end; ++it) {
                                        if ((*it)->id != util::NIDX16 && id <= (*it)->id) {
                                            id = (*it)->id + 1;
                                        }
                                    }
                                }
                                {
                                    typedef util::OwnerVector<io::CameraNode>::const_iterator const_iterator;
                                    for (const_iterator it = keyframer.cameraNodes.begin (),
                                            end = keyframer.cameraNodes.end (); it != end; ++it) {
                                        if ((*it)->id != util::NIDX16 && id <= (*it)->id) {
                                            id = (*it)->id + 1;
                                        }
                                    }
                                }
                            }
                            return id;
                        }
                    }

                    bool KeyframerValidateAfterOpenCommand::Execute () {
                        typedef std::pair<io::Node *, util::ui16> NodeIdPair;
                        typedef std::map<util::ui16, NodeIdPair> NodesMap;
                        NodesMap nodes;
                        std::set<std::string> objectNames;
                        std::set<std::string> lightNames;
                        std::set<std::string> spotLightTargetNames;
                        std::set<std::string> spotLightNames;
                        std::set<std::string> cameraTargetNames;
                        std::set<std::string> cameraNames;
                        util::ui16 uniqueId = GetFirstUniqueId (keyframer);
                        // Nodes
                        {
                            typedef util::OwnerVector<io::ObjectNode>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.objectNodes.begin (),
                                end = keyframer.objectNodes.end (); it != end; ++it) {
                                std::string name = (*it)->name;
                                if (name != io::ObjectNode::DUMMY_NAME &&
                                    keyframer.project.editor.GetMesh (name) == 0) {
                                    throw UnknownMesh (name);
                                }
                                if (!(*it)->instanceName.empty ()) {
                                    name += "." + (*it)->instanceName;
                                }
                                if (!objectNames.insert (name).second) {
                                    throw DuplicateName (name);
                                }
                                // This check is necessary because there are 3ds files out
                                // in the wild that were produced by brain dead exporters.
                                // Their node ids are NID. Since nodes with this id cannot
                                // be parents, its safe to assign them a unique one.
                                if ((*it)->id == util::NIDX16) {
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new ui16SetCommand ((*it)->id, uniqueId++)));
                                }
                                std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                    NodesMap::value_type ((*it)->id, NodeIdPair (*it, (*it)->parent)));
                                if (!result.second) {
                                    throw DuplicateId ((*it)->id);
                                }
                            }
                        }
                        {
                            typedef util::OwnerVector<io::LightNode>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.lightNodes.begin (),
                                end = keyframer.lightNodes.end (); it != end; ++it) {
                                if (keyframer.project.editor.GetLight ((*it)->name) == 0) {
                                    throw UnknownLight ((*it)->name);
                                }
                                if (!lightNames.insert ((*it)->name).second) {
                                    throw DuplicateName ((*it)->name);
                                }
                                // See comment in ObjectNode above.
                                if ((*it)->id == util::NIDX16) {
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new ui16SetCommand ((*it)->id, uniqueId++)));
                                }
                                std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                    NodesMap::value_type ((*it)->id, NodeIdPair (*it, (*it)->parent)));
                                if (!result.second) {
                                    throw DuplicateId ((*it)->id);
                                }
                            }
                        }
                        {
                            {
                                typedef util::OwnerVector<io::TargetNode>::const_iterator const_iterator;
                                for (const_iterator it = keyframer.spotLightTargetNodes.begin (),
                                    end = keyframer.spotLightTargetNodes.end (); it != end; ++it) {
                                    if (keyframer.project.editor.GetLight ((*it)->name) == 0) {
                                        throw UnknownLight ((*it)->name);
                                    }
                                    if (!spotLightTargetNames.insert ((*it)->name).second) {
                                        throw DuplicateName ((*it)->name);
                                    }
                                    // See comment in ObjectNode above.
                                    if ((*it)->id == util::NIDX16) {
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new ui16SetCommand ((*it)->id, uniqueId++)));
                                    }
                                    std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                        NodesMap::value_type ((*it)->id, NodeIdPair (*it, (*it)->parent)));
                                    if (!result.second) {
                                        throw DuplicateId ((*it)->id);
                                    }
                                }
                            }
                            {
                                typedef util::OwnerVector<io::SpotLightNode>::const_iterator const_iterator;
                                for (const_iterator it = keyframer.spotLightNodes.begin (),
                                    end = keyframer.spotLightNodes.end (); it != end; ++it) {
                                    if (keyframer.project.editor.GetLight ((*it)->name) == 0) {
                                        throw UnknownLight ((*it)->name);
                                    }
                                    if (!spotLightNames.insert ((*it)->name).second) {
                                        throw DuplicateName ((*it)->name);
                                    }
                                    // See comment in ObjectNode above.
                                    if ((*it)->id == util::NIDX16) {
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new ui16SetCommand ((*it)->id, uniqueId++)));
                                    }
                                    std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                        NodesMap::value_type ((*it)->id, NodeIdPair (*it, (*it)->parent)));
                                    if (!result.second) {
                                        throw DuplicateId ((*it)->id);
                                    }
                                }
                            }
                        }
                        {
                            {
                                typedef util::OwnerVector<io::TargetNode>::const_iterator const_iterator;
                                for (const_iterator it = keyframer.cameraTargetNodes.begin (),
                                    end = keyframer.cameraTargetNodes.end (); it != end; ++it) {
                                    if (keyframer.project.editor.GetCamera ((*it)->name) == 0) {
                                        throw UnknownCamera ((*it)->name);
                                    }
                                    if (!cameraTargetNames.insert ((*it)->name).second) {
                                        throw DuplicateName ((*it)->name);
                                    }
                                    // See comment in ObjectNode above.
                                    if ((*it)->id == util::NIDX16) {
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new ui16SetCommand ((*it)->id, uniqueId++)));
                                    }
                                    std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                        NodesMap::value_type ((*it)->id, NodeIdPair (*it, (*it)->parent)));
                                    if (!result.second) {
                                        throw DuplicateId ((*it)->id);
                                    }
                                }
                            }
                            {
                                typedef util::OwnerVector<io::CameraNode>::const_iterator const_iterator;
                                for (const_iterator it = keyframer.cameraNodes.begin (),
                                    end = keyframer.cameraNodes.end (); it != end; ++it) {
                                    if (keyframer.project.editor.GetCamera ((*it)->name) == 0) {
                                        throw UnknownCamera ((*it)->name);
                                    }
                                    if (!cameraNames.insert ((*it)->name).second) {
                                        throw DuplicateName ((*it)->name);
                                    }
                                    // See comment in ObjectNode above.
                                    if ((*it)->id == util::NIDX16) {
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new ui16SetCommand ((*it)->id, uniqueId++)));
                                    }
                                    std::pair<NodesMap::iterator, bool> result = nodes.insert (
                                        NodesMap::value_type ((*it)->id, NodeIdPair (*it, (*it)->parent)));
                                    if (!result.second) {
                                        throw DuplicateId ((*it)->id);
                                    }
                                }
                            }
                        }
                        // Check hierarchy.
                        {
                            for (NodesMap::const_iterator it = nodes.begin (),
                                    end = nodes.end (); it != end; ++it) {
                                if (it->second.second != util::NIDX16) {
                                    NodesMap::iterator parent = nodes.find (it->second.second);
                                    if (parent == nodes.end ()) {
                                        throw NoId (it->second.second);
                                    }
                                }
                            }
                        }
                        // Add default nodes for missing meshes/lights/cameras.
                        {
                            typedef util::OwnerVector<io::Mesh>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.project.editor.meshes.begin (),
                                end = keyframer.project.editor.meshes.end (); it != end; ++it) {
                                if (objectNames.find ((*it)->name) == objectNames.end ()) {
                                    io::ObjectNode::UniquePtr objectNode (
                                        new io::ObjectNode (uniqueId++, **it));
                                    assert (objectNode.get () != 0);
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new OwnerVectorObjectNodepush_backCommand (
                                                keyframer.objectNodes, std::move (objectNode))));
                                }
                            }
                        }
                        {
                            typedef util::OwnerVector<io::Light>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.project.editor.lights.begin (),
                                end = keyframer.project.editor.lights.end (); it != end; ++it) {
                                if ((*it)->spot.get ()) {
                                    if (spotLightTargetNames.find ((*it)->name) == spotLightTargetNames.end ()) {
                                        io::TargetNode::UniquePtr spotLightTargetNode (
                                            new io::TargetNode (uniqueId++, **it));
                                        assert (spotLightTargetNode.get () != 0);
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new OwnerVectorTargetNodepush_backCommand (
                                                    keyframer.spotLightTargetNodes, std::move (spotLightTargetNode))));
                                    }
                                    if (spotLightNames.find ((*it)->name) == spotLightNames.end ()) {
                                        io::SpotLightNode::UniquePtr spotLightNode (
                                            new io::SpotLightNode (uniqueId++, **it));
                                        assert (spotLightNode.get () != 0);
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new OwnerVectorSpotLightNodepush_backCommand (
                                                    keyframer.spotLightNodes, std::move (spotLightNode))));
                                    }
                                }
                                else {
                                    if (lightNames.find ((*it)->name) == lightNames.end ()) {
                                        io::LightNode::UniquePtr lightNode (
                                            new io::LightNode (uniqueId++, **it));
                                        assert (lightNode.get () != 0);
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new OwnerVectorLightNodepush_backCommand (
                                                    keyframer.lightNodes, std::move (lightNode))));
                                    }
                                }
                            }
                        }
                        {
                            typedef util::OwnerVector<io::Camera>::const_iterator const_iterator;
                            for (const_iterator it = keyframer.project.editor.cameras.begin (),
                                end = keyframer.project.editor.cameras.end (); it != end; ++it) {
                                if (cameraTargetNames.find ((*it)->name) == cameraTargetNames.end ()) {
                                    io::TargetNode::UniquePtr cameraTargetNode (
                                        new io::TargetNode (uniqueId++, **it));
                                    assert (cameraTargetNode.get () != 0);
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new OwnerVectorTargetNodepush_backCommand (
                                                keyframer.cameraTargetNodes, std::move (cameraTargetNode))));
                                }
                                if (cameraNames.find ((*it)->name) == cameraNames.end ()) {
                                    io::CameraNode::UniquePtr cameraNode (
                                        new io::CameraNode (uniqueId++, **it));
                                    assert (cameraNode.get () != 0);
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new OwnerVectorCameraNodepush_backCommand (
                                                keyframer.cameraNodes, std::move (cameraNode))));
                                }
                            }
                        }
                        // Sort camera/light head/target. We do this so that
                        // the same index can access both head, and target
                        // vectors.
                        {
                            assert (keyframer.spotLightNodes.size () == keyframer.spotLightTargetNodes.size ());
                            if (keyframer.spotLightNodes.size () != keyframer.spotLightTargetNodes.size ()) {
                                throw SpotLightHeadTargetMissmatch ("count");
                            }
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new OwnerVectorTargetNodesortCommand (
                                        keyframer.spotLightTargetNodes, CompareTargetNodes ())));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new OwnerVectorSpotLightNodesortCommand (
                                        keyframer.spotLightNodes, CompareSpotLightNodes ())));
                            for (std::size_t i = 0, count = keyframer.spotLightNodes.size (); i < count; ++i) {
                                if (keyframer.spotLightNodes[i]->name != keyframer.spotLightTargetNodes[i]->name) {
                                    throw SpotLightHeadTargetMissmatch (keyframer.spotLightNodes[i]->name);
                                }
                            }
                        }
                        {
                            assert (keyframer.cameraNodes.size () == keyframer.cameraTargetNodes.size ());
                            if (keyframer.cameraNodes.size () != keyframer.cameraTargetNodes.size ()) {
                                throw CameraHeadTargetMissmatch ("count");
                            }
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new OwnerVectorTargetNodesortCommand (
                                        keyframer.cameraTargetNodes, CompareTargetNodes ())));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new OwnerVectorCameraNodesortCommand (
                                        keyframer.cameraNodes, CompareCameraNodes ())));
                            for (std::size_t i = 0, count = keyframer.cameraNodes.size (); i < count; ++i) {
                                if (keyframer.cameraNodes[i]->name != keyframer.cameraTargetNodes[i]->name) {
                                    throw CameraHeadTargetMissmatch (keyframer.cameraNodes[i]->name);
                                }
                            }
                        }
                        // ViewLayout
                        // Guard against brain dead exporters that don't write ViewLayout.
                        // Reset to 3DStudio four view layout.
                        // Check layout sanity.
                        if (!keyframer.viewLayout.layout.IsValid () || keyframer.viewLayout.views3.size () != 5) {
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewLayoutlayoutSetCommand (
                                        keyframer.viewLayout.layout,
                                        io::ViewLayout::Layout (io::ViewLayout::Layout::Four))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewclearCommand (
                                        keyframer.viewLayout.views3)));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        keyframer.viewLayout.views3,
                                        io::TopView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        keyframer.viewLayout.views3,
                                        io::FrontView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        keyframer.viewLayout.views3,
                                        io::LeftView (blas::Size::Empty))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        keyframer.viewLayout.views3,
                                        io::UserView (blas::Size::Empty))));
                            // Place holder used in maximize/minimize logic.
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new stdVectorViewpush_backCommand (
                                        keyframer.viewLayout.views3,
                                        io::View ())));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewLayoutSetSizeCommand (
                                        keyframer.viewLayout,
                                        blas::Size (0, 0, 1024, 768))));
                            ext::Keyframer extKeyframer (keyframer);
                            extKeyframer.SetCurrentFrame ();
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewZoomBoundCommand (
                                        keyframer.viewLayout[0],
                                        extKeyframer.GetViewBound (ext::View (keyframer.viewLayout[0])))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewZoomBoundCommand (
                                        keyframer.viewLayout[1],
                                        extKeyframer.GetViewBound (ext::View (keyframer.viewLayout[1])))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewZoomBoundCommand (
                                        keyframer.viewLayout[2],
                                        extKeyframer.GetViewBound (ext::View (keyframer.viewLayout[2])))));
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ViewZoomBoundCommand (
                                        keyframer.viewLayout[3],
                                        extKeyframer.GetViewBound (ext::View (keyframer.viewLayout[3])))));
                        }
                        else {
                            bool maximized = keyframer.viewLayout.layout.maximized;
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::SharedPtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            keyframer.viewLayout)));
                            }
                            // Make sure Camera/SpotLight views have associated Camera/SpotLights.
                            for (std::size_t i = 0; i < keyframer.viewLayout.GetViewCount (); ++i) {
                                if (!keyframer.viewLayout[i].IsValid ()) {
                                    ExecuteAndAddCommand (
                                        Command::SharedPtr (
                                            new stdVectorViewassignCommand (
                                                keyframer.viewLayout.views3, i, io::View ())));
                                }
                                if (keyframer.viewLayout[i].IsCamera ()) {
                                    if (keyframer.GetCameraNode (keyframer.viewLayout[i].name.value) == 0) {
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new stdVectorViewassignCommand (
                                                    keyframer.viewLayout.views3, i, io::View ())));
                                    }
                                }
                                else if (keyframer.viewLayout[i].IsSpotLight ()) {
                                    if (keyframer.GetSpotLightNode (keyframer.viewLayout[i].name.value) == 0) {
                                        ExecuteAndAddCommand (
                                            Command::SharedPtr (
                                                new stdVectorViewassignCommand (
                                                    keyframer.viewLayout.views3, i, io::View ())));
                                    }
                                }
                            }
                            if (maximized) {
                                ExecuteAndAddCommand (
                                    Command::SharedPtr (
                                        new ViewLayoutToggleMaximizedCommand (
                                            keyframer.viewLayout)));
                            }
                        }
                        // From here on out we will write only newer views3.
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new stdVectorViewclearCommand (
                                    keyframer.viewLayout.views)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (KeyframerValidateAfterOpenCommand)

                    bool KeyframerValidateBeforeSaveCommand::Execute () {
                        util::ui16 id = 0;
                        ext::Keyframer extKeyframer (keyframer);
                        {
                            typedef std::vector<ext::ObjectNode *>::iterator iterator;
                            std::vector<ext::ObjectNode *> &objectNodeRoots = extKeyframer.objectNodeRoots;
                            for (iterator it = objectNodeRoots.begin (),
                                    end = objectNodeRoots.end (); it != end; ++it) {
                                SetID (**it, id);
                            }
                        }
                        {
                            typedef std::vector<ext::LightNode *>::iterator iterator;
                            std::vector<ext::LightNode *> &lightNodeRoots = extKeyframer.lightNodeRoots;
                            for (iterator it = lightNodeRoots.begin (),
                                    end = lightNodeRoots.end (); it != end; ++it) {
                                SetID (**it, id);
                            }
                        }
                        {
                            typedef std::vector<ext::SpotLightTargetNode *>::iterator iterator;
                            std::vector<ext::SpotLightTargetNode *> &spotLightTargetNodeRoots =
                                extKeyframer.spotLightTargetNodeRoots;
                            for (iterator it = spotLightTargetNodeRoots.begin (),
                                    end = spotLightTargetNodeRoots.end (); it != end; ++it) {
                                SetID (**it, id);
                            }
                        }
                        {
                            typedef std::vector<ext::SpotLightHeadNode *>::iterator iterator;
                            std::vector<ext::SpotLightHeadNode *> &spotLightHeadNodeRoots =
                                extKeyframer.spotLightHeadNodeRoots;
                            for (iterator it = spotLightHeadNodeRoots.begin (),
                                    end = spotLightHeadNodeRoots.end (); it != end; ++it) {
                                SetID (**it, id);
                            }
                        }
                        {
                            typedef std::vector<ext::CameraTargetNode *>::iterator iterator;
                            std::vector<ext::CameraTargetNode *> &cameraTargetNodeRoots =
                                extKeyframer.cameraTargetNodeRoots;
                            for (iterator it = cameraTargetNodeRoots.begin (),
                                    end = cameraTargetNodeRoots.end (); it != end; ++it) {
                                SetID (**it, id);
                            }
                        }
                        {
                            typedef std::vector<ext::CameraHeadNode *>::iterator iterator;
                            std::vector<ext::CameraHeadNode *> &cameraHeadNodeRoots =
                                extKeyframer.cameraHeadNodeRoots;
                            for (iterator it = cameraHeadNodeRoots.begin (),
                                    end = cameraHeadNodeRoots.end (); it != end; ++it) {
                                SetID (**it, id);
                            }
                        }
                        assert (extKeyframer.ambientNode.get () != 0);
                        if (extKeyframer.ambientNode.get () != 0) {
                            io::AmbientNode &node = const_cast<io::AmbientNode &> (
                                extKeyframer.ambientNode->ambientNode);
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ui16SetCommand (node.id, id)));
                        }
                        return false;
                    }

                    void KeyframerValidateBeforeSaveCommand::SetID (ext::Node &node, util::ui16 &id) {
                        io::Node &ioNode = const_cast<io::Node &> (node.node);
                        ExecuteAndAddCommand (Command::SharedPtr (new ui16SetCommand (ioNode.id, id++)));
                        if (node.parent != 0) {
                            io::Node &ioNode = const_cast<io::Node &> (node.parent->node);
                            ExecuteAndAddCommand (
                                Command::SharedPtr (
                                    new ui16SetCommand (ioNode.parent, node.parent->node.id)));
                        }
                        typedef std::vector<ext::Node *>::iterator iterator;
                        std::vector<ext::Node *> &children = node.children;
                        for (iterator it = children.begin (), end = children.end (); it != end; ++it) {
                            SetID (**it, id);
                        }
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (KeyframerValidateBeforeSaveCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
