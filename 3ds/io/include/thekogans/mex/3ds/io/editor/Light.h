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

#if !defined (__thekogans_mex_3ds_io_editor_Light_h)
#define __thekogans_mex_3ds_io_editor_Light_h

#include <memory>
#include <new>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/opengl/f32Color.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/Color.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Light {
                    typedef std::unique_ptr<Light> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    std::string name;
                    blas::Point3 position;
                    thekogans::mex::opengl::f32Color color;
                    util::f32 multiplier;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Attenuation {
                        util::f32 inner;
                        util::f32 outer;
                        bool on;

                        Attenuation () :
                            inner (10.0f),
                            outer (100.0f),
                            on (false) {}
                        Attenuation (
                            util::f32 inner_,
                            util::f32 outer_,
                            bool on_) :
                            inner (inner_),
                            outer (outer_),
                            on (on_) {}

                        inline void Swap (Attenuation &attenuation) {
                            std::swap (inner, attenuation.inner);
                            std::swap (outer, attenuation.outer);
                            std::swap (on, attenuation.on);
                        }
                    } attenuation;
                    std::vector<std::string> excludes;
                    bool on;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Spot {
                        typedef std::unique_ptr<Spot> UniquePtr;

                        THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                        blas::Point3 target;
                        util::f32 hotSpot;
                        util::f32 fallOff;
                        util::f32 roll;
                        struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Cone {
                            enum Type {
                                Circular,
                                Rectangular
                            } type;
                            util::f32 aspect;
                            bool show;
                            bool overshoot;

                            Cone () :
                                type (Circular),
                                aspect (1.0f),
                                show (false),
                                overshoot (false) {}
                            Cone (
                                Type type_,
                                util::f32 aspect_,
                                bool show_,
                                bool overshoot_) :
                                type (type_),
                                aspect (aspect_),
                                show (show_),
                                overshoot (overshoot_) {}

                            inline void Swap (Cone &cone) {
                                std::swap (type, cone.type);
                                std::swap (aspect, cone.aspect);
                                std::swap (show, cone.show);
                                std::swap (overshoot, cone.overshoot);
                            }

                            bool CalcPoints (
                                const blas::Point3 &position,
                                const blas::Point3 &target,
                                util::f32 fov,
                                util::f32 roll,
                                std::vector<blas::Point3> &points) const;
                        } cone;
                        struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Shadow {
                            enum Type {
                                UseShadowMap,
                                UseRayTraceShadow
                            } type;
                            util::f32 bias;
                            util::f32 filter;
                            util::i16 mapSize;
                            util::f32 rayBias;
                            bool cast;
                            bool local;

                            Shadow () :
                                type (UseShadowMap),
                                bias (1.0f),
                                filter (3.0f),
                                mapSize (512),
                                rayBias (1.0f),
                                cast (false),
                                local (false) {}
                            Shadow (
                                Type type_,
                                util::f32 bias_,
                                util::f32 filter_,
                                util::i16 mapSize_,
                                util::f32 rayBias_,
                                bool cast_,
                                bool local_) :
                                type (type_),
                                bias (bias_),
                                filter (filter_),
                                mapSize (mapSize_),
                                rayBias (rayBias_),
                                cast (cast_),
                                local (local_) {}

                            inline void Swap (Shadow &shadow) {
                                std::swap (type, shadow.type);
                                std::swap (bias, shadow.bias);
                                std::swap (filter, shadow.filter);
                                std::swap (mapSize, shadow.mapSize);
                                std::swap (rayBias, shadow.rayBias);
                                std::swap (cast, shadow.cast);
                                std::swap (local, shadow.local);
                            }
                        } shadow;
                        struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Projector {
                            std::string bitmap;
                            bool use;

                            Projector () :
                                use (false) {}
                            Projector (
                                const std::string &bitmap_,
                                bool use_) :
                                bitmap (bitmap_),
                                use (use_) {}

                            inline void Swap (Projector &projector) {
                                bitmap.swap (projector.bitmap);
                                std::swap (use, projector.use);
                            }
                        } projector;

                        Spot () :
                            target (1.0f, 1.0f, 1.0f),
                            hotSpot (44.0f),
                            fallOff (45.0f),
                            roll (0.0f) {}
                        Spot (
                            const blas::Point3 &target_,
                            util::f32 hotSpot_,
                            util::f32 fallOff_,
                            util::f32 roll_,
                            const Cone &cone_,
                            const Shadow &shadow_,
                            const Projector &projector_) :
                            target (target_),
                            hotSpot (hotSpot_),
                            fallOff (fallOff_),
                            roll (roll_),
                            cone (cone_),
                            shadow (shadow_),
                            projector (projector_) {}
                        Spot (const Spot &spot) :
                            target (spot.target),
                            hotSpot (spot.hotSpot),
                            fallOff (spot.fallOff),
                            roll (spot.roll),
                            cone (spot.cone),
                            shadow (spot.shadow),
                            projector (spot.projector) {}

                        inline void Swap (Spot &spot) {
                            std::swap (target, spot.target);
                            std::swap (hotSpot, spot.hotSpot);
                            std::swap (fallOff, spot.fallOff);
                            std::swap (roll, spot.roll);
                            cone.Swap (spot.cone);
                            shadow.Swap (spot.shadow);
                            projector.Swap (spot.projector);
                        }
                    };
                    Spot::UniquePtr spot;

                    Light (const std::string &name_ = std::string ()) :
                        name (name_),
                        position (blas::Point3::Zero),
                        color (0.7f, 0.7f, 0.7f),
                        multiplier (1.0f),
                        on (true) {}
                    Light (
                        const std::string &name_,
                        const blas::Point3 &position_,
                        const thekogans::mex::opengl::f32Color &color_,
                        util::f32 multiplier_,
                        const Attenuation &attenuation_,
                        const std::vector<std::string> &excludes_,
                        bool on_,
                        const Spot *spot_) :
                        name (name_),
                        position (position_),
                        color (color_),
                        multiplier (multiplier_),
                        attenuation (attenuation_),
                        excludes (excludes_),
                        on (on_),
                        spot (spot_ != 0 ? new Spot (*spot_) : 0) {}
                    Light (const Light &light) :
                        name (light.name),
                        position (light.position),
                        color (light.color),
                        multiplier (light.multiplier),
                        attenuation (light.attenuation),
                        excludes (light.excludes),
                        on (light.on),
                        spot (light.spot.get () != 0 ? new Spot (*light.spot) : 0) {}

                    inline Light &operator = (const Light &light) {
                        if (&light != this) {
                            name = light.name;
                            position = light.position;
                            color = light.color;
                            multiplier = light.multiplier;
                            attenuation = light.attenuation;
                            excludes = light.excludes;
                            on = light.on;
                            spot.reset (light.spot.get () ? new Spot (*light.spot) : 0);
                        }
                        return *this;
                    }

                    inline void Swap (Light &light) {
                        assert (&light != this);
                        name.swap (light.name);
                        std::swap (position, light.position);
                        std::swap (color, light.color);
                        std::swap (multiplier, light.multiplier);
                        attenuation.Swap (light.attenuation);
                        excludes.swap (light.excludes);
                        std::swap (on, light.on);
                        std::swap (spot, light.spot);
                    }
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_editor_Light_h)
