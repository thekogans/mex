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

#if !defined (__thekogans_mex_opengl_Font_h)
#define __thekogans_mex_opengl_Font_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/opengl/Config.h"
#include "thekogans/mex/opengl/FontMgr.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL Font {
                const FontMgr::Font *font;

                explicit Font (const FontMgr::Font *font_);
                ~Font ();

                void DrawText (
                    util::i32 x,
                    util::i32 y,
                    const char *text,
                    const blas::Size &size = blas::Size::Empty);
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Font_h)
