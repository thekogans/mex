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

#if !defined (__thekogans_mex_3ds_io_FixedString_h)
#define __thekogans_mex_3ds_io_FixedString_h

#include <string>
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/3ds/io/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL FixedString {
                    std::string value;
                    size_t count;

                    explicit FixedString (size_t count_) :
                        count (count_) {}
                    FixedString (
                        const char *value_,
                        size_t count_) :
                        value (value_),
                        count (count_) {}

                    inline FixedString &operator = (const char *value_) {
                        value = value_;
                        return *this;
                    }
                };

                inline bool operator == (
                        const FixedString &fixedString1,
                        const FixedString &fixedString2) {
                    return fixedString1.value == fixedString2.value;
                }

                _LIB_THEKOGANS_MEX_3DS_IO_DECL util::Serializer & _LIB_THEKOGANS_MEX_3DS_IO_API operator << (
                    util::Serializer &serializer,
                    const FixedString &fixedString);
                _LIB_THEKOGANS_MEX_3DS_IO_DECL util::Serializer & _LIB_THEKOGANS_MEX_3DS_IO_API operator >> (
                    util::Serializer &serializer,
                    FixedString &fixedString);

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_FixedString_h)
