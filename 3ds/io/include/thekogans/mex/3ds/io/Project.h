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

#if !defined (__thekogans_mex_3ds_io_Project_h)
#define __thekogans_mex_3ds_io_Project_h

#include <string>
#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/io/MaterialsEditor.h"
#include "thekogans/mex/3ds/io/VideoPost.h"
#include "thekogans/mex/3ds/io/Progress.h"
#include "thekogans/mex/3ds/io/ErrorHandler.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                // To do:
                // Fix all FIXME's
                // Fix all asserts

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Project {
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL ProgramConfiguration {
                        enum {
                            Mouse,
                            Tablet
                        };
                        util::ui16 inputDevice;
                        util::ui16 mouseSpeed;
                        util::ui16 commPort;
                        struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Tablet {
                            util::ui16 left;
                            util::ui16 top;
                            util::ui16 right;
                            util::ui16 bottom;

                            // FIXME: verify
                            Tablet () :
                                left (0),
                                top (0),
                                right (1024),
                                bottom (768) {}

                            inline void Swap (Tablet &tablet) {
                                std::swap (left, tablet.left);
                                std::swap (top, tablet.top);
                                std::swap (right, tablet.right);
                                std::swap (bottom, tablet.bottom);
                            }
                        } tablet;
                        util::ui16 pickBoxSize;
                        std::string shaperPath;
                        std::string lofterPath;
                        std::string meshPath;
                        std::string fontPath;
                        std::string imagePath;
                        std::vector<std::string> mapPaths;

                        // FIXME: verify
                        ProgramConfiguration () :
                            inputDevice (Mouse),
                            mouseSpeed (0),
                            commPort (1),
                            pickBoxSize (5) {}

                        inline void Swap (ProgramConfiguration &programConfiguration) {
                            std::swap (inputDevice, programConfiguration.inputDevice);
                            std::swap (mouseSpeed, programConfiguration.mouseSpeed);
                            std::swap (commPort, programConfiguration.commPort);
                            tablet.Swap (programConfiguration.tablet);
                            std::swap (pickBoxSize, programConfiguration.pickBoxSize);
                            shaperPath.swap (programConfiguration.shaperPath);
                            lofterPath.swap (programConfiguration.lofterPath);
                            meshPath.swap (programConfiguration.meshPath);
                            fontPath.swap (programConfiguration.fontPath);
                            imagePath.swap (programConfiguration.imagePath);
                            mapPaths.swap (programConfiguration.mapPaths);
                        }
                    } programConfiguration;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL SystemOptions {
                        bool backupFile;
                        bool regionToggle;
                        bool selectedReset;
                        util::f32 weldThreshold;
                        bool modalKFButtons;
                        bool viewPreserveRatio;
                        bool backgroundPreserveRatio;
                        util::ui16 tensionDefault;
                        util::ui16 continuityDefault;
                        util::ui16 biasDefault;

                        // FIXME: verify
                        SystemOptions () :
                            backupFile (true),
                            regionToggle (false),
                            selectedReset (true),
                            weldThreshold (0.01f),
                            modalKFButtons (false),
                            viewPreserveRatio (true),
                            backgroundPreserveRatio (true),
                            tensionDefault (25),
                            continuityDefault (25),
                            biasDefault (25) {}

                        inline void Swap (SystemOptions &systemOptions) {
                            std::swap (backupFile, systemOptions.backupFile);
                            std::swap (regionToggle, systemOptions.regionToggle);
                            std::swap (selectedReset, systemOptions.selectedReset);
                            std::swap (weldThreshold, systemOptions.weldThreshold);
                            std::swap (modalKFButtons, systemOptions.modalKFButtons);
                            std::swap (viewPreserveRatio, systemOptions.viewPreserveRatio);
                            std::swap (backgroundPreserveRatio, systemOptions.backgroundPreserveRatio);
                            std::swap (tensionDefault, systemOptions.tensionDefault);
                            std::swap (continuityDefault, systemOptions.continuityDefault);
                            std::swap (biasDefault, systemOptions.biasDefault);
                        }
                    } systemOptions;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL GammaControl {
                        util::f32 display;
                        util::f32 frameBuffer;
                        util::f32 inFile;
                        util::f32 outFile;
                        bool use;

                        GammaControl () :
                            display (1.8f),
                            frameBuffer (1.8f),
                            inFile (1.8f),
                            outFile (1.8f),
                            use (true) {}

                        inline void Swap (GammaControl &gammaControl) {
                            std::swap (display, gammaControl.display);
                            std::swap (frameBuffer, gammaControl.frameBuffer);
                            std::swap (inFile, gammaControl.inFile);
                            std::swap (outFile, gammaControl.outFile);
                            std::swap (use, gammaControl.use);
                        }
                    } gammaControl;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL DXF {
                        enum {
                            Layer,
                            Color,
                            Entuty
                        };
                        util::ui16 derive;
                        bool weldVertices;
                        bool unifyNormals;
                        bool autoSmooth;
                        util::f32 smoothingAngle;
                        util::f32 arcAngle;

                        DXF () :
                            derive (Layer),
                            weldVertices (true),
                            unifyNormals (true),
                            autoSmooth (true),
                            smoothingAngle (30.0f),
                            arcAngle (30.0f) {}

                        inline void Swap (DXF &dxf) {
                            std::swap (derive, dxf.derive);
                            std::swap (weldVertices, dxf.weldVertices);
                            std::swap (unifyNormals, dxf.unifyNormals);
                            std::swap (autoSmooth, dxf.autoSmooth);;
                            std::swap (smoothingAngle, dxf.smoothingAngle);
                            std::swap (arcAngle, dxf.arcAngle);
                        }
                    } dxf;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL DrawingAids {
                        blas::Point3 snapSpacing;
                        blas::Point3 gridSpacing;
                        blas::Point3 gridStart;
                        blas::Point3 gridEnd;
                        util::f32 angleSnap;
                        bool useAngleSnap;
                        bool useVertexSnap;

                        DrawingAids () :
                            snapSpacing (10.0f, 10.0f, 10.0f),
                            gridSpacing (10.0f, 10.0f, 10.0f),
                            gridStart (-200.0f, -200.0f, -200.0f),
                            gridEnd (200.0f, 200.0f, 200.0f),
                            angleSnap (10.0f),
                            useAngleSnap (false),
                            useVertexSnap (false) {}

                        inline void Swap (DrawingAids &drawingAids) {
                            std::swap (snapSpacing, drawingAids.snapSpacing);
                            std::swap (gridSpacing, drawingAids.gridSpacing);
                            std::swap (gridStart, drawingAids.gridStart);
                            std::swap (gridEnd, drawingAids.gridEnd);
                            std::swap (angleSnap, drawingAids.angleSnap);
                            std::swap (useAngleSnap, drawingAids.useAngleSnap);
                            std::swap (useVertexSnap, drawingAids.useVertexSnap);
                        }
                    } drawingAids;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Units {
                        enum Type {
                            Decimal,
                            Metric,
                            Architectural
                        };
                        util::ui16 type;
                        util::ui16 denominator;
                        util::f32 scale;
                        util::f32 conv2in;
                        enum Metric {
                            Inches,
                            Feet,
                            Centimeters,
                            Meters
                        };
                        util::ui16 metric;

                        Units () :
                            type (Decimal),
                            denominator (100),
                            scale (1.0f),
                            conv2in (1.0f),
                            metric (Inches) {}

                        inline void Swap (Units &units) {
                            std::swap (type, units.type);
                            std::swap (denominator, units.denominator);
                            std::swap (scale, units.scale);
                            std::swap (conv2in, units.conv2in);
                            std::swap (metric, units.metric);
                        }
                    } units;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL BackgroundContrast {
                        util::ui16 dark;
                        util::ui16 light;

                        BackgroundContrast () :
                            dark (33),
                            light (66) {}

                        inline void Swap (BackgroundContrast &backgroundContrast) {
                            std::swap (dark, backgroundContrast.dark);
                            std::swap (light, backgroundContrast.light);
                        }
                    } backgroundContrast;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL NetworkParameters {
                        inline void Swap (NetworkParameters &networkParameters) {
                        }
                    } networkParameters;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Display {
                        bool showAllLines;
                        bool seeThrough;
                        bool kfSeeThrough;
                        bool vertexTicks;
                        bool detail;
                        bool showLights;
                        bool showCameras;
                        util::ui16 speedCount;
                        bool fastDraw;

                        Display () :
                            showAllLines (false),
                            seeThrough (true),
                            kfSeeThrough (false),
                            vertexTicks (false),
                            detail (true),
                            showLights (false),
                            showCameras (false),
                            speedCount (10),
                            fastDraw (false) {}

                        inline void Swap (Display &display) {
                            std::swap (showAllLines, display.showAllLines);
                            std::swap (seeThrough, display.seeThrough);
                            std::swap (kfSeeThrough, display.kfSeeThrough);
                            std::swap (vertexTicks, display.vertexTicks);
                            std::swap (detail, display.detail);
                            std::swap (showLights, display.showLights);
                            std::swap (showCameras, display.showCameras);
                            std::swap (speedCount, display.speedCount);
                            std::swap (fastDraw, display.fastDraw);
                        }
                    } display;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL ConstructionPlane {
                        blas::Point3 plane;
                        bool visible;

                        ConstructionPlane () :
                            plane (blas::Point3::Zero),
                            visible (false) {}

                        inline void Swap (ConstructionPlane &constructionPlane) {
                            std::swap (plane, constructionPlane.plane);
                            std::swap (visible, constructionPlane.visible);
                        }
                    } constructionPlane;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Render {
                        enum {
                            Flat,
                            Gouraud,
                            Phong,
                            Metal
                        };
                        util::ui16 shadingLimit;
                        bool antialiasing;
                        bool filterMaps;
                        bool shadows;
                        bool mapping;
                        bool autoReflect;
                        bool force2Sided;
                        bool forceWire;
                        enum {
                            Show,
                            Hide
                        };
                        util::ui16 hiddenGeometry;
                        enum {
                            Rescale,
                            Tile
                        };
                        util::ui16 background;
                        struct DeviceConfiguration {
                            enum {
                                Flic,
                                GIF,
                                TGA,
                                ColorTIFF,
                                MonoTIFF,
                                BMPTrue,
                                BMP256,
                                JPEG
                            };
                            util::ui16 fileOutputStill;
                            util::ui16 fileOutputAnimation;
                            bool compressed;
                            util::ui16 jpegCompression;
                            bool imageFileComments;
                            std::string display;
                            std::string hardcopy;
                            enum {
                                Low,
                                Medium,
                                High,
                                Custom
                            };
                            util::ui16 paletteType;
                            struct Resolution {
                                util::ui16 width;
                                util::ui16 height;
                                util::f32 aspect;

                                Resolution () :
                                    width (0),
                                    height (0),
                                    aspect (0.0f) {}

                                inline void Swap (Resolution &resolution) {
                                    std::swap (width, resolution.width);
                                    std::swap (height, resolution.height);
                                    std::swap (aspect, resolution.aspect);
                                }
                            } resolution;

                            DeviceConfiguration () :
                                fileOutputStill (TGA),
                                fileOutputAnimation (Flic),
                                compressed (true),
                                jpegCompression (100),
                                imageFileComments (false),
                                paletteType (Low) {}

                            inline void Swap (DeviceConfiguration &deviceConfiguration) {
                                std::swap (fileOutputStill, deviceConfiguration.fileOutputStill);
                                std::swap (fileOutputAnimation, deviceConfiguration.fileOutputAnimation);
                                std::swap (compressed, deviceConfiguration.compressed);
                                std::swap (jpegCompression, deviceConfiguration.jpegCompression);
                                std::swap (imageFileComments, deviceConfiguration.imageFileComments);
                                display.swap (deviceConfiguration.display);
                                hardcopy.swap (deviceConfiguration.hardcopy);
                                std::swap (paletteType, deviceConfiguration.paletteType);
                                resolution.Swap (deviceConfiguration.resolution);
                            }
                        } deviceConfiguration;
                        struct Options {
                            struct ObjectMotionBlur {
                                util::ui16 number;
                                util::ui16 samples;
                                util::f32 duration;

                                ObjectMotionBlur () :
                                    number (7),
                                    samples (4),
                                    duration (1.0f) {}

                                inline void Swap (ObjectMotionBlur &objectMotionBlur) {
                                    std::swap (number, objectMotionBlur.number);
                                    std::swap (samples, objectMotionBlur.samples);
                                    std::swap (duration, objectMotionBlur.duration);
                                }
                            } objectMotionBlur;
                            util::ui16 motionBlurDither;
                            bool renderFields;
                            bool videoColorCheck;
                            enum {
                                Flag,
                                Correct
                            };
                            util::ui16 method;
                            bool ditherTrueColor;
                            bool superBlack;
                            bool dither256;
                            bool reflectFlip;
                            bool renderAlpha;
                            bool alphaSplit;
                            enum {
                                TGA_16,
                                TGA_24
                            };
                            util::ui16 tgaDepth;
                            bool saveLastImage;
                            bool nthSerialNumbering;
                            util::f32 zClipNear;
                            util::f32 safeFrame;
                            util::f32 pixelSize;

                            Options () :
                                motionBlurDither (50),
                                renderFields (false),
                                videoColorCheck (false),
                                method (Correct),
                                ditherTrueColor (true),
                                superBlack (false),
                                dither256 (true),
                                reflectFlip (false),
                                renderAlpha (false),
                                alphaSplit (false),
                                tgaDepth (TGA_24),
                                saveLastImage (false),
                                nthSerialNumbering (false),
                                zClipNear (1.0f),
                                safeFrame (10.0f),
                                pixelSize (1.1f) {}

                            inline void Swap (Options &options) {
                                objectMotionBlur.Swap (options.objectMotionBlur);
                                std::swap (motionBlurDither, options.motionBlurDither);
                                std::swap (renderFields, options.renderFields);
                                std::swap (videoColorCheck, options.videoColorCheck);
                                std::swap (method, options.method);
                                std::swap (ditherTrueColor, options.ditherTrueColor);
                                std::swap (superBlack, options.superBlack);
                                std::swap (dither256, options.dither256);
                                std::swap (reflectFlip, options.reflectFlip);
                                std::swap (renderAlpha, options.renderAlpha);
                                std::swap (alphaSplit, options.alphaSplit);
                                std::swap (tgaDepth, options.tgaDepth);
                                std::swap (saveLastImage, options.saveLastImage);
                                std::swap (nthSerialNumbering, options.nthSerialNumbering);
                                std::swap (zClipNear, options.zClipNear);
                                std::swap (safeFrame, options.safeFrame);
                                std::swap (pixelSize, options.pixelSize);
                            }
                        } options;
                        enum {
                            All,
                            Single,
                            Segment,
                            Range
                        };
                        util::ui16 frames;
                        util::ui16 from;
                        util::ui16 to;
                        util::ui16 nth;
                        enum {
                            Display,
                            NoDisplay,
                            Hardcopy,
                            NetASAP,
                            NetQueue
                        };
                        util::ui16 output;
                        bool disk;

                        Render () :
                            shadingLimit (Metal),
                            antialiasing (true),
                            filterMaps (true),
                            shadows (true),
                            mapping (true),
                            autoReflect (true),
                            force2Sided (false),
                            forceWire (false),
                            hiddenGeometry (Hide),
                            background (Tile),
                            frames (All),
                            from (0),
                            to (0),
                            nth (1),
                            output (Display),
                            disk (false) {}

                        inline void Swap (Render &render) {
                            std::swap (shadingLimit, render.shadingLimit);
                            std::swap (antialiasing, render.antialiasing);
                            std::swap (filterMaps, render.filterMaps);
                            std::swap (shadows, render.shadows);
                            std::swap (mapping, render.mapping);
                            std::swap (autoReflect, render.autoReflect);
                            std::swap (force2Sided, render.force2Sided);
                            std::swap (forceWire, render.forceWire);
                            std::swap (hiddenGeometry, render.hiddenGeometry);
                            std::swap (background, render.background);
                            deviceConfiguration.Swap (render.deviceConfiguration);
                            options.Swap (render.options);
                            std::swap (frames, render.frames);
                            std::swap (from, render.from);
                            std::swap (to, render.to);
                            std::swap (nth, render.nth);
                            std::swap (output, render.output);
                            std::swap (disk, render.disk);
                        }
                    } render;
                    Shaper shaper;
                    Lofter lofter;
                    util::i32 m3dVersion;
                    Editor editor;
                    Keyframer keyframer;
                    MaterialsEditor materialsEditor;
                    VideoPost videoPost;

                    // If you ever catch this exception, it means the environment
                    // in which lib3ds++ is being run is incompatible with the
                    // internal type assumptions. Because 3ds files are binary,
                    // and because they have certain basic type size requirements
                    // (see util/Types.h), we'll need to rethink the whole IO
                    // strategy for that environment.
                    struct BadEnvironment {
                        static void PrintEnvironment ();
                    };

                    Project ();

                    // Swap follows the semantics of std::swap. It guarantees not
                    // to throw. All major types defined by lib3ds implement Swap
                    // allowing for creation of software which guarantees transactional
                    // semantics.
                    void Swap (Project &project);

                    enum FileType {
                        Unknown,
                        PRJ,
                        SHP,
                        LFT,
                        _3DS,
                        MLI,
                        VP
                    };

                    // Open any of the known 3ds chunked (binary) file types. This function
                    // will look inside the file to determine the file type on its own.
                    // This is a DOM type interface in that it will parse the whole file,
                    // creating appropriate structures, before returning.
                    // path - identifies the file to open
                    // progress - optional util::Progress derived object to receive progress updates
                    // errorHandler - optional ErrorHandler derived object to receive chunk errors
                    //   NOTE: This interface will not be called back in the event of an exception.
                    // NOTE: This function offers a strong exception safety guarantee. It uses
                    // Swap (which guarantees not to throw) to either complete successfully or
                    // the state of this remains unchanged. Performance penalty for such a service
                    // is slight, but there might still be cases where even that is unacceptable. If
                    // you don't need exception safety, call OpenHelper directly.
                    FileType Open (
                        const std::string &path,
                        Progress *progress = 0,
                        ErrorHandler *errorHandler = 0);
                    FileType OpenHelper (
                        const std::string &path,
                        Progress *progress = 0,
                        ErrorHandler *errorHandler = 0);

                    // As the names imply, save project data in various 3D
                    // Studio recognized files.
                    void SavePRJ (const std::string &path) const;
                    void SaveSHP (const std::string &path) const;
                    void SaveLFT (const std::string &path) const;
                    void Save3DS (const std::string &path) const;
                    void SaveMLI (const std::string &path) const;
                    void SaveVP (const std::string &path) const;

                private:
                    void ReadCMAGIC (ChunkReader &mainChunk);
                    void ReadM3DMAGIC (ChunkReader &mainChunk);
                    void ReadC_WORKMTL (ChunkReader &mainChunk);

                    void WriteCMAGIC (util::File &stream) const;
                    void WriteM3D_VERSION (util::File &stream) const;
                    void WriteC_WORKMTL (util::File &stream) const;

                    Project (const Project &);
                    Project &operator = (const Project &);
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Project_h)
