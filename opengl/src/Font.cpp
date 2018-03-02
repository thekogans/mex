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

#include "thekogans/mex/opengl/FontMgr.h"
#include "thekogans/mex/opengl/Font.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            Font::Font (const FontMgr::Font *font_) :
                    font (font_) {
                // Save the current pixel state.
                glPushClientAttrib (GL_CLIENT_PIXEL_STORE_BIT);
                // Font glyphs are packed in bytes
                // without any other alignment.
                glPixelStorei (GL_UNPACK_SWAP_BYTES, GL_FALSE);
                glPixelStorei (GL_UNPACK_LSB_FIRST, GL_FALSE);
                glPixelStorei (GL_UNPACK_ROW_LENGTH, 0);
                glPixelStorei (GL_UNPACK_SKIP_ROWS, 0);
                glPixelStorei (GL_UNPACK_SKIP_PIXELS, 0);
                glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
            }

            Font::~Font () {
                glPopClientAttrib ();
            }

            void Font::DrawText (
                    util::i32 x,
                    util::i32 y,
                    const char *text,
                    const blas::Size &) {
                glRasterPos2i (x, y);
                for (; *text != '\0'; ++text) {
                    const FontMgr::Font::Glyph *glyph = font->GetGlyph (*text);
                    if (glyph != 0) {
                        glBitmap (glyph->GetWidth (), font->GetHeight (), 0.0f, 0.0f,
                            (GLfloat)glyph->GetWidth (), 0.0f, glyph->GetBitmap ());
                    }
                }
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
