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

#include "thekogans/util/Heap.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/Font2.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Font2::Glyph)

            Font2::Glyph::Glyph (
                    FT_Face face,
                    unsigned char c) :
                    width (0),
                    rows (0),
                    bearingX (0),
                    bearingY (0),
                    advance (0),
                    texture (0) {
                glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
                glEnable (GL_TEXTURE_2D);
                if (FT_Load_Char (face, c, FT_LOAD_RENDER) == 0) {
                    width = face->glyph->bitmap.width;
                    rows = face->glyph->bitmap.rows;
                    bearingX = face->glyph->bitmap_left;
                    bearingY = face->glyph->bitmap_top;
                    advance = face->glyph->advance.x;
#if 0
                    bitmap = new util::ui8[width * rows];
                    std::memcpy (bitmap, face->glyph->bitmap.buffer, width * rows);
#ele
                    glGenTextures (1, &texture);
                    glBindTexture (GL_TEXTURE_2D, texture);
                    glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA,
                        width, rows, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);
                    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
                }
            }

            Font2::Glyph::~Glyph () {
                //delete [] bitmap;
            }

            Font2::Font2 (
                    const std::string path,
                    FT_UInt pixelHeight) {
                if (FT_Init_FreeType (&library)) {
                }
                if (FT_New_Face (library, path.c_str (), 0, &face)) {
                    FT_Done_FreeType (library);
                }
                FT_Set_Pixel_Sizes (face, 0, pixelHeight);
            }

            Font2::~Font2 () {
                FT_Done_Face (face);
                FT_Done_FreeType (library);
            }

            void Font2::DrawText (
                    const char *text,
                    util::i32 x,
                    util::i32 y,
                    const ui8Color &color) {
#if 1
                // Activate state machine configurations for text transparency
                glEnable (GL_TEXTURE_2D);
                glEnable (GL_BLEND);
                glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                // Tell OpenGL to combine texture color with glColor
                glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                Color color_ (color); // Set text color
                FT_UInt prev_glyph_index = 0;
                bool has_kerning = FT_HAS_KERNING (face);
                while (*text != '\0') {
                    unsigned char c = *text++;
                    if (glyphs[c] == nullptr) {
                        glyphs[c].Reset (new Glyph (face, c));
                    }
                    Glyph::SharedPtr glyph = glyphs[c];
                    // Process True Font Kerning
                    if (has_kerning && prev_glyph_index && c) {
                        FT_UInt glyph_index = FT_Get_Char_Index (face, c);
                        FT_Vector delta;
                        FT_Get_Kerning (face, prev_glyph_index, glyph_index, FT_KERNING_DEFAULT, &delta);
                        x += delta.x >> 6; // Convert 1/64th pixels to screen pixels
                        prev_glyph_index = glyph_index;
                    }
                    else {
                        prev_glyph_index = FT_Get_Char_Index (face, c);
                    }
                    // Calculate positions relative to the font baseline matrix
                    float xpos = x + glyph->bearingX;
                    float ypos = y - (glyph->rows - glyph->bearingY); // Push downwards for descenders (g, j, p)
                    float w = glyph->width;
                    float h = glyph->rows;
                    glBindTexture (GL_TEXTURE_2D, glyph->texture);
                    // Draw the text quad immediate mode style
                    glBegin (GL_QUADS);
                    glTexCoord2f (0.0f, 0.0f);
                    glVertex2f (xpos, ypos + h); // Top Left
                    glTexCoord2f (0.0f, 1.0f);
                    glVertex2f (xpos, ypos);     // Bottom Left
                    glTexCoord2f (1.0f, 1.0f);
                    glVertex2f (xpos + w, ypos);     // Bottom Right
                    glTexCoord2f (1.0f, 0.0f);
                    glVertex2f (xpos + w, ypos + h); // Top Right
                    glEnd ();
                    // Advance character position pointer (advance is in 1/64th pixels)
                    x += glyph->advance >> 6;
                }
#else
                Color color_ (color); // Set text color
                glRasterPos2i (x, y);
                for (; *text != '\0'; ++text) {
                    if (glyphs[*text] == nullptr) {
                        glyphs[*text].Reset (new Glyph (face, *text));
                    }
                    Glyph::SharedPtr glyph = glyphs[*text];
                    glBitmap (glyph->width, glyph->rows, 0.0f, 0.0f,
                        (GLfloat)glyph->width, 0.0f, glyph->bitmap);
                }
#endif
            }

            std::size_t Font2::GetStringWidth (const char *text) {
                std::size_t width = 0;
                FT_UInt prev_glyph_index = 0;
                bool has_kerning = FT_HAS_KERNING (face);
                while (*text != '\0') {
                    unsigned char c = *text++;
                    if (glyphs[c] == nullptr) {
                        glyphs[c].Reset (new Glyph (face, c));
                    }
                    Glyph::SharedPtr glyph = glyphs[c];
                    if (has_kerning && prev_glyph_index && c) {
                        FT_UInt glyph_index = FT_Get_Char_Index (face, c);
                        FT_Vector delta;
                        FT_Get_Kerning (face, prev_glyph_index, glyph_index, FT_KERNING_DEFAULT, &delta);
                        width += (delta.x >> 6);
                        prev_glyph_index = glyph_index;
                    }
                    else {
                        prev_glyph_index = FT_Get_Char_Index (face, c);
                    }
                    width += glyph->advance >> 6;
                }
                return width;
            }


        } // namespace opengl
    } // namespace mex
} // namespace thekogans
