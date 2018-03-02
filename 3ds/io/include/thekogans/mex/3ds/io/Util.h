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

#if !defined (__thekogans_mex_3ds_io_Util_h)
#define __thekogans_mex_3ds_io_Util_h

#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void ReadColor (Color &color, ChunkReader &mainChunk);

                template<typename T>
                inline void WriteColor (util::ui16 id, const T &color, util::File &file) {
                    ChunkWriter chunk (file, id);
                    chunk << color;
                }

                template<>
                inline void WriteColor (util::ui16 id, const Color &color, util::File &file) {
                    ChunkWriter chunk (file, id);
                    if (color.gammaCorrected.type == Color::Parts::UI8_COLOR) {
                        WriteColor (COLOR_24, color.gammaCorrected.byteColor, file);
                    }
                    else if (color.gammaCorrected.type == Color::Parts::F32_COLOR) {
                        WriteColor (COLOR_F, color.gammaCorrected.floatColor, file);
                    }
                    if (color.raw.type == Color::Parts::UI8_COLOR) {
                        WriteColor (LIN_COLOR_24, color.raw.byteColor, file);
                    }
                    else if (color.raw.type == Color::Parts::F32_COLOR) {
                        WriteColor (LIN_COLOR_F, color.raw.floatColor, file);
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_IO_DECL const char * _LIB_THEKOGANS_MEX_3DS_IO_API ChunkNameFromID (
                    util::ui16 id);

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Util_h)
