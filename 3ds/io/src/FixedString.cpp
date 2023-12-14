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

#include <algorithm>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/FixedString.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                _LIB_THEKOGANS_MEX_3DS_IO_DECL util::Serializer & _LIB_THEKOGANS_MEX_3DS_IO_API operator << (
                        util::Serializer &serializer,
                        const FixedString &fixedString) {
                    const char *ptr = fixedString.value.c_str ();
                    size_t count = std::min (fixedString.count, strlen (ptr));
                    serializer.Write (ptr, (util::ui32)count);
                    const util::i8 zero = 0;
                    for (; count < fixedString.count; ++count) {
                        serializer << zero;
                    }
                    return serializer;
                }

                _LIB_THEKOGANS_MEX_3DS_IO_DECL util::Serializer & _LIB_THEKOGANS_MEX_3DS_IO_API operator >> (
                        util::Serializer &serializer,
                        FixedString &fixedString) {
                    fixedString.value.clear ();
                    for (size_t i = 0; i < fixedString.count; ++i) {
                        util::i8 ch;
                        serializer >> ch;
                        if (ch == 0) {
                            break;
                        }
                        fixedString.value += ch;
                    }
                    for (size_t i = fixedString.value.size () + 1; i < fixedString.count; ++i) {
                        util::i8 ch;
                        serializer >> ch;
                    }
                    return serializer;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
