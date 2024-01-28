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

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/canvas/Font.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL Font {
                std::unique_ptr<canvas::Font> font;

                Font (
                    const std::string &path,
                    util::ui32 width,
                    util::ui32 height,
                    util::ui32 horizontalResolution = 96,
                    util::ui32 verticalResolution = 96,
                    bool flipGliphRows = true) :
                    font (
                        new canvas::Font (
                            path,
                            width,
                            height,
                            horizontalResolution,
                            verticalResolution,
                            flipGliphRows)) {}

                void DrawText (
                    util::i32 x,
                    util::i32 y,
                    const std::string &text,
                    const blas::Size &size = blas::Size::Empty);
            };

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_Font_h)
