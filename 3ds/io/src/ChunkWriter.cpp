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

#include <cstdio>
#include "thekogans/mex/3ds/io/ChunkWriter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void ChunkWriter::Start (util::ui16 id_) {
                #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    THEKOGANS_UTIL_TRY {
                #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        id = id_;
                        start = end = stream.Tell ();
                        length = 6L;
                        stream.Seek (length, SEEK_CUR);
                #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }
                    THEKOGANS_UTIL_CATCH (std::exception) {
                        throw Exception (__FILE__, __LINE__, __DATE__ " " __TIME__,
                            Exception::Write, id, length, start, end);
                    }
                #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                }

                void ChunkWriter::End () {
                #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    THEKOGANS_UTIL_TRY {
                #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        end = stream.Tell ();
                        length = (util::ui32)(end - start);
                        stream.Seek (start, SEEK_SET);
                        stream << id << length;
                        stream.Seek (end, SEEK_SET);
                #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }
                    THEKOGANS_UTIL_CATCH (std::exception) {
                        throw Exception (__FILE__, __LINE__, __DATE__ " " __TIME__,
                            Exception::Write, id, length, start, end);
                    }
                #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
