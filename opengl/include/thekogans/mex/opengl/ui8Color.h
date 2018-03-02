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

#if !defined (__thekogans_mex_opengl_ui8Color_h)
#define __thekogans_mex_opengl_ui8Color_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Serializer.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct ui8Color {
                util::ui8 r;
                util::ui8 g;
                util::ui8 b;

                ui8Color () {}
                ui8Color (
                    util::ui8 r_,
                    util::ui8 g_,
                    util::ui8 b_) :
                    r (r_),
                    g (g_),
                    b (b_) {}

                static ui8Color Fromui32 (util::ui32 rgb) {
                    ui8Color color;
                    color.r = (rgb >> 16) & 0xff;
                    color.g = (rgb >> 8) & 0xff;
                    color.b = (rgb >> 0) & 0xff;
                    return color;
                }

                static util::ui32 Toui32 (const ui8Color &color) {
                    return ((util::ui32)color.r << 16) | ((util::ui32)color.g << 8) | ((util::ui32)color.b << 0);
                }
            };

            inline bool operator == (
                    const ui8Color &color1,
                    const ui8Color &color2) {
                return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
            }

            inline bool operator != (
                    const ui8Color &color1,
                    const ui8Color &color2) {
                return color1.r != color2.r || color1.g != color2.g || color1.b != color2.b;
            }

            inline ui8Color operator ^ (
                    const ui8Color &color1,
                    const ui8Color &color2) {
                return ui8Color (color1.r ^ color2.r, color1.g ^ color2.g, color1.b ^ color2.b);
            }

            inline util::Serializer &operator << (
                    util::Serializer &serializer,
                    const ui8Color &color) {
                serializer << color.r << color.g << color.b;
                return serializer;
            }

            inline util::Serializer &operator >> (
                    util::Serializer &serializer,
                    ui8Color &color) {
                serializer >> color.r >> color.g >> color.b;
                return serializer;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_ui8Color_h)
