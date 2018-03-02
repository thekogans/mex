// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_DefaultPalette_h)
#define __thekogans_mex_3ds_opengl_DefaultPalette_h

#include "thekogans/mex/opengl/ui8Color.h"
#include "thekogans/mex/3ds/io/Color.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                const thekogans::mex::opengl::ui8Color DefaultPalette[] = {
                    thekogans::mex::opengl::ui8Color (  0,   0,   0), // SYSTEM_PALETTE
                    thekogans::mex::opengl::ui8Color (  0, 114, 160),
                    thekogans::mex::opengl::ui8Color (  0, 168,   0),
                    thekogans::mex::opengl::ui8Color (129, 187, 202),
                    thekogans::mex::opengl::ui8Color (168,   0,   0),
                    thekogans::mex::opengl::ui8Color (232, 232, 232),
                    thekogans::mex::opengl::ui8Color (115, 115, 115),
                    thekogans::mex::opengl::ui8Color (141, 141, 141),
                    thekogans::mex::opengl::ui8Color (192, 192, 192),
                    thekogans::mex::opengl::ui8Color ( 84,  84, 255),
                    thekogans::mex::opengl::ui8Color ( 84, 255,  84),
                    thekogans::mex::opengl::ui8Color ( 84, 255, 255),
                    thekogans::mex::opengl::ui8Color (255,  84,  84),
                    thekogans::mex::opengl::ui8Color (192, 148,   0),
                    thekogans::mex::opengl::ui8Color (255, 255,   0),
                    thekogans::mex::opengl::ui8Color (255, 255, 255), // OBJECT_PALETTE
                    thekogans::mex::opengl::ui8Color (252, 184, 236),
                    thekogans::mex::opengl::ui8Color (236, 252, 184),
                    thekogans::mex::opengl::ui8Color (184, 252, 252),
                    thekogans::mex::opengl::ui8Color (252, 168, 168),
                    thekogans::mex::opengl::ui8Color (248,  96,  96),
                    thekogans::mex::opengl::ui8Color (196,  28,  28),
                    thekogans::mex::opengl::ui8Color (148,   4,   4),
                    thekogans::mex::opengl::ui8Color (252, 204, 168),
                    thekogans::mex::opengl::ui8Color (248, 156,  96),
                    thekogans::mex::opengl::ui8Color (196,  96,  28),
                    thekogans::mex::opengl::ui8Color (148,  64,   8),
                    thekogans::mex::opengl::ui8Color (252, 236, 168),
                    thekogans::mex::opengl::ui8Color (248, 220,  96),
                    thekogans::mex::opengl::ui8Color (196, 164,  28),
                    thekogans::mex::opengl::ui8Color (148, 120,   8),
                    thekogans::mex::opengl::ui8Color (236, 252, 168),
                    thekogans::mex::opengl::ui8Color (220, 248,  96),
                    thekogans::mex::opengl::ui8Color (164, 196,  28),
                    thekogans::mex::opengl::ui8Color (124, 148,   4),
                    thekogans::mex::opengl::ui8Color (204, 252, 168),
                    thekogans::mex::opengl::ui8Color (156, 248,  96),
                    thekogans::mex::opengl::ui8Color ( 96, 196,  28),
                    thekogans::mex::opengl::ui8Color ( 68, 148,   4),
                    thekogans::mex::opengl::ui8Color (168, 252, 168),
                    thekogans::mex::opengl::ui8Color ( 96, 248,  96),
                    thekogans::mex::opengl::ui8Color ( 28, 196,  28),
                    thekogans::mex::opengl::ui8Color (  4, 148,   4),
                    thekogans::mex::opengl::ui8Color (168, 252, 204),
                    thekogans::mex::opengl::ui8Color ( 96, 248, 156),
                    thekogans::mex::opengl::ui8Color ( 28, 196,  96),
                    thekogans::mex::opengl::ui8Color (  4, 148,  64),
                    thekogans::mex::opengl::ui8Color (168, 252, 236),
                    thekogans::mex::opengl::ui8Color ( 96, 248, 220),
                    thekogans::mex::opengl::ui8Color ( 28, 196, 164),
                    thekogans::mex::opengl::ui8Color (  4, 148, 124),
                    thekogans::mex::opengl::ui8Color (172, 236, 252),
                    thekogans::mex::opengl::ui8Color ( 96, 220, 248),
                    thekogans::mex::opengl::ui8Color ( 32, 164, 196),
                    thekogans::mex::opengl::ui8Color (  8, 120, 148),
                    thekogans::mex::opengl::ui8Color (172, 204, 252),
                    thekogans::mex::opengl::ui8Color ( 96, 160, 248),
                    thekogans::mex::opengl::ui8Color ( 32,  96, 196),
                    thekogans::mex::opengl::ui8Color (  8,  68, 152),
                    thekogans::mex::opengl::ui8Color (172, 172, 252),
                    thekogans::mex::opengl::ui8Color ( 96,  96, 248),
                    thekogans::mex::opengl::ui8Color ( 32,  32, 196),
                    thekogans::mex::opengl::ui8Color (  8,   8, 152),
                    thekogans::mex::opengl::ui8Color (204, 172, 252),
                    thekogans::mex::opengl::ui8Color (156,  96, 248),
                    thekogans::mex::opengl::ui8Color ( 92,  32, 196),
                    thekogans::mex::opengl::ui8Color ( 64,   8, 152),
                    thekogans::mex::opengl::ui8Color (236, 172, 252),
                    thekogans::mex::opengl::ui8Color (216,  96, 248),
                    thekogans::mex::opengl::ui8Color (160,  32, 196),
                    thekogans::mex::opengl::ui8Color (120,   8, 152),
                    thekogans::mex::opengl::ui8Color (252, 172, 236),
                    thekogans::mex::opengl::ui8Color (248,  96, 220),
                    thekogans::mex::opengl::ui8Color (196,  32, 164),
                    thekogans::mex::opengl::ui8Color (152,   8, 120),
                    thekogans::mex::opengl::ui8Color (252, 168, 204),
                    thekogans::mex::opengl::ui8Color (248,  96, 156),
                    thekogans::mex::opengl::ui8Color (196,  28,  96),
                    thekogans::mex::opengl::ui8Color (156,   8,  64)
                };

                const util::ui32 SYSTEM_PALETTE_START_INDEX = 0;
                const util::ui32 SYSTEM_PALETTE_SIZE = 16;

                const util::ui32 OBJECT_PALETTE_START_INDEX = 15;
                const util::ui32 OBJECT_PALETTE_SIZE = 64;

                const util::ui32 DEFAULT_PALETTE_SIZE =
                    sizeof (DefaultPalette) / sizeof (DefaultPalette[0]);

                // System Palette
                // RGB (0, 0, 0)
                const util::ui32 BLACK = 0;
                // RGB (0, 114, 160)
                const util::ui32 BLUE = 1;
                // RGB (0, 168, 0)
                const util::ui32 GREEN = 2;
                // RGB (129, 187, 202)
                const util::ui32 CYAN = 3;
                // RGB (168, 0, 0)
                const util::ui32 RED = 4;
                // RGB (232, 232, 232)
                const util::ui32 LIGHT_GRAY = 5;
                // RGB (115, 115, 115)
                const util::ui32 DARK_GRAY = 6;
                // RGB (141, 141, 141)
                const util::ui32 MEDIUM_GRAY = 7;
                // RGB (192, 192, 192)
                const util::ui32 GRAY = 8;
                // RGB (84, 84, 255)
                const util::ui32 LIGHT_BLUE = 9;
                // RGB (84, 255, 84)
                const util::ui32 LIGHT_GREEN = 10;
                // RGB (84, 255, 255)
                const util::ui32 LIGHT_CYAN = 11;
                // RGB (255, 84, 84)
                const util::ui32 LIGHT_RED = 12;
                // RGB (192, 148, 0)
                const util::ui32 ORANGE = 13;
                // RGB (255, 255, 0)
                const util::ui32 YELLOW = 14;
                // RGB (255, 255, 255)
                const util::ui32 WHITE = 15;
                // 63 object create colors

                const thekogans::mex::opengl::ui8Color SystemXORPalette[] = {
                    DefaultPalette[BLACK] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[BLUE] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[GREEN] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[CYAN] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[RED] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[LIGHT_GRAY] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[DARK_GRAY] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[MEDIUM_GRAY] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[GRAY] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[LIGHT_BLUE] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[LIGHT_GREEN] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[LIGHT_CYAN] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[LIGHT_RED] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[ORANGE] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[YELLOW] ^ DefaultPalette[MEDIUM_GRAY],
                    DefaultPalette[WHITE] ^ DefaultPalette[MEDIUM_GRAY]
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_DefaultPalette_h)
