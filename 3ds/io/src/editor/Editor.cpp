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

#include <climits>
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"
#include "thekogans/mex/3ds/io/MaterialsEditor.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void Editor::Swap (Editor &editor) {
                    Module::Swap (editor);
                    tape.Swap (editor.tape);
                    axis.Swap (editor.axis);
                    create.Swap (editor.create);
                    map.Swap (editor.map);
                    smoothingGroup.Swap (editor.smoothingGroup);
                    lastMeshName.swap (editor.lastMeshName);
                    lastCameraName.swap (editor.lastCameraName);
                    lastLightName.swap (editor.lastLightName);
                    std::swap (selectMask, editor.selectMask);
                    std::swap (unselectMask, editor.unselectMask);
                    std::swap (meshVersion, editor.meshVersion);
                    std::swap (masterScale, editor.masterScale);
                    materials.swap (editor.materials);
                    defaultView.Swap (editor.defaultView);
                    background.Swap (editor.background);
                    shadow.Swap (editor.shadow);
                    atmosphere.Swap (editor.atmosphere);
                    std::swap (constructionPlane, editor.constructionPlane);
                    ambientLight.Swap (editor.ambientLight);
                    meshes.swap (editor.meshes);
                    lights.swap (editor.lights);
                    cameras.swap (editor.cameras);
                }

                void Editor::ReadMDATA (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MESH_VERSION:
                                chunk >> meshVersion;
                                break;
                            case MASTER_SCALE:
                                chunk >> masterScale;
                                break;
                            case MAT_ENTRY: {
                                Material::UniquePtr material (new Material);
                                MaterialsEditor::ReadMAT_ENTRY (*material, chunk);
                                materials.push_back (material.get ());
                                material.release ();
                                break;
                            }
                            case VIEWPORT_LAYOUT:
                                ReadVIEWPORT_LAYOUT (chunk);
                                break;
                            case BIT_MAP:
                                chunk >> background.bitmap;
                                break;
                            case USE_BIT_MAP:
                                background.type = Background::UseBitmap;
                                break;
                            case SOLID_BGND:
                                ReadColor (background.solidColor, chunk);
                                break;
                            case USE_SOLID_BGND:
                                background.type = Background::UseSolidColor;
                                break;
                            case V_GRADIENT:
                                chunk >> background.vGradient.percent;
                                ReadColor (background.vGradient.bottom, chunk);
                                ReadColor (background.vGradient.middle, chunk);
                                ReadColor (background.vGradient.top, chunk);
                                break;
                            case USE_V_GRADIENT:
                                background.type = Background::UseVGradient;
                                break;
                            case LO_SHADOW_BIAS:
                                chunk >> shadow.loBias;
                                break;
                            case HI_SHADOW_BIAS:
                                chunk >> shadow.hiBias;
                                break;
                            case SHADOW_MAP_SIZE:
                                chunk >> shadow.mapSize;
                                break;
                            case SHADOW_SAMPLES:
                                chunk >> shadow.samples;
                                break;
                            case SHADOW_RANGE:
                                chunk >> shadow.range;
                                break;
                            case SHADOW_FILTER:
                                chunk >> shadow.filter;
                                break;
                            case RAY_BIAS:
                                chunk >> shadow.rayBias;
                                break;
                            case FOG:
                                ReadFOG (chunk);
                                break;
                            case USE_FOG:
                                atmosphere.type = Atmosphere::UseFog;
                                break;
                            case DISTANCE_CUE:
                                ReadDISTANCE_CUE (chunk);
                                break;
                            case USE_DISTANCE_CUE:
                                atmosphere.type = Atmosphere::UseDistanceCue;
                                break;
                            case LAYER_FOG:
                                ReadLAYER_FOG (chunk);
                                break;
                            case USE_LAYER_FOG:
                                atmosphere.type = Atmosphere::UseLayerFog;
                                break;
                            case O_CONSTS:
                                chunk >> constructionPlane;
                                break;
                            case AMBIENT_LIGHT:
                                ReadColor (ambientLight, chunk);
                                break;
                            case NAMED_OBJECT:
                                ReadNAMED_OBJECT (chunk);
                                break;
                            case DEFAULT_VIEW:
                                ReadDEFAULT_VIEW (chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadMDATA");
                                break;
                        }
                    }
                }

                void Editor::WriteMDATA (util::File &file) const {
                    ChunkWriter chunk (file, MDATA);
                    WriteMESH_VERSION (file);
                    WriteMASTER_SCALE (file);
                    {
                        for (std::size_t i = 0, count = materials.size (); i < count; ++i) {
                            MaterialsEditor::WriteMAT_ENTRY (*materials[i], file);
                        }
                    }
                    WriteVIEWPORT_LAYOUT (file);
                    WriteDEFAULT_VIEW (defaultView, file);
                    WriteBackground (file);
                    WriteShadow (file);
                    WriteAtmosphere (file);
                    WriteO_CONSTS (file);
                    WriteColor (AMBIENT_LIGHT, ambientLight, file);
                    {
                        for (std::size_t i = 0, count = meshes.size (); i < count; ++i) {
                            WriteMesh (*meshes[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = lights.size (); i < count; ++i) {
                            WriteLight (*lights[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = cameras.size (); i < count; ++i) {
                            WriteCamera (*cameras[i], file);
                        }
                    }
                }

                Mesh *Editor::GetMesh (const std::string &name) const {
                    for (std::size_t i = 0, count = meshes.size (); i < count; ++i) {
                        if (meshes[i]->name == name) {
                            return meshes[i];
                        }
                    }
                    return 0;
                }

                Light *Editor::GetLight (const std::string &name) const {
                    for (size_t i = 0, count = lights.size (); i < count; ++i) {
                        if (lights[i]->name == name) {
                            return lights[i];
                        }
                    }
                    return 0;
                }

                Camera *Editor::GetCamera (const std::string &name) const {
                    for (size_t i = 0, count = cameras.size (); i < count; ++i) {
                        if (cameras[i]->name == name) {
                            return cameras[i];
                        }
                    }
                    return 0;
                }

                void Editor::ReadFOG (ChunkReader &mainChunk) {
                    mainChunk >>
                        atmosphere.fog.nearPlane >>
                        atmosphere.fog.nearDensity >>
                        atmosphere.fog.farPlane >>
                        atmosphere.fog.farDensity;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case COLOR_F:
                                chunk >> atmosphere.fog.color;
                                break;
                            case FOG_BGND:
                                atmosphere.fog.background = true;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadFOG");
                                break;
                        }
                    }
                }

                void Editor::ReadDISTANCE_CUE (ChunkReader &mainChunk) {
                    mainChunk >>
                        atmosphere.distanceCue.nearPlane >>
                        atmosphere.distanceCue.nearDim >>
                        atmosphere.distanceCue.farPlane >>
                        atmosphere.distanceCue.farDim;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case DCUE_BGND:
                                atmosphere.distanceCue.background = true;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadDISTANCE_CUE");
                                break;
                        }
                    }
                }

                void Editor::ReadLAYER_FOG (ChunkReader &mainChunk) {
                    mainChunk >>
                        atmosphere.layerFog.zmin >>
                        atmosphere.layerFog.zmax >>
                        atmosphere.layerFog.density >>
                        atmosphere.layerFog.flags;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case COLOR_F:
                                chunk >> atmosphere.layerFog.color;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadLAYER_FOG");
                                break;
                        }
                    }
                }

                void Editor::ReadDEFAULT_VIEW (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case VIEW_TOP:
                                defaultView.type = View::Top;
                                chunk >> defaultView.target >> defaultView.width;
                                break;
                            case VIEW_BOTTOM:
                                defaultView.type = View::Bottom;
                                chunk >> defaultView.target >> defaultView.width;
                                break;
                            case VIEW_LEFT:
                                defaultView.type = View::Left;
                                chunk >> defaultView.target >> defaultView.width;
                                break;
                            case VIEW_RIGHT:
                                defaultView.type = View::Right;
                                chunk >> defaultView.target >> defaultView.width;
                                break;
                            case VIEW_FRONT:
                                defaultView.type = View::Front;
                                chunk >> defaultView.target >> defaultView.width;
                                break;
                            case VIEW_BACK:
                                defaultView.type = View::Back;
                                chunk >> defaultView.target >> defaultView.width;
                                break;
                            case VIEW_USER:
                                defaultView.type = View::User;
                                chunk >>
                                    defaultView.target >>
                                    defaultView.width >>
                                    defaultView.horizontalAngle >>
                                    defaultView.verticalAngle >>
                                    defaultView.bankAngle;
                                break;
                            case VIEW_CAMERA:
                                defaultView.type = View::Camera;
                                chunk >> defaultView.cameraName;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadDEFAULT_VIEW");
                                break;
                        }
                    }
                }

                void Editor::ReadNAMED_OBJECT (ChunkReader &mainChunk) {
                    std::string name;
                    mainChunk >> name;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case N_TRI_OBJECT: {
                                Mesh::UniquePtr mesh (new Mesh (name));
                                ReadN_TRI_OBJECT (*mesh, chunk);
                                meshes.push_back (mesh.get ());
                                mesh.release ();
                                break;
                            }
                            case N_DIRECT_LIGHT: {
                                Light::UniquePtr light (new Light (name));
                                ReadN_DIRECT_LIGHT (*light, chunk);
                                lights.push_back (light.get ());
                                light.release ();
                                break;
                            }
                            case N_CAMERA: {
                                Camera::UniquePtr camera (new Camera (name));
                                ReadN_CAMERA (*camera, chunk);
                                cameras.push_back (camera.get ());
                                camera.release ();
                                break;
                            }
                            case OBJ_HIDDEN:
                                meshes.back ()->hidden = true;
                                break;
                            case OBJ_VIS_LOFTER:
                                meshes.back ()->visibleInLofter = true;
                                break;
                            case OBJ_DOESNT_CAST:
                                meshes.back ()->castShadow = false;
                                break;
                            case OBJ_DONT_RCVSHADOW:
                                meshes.back ()->receiveShadows = false;
                                break;
                            case OBJ_MATTE:
                                meshes.back ()->matte = true;
                                break;
                            case OBJ_FAST:
                                meshes.back ()->fastDraw = true;
                                break;
                            case OBJ_PROCEDURAL:
                                meshes.back ()->useProcData = true;
                                break;
                            case OBJ_FROZEN:
                                meshes.back ()->frozen = true;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadNAMED_OBJECT");
                                break;
                        }
                    }
                }

                inline ChunkReader &operator >> (
                        ChunkReader &chunk,
                        Mesh::Map &map) {
                    chunk >>
                        map.type >>
                        map.tilex >>
                        map.tiley >>
                        map.center >>
                        map.scale >>
                        map.xform >>
                        map.planarWidth >>
                        map.planarHeight >>
                        map.cylindricalHeight;
                    return chunk;
                }

                void Editor::ReadN_TRI_OBJECT (
                        Mesh &mesh,
                        ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case POINT_ARRAY: {
                                util::ui16 vertexCount;
                                chunk >> vertexCount;
                                mesh.vertices.resize (vertexCount);
                                for (util::ui16 i = 0; i < vertexCount; ++i) {
                                    chunk >> mesh.vertices[i];
                                }
                                break;
                            }
                            case POINT_FLAG_ARRAY: {
                                util::ui16 vertexFlagCount;
                                chunk >> vertexFlagCount;
                                mesh.vertexFlags.resize (vertexFlagCount);
                                for (util::ui16 i = 0; i < vertexFlagCount; ++i) {
                                    chunk >> mesh.vertexFlags[i];
                                }
                                break;
                            }
                            case TEX_VERTS: {
                                util::ui16 textureVertexCount;
                                chunk >> textureVertexCount;
                                mesh.textureVertices.resize (textureVertexCount);
                                for (util::ui16 i = 0; i < textureVertexCount; ++i) {
                                    chunk >> mesh.textureVertices[i];
                                }
                                break;
                            }
                            case FACE_ARRAY:
                                ReadFACE_ARRAY (mesh, chunk);
                                break;
                            case MESH_MATRIX:
                                chunk >> mesh.xform;
                                break;
                            case MESH_COLOR:
                                chunk >> mesh.paletteIndex;
                                break;
                            case MESH_TEXTURE_INFO:
                                chunk >> mesh.map;
                                break;
                            case PROC_NAME:
                                chunk >> mesh.procName;
                                break;
                            case PROC_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - CHUNK_HEADER_LENGTH;
                                mesh.procData.resize (dataSize);
                                chunk.Read (&mesh.procData[0], dataSize);
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadN_TRI_OBJECT");
                                break;
                        }
                    }
                }

                inline ChunkReader &operator >> (
                        ChunkReader &chunk,
                        Mesh::Face &face) {
                    chunk >> face.a >> face.b >> face.c >> face.flags;
                    return chunk;
                }

                void Editor::ReadFACE_ARRAY (
                        Mesh &mesh,
                        ChunkReader &mainChunk) {
                    util::ui16 faceCount;
                    mainChunk >> faceCount;
                    mesh.faces.resize (faceCount);
                    for (util::ui16 i = 0; i < faceCount; ++i) {
                        mainChunk >> mesh.faces[i];
                    }
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case SMOOTH_GROUP: {
                                mesh.faceSmoothGroups.resize (faceCount);
                                for (util::ui16 i = 0; i < faceCount; ++i) {
                                    mainChunk >> mesh.faceSmoothGroups[i];
                                }
                                break;
                            }
                            case MSH_MAT_GROUP: {
                                std::string name;
                                chunk >> name;
                                Mesh::Material material (name);
                                util::ui16 faceCount;
                                chunk >> faceCount;
                                material.faces.resize (faceCount);
                                for (util::ui16 i = 0; i < faceCount; ++i) {
                                    chunk >> material.faces[i];
                                }
                                mesh.materials.push_back (material);
                                break;
                            }
                            case MSH_BOXMAP: {
                                for (util::ui32 i = 0; i < 6; ++i) {
                                    std::string map;
                                    chunk >> map;
                                    mesh.boxMap.push_back (map);
                                }
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadFACE_ARRAY");
                                break;
                        }
                    }
                }

                void Editor::ReadN_DIRECT_LIGHT (
                        Light &light,
                        ChunkReader &mainChunk) {
                    mainChunk >> light.position;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case COLOR_F:
                                chunk >> light.color;
                                break;
                            case DL_MULTIPLIER:
                                chunk >> light.multiplier;
                                break;
                            case DL_OUTER_RANGE:
                                chunk >> light.attenuation.outer;
                                break;
                            case DL_INNER_RANGE:
                                chunk >> light.attenuation.inner;
                                break;
                            case DL_ATTENUATE:
                                light.attenuation.on = true;
                                break;
                            case DL_EXCLUDE: {
                                std::string exclude;
                                chunk >> exclude;
                                light.excludes.push_back (exclude);
                                break;
                            }
                            case DL_OFF:
                                light.on = false;
                                break;
                            case DL_SPOTLIGHT: {
                                light.spot.reset (new Light::Spot);
                                ReadDL_SPOTLIGHT (*light.spot, chunk);
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadN_DIRECT_LIGHT");
                                break;
                        }
                    }
                }

                void Editor::ReadDL_SPOTLIGHT (
                        Light::Spot &spot,
                        ChunkReader &mainChunk) {
                    mainChunk >> spot.target >> spot.hotSpot >> spot.fallOff;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case DL_SPOT_ROLL:
                                chunk >> spot.roll;
                                break;
                            case DL_SPOT_RECTANGULAR:
                                spot.cone.type = Light::Spot::Cone::Rectangular;
                                break;
                            case DL_SPOT_ASPECT:
                                chunk >> spot.cone.aspect;
                                break;
                            case DL_SEE_CONE:
                                spot.cone.show = true;
                                break;
                            case DL_SPOT_OVERSHOOT:
                                spot.cone.overshoot = true;
                                break;
                            case DL_RAYSHAD:
                                spot.shadow.type = Light::Spot::Shadow::UseRayTraceShadow;
                                break;
                            case DL_LOCAL_SHADOW2:
                                chunk >>
                                    spot.shadow.bias >>
                                    spot.shadow.filter >>
                                    spot.shadow.mapSize;
                                spot.shadow.local = true;
                                break;
                            case DL_RAY_BIAS:
                                chunk >> spot.shadow.rayBias;
                                break;
                            case DL_SHADOWED:
                                spot.shadow.cast = true;
                                break;
                            case DL_PROJECTOR:
                                chunk >> spot.projector.bitmap;
                                spot.projector.use = true;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadDL_SPOTLIGHT");
                                break;
                        }
                    }
                }

                void Editor::ReadN_CAMERA (
                        Camera &camera,
                        ChunkReader &mainChunk) {
                    mainChunk >>
                        camera.position >>
                        camera.target >>
                        camera.roll >>
                        camera.lens;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case CAM_SEE_CONE:
                                camera.showCone = true;
                                break;
                            case CAM_RANGES:
                                chunk >> camera.ranges.hither >> camera.ranges.yon;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Editor", "ReadN_CAMERA");
                                break;
                        }
                    }
                }

                void Editor::WriteMESH_VERSION (util::File &file) const {
                    ChunkWriter chunk (file, MESH_VERSION);
                    chunk << meshVersion;
                }

                void Editor::WriteMASTER_SCALE (util::File &file) const {
                    ChunkWriter chunk (file, MASTER_SCALE);
                    chunk << masterScale;
                }

                void Editor::WriteDEFAULT_VIEW (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    if (defaultView.type != View::None) {
                        ChunkWriter chunk (file, DEFAULT_VIEW);
                        switch (defaultView.type) {
                            case View::None:
                                // FIXME: implement
                                assert (0);
                                break;
                            case View::Top:
                                WriteVIEW_TOP (defaultView, file);
                                break;
                            case View::Bottom:
                                WriteVIEW_BOTTOM (defaultView, file);
                                break;
                            case View::Left:
                                WriteVIEW_LEFT (defaultView, file);
                                break;
                            case View::Right:
                                WriteVIEW_RIGHT (defaultView, file);
                                break;
                            case View::Front:
                                WriteVIEW_FRONT (defaultView, file);
                                break;
                            case View::Back:
                                WriteVIEW_BACK (defaultView, file);
                                break;
                            case View::User:
                                WriteVIEW_USER (defaultView, file);
                                break;
                            case View::SpotLight:
                                // FIXME: implement
                                assert (0);
                                break;
                            case View::Camera:
                                WriteVIEW_CAMERA (defaultView, file);
                                break;
                        }
                    }
                }

                void Editor::WriteVIEW_TOP (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_TOP);
                    chunk << defaultView.target << defaultView.width;
                }

                void Editor::WriteVIEW_BOTTOM (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_BOTTOM);
                    chunk << defaultView.target << defaultView.width;
                }

                void Editor::WriteVIEW_LEFT (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_LEFT);
                    chunk << defaultView.target << defaultView.width;
                }

                void Editor::WriteVIEW_RIGHT (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_RIGHT);
                    chunk << defaultView.target << defaultView.width;
                }

                void Editor::WriteVIEW_FRONT (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_FRONT);
                    chunk << defaultView.target << defaultView.width;
                }

                void Editor::WriteVIEW_BACK (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_BACK);
                    chunk << defaultView.target << defaultView.width;
                }

                void Editor::WriteVIEW_USER (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_USER);
                    chunk <<
                        defaultView.target <<
                        defaultView.width <<
                        defaultView.horizontalAngle <<
                        defaultView.verticalAngle <<
                        defaultView.bankAngle;
                }

                void Editor::WriteVIEW_CAMERA (
                        const DefaultView &defaultView,
                        util::File &file) const {
                    ChunkWriter chunk (file, VIEW_CAMERA);
                    chunk << defaultView.cameraName;
                }

                void Editor::WriteBackground (util::File &file) const {
                    WriteColor (SOLID_BGND, background.solidColor, file);
                    {
                        ChunkWriter chunk (file, V_GRADIENT);
                        chunk << background.vGradient.percent;
                        WriteColor (COLOR_F, background.vGradient.bottom.gammaCorrected.floatColor, file);
                        WriteColor (LIN_COLOR_F, background.vGradient.bottom.raw.floatColor, file);
                        WriteColor (COLOR_F, background.vGradient.middle.gammaCorrected.floatColor, file);
                        WriteColor (LIN_COLOR_F, background.vGradient.middle.raw.floatColor, file);
                        WriteColor (COLOR_F, background.vGradient.top.gammaCorrected.floatColor, file);
                        WriteColor (LIN_COLOR_F, background.vGradient.top.raw.floatColor, file);
                    }
                    {
                        ChunkWriter chunk (file, BIT_MAP);
                        chunk << background.bitmap;
                    }
                    switch (background.type) {
                        case Background::NoBackground:
                            // This is not an error.
                            break;
                        case Background::UseSolidColor:
                            WriteUSE_SOLID_BGND (file);
                            break;
                        case Background::UseVGradient:
                            WriteUSE_V_GRADIENT (file);
                            break;
                        case Background::UseBitmap:
                            WriteUSE_BIT_MAP (file);
                            break;
                    }
                }

                void Editor::WriteUSE_SOLID_BGND (util::File &file) const {
                    ChunkWriter chunk (file, USE_SOLID_BGND);
                }

                void Editor::WriteUSE_V_GRADIENT (util::File &file) const {
                    ChunkWriter chunk (file, USE_V_GRADIENT);
                }

                void Editor::WriteUSE_BIT_MAP (util::File &file) const {
                    ChunkWriter chunk (file, USE_BIT_MAP);
                }

                void Editor::WriteShadow (util::File &file) const {
                    {
                        ChunkWriter chunk (file, LO_SHADOW_BIAS);
                        chunk << shadow.loBias;
                    }
                    {
                        ChunkWriter chunk (file, HI_SHADOW_BIAS);
                        chunk << shadow.hiBias;
                    }
                    {
                        ChunkWriter chunk (file, SHADOW_MAP_SIZE);
                        chunk << shadow.mapSize;
                    }
                    {
                        ChunkWriter chunk (file, SHADOW_SAMPLES);
                        chunk << shadow.samples;
                    }
                    {
                        ChunkWriter chunk (file, SHADOW_RANGE);
                        chunk << shadow.range;
                    }
                    {
                        ChunkWriter chunk (file, SHADOW_FILTER);
                        chunk << shadow.filter;
                    }
                    {
                        ChunkWriter chunk (file, RAY_BIAS);
                        chunk << shadow.rayBias;
                    }
                }

                void Editor::WriteAtmosphere (util::File &file) const {
                    {
                        ChunkWriter chunk (file, FOG);
                        chunk <<
                            atmosphere.fog.nearPlane <<
                            atmosphere.fog.nearDensity <<
                            atmosphere.fog.farPlane <<
                            atmosphere.fog.farDensity;
                        WriteColor (COLOR_F, atmosphere.fog.color, file);
                        if (atmosphere.fog.background) {
                            WriteFOG_BGND (file);
                        }
                    }
                    {
                        ChunkWriter chunk (file, LAYER_FOG);
                        chunk <<
                            atmosphere.layerFog.zmin <<
                            atmosphere.layerFog.zmax <<
                            atmosphere.layerFog.density <<
                            atmosphere.layerFog.flags;
                        WriteColor (COLOR_F, atmosphere.layerFog.color, file);
                    }
                    {
                        ChunkWriter chunk (file, DISTANCE_CUE);
                        chunk <<
                            atmosphere.distanceCue.nearPlane <<
                            atmosphere.distanceCue.nearDim <<
                            atmosphere.distanceCue.farPlane <<
                            atmosphere.distanceCue.farDim;
                        if (atmosphere.distanceCue.background) {
                            WriteDCUE_BGND (file);
                        }
                    }
                    switch (atmosphere.type) {
                        case Atmosphere::NoAtmosphere:
                            // This is not an error.
                            break;
                        case Atmosphere::UseFog:
                            WriteUSE_FOG (file);
                            break;
                        case Atmosphere::UseLayerFog:
                            WriteUSE_LAYER_FOG (file);
                            break;
                        case Atmosphere::UseDistanceCue:
                            WriteUSE_DISTANCE_CUE (file);
                            break;
                    }
                }

                void Editor::WriteFOG_BGND (util::File &file) const {
                    ChunkWriter chunk (file, FOG_BGND);
                }

                void Editor::WriteDCUE_BGND (util::File &file) const {
                    ChunkWriter chunk (file, DCUE_BGND);
                }

                void Editor::WriteUSE_FOG (util::File &file) const {
                    ChunkWriter chunk (file, USE_FOG);
                }

                void Editor::WriteUSE_LAYER_FOG (util::File &file) const {
                    ChunkWriter chunk (file, USE_LAYER_FOG);
                }

                void Editor::WriteUSE_DISTANCE_CUE (util::File &file) const {
                    ChunkWriter chunk (file, USE_DISTANCE_CUE);
                }

                void Editor::WriteO_CONSTS (util::File &file) const {
                    ChunkWriter chunk (file, O_CONSTS);
                    chunk << constructionPlane;
                }

                void Editor::WriteMesh (
                        const Mesh &mesh,
                        util::File &file) const {
                    ChunkWriter chunk (file, NAMED_OBJECT);
                    chunk << mesh.name;
                    WriteN_TRI_OBJECT (mesh, file);
                    if (mesh.hidden) {
                        WriteOBJ_HIDDEN (file);
                    }
                    if (mesh.visibleInLofter) {
                        WriteOBJ_VIS_LOFTER (file);
                    }
                    if (!mesh.castShadow) {
                        WriteOBJ_DOESNT_CAST (file);
                    }
                    if (!mesh.receiveShadows) {
                        WriteOBJ_DONT_RCVSHADOW (file);
                    }
                    if (mesh.matte) {
                        WriteOBJ_MATTE (file);
                    }
                    if (mesh.fastDraw) {
                        WriteOBJ_FAST (file);
                    }
                    if (mesh.useProcData) {
                        WriteOBJ_PROCEDURAL (file);
                    }
                    if (mesh.frozen) {
                        WriteOBJ_FROZEN (file);
                    }
                }

                void Editor::WriteLight (
                        const Light &light,
                        util::File &file) const {
                    ChunkWriter chunk (file, NAMED_OBJECT);
                    chunk << light.name;
                    WriteN_DIRECT_LIGHT (light, file);
                }

                void Editor::WriteCamera (
                        const Camera &camera,
                        util::File &file) const {
                    ChunkWriter chunk (file, NAMED_OBJECT);
                    chunk << camera.name;
                    WriteN_CAMERA (camera, file);
                }

                void Editor::WriteN_TRI_OBJECT (
                        const Mesh &mesh,
                        util::File &file) const {
                    if (mesh.vertices.size () > USHRT_MAX ||
                        mesh.vertexFlags.size () > USHRT_MAX ||
                        mesh.textureVertices.size () > USHRT_MAX ||
                        mesh.faces.size () > USHRT_MAX ||
                        mesh.faceSmoothGroups.size () > USHRT_MAX) {
                        throw InvalidMesh (mesh.name);
                    }
                    ChunkWriter chunk (file, N_TRI_OBJECT);
                    if (!mesh.vertices.empty ()) {
                        WritePOINT_ARRAY (mesh.vertices, file);
                    }
                    if (!mesh.vertexFlags.empty ()) {
                        WritePOINT_FLAG_ARRAY (mesh.vertexFlags, file);
                    }
                    if (!mesh.textureVertices.empty ()) {
                        WriteTEX_VERTS (mesh.textureVertices, file);
                    }
                    if (mesh.useMap) {
                        WriteMESH_TEXTURE_INFO (mesh.map, file);
                    }
                    WriteMESH_MATRIX (mesh.xform, file);
                    WriteMESH_COLOR (mesh.paletteIndex, file);
                    if (!mesh.faces.empty ()) {
                        WriteFACE_ARRAY (mesh, file);
                    }
                    if (!mesh.procName.empty ()) {
                        WritePROC_NAME (mesh.procName, file);
                    }
                    if (!mesh.procData.empty ()) {
                        WritePROC_DATA (mesh.procData, file);
                    }
                }

                void Editor::WritePOINT_ARRAY (
                        const std::vector<blas::Point3> &vertices,
                        util::File &file) const {
                    ChunkWriter chunk (file, POINT_ARRAY);
                    util::ui16 count = (util::ui16)vertices.size ();
                    chunk << count;
                    for (util::ui16 i = 0; i < count; ++i) {
                        chunk << vertices[i];
                    }
                }

                void Editor::WritePOINT_FLAG_ARRAY (
                        const std::vector<util::ui16> &vertexFlags,
                        util::File &file) const {
                    ChunkWriter chunk (file, POINT_FLAG_ARRAY);
                    util::ui16 count = (util::ui16)vertexFlags.size ();
                    chunk << count;
                    for (util::ui16 i = 0; i < count; ++i) {
                        chunk << vertexFlags[i];
                    }
                }

                void Editor::WriteTEX_VERTS (
                        const std::vector<blas::Point2> &textureVertices,
                        util::File &file) const {
                    ChunkWriter chunk (file, TEX_VERTS);
                    util::ui16 count = (util::ui16)textureVertices.size ();
                    chunk << count;
                    for (util::ui16 i = 0; i < count; ++i) {
                        chunk << textureVertices[i];
                    }
                }

                void Editor::WriteMESH_TEXTURE_INFO (
                        const Mesh::Map &map,
                        util::File &file) const {
                    ChunkWriter chunk (file, MESH_TEXTURE_INFO);
                    chunk << map.type << map.tilex << map.tiley << map.center << map.scale <<
                        map.xform << map.planarWidth << map.planarHeight << map.cylindricalHeight;
                }

                void Editor::WriteMESH_MATRIX (
                        const blas::Matrix3 &xform,
                        util::File &file) const {
                    ChunkWriter chunk (file, MESH_MATRIX);
                    chunk << xform;
                }

                void Editor::WriteMESH_COLOR (
                        util::ui8 paletteIndex,
                        util::File &file) const {
                    ChunkWriter chunk (file, MESH_COLOR);
                    chunk << paletteIndex;
                }

                namespace {
                    inline ChunkWriter &operator << (
                            ChunkWriter &chunk,
                            const Mesh::Face &face) {
                        chunk << face.a << face.b << face.c << face.flags;
                        return chunk;
                    }
                }

                void Editor::WriteFACE_ARRAY (
                        const Mesh &mesh,
                        util::File &file) const {
                    ChunkWriter chunk (file, FACE_ARRAY);
                    util::ui16 count = (util::ui16)mesh.faces.size ();
                    chunk << count;
                    util::ui16 i;
                    for (i = 0; i < count; ++i) {
                        chunk << mesh.faces[i];
                    }
                    if (!mesh.faceSmoothGroups.empty ()) {
                        WriteSMOOTH_GROUP (mesh.faceSmoothGroups, file);
                    }
                    for (i = 0, count = (util::ui16)mesh.materials.size (); i < count; ++i) {
                        WriteMSH_MAT_GROUP (mesh.materials[i], file);
                    }
                    if (mesh.useBoxMap) {
                        WriteMSH_BOXMAP (mesh.boxMap, file);
                    }
                }

                void Editor::WriteSMOOTH_GROUP (
                        const std::vector<util::ui32> &faceSmoothGroups,
                        util::File &file) const {
                    ChunkWriter chunk (file, SMOOTH_GROUP);
                    for (std::size_t i = 0, count = faceSmoothGroups.size (); i < count; ++i) {
                        chunk << faceSmoothGroups[i];
                    }
                }

                void Editor::WriteMSH_MAT_GROUP (
                        const Mesh::Material &material,
                        util::File &file) const {
                    if (!material.faces.empty ()) {
                        ChunkWriter chunk (file, MSH_MAT_GROUP);
                        chunk << material.name;
                        util::ui16 count = (util::ui16)material.faces.size ();
                        chunk << count;
                        for (util::ui16 i = 0; i < count; ++i) {
                            chunk << material.faces[i];
                        }
                    }
                }

                void Editor::WriteMSH_BOXMAP (
                        const std::vector<std::string> &boxMap,
                        util::File &file) const {
                    assert (boxMap.size () == 6);
                    ChunkWriter chunk (file, MSH_BOXMAP);
                    for (util::ui32 i = 0; i < 6; ++i) {
                        chunk << boxMap[i];
                    }
                }

                void Editor::WritePROC_NAME (
                        const std::string &procName,
                        util::File &file) const {
                    ChunkWriter chunk (file, PROC_NAME);
                    chunk << procName;
                }

                void Editor::WritePROC_DATA (
                        const std::vector<util::ui8> &procData,
                        util::File &file) const {
                    ChunkWriter chunk (file, PROC_DATA);
                    chunk.Write (&procData[0], (util::ui32)procData.size ());
                }

                void Editor::WriteOBJ_HIDDEN (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_HIDDEN);
                }

                void Editor::WriteOBJ_VIS_LOFTER (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_VIS_LOFTER);
                }

                void Editor::WriteOBJ_DOESNT_CAST (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_DOESNT_CAST);
                }

                void Editor::WriteOBJ_DONT_RCVSHADOW (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_DONT_RCVSHADOW);
                }

                void Editor::WriteOBJ_MATTE (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_MATTE);
                }

                void Editor::WriteOBJ_FAST (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_FAST);
                }

                void Editor::WriteOBJ_PROCEDURAL (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_PROCEDURAL);
                }

                void Editor::WriteOBJ_FROZEN (util::File &file) const {
                    ChunkWriter chunk (file, OBJ_FROZEN);
                }

                void Editor::WriteN_DIRECT_LIGHT (const Light &light, util::File &file) const {
                    ChunkWriter chunk (file, N_DIRECT_LIGHT);
                    chunk << light.position;
                    WriteColor (COLOR_F, light.color, file);
                    WriteDL_MULTIPLIER (light.multiplier, file);
                    WriteDL_OUTER_RANGE (light.attenuation.outer, file);
                    WriteDL_INNER_RANGE (light.attenuation.inner, file);
                    if (light.attenuation.on) {
                        WriteDL_ATTENUATE (file);
                    }
                    for (std::size_t i = 0, count = light.excludes.size (); i < count; ++i) {
                        WriteDL_EXCLUDE (light.excludes[i], file);
                    }
                    if (!light.on) {
                        WriteDL_OFF (file);
                    }
                    if (light.spot.get ()) {
                        WriteDL_SPOTLIGHT (*light.spot, file);
                    }
                }

                void Editor::WriteDL_OFF (util::File &file) const {
                    ChunkWriter chunk (file, DL_OFF);
                }

                void Editor::WriteDL_OUTER_RANGE (
                        util::f32 outer,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_OUTER_RANGE);
                    chunk << outer;
                }

                void Editor::WriteDL_INNER_RANGE (
                        util::f32 inner,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_INNER_RANGE);
                    chunk << inner;
                }

                void Editor::WriteDL_ATTENUATE (util::File &file) const {
                    ChunkWriter chunk (file, DL_ATTENUATE);
                }

                void Editor::WriteDL_MULTIPLIER (
                        util::f32 multiplier,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_MULTIPLIER);
                    chunk << multiplier;
                }

                void Editor::WriteDL_EXCLUDE (
                        const std::string &exclude,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_EXCLUDE);
                    chunk << exclude;
                }

                void Editor::WriteDL_SPOTLIGHT (
                        const Light::Spot &spot,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_SPOTLIGHT);
                    chunk << spot.target << spot.hotSpot << spot.fallOff;
                    WriteDL_SPOT_ROLL (spot.roll, file);
                    if (spot.cone.type == Light::Spot::Cone::Rectangular) {
                        WriteDL_SPOT_RECTANGULAR (file);
                    }
                    WriteDL_SPOT_ASPECT (spot.cone.aspect, file);
                    if (spot.cone.show) {
                        WriteDL_SEE_CONE (file);
                    }
                    if (spot.cone.overshoot) {
                        WriteDL_SPOT_OVERSHOOT (file);
                    }
                    if (spot.shadow.type == Light::Spot::Shadow::UseRayTraceShadow) {
                        WriteDL_RAYSHAD (file);
                    }
                    if (spot.shadow.local) {
                        WriteDL_LOCAL_SHADOW2 (spot.shadow, file);
                    }
                    WriteDL_RAY_BIAS (spot.shadow.rayBias, file);
                    if (spot.shadow.cast) {
                        WriteDL_SHADOWED (file);
                    }
                    if (spot.projector.use) {
                        WriteDL_PROJECTOR (spot.projector.bitmap, file);
                    }
                }

                void Editor::WriteDL_SPOT_ROLL (
                        util::f32 roll,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_SPOT_ROLL);
                    chunk << roll;
                }

                void Editor::WriteDL_SHADOWED (util::File &file) const {
                    ChunkWriter chunk (file, DL_SHADOWED);
                }

                void Editor::WriteDL_LOCAL_SHADOW2 (
                        const Light::Spot::Shadow &shadow,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_LOCAL_SHADOW2);
                    chunk << shadow.bias << shadow.filter << shadow.mapSize;
                }

                void Editor::WriteDL_SEE_CONE (util::File &file) const {
                    ChunkWriter chunk (file, DL_SEE_CONE);
                }

                void Editor::WriteDL_SPOT_RECTANGULAR (util::File &file) const {
                    ChunkWriter chunk (file, DL_SPOT_RECTANGULAR);
                }

                void Editor::WriteDL_SPOT_ASPECT (
                        util::f32 aspect,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_SPOT_ASPECT);
                    chunk << aspect;
                }

                void Editor::WriteDL_PROJECTOR (
                        const std::string &bitmap,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_PROJECTOR);
                    chunk << bitmap;
                }

                void Editor::WriteDL_SPOT_OVERSHOOT (util::File &file) const {
                    ChunkWriter chunk (file, DL_SPOT_OVERSHOOT);
                }

                void Editor::WriteDL_RAY_BIAS (
                        util::f32 rayBias,
                        util::File &file) const {
                    ChunkWriter chunk (file, DL_RAY_BIAS);
                    chunk << rayBias;
                }

                void Editor::WriteDL_RAYSHAD (util::File &file) const {
                    ChunkWriter chunk (file, DL_RAYSHAD);
                }

                void Editor::WriteN_CAMERA (
                        const Camera &camera,
                        util::File &file) const {
                    ChunkWriter chunk (file, N_CAMERA);
                    chunk << camera.position << camera.target << camera.roll << camera.lens;
                    if (camera.showCone) {
                        WriteCAM_SEE_CONE (file);
                    }
                    WriteCAM_RANGES (camera.ranges, file);
                }

                void Editor::WriteCAM_SEE_CONE (util::File &file) const {
                    ChunkWriter chunk (file, CAM_SEE_CONE);
                }

                void Editor::WriteCAM_RANGES (
                        const Camera::Ranges &ranges,
                        util::File &file) const {
                    ChunkWriter chunk (file, CAM_RANGES);
                    chunk << ranges.hither << ranges.yon;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
