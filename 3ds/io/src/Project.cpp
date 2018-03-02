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

#include <iostream>
#include "thekogans/util/Types.h"
#include "thekogans/util/ByteSwap.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/Project.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                namespace {
                    // ChunkReader and ChunkWriter read and write various
                    // multi byte values. This check ensures that these
                    // values will be of proper size.
                    inline bool CheckUtilTypes () {
                        static const bool utilTypesOk =
                            util::I8_SIZE == 1 && util::UI8_SIZE == 1 &&
                            util::I16_SIZE == 2 && util::UI16_SIZE == 2 &&
                            util::I32_SIZE == 4 && util::UI32_SIZE == 4 &&
                            util::I64_SIZE == 8 && util::UI64_SIZE == 8 &&
                            util::F32_SIZE == 4 && util::F64_SIZE == 8;
                        return utilTypesOk;
                    }
                }

                void Project::BadEnvironment::PrintEnvironment () {
                    std::cout <<
                        "I8_SIZE = " << util::I8_SIZE << std::endl <<
                        "UI8_SIZE = " << util::UI8_SIZE << std::endl <<
                        "I16_SIZE = " << util::I16_SIZE << std::endl <<
                        "UI16_SIZE = " << util::UI16_SIZE << std::endl <<
                        "I32_SIZE = " << util::I32_SIZE << std::endl <<
                        "UI32_SIZE = " << util::UI32_SIZE << std::endl <<
                        "I64_SIZE = " << util::I64_SIZE << std::endl <<
                        "UI64_SIZE = " << util::UI64_SIZE << std::endl <<
                        "F32_SIZE = " << util::F32_SIZE << std::endl <<
                        "F64_SIZE = " << util::F64_SIZE << std::endl;
                }

                Project::Project () :
                        shaper (*this),
                        lofter (*this),
                        m3dVersion (3),
                        editor (*this),
                        keyframer (*this) {
                    if (!CheckUtilTypes ()) {
                        throw BadEnvironment ();
                    }
                }

                void Project::Swap (Project &project) {
                    programConfiguration.Swap (project.programConfiguration);
                    systemOptions.Swap (project.systemOptions);
                    gammaControl.Swap (project.gammaControl);
                    dxf.Swap (project.dxf);
                    drawingAids.Swap (project.drawingAids);
                    units.Swap (project.units);
                    backgroundContrast.Swap (project.backgroundContrast);
                    networkParameters.Swap (project.networkParameters);
                    display.Swap (project.display);
                    constructionPlane.Swap (project.constructionPlane);
                    render.Swap (project.render);
                    shaper.Swap (project.shaper);
                    lofter.Swap (project.lofter);
                    std::swap (m3dVersion, project.m3dVersion);
                    editor.Swap (project.editor);
                    keyframer.Swap (project.keyframer);
                    materialsEditor.Swap (project.materialsEditor);
                    videoPost.Swap (project.videoPost);
                }

                Project::FileType Project::Open (
                        const std::string &path,
                        Progress *progress,
                        ErrorHandler *errorHandler) {
                    Project project;
                    Project::FileType fileType = project.OpenHelper (
                        path, progress, errorHandler);
                    switch (fileType) {
                        case Unknown:
                            if (errorHandler != 0) {
                                errorHandler->HandleGenericError ("Unknown file type!");
                            }
                            break;
                        case PRJ:
                            Swap (project);
                            break;
                        case SHP:
                            shaper.Swap (project.shaper);
                            break;
                        case LFT:
                            lofter.Swap (project.lofter);
                            break;
                        case _3DS:
                            std::swap (m3dVersion, project.m3dVersion);
                            editor.Swap (project.editor);
                            keyframer.Swap (project.keyframer);
                            break;
                        case MLI:
                            materialsEditor.Swap (project.materialsEditor);
                            break;
                        case VP:
                            videoPost.Swap (project.videoPost);
                            break;
                    }
                    return fileType;
                }

                Project::FileType Project::OpenHelper (
                        const std::string &path,
                        Progress *progress,
                        ErrorHandler *errorHandler) {
                    Project::FileType fileType = Unknown;
                    util::ReadOnlyFile file (util::LittleEndian, path);
                    ChunkReader mainChunk (file, progress, errorHandler);
                    switch (mainChunk.GetID ()) {
                        case CMAGIC:
                            ReadCMAGIC (mainChunk);
                            fileType = PRJ;
                            break;
                        case SMAGIC:
                            shaper.ReadSMAGIC (mainChunk);
                            fileType = SHP;
                            break;
                        case LMAGIC:
                            lofter.ReadLMAGIC (mainChunk);
                            fileType = LFT;
                            break;
                        case M3DMAGIC:
                            ReadM3DMAGIC (mainChunk);
                            fileType = _3DS;
                            break;
                        case MDATA:
                            editor.ReadMDATA (mainChunk);
                            fileType = _3DS;
                            break;
                        case KFDATA:
                            keyframer.ReadKFDATA (mainChunk);
                            fileType = _3DS;
                            break;
                        case MLIBMAGIC:
                            materialsEditor.ReadMLIBMAGIC (mainChunk);
                            fileType = MLI;
                            break;
                        case VPDATA:
                            videoPost.ReadVPDATA (mainChunk);
                            fileType = VP;
                            break;
                        default:
                            mainChunk.HandleUknownChunk ("Project", "Open");
                            break;
                    }
                    return fileType;
                }

                void Project::SavePRJ (const std::string &path) const {
                    util::File file (util::LittleEndian, path);
                    WriteCMAGIC (file);
                }

                void Project::SaveSHP (const std::string &path) const {
                    util::File file (util::LittleEndian, path);
                    shaper.WriteSMAGIC (file);
                }

                void Project::SaveLFT (const std::string &path) const {
                    util::File file (util::LittleEndian, path);
                    lofter.WriteLMAGIC (file);
                }

                void Project::Save3DS (const std::string &path) const {
                    util::File file (util::LittleEndian, path);
                    ChunkWriter chunk (file, M3DMAGIC);
                    WriteM3D_VERSION (file);
                    editor.WriteMDATA (file);
                    keyframer.WriteKFDATA (file);
                }

                void Project::SaveMLI (const std::string &path) const {
                    util::File file (util::LittleEndian, path);
                    materialsEditor.WriteMLIBMAGIC (file);
                }

                void Project::SaveVP (const std::string &path) const {
                    util::File file (util::LittleEndian, path);
                    videoPost.WriteVPDATA (file);
                }

                // FIXME: implement
                void Project::ReadCMAGIC (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case C_IN_DEVICE:
                                chunk >> programConfiguration.inputDevice;
                                break;
                            case C_MSCALE:
                                chunk >> programConfiguration.mouseSpeed;
                                break;
                            case C_COMM_PORT:
                                chunk >> programConfiguration.commPort;
                                break;
                            case C_TAB_BASES:
                                chunk >> programConfiguration.tablet.left >> programConfiguration.tablet.top;
                                break;
                            case C_TAB_DIVS:
                                chunk >> programConfiguration.tablet.right >> programConfiguration.tablet.bottom;
                                break;
                            case C_PICKSIZE:
                                chunk >> programConfiguration.pickBoxSize;
                                break;
                            case C_BACKUP_FILE:
                                chunk >> systemOptions.backupFile;
                                break;
                            case C_SEL_ITEMTOG:
                                chunk >> systemOptions.regionToggle;
                                break;
                            case C_SEL_RESET:
                                chunk >> systemOptions.selectedReset;
                                break;
                            case C_WELD_THRESHOLD:
                                chunk >> systemOptions.weldThreshold;
                                break;
                            case C_STICKY_KEYINF:
                                chunk >> systemOptions.modalKFButtons;
                                break;
                            case C_VIEW_PRES_RATIO:
                                chunk >> systemOptions.viewPreserveRatio;
                                break;
                            case C_BGND_PRES_RATIO:
                                chunk >> systemOptions.backgroundPreserveRatio;
                                break;
                            case C_TENS_DEFAULT:
                                chunk >> systemOptions.tensionDefault;
                                break;
                            case C_CONT_DEFAULT:
                                chunk >> systemOptions.continuityDefault;
                                break;
                            case C_BIAS_DEFAULT:
                                chunk >> systemOptions.biasDefault;
                                break;
                            case C_DISP_GAMMA:
                                chunk >> gammaControl.display;
                                break;
                            case C_FBUF_GAMMA:
                                chunk >> gammaControl.frameBuffer;
                                break;
                            case C_FILE_OUT_GAMMA:
                                chunk >> gammaControl.outFile;
                                break;
                            case C_FILE_IN_GAMMA:
                                chunk >> gammaControl.inFile;
                                break;
                            case C_GAMMA_CORRECT:
                                chunk >> gammaControl.use;
                                break;
                            case C_DXFNAME_SRC:
                                chunk >> dxf.derive;
                                break;
                            case C_AUTO_WELD:
                                chunk >> dxf.weldVertices;
                                break;
                            case C_AUTO_UNIFY:
                                chunk >> dxf.unifyNormals;
                                break;
                            case C_AUTO_SMOOTH:
                                chunk >> dxf.autoSmooth;
                                break;
                            case C_DXF_SMOOTH_ANG:
                                chunk >> dxf.smoothingAngle;
                                break;
                            case C_SMOOTH_ANG:
                                chunk >> dxf.arcAngle;
                                break;
                            case C_SNAPS:
                                chunk >> drawingAids.snapSpacing;
                                break;
                            case C_GRIDS:
                                chunk >> drawingAids.gridSpacing;
                                break;
                            case C_GRID_RANGE:
                                chunk >> drawingAids.gridStart >> drawingAids.gridEnd;
                                break;
                            case C_ASNAPS:
                                chunk >> drawingAids.angleSnap >> drawingAids.useAngleSnap;
                                break;
                            case C_NUM_FORMAT:
                                chunk >> units.type;
                                break;
                            case C_ARCH_DENOM:
                                chunk >> units.denominator;
                                break;
                            case C_MASTER_SCALES:
                                chunk >> units.scale >> units.conv2in >> units.metric;
                                break;
                            case C_LO_CONTRAST:
                                chunk >> backgroundContrast.dark;
                                break;
                            case C_HI_CONTRAST:
                                chunk >> backgroundContrast.light;
                                break;
                            case C_ALL_LINES:
                                chunk >> display.showAllLines;
                                break;
                            case C_BACK_TYPE:
                                chunk >> display.seeThrough;
                                break;
                            case C_KF_SHOW_BACKFACE:
                                chunk >> display.kfSeeThrough;
                                break;
                            case C_VERTMARK:
                                chunk >> display.vertexTicks;
                                break;
                            case C_DETAIL:
                                chunk >> display.detail;
                                break;
                            case C_SEE_LIGHTS:
                                chunk >> display.showLights;
                                break;
                            case C_SEE_CAMERAS:
                                chunk >> display.showCameras;
                                break;
                            case C_QUIKSTUFF:
                                chunk >> display.speedCount >> display.fastDraw;
                                break;
                            case C_CONSTS:
                                chunk >> constructionPlane.plane;
                                break;
                            case C_MSHCP:
                                chunk >> constructionPlane.visible >> constructionPlane.plane;
                                break;

                            case C_RGB_RMODE:
                                chunk >> render.shadingLimit;
                                break;
                            case C_RGB_AA:
                                chunk >> render.antialiasing;
                                break;
                            case C_RND_MIP:
                                chunk >> render.filterMaps;
                                break;
                            case C_RGB_SHADOW:
                                chunk >> render.shadows;
                                break;
                            case C_RGB_MAPSW:
                                chunk >> render.mapping;
                                break;
                            case C_AUTO_REFLECT:
                                chunk >> render.autoReflect;
                                break;
                            case C_RGB_TWOSIDE:
                                chunk >> render.force2Sided;
                                break;
                            case C_FORCE_WIRE:
                                chunk >> render.forceWire;
                                break;
                            case C_RGB_HIDE:
                                chunk >> render.hiddenGeometry;
                                break;
                            case C_BGTYPE:
                                chunk >> render.background;
                                break;

                            case C_RGB_PICTYPE:
                                chunk >> render.deviceConfiguration.fileOutputStill;
                                break;
                            case C_RGB_ANIMTYPE:
                                chunk >> render.deviceConfiguration.fileOutputAnimation;
                                break;
                            case C_RGB_COMPRESS:
                                chunk >> render.deviceConfiguration.compressed;
                                break;
                            case C_JPEG_COMPRESSION:
                                chunk >> render.deviceConfiguration.jpegCompression;
                                break;

                            //bool imageFileComments;

                            case C_RGB_DISPDEV:
                                chunk >> render.deviceConfiguration.display;
                                break;
                            case C_RGB_HARDDEV:
                                chunk >> render.deviceConfiguration.hardcopy;
                                break;
                            case C_PAL_TYPE:
                                chunk >> render.deviceConfiguration.paletteType;
                                break;
                            case C_RGB_OVW:
                                chunk >> render.deviceConfiguration.resolution.width;
                                break;
                            case C_RGB_OVH:
                                chunk >> render.deviceConfiguration.resolution.height;
                                break;
                            case C_RGB_OVASPECT:
                                chunk >> render.deviceConfiguration.resolution.aspect;
                                break;

                            case C_BLUR_FRAMES:
                                chunk >> render.options.objectMotionBlur.number;
                                break;
                            case C_BLUR_SAMPLES:
                                chunk >> render.options.objectMotionBlur.samples;
                                break;
                            case C_BLUR_DUR:
                                chunk >> render.options.objectMotionBlur.duration;
                                break;
                            case C_MBLUR_DITHER:
                                chunk >> render.options.motionBlurDither;
                                break;
                            case C_REND_FIELDS:
                                chunk >> render.options.renderFields;
                                break;
                            case C_HOT_CHECK:
                                chunk >> render.options.videoColorCheck;
                                break;
                            case C_HOT_METHOD:
                                chunk >> render.options.method;
                                break;
                            case C_DITHER_24:
                                chunk >> render.options.ditherTrueColor;
                                break;
                            case C_SUPER_BLACK:
                                chunk >> render.options.superBlack;
                                break;
                            case C_DITHER_256:
                                chunk >> render.options.dither256;
                                break;
                            case C_REFLIP:
                                chunk >> render.options.reflectFlip;
                                break;
                            case C_USE_ALPHA:
                                chunk >> render.options.renderAlpha;
                                break;
                            case C_ALPHA_SPLIT:
                                chunk >> render.options.alphaSplit;
                                break;
                            case C_TGA_DEPTH:
                                chunk >> render.options.tgaDepth;
                                break;
                            case C_SAVE_LAST:
                                chunk >> render.options.saveLastImage;
                                break;
                            case C_NTH_SERIAL_NUM:
                                chunk >> render.options.nthSerialNumbering;
                                break;
                            case C_ZCLIP_POINT:
                                chunk >> render.options.zClipNear;
                                break;
                            case C_SAFE_FRAME:
                                chunk >> render.options.safeFrame;
                                break;
                            case C_PIXEL_SIZE:
                                chunk >> render.options.pixelSize;
                                break;
                            case C_RENDER_ALL:
                                chunk >> render.frames;
                                break;
                            case C_REND_FROM:
                                chunk >> render.from;
                                break;
                            case C_REND_TO:
                                chunk >> render.to;
                                break;
                            case C_REND_NTH:
                                chunk >> render.nth;
                                break;
                            case C_RGB_OUTPUT:
                                chunk >> render.output;
                                break;
                            case C_RGB_TODISK:
                                chunk >> render.disk;
                                break;

                            // *** Shaper ***
                            case C_SHPDRAWER:
                                chunk >> shaper.openFileDirectory;
                                break;
                            case C_SHPFILE:
                                chunk >> shaper.openFileName;
                                break;
                            case C_STAPE:
                                chunk >> shaper.tape;
                                break;
                            case C_RAX:
                                chunk >> shaper.axis.visible >> shaper.axis.axis;
                                break;
                            case C_SHOOK:
                                chunk >> shaper.hook.visible >> shaper.hook.pt;
                                break;
                            case C_SHOW_1STVERT:
                                chunk >> shaper.showFirst;
                                break;
                            case C_MODICOUNT:
                                chunk >> shaper.steps;
                                break;

                            //util::ui16 freehandSmoothness;

                            case C_NGPARMS:
                                chunk >> shaper.create.ngonVertices;
                                break;
                            case C_TXDRAWER:
                                chunk >> shaper.create.fontsPath;
                                break;
                            case C_TXFILE:
                                chunk >> shaper.create.fontFile;
                                break;
                            case C_TX_STRING:
                                chunk >> shaper.create.text;
                                break;
                            case C_POLYSEL:
                                chunk >> shaper.selectMask;
                                break;
                            case C_POLYUNSEL:
                                chunk >> shaper.unselectMask;
                                break;

                            // *** Lofter ***
                            case C_MODDRAWER:
                                chunk >> lofter.openFileDirectory;
                                break;
                            case C_MODFILE:
                                chunk >> lofter.openFileName;
                                break;
                            case C_LTAPE:
                                chunk >> lofter.tape;
                                break;
                            case C_SEE_3D:
                                chunk >> lofter.show3D;
                                break;
                            case C_PTHLEVEL:
                                chunk >> lofter.path.level;
                                break;
                            case C_SRDIAM:
                                chunk >> lofter.surfaceOfRevolution.diameter;
                                break;
                            case C_SRDEG:
                                chunk >> lofter.surfaceOfRevolution.degrees;
                                break;
                            case C_SRSEG:
                                chunk >> lofter.surfaceOfRevolution.vertices;
                                break;
                            case C_SRDIR:
                                chunk >> lofter.surfaceOfRevolution.direction;
                                break;
                            case C_HETOP:
                                chunk >> lofter.helix.startDiameter;
                                break;
                            case C_HEBOT:
                                chunk >> lofter.helix.endDiameter;
                                break;
                            case C_HEHT:
                                chunk >> lofter.helix.height;
                                break;
                            case C_HETURNS:
                                chunk >> lofter.helix.turns;
                                break;
                            case C_HEDEG:
                                chunk >> lofter.helix.degrees;
                                break;
                            case C_HESEG:
                                chunk >> lofter.helix.vertices;
                                break;
                            case C_HEDIR:
                                chunk >> lofter.helix.direction;
                                break;
                            case C_MSCSYM:
                                chunk >> lofter.scaleDeformation.symmetry;
                                break;
                            case C_MTTSYM:
                                chunk >> lofter.teeterDeformation.symmetry;
                                break;
                            case C_MFTSYM:
                                chunk >> lofter.fitDeformation.symmetry;
                                break;
                            case C_MD_CS:
                                chunk >> lofter.modelParams.capStart;
                                break;
                            case C_MD_CE:
                                chunk >> lofter.modelParams.capEnd;
                                break;
                            case C_MD_SML:
                                chunk >> lofter.modelParams.smoothLength;
                                break;
                            case C_MD_SMW:
                                chunk >> lofter.modelParams.smoothWidth;
                                break;
                            case C_LOFT_WITH_TEXTURE:
                                chunk >> lofter.modelParams.mapping.use;
                                break;
                            case C_LOFT_L_REPEAT:
                                chunk >> lofter.modelParams.mapping.length;
                                break;
                            case C_LOFT_UV_NORMALIZE:
                                chunk >> lofter.modelParams.mapping.normalizeLength;
                                break;
                            case C_LOFT_W_REPEAT:
                                chunk >> lofter.modelParams.mapping.perimeter;
                                break;
                            case C_OPTIMIZE_LOFT:
                                chunk >> lofter.modelParams.optimize;
                                break;
                            case C_WELD_LOFT:
                                chunk >> lofter.modelParams.weldVertices;
                                break;
                            case C_MD_PDET:
                                chunk >> lofter.modelParams.pathDetail;
                                break;
                            case C_MD_SDET:
                                chunk >> lofter.modelParams.shapeDetail;
                                break;

                            // *** Editor ***
                            case C_MDRAWER:
                                chunk >> editor.openFileDirectory;
                                break;
                            case C_MFILE:
                                chunk >> editor.openFileName;
                                break;
                            case C_ETAPE:
                                chunk >> editor.tape;
                                break;
                            case C_MSHAX:
                                chunk >> editor.axis.visible >> editor.axis.axis;
                                break;
                            case C_USERAX:
                                // FIXME: implement
                                //chunk >> editor.userView.visible >> editor.userView.center;
                                break;
                            case C_SPHSEGS:
                                chunk >> editor.create.lsphereSegments;
                                break;
                            case C_GEOSMOOTH:
                                chunk >> editor.create.gsphereSegments;
                                break;
                            case C_HEMISEGS:
                                chunk >> editor.create.hemisphereSegments;
                                break;
                            case C_PRISMSEGS:
                                chunk >> editor.create.cylinder.segments;
                                break;
                            case C_PRISMSIDES:
                                chunk >> editor.create.cylinder.sides;
                                break;
                            case C_TUBESEGS:
                                chunk >> editor.create.tube.segments;
                                break;
                            case C_TUBESIDES:
                                chunk >> editor.create.tube.sides;
                                break;
                            case C_TORSEGS:
                                chunk >> editor.create.torus.segments;
                                break;
                            case C_TORSIDES:
                                chunk >> editor.create.torus.sides;
                                break;
                            case C_CONESEGS:
                                chunk >> editor.create.cone.segments;
                                break;
                            case C_CONESIDES:
                                chunk >> editor.create.cone.sides;
                                break;
                            case C_TESS_TYPE:
                                chunk >> editor.create.objectTesselate.type;
                                break;
                            case C_TESS_TENSION:
                                chunk >> editor.create.objectTesselate.tension;
                                break;
                            case C_MAPTYPE:
                                chunk >> editor.map.type;
                                break;
                            case C_MAP_DISPLAY:
                                chunk >> editor.map.visible;
                                break;
                            case C_TILE_XY:
                                chunk >> editor.map.tile;
                                break;
                            case C_MAP_XYZ:
                                chunk >> editor.map.center;
                                break;
                            case C_MAP_SCALE:
                                chunk >> editor.map.scale;
                                break;
                            case C_MAP_MATRIX:
                                chunk >> editor.map.xform;
                                break;
                            case C_MAP_WID_HT:
                                chunk >> editor.map.planarWidth >>
                                    editor.map.planarHeight >> editor.map.cylindricalHeight;
                                break;
                            case C_SMOOTHING:
                                chunk >> editor.smoothingGroup.group >>
                                    editor.smoothingGroup.mask1 >> editor.smoothingGroup.mask2;
                                break;
                            case C_OBNAME:
                                chunk >> editor.lastMeshName;
                                break;
                            case C_CAMNAME:
                                chunk >> editor.lastCameraName;
                                break;
                            case C_LTNAME:
                                chunk >> editor.lastLightName;
                                break;
                            case C_MESHSEL:
                                chunk >> editor.selectMask;
                                break;
                            case C_MESHUNSEL:
                                chunk >> editor.unselectMask;
                                break;

                            // *** Keyframer ***
                            case C_KTAPE:
                                chunk >> keyframer.tape;
                                break;

                            // *** MaterialsEditor ***
                            case C_MTLDRAWER:
                                chunk >> materialsEditor.materialsPath;
                                break;
                            case C_MTLFILE:
                                chunk >> materialsEditor.materialFile;
                                break;
                            case C_WORKMTL:
                                ReadC_WORKMTL (chunk);
                                break;

                            case SMAGIC:
                                shaper.ReadSMAGIC (chunk);
                                break;
                            case LMAGIC:
                                lofter.ReadLMAGIC (chunk);
                                break;
                            case MDATA:
                                editor.ReadMDATA (chunk);
                                break;
                            case KFDATA:
                                keyframer.ReadKFDATA (chunk);
                                break;
                            case VPDATA:
                                videoPost.ReadVPDATA (chunk);
                                break;

                            case C_RENDTYPE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PROGMODE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PREVMODE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MODWMODE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MODMODEL:
                                // FIXME: implement
                                //assert (0);
                                break;

                            case C_FLIDRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_CUBDRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_FLIFILE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PALFILE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_BITMAP_DRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER_ENTRY:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_RGB_PATH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_RGB_FILE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_CUR_MNAME:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MASTER_AMBIENT:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_GET_SHAPE_MAKE_FACES:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VP_FROM:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VP_TO:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VP_NTH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_RAY_SHADOWS:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SUPER_SAMPLE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_OBJECT_MBLUR:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SHAPER_OK:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_LOFTER_OK:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_EDITOR_OK:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_KEYFRAMER_OK:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_FONTSEL:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SEG_START:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SEG_END:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_CURTIME:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_ANIMLENGTH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_TYPE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_FROM:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_TO:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_DOFNUM:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_RNG:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_NTH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_METHOD:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PV_FPS:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MEDTILE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_FRAMES:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_HDTL:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_HD:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_TL:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_IN:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_PK:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VTR_SH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_BOOLWELD:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_BOOLTYPE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_RND_TURBO:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_BGND_METHOD:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_ANG_THRESH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SS_THRESH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_TEXTURE_BLUR_DEFAULT:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PFILE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_TDRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_RIPDRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_PDRAWER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_RIPFILE:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAP_MATRIX_OLD:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_REND_TSTEP:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_VP_TSTEP:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SHPLOCAL:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MSHLOCAL:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_CURMTL_FROM_MESH:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_2:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_3:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_4:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_5:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_6:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_7:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_WORK_MTLS_8:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_TEXT_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_TEXT2_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_OPAC_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_BUMP_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_SPEC_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_SHIN_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_SELFI_DATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_TEXT_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_TEXT2_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_OPAC_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_BUMP_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_SPEC_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_SHIN_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_SELFI_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_SXP_REFL_MASKDATA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_FROZ_DISPLAY:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER1:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER2:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER3:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER4:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER5:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER6:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER7:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER8:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_MAPDRAWER9:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_NET_USE_VPOST:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_NET_USE_GAMMA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_NET_FIELD_ORDER:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_APPLY_DISP_GAMMA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_APPLY_FBUF_GAMMA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            case C_APPLY_FILE_GAMMA:
                                // FIXME: implement
                                //assert (0);
                                break;
                            default:
                                assert (0);
                                chunk.HandleUknownChunk ("Project", "ReadCMAGIC");
                                break;
                        }
                    }
                }

                void Project::ReadM3DMAGIC (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case M3D_VERSION:
                                chunk >> m3dVersion;
                                break;
                            case MDATA:
                                editor.ReadMDATA (chunk);
                                break;
                            case KFDATA:
                                keyframer.ReadKFDATA (chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Project", "ReadM3DMAGIC");
                                break;
                        }
                    }
                }

                void Project::ReadC_WORKMTL (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MAT_ENTRY: {
                                Material::UniquePtr material (new Material);
                                MaterialsEditor::ReadMAT_ENTRY (*material, chunk);
                                materialsEditor.workMaterials.push_back (material.get ());
                                material.release ();
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Project", "ReadC_WORKMTL");
                                break;
                        }
                    }
                }

                // FIXME: implement
                void Project::WriteCMAGIC (util::File &file) const {
                    ChunkWriter chunk (file, CMAGIC);
                    shaper.WriteSMAGIC (file);
                    lofter.WriteLMAGIC (file);
                    editor.WriteMDATA (file);
                    keyframer.WriteKFDATA (file);
                    WriteC_WORKMTL (file);
                    videoPost.WriteVPDATA (file);
                }

                void Project::WriteM3D_VERSION (util::File &file) const {
                    ChunkWriter chunk (file, M3D_VERSION);
                    chunk << m3dVersion;
                }

                void Project::WriteC_WORKMTL (util::File &file) const {
                    ChunkWriter chunk (file, C_WORKMTL);
                    typedef util::OwnerVector<Material>::const_iterator const_iterator;
                    for (const_iterator it = materialsEditor.workMaterials.begin (),
                        end = materialsEditor.workMaterials.end (); it != end; ++it) {
                        MaterialsEditor::WriteMAT_ENTRY (**it, file);
                    }
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
