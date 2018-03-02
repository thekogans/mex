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

#if !defined (__thekogans_mex_3ds_io_Color_h)
#define __thekogans_mex_3ds_io_Color_h

#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/opengl/ui8Color.h"
#include "thekogans/mex/opengl/f32Color.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Color {
                    struct Parts {
                        enum Type {
                            NO_COLOR,
                            UI8_COLOR,
                            F32_COLOR
                        } type;
                        thekogans::mex::opengl::ui8Color byteColor;
                        thekogans::mex::opengl::f32Color floatColor;

                        Parts () : type (NO_COLOR) {}
                        Parts (const thekogans::mex::opengl::ui8Color &byteColor_) :
                            type (UI8_COLOR), byteColor (byteColor_) {}
                        Parts (const thekogans::mex::opengl::f32Color &floatColor_) :
                            type (F32_COLOR), floatColor (floatColor_) {}

                        inline void Swap (Parts &parts) {
                            std::swap (byteColor, parts.byteColor);
                            std::swap (floatColor, parts.floatColor);
                        }
                    };
                    Parts gammaCorrected;
                    Parts raw;

                    Color () {}
                    Color (const thekogans::mex::opengl::ui8Color &byteColor) :
                        gammaCorrected (byteColor), raw (byteColor) {}
                    Color (const thekogans::mex::opengl::f32Color &floatColor) :
                        gammaCorrected (floatColor), raw (floatColor) {}

                    inline void Swap (Color &color) {
                        std::swap (gammaCorrected, color.gammaCorrected);
                        std::swap (raw, color.raw);
                    }

                    thekogans::mex::opengl::f32Color Getf32Color (
                        const thekogans::mex::opengl::f32Color &defaultColor =
                            thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)) const;
                    thekogans::mex::opengl::ui8Color Getui8Color (
                        const thekogans::mex::opengl::ui8Color &defaultColor =
                            thekogans::mex::opengl::ui8Color (0, 0, 0)) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Color_h)
