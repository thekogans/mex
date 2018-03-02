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

#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/Util.h"
#include "thekogans/mex/3ds/io/MaterialsEditor.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void MaterialsEditor::Validate () {
                    // FIXME: implement
                    assert (0);
                }

                void MaterialsEditor::ReadMLIBMAGIC (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MAT_ENTRY: {
                                Material::UniquePtr material (new Material);
                                ReadMAT_ENTRY (*material, chunk);
                                materials.push_back (material.get ());
                                material.release ();
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("MaterialsEditor", "ReadMLIBMAGIC");
                                break;
                        }
                    }
                }

                static void ReadMaterialMap (Material::Map &map, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MAT_MAPNAME:
                                chunk >> map.name;
                                break;
                            case INT_PERCENTAGE:
                                chunk >> map.percent;
                                break;
                            case MAT_BUMP_PERCENT:
                                chunk >> map.bumpPercent;
                                break;
                            case MAT_MAP_TILING:
                                chunk >> map.tilingFlags;
                                break;
                            case MAT_MAP_TEXBLUR:
                                chunk >> map.textureBlur;
                                break;
                            case MAT_MAP_USCALE:
                                chunk >> map.uScale;
                                break;
                            case MAT_MAP_VSCALE:
                                chunk >> map.vScale;
                                break;
                            case MAT_MAP_UOFFSET:
                                chunk >> map.uOffset;
                                break;
                            case MAT_MAP_VOFFSET:
                                chunk >> map.vOffset;
                                break;
                            case MAT_MAP_ANG:
                                chunk >> map.rotation;
                                break;
                            case MAT_MAP_COL1:
                                chunk >> map.tint1;
                                break;
                            case MAT_MAP_COL2:
                                chunk >> map.tint2;
                                break;
                            case MAT_MAP_RCOL:
                                chunk >> map.redTint;
                                break;
                            case MAT_MAP_GCOL:
                                chunk >> map.greenTint;
                                break;
                            case MAT_MAP_BCOL:
                                chunk >> map.blueTint;
                                break;
                            default:
                                chunk.HandleUknownChunk ("MaterialsEditor", "ReadMaterialMap");
                                break;
                        }
                    }
                }

                static util::i16 ReadINT_PERCENTAGE (ChunkReader &mainChunk) {
                    if (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        if (chunk.GetID () == INT_PERCENTAGE) {
                            util::i16 data;
                            chunk >> data;
                            return data;
                        }
                        else {
                            chunk.HandleUknownChunk ("MaterialsEditor", "ReadINT_PERCENTAGE");
                        }
                    }
                    return 0;
                }

                void MaterialsEditor::ReadMAT_ENTRY (Material &material, ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MAT_NAME:
                                chunk >> material.name;
                                break;
                            case MAT_AMBIENT:
                                ReadColor (material.ambient, chunk);
                                break;
                            case MAT_DIFFUSE:
                                ReadColor (material.diffuse, chunk);
                                break;
                            case MAT_SPECULAR:
                                ReadColor (material.specular, chunk);
                                break;
                            case MAT_SHININESS:
                                material.shininess = ReadINT_PERCENTAGE (chunk);
                                break;
                            case MAT_SHIN2PCT:
                                material.shininessStrength = ReadINT_PERCENTAGE (chunk);
                                break;
                            case MAT_TRANSPARENCY:
                                material.transparency = ReadINT_PERCENTAGE (chunk);
                                break;
                            case MAT_XPFALL:
                                material.fallOff = ReadINT_PERCENTAGE (chunk);
                                break;
                            case MAT_REFBLUR:
                                material.blur = ReadINT_PERCENTAGE (chunk);
                                break;
                            case MAT_SELF_ILPCT:
                                material.selfIllumPct = ReadINT_PERCENTAGE (chunk);
                                break;
                            case MAT_WIRESIZE:
                                chunk >> material.wireSize;
                                break;
                            case MAT_SHADING:
                                chunk >> material.shading;
                                break;
                            case MAT_SELF_ILLUM:
                                material.selfIllum = true;
                                break;
                            case MAT_USE_XPFALL:
                                material.useFallOff = true;
                                break;
                            case MAT_USE_REFBLUR:
                                material.useBlur = true;
                                break;
                            case MAT_TWO_SIDE:
                                material.twoSided = true;
                                break;
                            case MAT_ADDITIVE:
                                material.additive = true;
                                break;
                            case MAT_WIRE:
                                material.wire = true;
                                break;
                            case MAT_FACEMAP:
                                material.faceMap = true;
                                break;
                            case MAT_XPFALLIN:
                                material.fallOffOut = false;
                                break;
                            case MAT_PHONGSOFT:
                                material.phongSoften = true;
                                break;
                            case MAT_WIREABS:
                                material.wireAbs = true;
                                break;
                            case MAT_DECAL:
                                material.decal = true;
                                break;
                            case MAT_TEXMAP:
                                ReadMaterialMap (material.textureMap.map, chunk);
                                material.textureMap.useMap = true;
                                break;
                            case MAT_SXP_TEXT_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.textureMap.map.xdata.resize (dataSize);
                                    chunk.Read (&material.textureMap.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_TEXMASK:
                                ReadMaterialMap (material.textureMap.mask, chunk);
                                material.textureMap.useMask = true;
                                break;
                            case MAT_SXP_TEXT_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.textureMap.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.textureMap.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_TEX2MAP:
                                ReadMaterialMap (material.textureMap2.map, chunk);
                                material.textureMap2.useMap = true;
                                break;
                            case MAT_SXP_TEXT2_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.textureMap2.map.xdata.resize (dataSize);
                                    chunk.Read (&material.textureMap2.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_TEX2MASK:
                                ReadMaterialMap (material.textureMap2.mask, chunk);
                                material.textureMap2.useMask = true;
                                break;
                            case MAT_SXP_TEXT2_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.textureMap2.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.textureMap2.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_OPACMAP:
                                ReadMaterialMap (material.opacityMap.map, chunk);
                                material.opacityMap.useMap = true;
                                break;
                            case MAT_SXP_OPAC_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.opacityMap.map.xdata.resize (dataSize);
                                    chunk.Read (&material.opacityMap.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_OPACMASK:
                                ReadMaterialMap (material.opacityMap.mask, chunk);
                                material.opacityMap.useMask = true;
                                break;
                            case MAT_SXP_OPAC_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.opacityMap.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.opacityMap.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_BUMPMAP:
                                ReadMaterialMap (material.bumpMap.map, chunk);
                                material.bumpMap.useMap = true;
                                break;
                            case MAT_SXP_BUMP_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.bumpMap.map.xdata.resize (dataSize);
                                    chunk.Read (&material.bumpMap.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_BUMPMASK:
                                ReadMaterialMap (material.bumpMap.mask, chunk);
                                material.bumpMap.useMask = true;
                                break;
                            case MAT_SXP_BUMP_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.bumpMap.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.bumpMap.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_SPECMAP:
                                ReadMaterialMap (material.specularMap.map, chunk);
                                material.specularMap.useMap = true;
                                break;
                            case MAT_SXP_SPEC_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.specularMap.map.xdata.resize (dataSize);
                                    chunk.Read (&material.specularMap.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_SPECMASK:
                                ReadMaterialMap (material.specularMap.mask, chunk);
                                material.specularMap.useMask = true;
                                break;
                            case MAT_SXP_SPEC_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.specularMap.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.specularMap.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_SHINMAP:
                                ReadMaterialMap (material.shininessMap.map, chunk);
                                material.shininessMap.useMap = true;
                                break;
                            case MAT_SXP_SHIN_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.shininessMap.map.xdata.resize (dataSize);
                                    chunk.Read (&material.shininessMap.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_SHINMASK:
                                ReadMaterialMap (material.shininessMap.mask, chunk);
                                material.shininessMap.useMask = true;
                                break;
                            case MAT_SXP_SHIN_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.shininessMap.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.shininessMap.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_SELFIMAP:
                                ReadMaterialMap (material.illuminationMap.map, chunk);
                                material.illuminationMap.useMap = true;
                                break;
                            case MAT_SXP_SELFI_DATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.illuminationMap.map.xdata.resize (dataSize);
                                    chunk.Read (&material.illuminationMap.map.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_SELFIMASK:
                                ReadMaterialMap (material.illuminationMap.mask, chunk);
                                material.illuminationMap.useMask = true;
                                break;
                            case MAT_SXP_SELFI_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.illuminationMap.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.illuminationMap.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_REFLMAP:
                                ReadMaterialMap (material.reflectionMap.mapSet.map, chunk);
                                material.reflectionMap.mapSet.useMap = true;
                                break;
                            case MAT_REFLMASK:
                                ReadMaterialMap (material.reflectionMap.mapSet.mask, chunk);
                                material.reflectionMap.mapSet.useMask = true;
                                break;
                            case MAT_SXP_REFL_MASKDATA: {
                                util::ui32 dataSize = chunk.GetLength () - 6;
                                if (dataSize) {
                                    material.reflectionMap.mapSet.mask.xdata.resize (dataSize);
                                    chunk.Read (&material.reflectionMap.mapSet.mask.xdata[0], dataSize);
                                }
                                break;
                            }
                            case MAT_ACUBIC:
                                chunk >>
                                    material.reflectionMap.autoCubic.shade >>
                                    material.reflectionMap.autoCubic.antiAlias >>
                                    material.reflectionMap.autoCubic.flags >>
                                    material.reflectionMap.autoCubic.size >>
                                    material.reflectionMap.autoCubic.nthFrame;
                                material.reflectionMap.autoCubic.use = true;
                                break;
                            default:
                                chunk.HandleUknownChunk ("MaterialsEditor", "ReadMAT_ENTRY");
                                break;
                        }
                    }
                }

                void MaterialsEditor::WriteMLIBMAGIC (util::File &file) const {
                    ChunkWriter chunk (file, MLIBMAGIC);
                    for (size_t i = 0, count = materials.size (); i < count; ++i) {
                        WriteMAT_ENTRY (*materials[i], file);
                    }
                }

                static void WriteMAT_NAME (const std::string &name, util::File &file) {
                    ChunkWriter chunk (file, MAT_NAME);
                    chunk << name;
                }

                static void WriteINT_PERCENTAGE (util::i16 percent, util::File &file);

                static void WriteMAT_SHININESS (util::i16 shininess, util::File &file) {
                    ChunkWriter chunk (file, MAT_SHININESS);
                    WriteINT_PERCENTAGE (shininess, file);
                }

                static void WriteMAT_SHIN2PCT (util::i16 shininessStrength, util::File &file) {
                    ChunkWriter chunk (file, MAT_SHIN2PCT);
                    WriteINT_PERCENTAGE (shininessStrength, file);
                }

                static void WriteMAT_TRANSPARENCY (util::i16 transparency, util::File &file) {
                    ChunkWriter chunk (file, MAT_TRANSPARENCY);
                    WriteINT_PERCENTAGE (transparency, file);
                }

                static void WriteMAT_XPFALL (util::i16 fallOff, util::File &file) {
                    ChunkWriter chunk (file, MAT_XPFALL);
                    WriteINT_PERCENTAGE (fallOff, file);
                }

                static void WriteMAT_REFBLUR (util::i16 blur, util::File &file) {
                    ChunkWriter chunk (file, MAT_REFBLUR);
                    WriteINT_PERCENTAGE (blur, file);
                }

                static void WriteMAT_SELF_ILPCT (util::i16 selfIllumPct, util::File &file) {
                    ChunkWriter chunk (file, MAT_SELF_ILPCT);
                    WriteINT_PERCENTAGE (selfIllumPct, file);
                }

                static void WriteMAT_WIRESIZE (util::f32 wireSize, util::File &file) {
                    ChunkWriter chunk (file, MAT_WIRESIZE);
                    chunk << wireSize;
                }

                static void WriteMAT_SHADING (util::i16 shading, util::File &file) {
                    ChunkWriter chunk (file, MAT_SHADING);
                    chunk << shading;
                }

                static void WriteMAT_SELF_ILLUM (util::File &file) {
                    ChunkWriter chunk (file, MAT_SELF_ILLUM);
                }

                static void WriteMAT_USE_XPFALL (util::File &file) {
                    ChunkWriter chunk (file, MAT_USE_XPFALL);
                }

                static void WriteMAT_USE_REFBLUR (util::File &file) {
                    ChunkWriter chunk (file, MAT_USE_REFBLUR);
                }

                static void WriteMAT_TWO_SIDE (util::File &file) {
                    ChunkWriter chunk (file, MAT_TWO_SIDE);
                }

                static void WriteMAT_ADDITIVE (util::File &file) {
                    ChunkWriter chunk (file, MAT_ADDITIVE);
                }

                static void WriteMAT_WIRE (util::File &file) {
                    ChunkWriter chunk (file, MAT_WIRE);
                }

                static void WriteMAT_FACEMAP (util::File &file) {
                    ChunkWriter chunk (file, MAT_FACEMAP);
                }

                static void WriteMAT_XPFALLIN (util::File &file) {
                    ChunkWriter chunk (file, MAT_XPFALLIN);
                }

                static void WriteMAT_PHONGSOFT (util::File &file) {
                    ChunkWriter chunk (file, MAT_PHONGSOFT);
                }

                static void WriteMAT_WIREABS (util::File &file) {
                    ChunkWriter chunk (file, MAT_WIREABS);
                }

                static void WriteMAT_DECAL (util::File &file) {
                    ChunkWriter chunk (file, MAT_DECAL);
                }

                static void WriteMAT_MAPNAME (const std::string &name, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAPNAME);
                    chunk << name;
                }

                static void WriteINT_PERCENTAGE (util::i16 percent, util::File &file) {
                    ChunkWriter chunk (file, INT_PERCENTAGE);
                    chunk << percent;
                }

                static void WriteMAT_BUMP_PERCENT (util::i16 bumpPercent, util::File &file) {
                    ChunkWriter chunk (file, MAT_BUMP_PERCENT);
                    chunk << bumpPercent;
                }

                static void WriteMAT_MAP_TILING (util::ui16 tilingFlags, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_TILING);
                    chunk << tilingFlags;
                }

                static void WriteMAT_MAP_TEXBLUR (util::f32 textureBlur, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_TEXBLUR);
                    chunk << textureBlur;
                }

                static void WriteMAT_MAP_USCALE (util::f32 uScale, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_USCALE);
                    chunk << uScale;
                }

                static void WriteMAT_MAP_VSCALE (util::f32 vScale, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_VSCALE);
                    chunk << vScale;
                }

                static void WriteMAT_MAP_UOFFSET (util::f32 uOffset, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_UOFFSET);
                    chunk << uOffset;
                }

                static void WriteMAT_MAP_VOFFSET (util::f32 vOffset, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_VOFFSET);
                    chunk << vOffset;
                }

                static void WriteMAT_MAP_ANG (util::f32 rotation, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_ANG);
                    chunk << rotation;
                }

                static void WriteMAT_MAP_COL1 (const opengl::ui8Color &tint1, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_COL1);
                    chunk << tint1;
                }

                static void WriteMAT_MAP_COL2 (const opengl::ui8Color &tint2, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_COL2);
                    chunk << tint2;
                }

                static void WriteMAT_MAP_RCOL (const opengl::ui8Color &redTint, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_RCOL);
                    chunk << redTint;
                }

                static void WriteMAT_MAP_GCOL (const opengl::ui8Color &greenTint, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_GCOL);
                    chunk << greenTint;
                }

                static void WriteMAT_MAP_BCOL (const opengl::ui8Color &blueTint, util::File &file) {
                    ChunkWriter chunk (file, MAT_MAP_BCOL);
                    chunk << blueTint;
                }

                static void WriteMaterialMap (util::i16 id, const Material::Map &map, util::File &file) {
                    ChunkWriter chunk (file, id);
                    WriteMAT_MAPNAME (map.name, file);
                    WriteINT_PERCENTAGE (map.percent, file);
                    WriteMAT_BUMP_PERCENT (map.bumpPercent, file);
                    WriteMAT_MAP_TILING (map.tilingFlags, file);
                    WriteMAT_MAP_TEXBLUR (map.textureBlur, file);
                    WriteMAT_MAP_USCALE (map.uScale, file);
                    WriteMAT_MAP_VSCALE (map.vScale, file);
                    WriteMAT_MAP_UOFFSET (map.uOffset, file);
                    WriteMAT_MAP_VOFFSET (map.vOffset, file);
                    WriteMAT_MAP_ANG (map.rotation, file);
                    WriteMAT_MAP_COL1 (map.tint1, file);
                    WriteMAT_MAP_COL2 (map.tint2, file);
                    WriteMAT_MAP_RCOL (map.redTint, file);
                    WriteMAT_MAP_GCOL (map.greenTint, file);
                    WriteMAT_MAP_BCOL (map.blueTint, file);
                }

                static void WriteMaterialMapData (util::i16 id, const std::vector<util::ui8> &xdata, util::File &file) {
                    ChunkWriter chunk (file, id);
                    chunk.Write (&xdata[0], (util::ui32)xdata.size ());
                }

                static void WriteMAT_ACUBIC (const Material::ReflectionMapSet::AutoCubic &autoCubic, util::File &file) {
                    ChunkWriter chunk (file, MAT_ACUBIC);
                    chunk << autoCubic.shade << autoCubic.antiAlias <<
                        autoCubic.flags << autoCubic.size << autoCubic.nthFrame;
                }

                void MaterialsEditor::WriteMAT_ENTRY (const Material &material, util::File &file) {
                    ChunkWriter chunk (file, MAT_ENTRY);
                    WriteMAT_NAME (material.name, file);
                    WriteColor (MAT_AMBIENT, material.ambient, file);
                    WriteColor (MAT_DIFFUSE, material.diffuse, file);
                    WriteColor (MAT_SPECULAR, material.specular, file);
                    WriteMAT_SHININESS (material.shininess, file);
                    WriteMAT_SHIN2PCT (material.shininessStrength, file);
                    WriteMAT_TRANSPARENCY (material.transparency, file);
                    WriteMAT_XPFALL (material.fallOff, file);
                    WriteMAT_REFBLUR (material.blur, file);
                    WriteMAT_SELF_ILPCT (material.selfIllumPct, file);
                    WriteMAT_WIRESIZE (material.wireSize, file);
                    WriteMAT_SHADING (material.shading, file);
                    if (material.selfIllum) {
                        WriteMAT_SELF_ILLUM (file);
                    }
                    if (material.useFallOff) {
                        WriteMAT_USE_XPFALL (file);
                    }
                    if (material.useBlur) {
                        WriteMAT_USE_REFBLUR (file);
                    }
                    if (material.twoSided) {
                        WriteMAT_TWO_SIDE (file);
                    }
                    if (material.additive) {
                        WriteMAT_ADDITIVE (file);
                    }
                    if (material.wire) {
                        WriteMAT_WIRE (file);
                    }
                    if (material.faceMap) {
                        WriteMAT_FACEMAP (file);
                    }
                    if (material.fallOffOut) {
                        WriteMAT_XPFALLIN (file);
                    }
                    if (material.phongSoften) {
                        WriteMAT_PHONGSOFT (file);
                    }
                    if (material.wireAbs) {
                        WriteMAT_WIREABS (file);
                    }
                    if (material.decal) {
                        WriteMAT_DECAL (file);
                    }
                    if (material.textureMap.useMap) {
                        WriteMaterialMap (MAT_TEXMAP, material.textureMap.map, file);
                    }
                    if (!material.textureMap.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_TEXT_DATA, material.textureMap.map.xdata, file);
                    }
                    if (material.textureMap.useMask) {
                        WriteMaterialMap (MAT_TEXMASK, material.textureMap.mask, file);
                    }
                    if (!material.textureMap.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_TEXT_MASKDATA, material.textureMap.mask.xdata, file);
                    }
                    if (material.textureMap2.useMap) {
                        WriteMaterialMap (MAT_TEX2MAP, material.textureMap2.map, file);
                    }
                    if (!material.textureMap2.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_TEXT2_DATA, material.textureMap2.map.xdata, file);
                    }
                    if (material.textureMap2.useMask) {
                        WriteMaterialMap (MAT_TEX2MASK, material.textureMap2.mask, file);
                    }
                    if (!material.textureMap2.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_TEXT2_MASKDATA, material.textureMap2.mask.xdata, file);
                    }
                    if (material.opacityMap.useMap) {
                        WriteMaterialMap (MAT_OPACMAP, material.opacityMap.map, file);
                    }
                    if (!material.opacityMap.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_OPAC_DATA, material.opacityMap.map.xdata, file);
                    }
                    if (material.opacityMap.useMask) {
                        WriteMaterialMap (MAT_OPACMASK, material.opacityMap.mask, file);
                    }
                    if (!material.opacityMap.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_OPAC_MASKDATA, material.opacityMap.mask.xdata, file);
                    }
                    if (material.bumpMap.useMap) {
                        WriteMaterialMap (MAT_BUMPMAP, material.bumpMap.map, file);
                    }
                    if (!material.bumpMap.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_BUMP_DATA, material.bumpMap.map.xdata, file);
                    }
                    if (material.bumpMap.useMask) {
                        WriteMaterialMap (MAT_BUMPMASK, material.bumpMap.mask, file);
                    }
                    if (!material.bumpMap.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_BUMP_MASKDATA, material.bumpMap.mask.xdata, file);
                    }
                    if (material.specularMap.useMap) {
                        WriteMaterialMap (MAT_SPECMAP, material.specularMap.map, file);
                    }
                    if (!material.specularMap.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_SPEC_DATA, material.specularMap.map.xdata, file);
                    }
                    if (material.specularMap.useMask) {
                        WriteMaterialMap (MAT_SPECMASK, material.specularMap.mask, file);
                    }
                    if (!material.specularMap.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_SPEC_MASKDATA, material.specularMap.mask.xdata, file);
                    }
                    if (material.shininessMap.useMap) {
                        WriteMaterialMap (MAT_SHINMAP, material.shininessMap.map, file);
                    }
                    if (!material.shininessMap.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_SHIN_DATA, material.shininessMap.map.xdata, file);
                    }
                    if (material.shininessMap.useMask) {
                        WriteMaterialMap (MAT_SHINMASK, material.shininessMap.mask, file);
                    }
                    if (!material.shininessMap.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_SHIN_MASKDATA, material.shininessMap.mask.xdata, file);
                    }
                    if (material.illuminationMap.useMap) {
                        WriteMaterialMap (MAT_SELFIMAP, material.illuminationMap.map, file);
                    }
                    if (!material.illuminationMap.map.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_SELFI_DATA, material.illuminationMap.map.xdata, file);
                    }
                    if (material.illuminationMap.useMask) {
                        WriteMaterialMap (MAT_SELFIMASK, material.illuminationMap.mask, file);
                    }
                    if (!material.illuminationMap.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_SELFI_MASKDATA, material.illuminationMap.mask.xdata, file);
                    }
                    if (material.reflectionMap.mapSet.useMap) {
                        WriteMaterialMap (MAT_REFLMAP, material.reflectionMap.mapSet.map, file);
                    }
                    if (material.reflectionMap.mapSet.useMask) {
                        WriteMaterialMap (MAT_REFLMASK, material.reflectionMap.mapSet.mask, file);
                    }
                    if (!material.reflectionMap.mapSet.mask.xdata.empty ()) {
                        WriteMaterialMapData (MAT_SXP_REFL_MASKDATA, material.reflectionMap.mapSet.mask.xdata, file);
                    }
                    if (material.reflectionMap.autoCubic.use) {
                        WriteMAT_ACUBIC (material.reflectionMap.autoCubic, file);
                    }
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
