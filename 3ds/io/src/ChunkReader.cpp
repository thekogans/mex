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

#include "thekogans/mex/3ds/io/ChunkReader.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                ChunkReader::ChunkReader (
                        util::File &file_,
                        Progress *progress_,
                        ErrorHandler *errorHandler_) :
                        file (file_),
                        progress (progress_),
                        errorHandler (errorHandler_) {
                    if (progress) {
                        progress->SetTotal ((util::ui32)file.GetSize ());
                    }
                    Start ();
                }

                ChunkReader::ChunkReader (const ChunkReader &chunk) :
                        file (chunk.file),
                        progress (chunk.progress),
                        errorHandler (chunk.errorHandler) {
                    Start ();
                }

                ChunkReader::~ChunkReader () {
                    if (!rewind) {
                        if (errorHandler) {
                            util::i64 position = Tell ();
                            if (position != end) {
                                errorHandler->HandleUnreadChunk (position, end, *this);
                            }
                        }
                        if (length) {
                            Seek (end, SEEK_SET);
                        }
                        if (progress) {
                            progress->SetPosition ((util::ui32)Tell ());
                        }
                    }
                }

                void ChunkReader::Start () {
                    start = Tell ();
                #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    THEKOGANS_UTIL_TRY {
                #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        file >> id >> length;
                #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }
                    THEKOGANS_UTIL_CATCH (std::exception) {
                        throw Exception (__FILE__, __LINE__, __DATE__ " " __TIME__,
                            Exception::Tell, id, length, start, end);
                    }
                #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    end = start + length;
                    rewind = false;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
