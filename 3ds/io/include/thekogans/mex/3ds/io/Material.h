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

#if !defined (__thekogans_mex_3ds_io_Material_h)
#define __thekogans_mex_3ds_io_Material_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/Color.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Material {
                    typedef std::unique_ptr<Material> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Material)

                    struct Map {
                        std::string name;
                        util::i16 percent;
                        util::i16 bumpPercent;
                        util::ui16 tilingFlags;
                        util::f32 textureBlur;
                        util::f32 uScale;
                        util::f32 vScale;
                        util::f32 uOffset;
                        util::f32 vOffset;
                        util::f32 rotation;
                        thekogans::mex::opengl::ui8Color tint1;
                        thekogans::mex::opengl::ui8Color tint2;
                        thekogans::mex::opengl::ui8Color redTint;
                        thekogans::mex::opengl::ui8Color greenTint;
                        thekogans::mex::opengl::ui8Color blueTint;
                        std::vector<util::ui8> xdata;

                        // FIXME: verify defaults
                        Map () :
                            percent (100),
                            bumpPercent (0),
                            tilingFlags (0),
                            textureBlur (0.0f),
                            uScale (1.0f),
                            vScale (1.0f),
                            uOffset (0.0f),
                            vOffset (0.0f),
                            rotation (0.0f) {}
                    };
                    struct MapSet {
                        Map map;
                        Map mask;
                        bool useMap;
                        bool useMask;

                        MapSet () :
                            useMap (false),
                            useMask (false) {}
                    };
                    struct ReflectionMapSet {
                        MapSet mapSet;
                        struct AutoCubic {
                            util::ui8 shade;
                            util::ui8 antiAlias;
                            util::i16 flags;
                            util::i32 size;
                            util::i32 nthFrame;
                            bool use;

                            // FIXME: initialize
                        } autoCubic;
                    };
                    std::string name;
                    Color ambient;
                    Color diffuse;
                    Color specular;
                    util::i16 shininess;
                    util::i16 shininessStrength;
                    util::i16 transparency;
                    util::i16 fallOff;
                    util::i16 blur;
                    util::i16 selfIllumPct;
                    util::f32 wireSize;
                    util::i16 shading;
                    bool selfIllum;
                    bool useFallOff;
                    bool useBlur;
                    bool twoSided;
                    bool additive;
                    bool wire;
                    bool faceMap;
                    bool fallOffOut;
                    bool phongSoften;
                    bool wireAbs;
                    bool decal;
                    MapSet textureMap;
                    MapSet textureMap2;
                    MapSet opacityMap;
                    MapSet bumpMap;
                    MapSet specularMap;
                    MapSet shininessMap;
                    MapSet illuminationMap;
                    ReflectionMapSet reflectionMap;

                    // FIXME: initialize
                };

                const Material defaultMaterial = Material ();

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Material_h)
