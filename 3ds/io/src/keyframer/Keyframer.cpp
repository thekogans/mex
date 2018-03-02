// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#include <set>
#include <map>
#include <string>
#include <algorithm>
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void Keyframer::Swap (Keyframer &keyframer) {
                    Module::Swap (keyframer);
                    tape.Swap (keyframer.tape);
                    std::swap (version, keyframer.version);
                    fileName.swap (keyframer.fileName);
                    segment.Swap (keyframer.segment);
                    objectNodes.swap (keyframer.objectNodes);
                    lightNodes.swap (keyframer.lightNodes);
                    spotLightTargetNodes.swap (keyframer.spotLightTargetNodes);
                    spotLightNodes.swap (keyframer.spotLightNodes);
                    cameraTargetNodes.swap (keyframer.cameraTargetNodes);
                    cameraNodes.swap (keyframer.cameraNodes);
                    ambientNode.Swap (keyframer.ambientNode);
                }

                void Keyframer::ReadKFDATA (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case KFHDR:
                                chunk >> version >> fileName >> segment.length;
                                break;
                            case VIEWPORT_LAYOUT:
                                ReadVIEWPORT_LAYOUT (chunk);
                                break;
                            case KFSEG:
                                chunk >> segment.startFrame >> segment.endFrame;
                                break;
                            case KFCURTIME:
                                chunk >> segment.currFrame;
                                break;
                            case OBJECT_NODE_TAG: {
                                ObjectNode::UniquePtr objectNode (new ObjectNode);
                                ReadOBJECT_NODE_TAG (*objectNode, chunk);
                                objectNodes.push_back (objectNode.get ());
                                objectNode.release ();
                                break;
                            }
                            case LIGHT_NODE_TAG: {
                                LightNode::UniquePtr lightNode (new LightNode);
                                ReadLIGHT_NODE_TAG (*lightNode, chunk);
                                lightNodes.push_back (lightNode.get ());
                                lightNode.release ();
                                break;
                            }
                            case L_TARGET_NODE_TAG: {
                                TargetNode::UniquePtr spotLightTargetNode (new TargetNode);
                                ReadTargetNode (*spotLightTargetNode, chunk);
                                spotLightTargetNodes.push_back (spotLightTargetNode.get ());
                                spotLightTargetNode.release ();
                                break;
                            }
                            case SPOTLIGHT_NODE_TAG: {
                                SpotLightNode::UniquePtr spotLightNode (new SpotLightNode);
                                ReadSPOTLIGHT_NODE_TAG (*spotLightNode, chunk);
                                spotLightNodes.push_back (spotLightNode.get ());
                                spotLightNode.release ();
                                break;
                            }
                            case TARGET_NODE_TAG: {
                                TargetNode::UniquePtr cameraTargetNode (new TargetNode);
                                ReadTargetNode (*cameraTargetNode, chunk);
                                cameraTargetNodes.push_back (cameraTargetNode.get ());
                                cameraTargetNode.release ();
                                break;
                            }
                            case CAMERA_NODE_TAG: {
                                CameraNode::UniquePtr cameraNode (new CameraNode);
                                ReadCAMERA_NODE_TAG (*cameraNode, chunk);
                                cameraNodes.push_back (cameraNode.get ());
                                cameraNode.release ();
                                break;
                            }
                            case AMBIENT_NODE_TAG:
                                ReadAMBIENT_NODE_TAG (ambientNode, chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadKFDATA");
                                break;
                        }
                    }
                }

                void Keyframer::WriteKFDATA (util::File &file) const {
                    ChunkWriter chunk (file, KFDATA);
                    WriteKFHDR (version, fileName, segment.length, file);
                    WriteVIEWPORT_LAYOUT (file);
                    WriteKFSEG (segment, file);
                    WriteKFCURTIME (segment.currFrame, file);
                    {
                        for (std::size_t i = 0, count = objectNodes.size (); i < count; ++i) {
                            WriteOBJECT_NODE_TAG (*objectNodes[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = lightNodes.size (); i < count; ++i) {
                            WriteLIGHT_NODE_TAG (*lightNodes[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = spotLightTargetNodes.size (); i < count; ++i) {
                            WriteTarget (L_TARGET_NODE_TAG, *spotLightTargetNodes[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = spotLightNodes.size (); i < count; ++i) {
                            WriteSPOTLIGHT_NODE_TAG (*spotLightNodes[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = cameraTargetNodes.size (); i < count; ++i) {
                            WriteTarget (TARGET_NODE_TAG, *cameraTargetNodes[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = cameraNodes.size (); i < count; ++i) {
                            WriteCAMERA_NODE_TAG (*cameraNodes[i], file);
                        }
                    }
                    WriteAMBIENT_NODE_TAG (ambientNode, file);
                }

                const ObjectNode *Keyframer::GetObjectNode (const std::string &name, const std::string &instanceName) const {
                    for (std::size_t i = 0, count = objectNodes.size (); i < count; ++i) {
                        if (objectNodes[i]->name == name && objectNodes[i]->instanceName == instanceName) {
                            return objectNodes[i];
                        }
                    }
                    return 0;
                }

                const LightNode *Keyframer::GetLightNode (const std::string &name) const {
                    for (std::size_t i = 0, count = lightNodes.size (); i < count; ++i) {
                        if (lightNodes[i]->name == name) {
                            return lightNodes[i];
                        }
                    }
                    return 0;
                }

                const TargetNode *Keyframer::GetSpotLightTargetNode (const std::string &name) const {
                    for (std::size_t i = 0, count = spotLightTargetNodes.size (); i < count; ++i) {
                        if (spotLightTargetNodes[i]->name == name) {
                            return spotLightTargetNodes[i];
                        }
                    }
                    return 0;
                }

                const SpotLightNode *Keyframer::GetSpotLightNode (const std::string &name) const {
                    for (std::size_t i = 0, count = spotLightNodes.size (); i < count; ++i) {
                        if (spotLightNodes[i]->name == name) {
                            return spotLightNodes[i];
                        }
                    }
                    return 0;
                }

                const TargetNode *Keyframer::GetCameraTargetNode (const std::string &name) const {
                    for (std::size_t i = 0, count = cameraTargetNodes.size (); i < count; ++i) {
                        if (cameraTargetNodes[i]->name == name) {
                            return cameraTargetNodes[i];
                        }
                    }
                    return 0;
                }

                const CameraNode *Keyframer::GetCameraNode (const std::string &name) const {
                    for (std::size_t i = 0, count = cameraNodes.size (); i < count; ++i) {
                        if (cameraNodes[i]->name == name) {
                            return cameraNodes[i];
                        }
                    }
                    return 0;
                }

                void Keyframer::ReadOBJECT_NODE_TAG (ObjectNode &objectNode, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case NODE_ID:
                                chunk >> objectNode.id;
                                break;
                            case NODE_HDR:
                                chunk >> objectNode.name >> objectNode.flags1 >> objectNode.flags2 >> objectNode.parent;
                                break;
                            case PIVOT:
                                chunk >> objectNode.pivot;
                                break;
                            case INSTANCE_NAME:
                                chunk >> objectNode.instanceName;
                                break;
                            case BOUNDBOX:
                                chunk >> objectNode.bound.min >> objectNode.bound.max;
                                break;
                            case POS_TRACK_TAG:
                                chunk >> objectNode.positionTrack;
                                break;
                            case ROT_TRACK_TAG:
                                chunk >> objectNode.rotationTrack;
                                break;
                            case SCL_TRACK_TAG:
                                chunk >> objectNode.scaleTrack;
                                break;
                            case MORPH_TRACK_TAG:
                                chunk >> objectNode.morphTrack;
                                break;
                            case HIDE_TRACK_TAG:
                                chunk >> objectNode.hideTrack;
                                break;
                            case MORPH_SMOOTH:
                                chunk >> objectNode.morphSmooth;
                                break;
                            case XDATA_SECTION: {
                                util::ui32 dataSize = chunk.GetLength () - CHUNK_HEADER_LENGTH;
                                if (dataSize != 0) {
                                    objectNode.xdata.resize (dataSize);
                                    chunk.Read (&objectNode.xdata[0], dataSize);
                                }
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadOBJECT_NODE_TAG");
                                break;
                        }
                    }
                }

                void Keyframer::ReadLIGHT_NODE_TAG (LightNode &lightNode, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case NODE_ID:
                                chunk >> lightNode.id;
                                break;
                            case NODE_HDR:
                                chunk >> lightNode.name >> lightNode.flags1 >> lightNode.flags2 >> lightNode.parent;
                                break;
                            case POS_TRACK_TAG:
                                chunk >> lightNode.positionTrack;
                                break;
                            case COL_TRACK_TAG:
                                chunk >> lightNode.colorTrack;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadLIGHT_NODE_TAG");
                                break;
                        }
                    }
                }

                void Keyframer::ReadTargetNode (TargetNode &targetNode, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case NODE_ID:
                                chunk >> targetNode.id;
                                break;
                            case NODE_HDR:
                                chunk >> targetNode.name >> targetNode.flags1 >> targetNode.flags2 >> targetNode.parent;
                                break;
                            case POS_TRACK_TAG:
                                chunk >> targetNode.targetTrack;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadTargetNode");
                                break;
                        }
                    }
                }

                void Keyframer::ReadSPOTLIGHT_NODE_TAG (SpotLightNode &spotLightNode, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case NODE_ID:
                                chunk >> spotLightNode.id;
                                break;
                            case NODE_HDR:
                                chunk >> spotLightNode.name >> spotLightNode.flags1 >> spotLightNode.flags2 >> spotLightNode.parent;
                                break;
                            case POS_TRACK_TAG:
                                chunk >> spotLightNode.positionTrack;
                                break;
                            case COL_TRACK_TAG:
                                chunk >> spotLightNode.colorTrack;
                                break;
                            case HOT_TRACK_TAG:
                                chunk >> spotLightNode.hotSpotTrack;
                                break;
                            case FALL_TRACK_TAG:
                                chunk >> spotLightNode.fallOffTrack;
                                break;
                            case ROLL_TRACK_TAG:
                                chunk >> spotLightNode.rollTrack;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadSPOTLIGHT_NODE_TAG");
                                break;
                        }
                    }
                }

                void Keyframer::ReadCAMERA_NODE_TAG (CameraNode &cameraNode, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case NODE_ID:
                                chunk >> cameraNode.id;
                                break;
                            case NODE_HDR:
                                chunk >> cameraNode.name >> cameraNode.flags1 >> cameraNode.flags2 >> cameraNode.parent;
                                break;
                            case POS_TRACK_TAG:
                                chunk >> cameraNode.positionTrack;
                                break;
                            case FOV_TRACK_TAG:
                                chunk >> cameraNode.fovTrack;
                                break;
                            case ROLL_TRACK_TAG:
                                chunk >> cameraNode.rollTrack;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadCAMERA_NODE_TAG");
                                break;
                        }
                    }
                }

                void Keyframer::ReadAMBIENT_NODE_TAG (AmbientNode &ambientNode, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case NODE_ID:
                                chunk >> ambientNode.id;
                                assert (ambientNode.id == util::NIDX16);
                                break;
                            case NODE_HDR:
                                chunk >> ambientNode.name >> ambientNode.flags1 >> ambientNode.flags2 >> ambientNode.parent;
                                assert (ambientNode.name == "$AMBIENT$");
                                assert (ambientNode.parent == util::NIDX16);
                                break;
                            case COL_TRACK_TAG:
                                chunk >> ambientNode.colorTrack;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Keyframer", "ReadAMBIENT_NODE_TAG");
                                break;
                        }
                    }
                }

                void Keyframer::WriteKFHDR (util::ui16 version, const std::string &fileName,
                    util::ui32 segmentLength, util::File &file) const {
                    ChunkWriter chunk (file, KFHDR);
                    chunk << version << fileName << segmentLength;
                }

                void Keyframer::WriteKFSEG (const Segment &segment, util::File &file) const {
                    ChunkWriter chunk (file, KFSEG);
                    chunk << segment.startFrame << segment.endFrame;
                }

                void Keyframer::WriteKFCURTIME (util::ui32 segmentCurrFrame, util::File &file) const {
                    ChunkWriter chunk (file, KFCURTIME);
                    chunk << segmentCurrFrame;
                }

                template<typename T>
                void WriteTrack (util::File &file, util::ui16 id, const Track<T> &track) {
                    ChunkWriter chunk (file, id);
                    file << track;
                }

                void Keyframer::WriteOBJECT_NODE_TAG (const ObjectNode &objectNode, util::File &file) const {
                    ChunkWriter chunk (file, OBJECT_NODE_TAG);
                    WriteNODE_ID (objectNode.id, file);
                    WriteNODE_HDR (objectNode.name, objectNode.flags1,objectNode.flags2, objectNode.parent, file);
                    WritePIVOT (objectNode.pivot, file);
                    if (!objectNode.instanceName.empty ()) {
                        WriteINSTANCE_NAME (objectNode.instanceName, file);
                    }
                    WriteBOUNDBOX (objectNode.bound, file);
                    if (!objectNode.positionTrack.keys.empty ()) {
                        WriteTrack (file, POS_TRACK_TAG, objectNode.positionTrack);
                    }
                    if (!objectNode.rotationTrack.keys.empty ()) {
                        WriteTrack (file, ROT_TRACK_TAG, objectNode.rotationTrack);
                    }
                    if (!objectNode.scaleTrack.keys.empty ()) {
                        WriteTrack (file, SCL_TRACK_TAG, objectNode.scaleTrack);
                    }
                    if (!objectNode.morphTrack.keys.empty ()) {
                        WriteTrack (file, MORPH_TRACK_TAG, objectNode.morphTrack);
                    }
                    if (!objectNode.hideTrack.keys.empty ()) {
                        WriteTrack (file, HIDE_TRACK_TAG, objectNode.hideTrack);
                    }
                    WriteMORPH_SMOOTH (objectNode.morphSmooth, file);
                    if (!objectNode.xdata.empty ()) {
                        WriteXDATA_SECTION (objectNode.xdata, file);
                    }
                }

                void Keyframer::WriteNODE_ID (util::ui16 id, util::File &file) const {
                    ChunkWriter chunk (file, NODE_ID);
                    chunk << id;
                }

                void Keyframer::WriteNODE_HDR (const std::string &name, util::ui16 flags1, util::ui16 flags2, util::ui16 parent, util::File &file) const {
                    ChunkWriter chunk (file, NODE_HDR);
                    chunk << name << flags1 << flags2 << parent;
                }

                void Keyframer::WritePIVOT (const blas::Point3 &pivot, util::File &file) const {
                    ChunkWriter chunk (file, PIVOT);
                    chunk << pivot;
                }

                void Keyframer::WriteINSTANCE_NAME (const std::string &instanceName, util::File &file) const {
                    ChunkWriter chunk (file, INSTANCE_NAME);
                    chunk << instanceName;
                }

                void Keyframer::WriteBOUNDBOX (const blas::Bound3 &bound, util::File &file) const {
                    ChunkWriter chunk (file, BOUNDBOX);
                    chunk << bound;
                }

                void Keyframer::WriteMORPH_SMOOTH (util::f32 morphSmooth, util::File &file) const {
                    ChunkWriter chunk (file, MORPH_SMOOTH);
                    chunk << morphSmooth;
                }

                void Keyframer::WriteXDATA_SECTION (const std::vector<util::ui8> &xdata, util::File &file) const {
                    ChunkWriter chunk (file, XDATA_SECTION);
                    chunk.Write (&xdata[0], (util::ui32)xdata.size ());
                }

                void Keyframer::WriteLIGHT_NODE_TAG (const LightNode &lightNode, util::File &file) const {
                    ChunkWriter chunk (file, LIGHT_NODE_TAG);
                    WriteNODE_ID (lightNode.id, file);
                    WriteNODE_HDR (lightNode.name, lightNode.flags1, lightNode.flags2, lightNode.parent, file);
                    if (!lightNode.positionTrack.keys.empty ()) {
                        WriteTrack (file, POS_TRACK_TAG, lightNode.positionTrack);
                    }
                    if (!lightNode.colorTrack.keys.empty ()) {
                        WriteTrack (file, COL_TRACK_TAG, lightNode.colorTrack);
                    }
                }

                void Keyframer::WriteTarget (util::ui16 id, const TargetNode &targetNode, util::File &file) const {
                    ChunkWriter chunk (file, id);
                    WriteNODE_ID (targetNode.id, file);
                    WriteNODE_HDR (targetNode.name, targetNode.flags1, targetNode.flags2, targetNode.parent, file);
                    if (!targetNode.targetTrack.keys.empty ()) {
                        WriteTrack (file, POS_TRACK_TAG, targetNode.targetTrack);
                    }
                }

                void Keyframer::WriteSPOTLIGHT_NODE_TAG (const SpotLightNode &spotLightNode, util::File &file) const {
                    ChunkWriter chunk (file, SPOTLIGHT_NODE_TAG);
                    WriteNODE_ID (spotLightNode.id, file);
                    WriteNODE_HDR (spotLightNode.name, spotLightNode.flags1, spotLightNode.flags2, spotLightNode.parent, file);
                    if (!spotLightNode.positionTrack.keys.empty ()) {
                        WriteTrack (file, POS_TRACK_TAG, spotLightNode.positionTrack);
                    }
                    if (!spotLightNode.colorTrack.keys.empty ()) {
                        WriteTrack (file, COL_TRACK_TAG, spotLightNode.colorTrack);
                    }
                    if (!spotLightNode.hotSpotTrack.keys.empty ()) {
                        WriteTrack (file, HOT_TRACK_TAG, spotLightNode.hotSpotTrack);
                    }
                    if (!spotLightNode.fallOffTrack.keys.empty ()) {
                        WriteTrack (file, FALL_TRACK_TAG, spotLightNode.fallOffTrack);
                    }
                    if (!spotLightNode.rollTrack.keys.empty ()) {
                        WriteTrack (file, ROLL_TRACK_TAG, spotLightNode.rollTrack);
                    }
                }

                void Keyframer::WriteCAMERA_NODE_TAG (const CameraNode &cameraNode, util::File &file) const {
                    ChunkWriter chunk (file, CAMERA_NODE_TAG);
                    WriteNODE_ID (cameraNode.id, file);
                    WriteNODE_HDR (cameraNode.name, cameraNode.flags1, cameraNode.flags2, cameraNode.parent, file);
                    if (!cameraNode.positionTrack.keys.empty ()) {
                        WriteTrack (file, POS_TRACK_TAG, cameraNode.positionTrack);
                    }
                    if (!cameraNode.fovTrack.keys.empty ()) {
                        WriteTrack (file, FOV_TRACK_TAG, cameraNode.fovTrack);
                    }
                    if (!cameraNode.rollTrack.keys.empty ()) {
                        WriteTrack (file, ROLL_TRACK_TAG, cameraNode.rollTrack);
                    }
                }

                void Keyframer::WriteAMBIENT_NODE_TAG (const AmbientNode &ambientNode, util::File &file) const {
                    ChunkWriter chunk (file, AMBIENT_NODE_TAG);
                    WriteNODE_ID (ambientNode.id, file);
                    WriteNODE_HDR (ambientNode.name, ambientNode.flags1, ambientNode.flags2, ambientNode.parent, file);
                    if (!ambientNode.colorTrack.keys.empty ()) {
                        WriteTrack (file, COL_TRACK_TAG, ambientNode.colorTrack);
                    }
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
