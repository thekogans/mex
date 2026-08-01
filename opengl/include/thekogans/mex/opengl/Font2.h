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

#if !defined (__thekogans_mex_opengl_Font2_h)
#define __thekogans_mex_opengl_Font2_h

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "thekogans/util/Types.h"
#include "thekogans/util/RefCounted.h"
#include "thekogans/mex/opengl/Config.h"
#include "thekogans/mex/opengl/ui8Color.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL Font2 {
            private:
                FT_Library library;
                FT_Face face;
                struct Glyph : public util::RefCounted {
                    THEKOGANS_UTIL_DECLARE_REF_COUNTED_POINTERS (Glyph)
                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    int width;
                    int rows;
                    int bearingX; // Offset from baseline to left of glyph
                    int bearingY; // Offset from baseline to top of glyph
                    long advance; // Horizontal advance (scaled by 1/64th pixel)
                    //util::ui8 *bitmap;
                    GLuint texture; // ID handle of the glyph texture

                    Glyph (
                        FT_Face face,
                        unsigned char c);
                    ~Glyph ();
                };
                Glyph::SharedPtr glyphs[256];

            public:
                explicit Font2 (
                    const std::string path,
                    FT_UInt pixelHeight);
                ~Font2 ();

                void DrawText (
                    const char *text,
                    util::i32 x,
                    util::i32 y,
                    const ui8Color &color);
                std::size_t GetStringWidth (const char *text);
                inline std::size_t GetHeight () const {
                    return face->size->metrics.height >> 6;
                }
                inline std::size_t GetAveCharWidth () const {
                    return face->size->metrics.height >> 6;
                }
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Font2_h)
