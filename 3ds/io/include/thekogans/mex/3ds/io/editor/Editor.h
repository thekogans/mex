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

#if !defined (__thekogans_mex_3ds_io_editor_Editor_h)
#define __thekogans_mex_3ds_io_editor_Editor_h

#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/Tape.h"
#include "thekogans/mex/3ds/io/Axis.h"
#include "thekogans/mex/3ds/io/Material.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/io/editor/Camera.h"
#include "thekogans/mex/3ds/io/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct Keyframer;

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Editor : public Module {
                    Tape3 tape;
                    Axis3 axis;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Create {
                        util::ui16 lsphereSegments;
                        util::ui16 gsphereSegments;
                        util::ui16 hemisphereSegments;
                        struct Cylinder {
                            util::ui16 segments;
                            util::ui16 sides;

                            Cylinder () :
                                segments (6),
                                sides (1) {}

                            inline void Swap (Cylinder &cylinder) {
                                std::swap (segments, cylinder.segments);
                                std::swap (sides, cylinder.sides);
                            }
                        } cylinder;
                        struct Tube {
                            util::ui16 segments;
                            util::ui16 sides;

                            Tube () :
                                segments (16),
                                sides (1) {}

                            inline void Swap (Tube &tube) {
                                std::swap (segments, tube.segments);
                                std::swap (sides, tube.sides);
                            }
                        } tube;
                        struct Torus {
                            util::ui16 segments;
                            util::ui16 sides;

                            Torus () :
                                segments (8),
                                sides (16) {}

                            inline void Swap (Torus &torus) {
                                std::swap (segments, torus.segments);
                                std::swap (sides, torus.sides);
                            }
                        } torus;
                        struct Cone {
                            util::ui16 segments;
                            util::ui16 sides;

                            Cone () :
                                segments (16),
                                sides (1) {}

                            inline void Swap (Cone &cone) {
                                std::swap (segments, cone.segments);
                                std::swap (sides, cone.sides);
                            }
                        } cone;
                        struct ObjectTesselate {
                            enum {
                                FaceCenter,
                                Edge
                            };
                            util::ui16 type;
                            util::ui16 tension;

                            ObjectTesselate () :
                                type (FaceCenter),
                                tension (25) {}

                            inline void Swap (ObjectTesselate &objectTesselate) {
                                std::swap (type, objectTesselate.type);
                                std::swap (tension, objectTesselate.tension);
                            }
                        } objectTesselate;

                        Create () :
                            lsphereSegments (16),
                            gsphereSegments (256),
                            hemisphereSegments (16) {}

                        inline void Swap (Create &create) {
                            std::swap (lsphereSegments, create.lsphereSegments);
                            std::swap (gsphereSegments, create.gsphereSegments);
                            std::swap (hemisphereSegments, create.hemisphereSegments);
                            cylinder.Swap (create.cylinder);
                            tube.Swap (create.tube);
                            torus.Swap (create.torus);
                            cone.Swap (create.cone);
                            objectTesselate.Swap (create.objectTesselate);
                        }
                    } create;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Map {
                        enum {
                            Planar,
                            Cylindrical,
                            Spherical
                        };
                        util::ui16 type;
                        bool visible;
                        blas::Point2 tile;
                        blas::Point3 center;
                        util::f32 scale;
                        blas::Matrix3 xform;
                        util::f32 planarWidth;
                        util::f32 planarHeight;
                        util::f32 cylindricalHeight;

                        Map () :
                            type (Planar),
                            visible (false),
                            tile (1.0f, 1.0f),
                            center (blas::Point3::Zero),
                            scale (1.0f),
                            xform (blas::Matrix3::Identity),
                            planarWidth (1.0f),
                            planarHeight (1.0f),
                            cylindricalHeight (1.0f) {}

                        inline void Swap (Map &map) {
                            std::swap (type, map.type);
                            std::swap (visible, map.visible);
                            std::swap (tile, map.tile);
                            std::swap (center, map.center);
                            std::swap (scale, map.scale);
                            std::swap (xform, map.xform);
                            std::swap (planarWidth, map.planarWidth);
                            std::swap (planarHeight, map.planarHeight);
                            std::swap (cylindricalHeight, map.cylindricalHeight);
                        }
                    } map;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL SmoothingGroup {
                        util::ui16 group;
                        util::ui32 mask1;
                        util::ui32 mask2;

                        SmoothingGroup () :
                            group (0),
                            mask1 (0),
                            mask2 (0) {}

                        inline void Swap (SmoothingGroup &smoothingGroup) {
                            std::swap (group, smoothingGroup.group);
                            std::swap (mask1, smoothingGroup.mask1);
                            std::swap (mask2, smoothingGroup.mask2);
                        }
                    } smoothingGroup;
                    std::string lastMeshName;
                    std::string lastCameraName;
                    std::string lastLightName;
                    util::ui16 selectMask;
                    util::ui16 unselectMask;
                    util::i32 meshVersion;
                    util::f32 masterScale;
                    util::OwnerVector<Material> materials;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL DefaultView {
                        View::Type type;
                        blas::Point3 target;
                        util::f32 width;
                        util::f32 horizontalAngle;
                        util::f32 verticalAngle;
                        util::f32 bankAngle;
                        std::string cameraName;

                        DefaultView () :
                            type (View::None),
                            target (blas::Point3::Zero),
                            width (0.0f),
                            horizontalAngle (0.0f),
                            verticalAngle (0.0f),
                            bankAngle (0.0f) {}

                        inline void Swap (DefaultView &defaultView) {
                            std::swap (type, defaultView.type);
                            std::swap (target, defaultView.target);
                            std::swap (width, defaultView.width);
                            std::swap (horizontalAngle, defaultView.horizontalAngle);
                            std::swap (verticalAngle, defaultView.verticalAngle);
                            std::swap (bankAngle, defaultView.bankAngle);
                            cameraName.swap (defaultView.cameraName);
                        }
                    } defaultView;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Background {
                        enum Type {
                            NoBackground,
                            UseSolidColor,
                            UseVGradient,
                            UseBitmap
                        } type;
                        Color solidColor;
                        struct VGradient {
                            util::f32 percent;
                            Color bottom;
                            Color middle;
                            Color top;

                            VGradient () :
                                percent (0.5f),
                                bottom (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)),
                                middle (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)),
                                top (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)) {}

                            inline void Swap (VGradient &vGradient) {
                                std::swap (percent, vGradient.percent);
                                bottom.Swap (vGradient.bottom);
                                middle.Swap (vGradient.middle);
                                top.Swap (vGradient.top);
                            }
                        } vGradient;
                        std::string bitmap;

                        Background () :
                            type (NoBackground),
                            solidColor (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)) {}

                        inline void Swap (Background &background) {
                            std::swap (type, background.type);
                            solidColor.Swap (background.solidColor);
                            vGradient.Swap (background.vGradient);
                            bitmap.swap (background.bitmap);
                        }
                    } background;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Shadow {
                        util::f32 loBias;
                        util::f32 hiBias;
                        util::f32 filter;
                        util::i16 mapSize;
                        util::f32 rayBias;
                        util::i16 samples;
                        util::i16 range;

                        // FIXME: samples and range need real values
                        Shadow () :
                            loBias (1.0f),
                            hiBias (1.0f),
                            filter (3.0f),
                            mapSize (512),
                            rayBias (1.0f),
                            samples (1),
                            range (1) {}

                        inline void Swap (Shadow &shadow) {
                            std::swap (loBias, shadow.loBias);
                            std::swap (hiBias, shadow.hiBias);
                            std::swap (filter, shadow.filter);
                            std::swap (mapSize, shadow.mapSize);
                            std::swap (rayBias, shadow.rayBias);
                            std::swap (samples, shadow.samples);
                            std::swap (range, shadow.range);
                        }
                    } shadow;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Atmosphere {
                        enum Type {
                            NoAtmosphere,
                            UseFog,
                            UseLayerFog,
                            UseDistanceCue
                        } type;
                        struct Fog {
                            util::f32 nearPlane;
                            util::f32 nearDensity;
                            util::f32 farPlane;
                            util::f32 farDensity;
                            thekogans::mex::opengl::f32Color color;
                            bool background;

                            Fog () :
                                nearPlane (0.0f),
                                nearDensity (0.0f),
                                farPlane (1000.0f),
                                farDensity (100.0f),
                                color (1.0f, 1.0f, 1.0f),
                                background (false) {}

                            inline void Swap (Fog &fog) {
                                std::swap (nearPlane, fog.nearPlane);
                                std::swap (nearDensity, fog.nearDensity);
                                std::swap (farPlane, fog.farPlane);
                                std::swap (farDensity, fog.farDensity);
                                std::swap (color, fog.color);
                                std::swap (background, fog.background);
                            }
                        } fog;
                        struct LayerFog {
                            util::f32 zmin;
                            util::f32 zmax;
                            util::f32 density;
                            util::ui32 flags;
                            thekogans::mex::opengl::f32Color color;

                            LayerFog () :
                                zmin (0.0f),
                                zmax (100.0f),
                                density (50.0f),
                                flags (0),
                                color (1.0f, 1.0f, 1.0f) {}

                            inline void Swap (LayerFog &layerFog) {
                                std::swap (zmin, layerFog.zmin);
                                std::swap (zmax, layerFog.zmax);
                                std::swap (density, layerFog.density);
                                std::swap (flags, layerFog.flags);
                                std::swap (color, layerFog.color);
                            }
                        } layerFog;
                        struct _LIB_THEKOGANS_MEX_3DS_IO_DECL DistanceCue {
                            util::f32 nearPlane;
                            util::f32 nearDim;
                            util::f32 farPlane;
                            util::f32 farDim;
                            bool background;

                            DistanceCue () :
                                nearPlane (0.0f),
                                nearDim (0.0f),
                                farPlane (1000.0f),
                                farDim (100.0f),
                                background (false) {}

                            inline void Swap (DistanceCue &distanceCue) {
                                std::swap (nearPlane, distanceCue.nearPlane);
                                std::swap (nearDim, distanceCue.nearDim);
                                std::swap (farPlane, distanceCue.farPlane);
                                std::swap (farDim, distanceCue.farDim);
                                std::swap (background, distanceCue.background);
                            }
                        } distanceCue;

                        Atmosphere () : type (NoAtmosphere) {}

                        inline void Swap (Atmosphere &atmosphere) {
                            std::swap (type, atmosphere.type);
                            fog.Swap (atmosphere.fog);
                            layerFog.Swap (atmosphere.layerFog);
                            distanceCue.Swap (atmosphere.distanceCue);
                        }
                    } atmosphere;
                    blas::Point3 constructionPlane;
                    Color ambientLight;
                    util::OwnerVector<Mesh> meshes;
                    util::OwnerVector<Light> lights;
                    util::OwnerVector<Camera> cameras;

                    // If you ever catch this, its because the
                    // name mesh contains more then USHRT_MAX
                    // vertices, vertexFlags, textureVertices,
                    // faces or faceSmoothGroups.
                    struct InvalidMesh {
                        std::string name;
                        InvalidMesh (const std::string &name_) :
                            name (name_) {}
                    };

                    explicit Editor (Project &project) :
                        Module (project),
                        lastMeshName ("Object"),
                        lastCameraName ("Camera"),
                        lastLightName ("Light"),
                        selectMask (0x8000),
                        unselectMask (0x7fff),
                        meshVersion (3),
                        masterScale (1.0f),
                        constructionPlane (blas::Point3::Zero),
                        ambientLight (thekogans::mex::opengl::f32Color (0.1f, 0.1f, 0.1f)) {}

                    void Swap (Editor &editor);

                    void ReadMDATA (ChunkReader &mainChunk);
                    void WriteMDATA (util::File &stream) const;

                    Mesh *GetMesh (const std::string &name) const;
                    Light *GetLight (const std::string &name) const;
                    Camera *GetCamera (const std::string &name) const;

                private:
                    Editor (const Editor &);
                    Editor &operator = (const Editor &);

                    void ReadFOG (ChunkReader &mainChunk);
                    void ReadDISTANCE_CUE (ChunkReader &mainChunk);
                    void ReadLAYER_FOG (ChunkReader &mainChunk);
                    void ReadDEFAULT_VIEW (ChunkReader &mainChunk);
                    void ReadNAMED_OBJECT (ChunkReader &mainChunk);
                    void ReadN_TRI_OBJECT (Mesh &mesh, ChunkReader &mainChunk);
                    void ReadFACE_ARRAY (Mesh &mesh, ChunkReader &mainChunk);
                    void ReadN_DIRECT_LIGHT (Light &light, ChunkReader &mainChunk);
                    void ReadDL_SPOTLIGHT (Light::Spot &spot, ChunkReader &mainChunk);
                    void ReadN_CAMERA (Camera &camera, ChunkReader &mainChunk);

                    void WriteMESH_VERSION (
                        util::File &stream) const;
                    void WriteMASTER_SCALE (
                        util::File &stream) const;
                    void WriteDEFAULT_VIEW (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_TOP (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_BOTTOM (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_LEFT (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_RIGHT (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_FRONT (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_BACK (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_USER (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteVIEW_CAMERA (
                        const DefaultView &defaultView,
                        util::File &stream) const;
                    void WriteBackground (
                        util::File &stream) const;
                    void WriteUSE_SOLID_BGND (
                        util::File &stream) const;
                    void WriteUSE_V_GRADIENT (
                        util::File &stream) const;
                    void WriteUSE_BIT_MAP (
                        util::File &stream) const;
                    void WriteShadow (
                        util::File &stream) const;
                    void WriteAtmosphere (
                        util::File &stream) const;
                    void WriteFOG_BGND (
                        util::File &stream) const;
                    void WriteDCUE_BGND (
                        util::File &stream) const;
                    void WriteUSE_FOG (
                        util::File &stream) const;
                    void WriteUSE_LAYER_FOG (
                        util::File &stream) const;
                    void WriteUSE_DISTANCE_CUE (
                        util::File &stream) const;
                    void WriteO_CONSTS (
                        util::File &stream) const;
                    void WriteMesh (
                        const Mesh &mesh,
                        util::File &stream) const;
                    void WriteLight (
                        const Light &light,
                        util::File &stream) const;
                    void WriteCamera (
                        const Camera &camera,
                        util::File &stream) const;
                    void WriteN_TRI_OBJECT (
                        const Mesh &mesh,
                        util::File &stream) const;
                    void WritePOINT_ARRAY (
                        const std::vector<blas::Point3> &vertices,
                        util::File &stream) const;
                    void WritePOINT_FLAG_ARRAY (
                        const std::vector<util::ui16> &vertexFlags,
                        util::File &stream) const;
                    void WriteTEX_VERTS (
                        const std::vector<blas::Point2> &textureVertices,
                        util::File &stream) const;
                    void WriteFACE_NORMALS (
                        const std::vector<blas::Point3> &faceNormals,
                        util::File &stream) const;
                    void WriteMESH_TEXTURE_INFO (
                        const Mesh::Map &map,
                        util::File &stream) const;
                    void WriteMESH_MATRIX (
                        const blas::Matrix3 &xform,
                        util::File &stream) const;
                    void WriteMESH_COLOR (
                        util::ui8 paletteIndex,
                        util::File &stream) const;
                    void WriteFACE_ARRAY (
                        const Mesh &mesh,
                        util::File &stream) const;
                    void WriteSMOOTH_GROUP (
                        const std::vector<util::ui32> &faceSmoothGroups,
                        util::File &stream) const;
                    void WriteMSH_MAT_GROUP (
                        const Mesh::Material &material,
                        util::File &stream) const;
                    void WriteMSH_BOXMAP (
                        const std::vector<std::string> &boxMap,
                        util::File &stream) const;
                    void WritePROC_NAME (
                        const std::string &procName,
                        util::File &stream) const;
                    void WritePROC_DATA (
                        const std::vector<util::ui8> &procData,
                        util::File &stream) const;
                    void WriteOBJ_HIDDEN (
                        util::File &stream) const;
                    void WriteOBJ_VIS_LOFTER (
                        util::File &stream) const;
                    void WriteOBJ_DOESNT_CAST (
                        util::File &stream) const;
                    void WriteOBJ_DONT_RCVSHADOW (
                        util::File &stream) const;
                    void WriteOBJ_MATTE (
                        util::File &stream) const;
                    void WriteOBJ_FAST (
                        util::File &stream) const;
                    void WriteOBJ_PROCEDURAL (
                        util::File &stream) const;
                    void WriteOBJ_FROZEN (
                        util::File &stream) const;
                    void WriteN_DIRECT_LIGHT (
                        const Light &light,
                        util::File &stream) const;
                    void WriteDL_OFF (
                        util::File &stream) const;
                    void WriteDL_OUTER_RANGE (
                        util::f32 outer,
                        util::File &stream) const;
                    void WriteDL_INNER_RANGE (
                        util::f32 inner,
                        util::File &stream) const;
                    void WriteDL_ATTENUATE (
                        util::File &stream) const;
                    void WriteDL_MULTIPLIER (
                        util::f32 multiplier,
                        util::File &stream) const;
                    void WriteDL_EXCLUDE (
                        const std::string &exclude,
                        util::File &stream) const;
                    void WriteDL_SPOTLIGHT (
                        const Light::Spot &spot,
                        util::File &stream) const;
                    void WriteDL_SPOT_ROLL (
                        util::f32 roll,
                        util::File &stream) const;
                    void WriteDL_SHADOWED (
                        util::File &stream) const;
                    void WriteDL_LOCAL_SHADOW2 (
                        const Light::Spot::Shadow &shadow,
                        util::File &stream) const;
                    void WriteDL_SEE_CONE (
                        util::File &stream) const;
                    void WriteDL_SPOT_RECTANGULAR (
                        util::File &stream) const;
                    void WriteDL_SPOT_ASPECT (
                        util::f32 aspect,
                        util::File &stream) const;
                    void WriteDL_PROJECTOR (
                        const std::string &bitmap,
                        util::File &stream) const;
                    void WriteDL_SPOT_OVERSHOOT (
                        util::File &stream) const;
                    void WriteDL_RAY_BIAS (
                        util::f32 rayBias,
                        util::File &stream) const;
                    void WriteDL_RAYSHAD (
                        util::File &stream) const;
                    void WriteN_CAMERA (
                        const Camera &camera,
                        util::File &stream) const;
                    void WriteCAM_SEE_CONE (
                        util::File &stream) const;
                    void WriteCAM_RANGES (
                        const Camera::Ranges &ranges,
                        util::File &stream) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_editor_Editor_h)
