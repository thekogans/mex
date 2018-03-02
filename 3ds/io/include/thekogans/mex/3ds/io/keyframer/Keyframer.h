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

#if !defined (__thekogans_mex_3ds_io_keyframer_Keyframer_h)
#define __thekogans_mex_3ds_io_keyframer_Keyframer_h

#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/Tape.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/keyframer/ObjectNode.h"
#include "thekogans/mex/3ds/io/keyframer/LightNode.h"
#include "thekogans/mex/3ds/io/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/io/keyframer/CameraNode.h"
#include "thekogans/mex/3ds/io/keyframer/TargetNode.h"
#include "thekogans/mex/3ds/io/keyframer/AmbientNode.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Keyframer : public Module {
                    Tape3 tape;
                    util::i16 version;
                    std::string fileName;
                    struct Segment {
                        util::ui32 length;
                        util::ui32 startFrame;
                        util::ui32 endFrame;
                        util::ui32 currFrame;

                        Segment () :
                            length (30),
                            startFrame (0),
                            endFrame (30),
                            currFrame (0) {}

                        inline void Swap (Segment &segment) {
                            std::swap (length, segment.length);
                            std::swap (startFrame, segment.startFrame);
                            std::swap (endFrame, segment.endFrame);
                            std::swap (currFrame, segment.currFrame);
                        }
                    } segment;
                    util::OwnerVector<ObjectNode> objectNodes;
                    util::OwnerVector<LightNode> lightNodes;
                    util::OwnerVector<TargetNode> spotLightTargetNodes;
                    util::OwnerVector<SpotLightNode> spotLightNodes;
                    util::OwnerVector<TargetNode> cameraTargetNodes;
                    util::OwnerVector<CameraNode> cameraNodes;
                    AmbientNode ambientNode;

                    explicit Keyframer (Project &project) :
                        Module (project),
                        version (3) {}

                    void Swap (Keyframer &keyframer);

                    void ReadKFDATA (ChunkReader &mainChunk);
                    void WriteKFDATA (util::File &stream) const;

                    const ObjectNode *GetObjectNode (
                        const std::string &name,
                        const std::string &instanceName) const;
                    const LightNode *GetLightNode (const std::string &name) const;
                    const TargetNode *GetSpotLightTargetNode (const std::string &name) const;
                    const SpotLightNode *GetSpotLightNode (const std::string &name) const;
                    const TargetNode *GetCameraTargetNode (const std::string &name) const;
                    const CameraNode *GetCameraNode (const std::string &name) const;

                private:
                    Keyframer (const Keyframer &);
                    Keyframer &operator = (const Keyframer &);

                    void ReadOBJECT_NODE_TAG (
                        ObjectNode &objectNode,
                        ChunkReader &mainChunk);
                    void ReadLIGHT_NODE_TAG (
                        LightNode &lightNode,
                        ChunkReader &mainChunk);
                    void ReadTargetNode (
                        TargetNode &targetNode,
                        ChunkReader &mainChunk);
                    void ReadSPOTLIGHT_NODE_TAG (
                        SpotLightNode &spotLightNode,
                        ChunkReader &mainChunk);
                    void ReadCAMERA_NODE_TAG (
                        CameraNode &cameraNode,
                        ChunkReader &mainChunk);
                    void ReadAMBIENT_NODE_TAG (
                        AmbientNode &ambientNode,
                        ChunkReader &mainChunk);

                    void WriteKFHDR (
                        util::ui16 version,
                        const std::string &fileName,
                        util::ui32 segmentLength,
                        util::File &stream) const;
                    void WriteKFSEG (
                        const Segment &segment,
                        util::File &stream) const;
                    void WriteKFCURTIME (
                        util::ui32 segmentCurrFrame,
                        util::File &stream) const;
                    void WriteOBJECT_NODE_TAG (
                        const ObjectNode &objectNode,
                        util::File &stream) const;
                    void WriteNODE_ID (
                        util::ui16 id,
                        util::File &stream) const;
                    void WriteNODE_HDR (
                        const std::string &name,
                        util::ui16 flags1,
                        util::ui16 flags2,
                        util::ui16 parent,
                        util::File &stream) const;
                    void WritePIVOT (
                        const blas::Point3 &pivot,
                        util::File &stream) const;
                    void WriteINSTANCE_NAME (
                        const std::string &instanceName,
                        util::File &stream) const;
                    void WriteBOUNDBOX (
                        const blas::Bound3 &bound,
                        util::File &stream) const;
                    void WriteMORPH_SMOOTH (
                        util::f32 morphSmooth,
                        util::File &stream) const;
                    void WriteXDATA_SECTION (
                        const std::vector<util::ui8> &xdata,
                        util::File &stream) const;
                    void WriteLIGHT_NODE_TAG (
                        const LightNode &lightNode,
                        util::File &stream) const;
                    void WriteTarget (
                        util::ui16 id,
                        const TargetNode &targetNode,
                        util::File &stream) const;
                    void WriteSPOTLIGHT_NODE_TAG (
                        const SpotLightNode &spotLightNode,
                        util::File &stream) const;
                    void WriteCAMERA_NODE_TAG (
                        const CameraNode &cameraNode,
                        util::File &stream) const;
                    void WriteAMBIENT_NODE_TAG (
                        const AmbientNode &ambientNode,
                        util::File &stream) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_keyframer_Keyframer_h)
