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

#include <vector>
#include "thekogans/util/Rectangle.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/BlendFunc.h"
#include "thekogans/mex/opengl/UnpackAlignment.h"
#include "thekogans/mex/opengl/Font.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            void Font::DrawText (
                    util::i32 x,
                    util::i32 y,
                    const std::string &text,
                    const blas::Size &) {
                std::vector<canvas::Font::Glyph *> glyphs;
                std::vector<FT_Vector> positions;
                font->GetGlyphs (text, glyphs, positions);
                Enable blend (GL_BLEND, true);
                BlendFunc blendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                UnpackAlignment unpackAlignment (1);
                for (std::size_t i = 0, count = glyphs.size (); i < count; ++i) {
                    glRasterPos2i (
                        x + positions[i].x + glyphs[i]->GetLeftSideBearing (),
                        y + positions[i].y + glyphs[i]->GetTopSideBearing () - glyphs[i]->size.height);
                    glDrawPixels (glyphs[i]->size.width, glyphs[i]->size.height, GL_ALPHA,
                        GL_UNSIGNED_BYTE, (const GLubyte *)glyphs[i]->GetData ());
                }
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
