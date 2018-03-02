// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_opengl_f32Color_h)
#define __thekogans_mex_opengl_f32Color_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Serializer.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct f32Color {
                util::f32 r;
                util::f32 g;
                util::f32 b;

                f32Color () {}
                f32Color (
                    util::f32 r_,
                    util::f32 g_,
                    util::f32 b_) :
                    r (r_),
                    g (g_),
                    b (b_) {}
            };

            inline bool operator == (
                    const f32Color &color1,
                    const f32Color &color2) {
                return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
            }

            inline bool operator != (
                    const f32Color &color1,
                    const f32Color &color2) {
                return color1.r != color2.r || color1.g != color2.g || color1.b != color2.b;
            }

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const f32Color &color) {
                serializer << color.r << color.g << color.b;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    f32Color &color) {
                serializer >> color.r >> color.g >> color.b;
                return serializer;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_f32Color_h)
