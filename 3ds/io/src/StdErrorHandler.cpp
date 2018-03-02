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

#include <iostream>
#include <cctype>
#include "thekogans/mex/3ds/io/StdErrorHandler.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/Util.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                namespace {
                    inline char ASCIIchar (util::ui8 ch) {
                        return isprint (ch) ? (char)ch : '.';
                    }

                    void DumpLine (const util::ui8 *line, util::ui32 count) {
                        {
                            const char *hexChar = "0123456789abcdef";
                            util::ui32 i;
                            for (i = 0; i < count; ++i) {
                                std::cout << hexChar[(line[i] >> 4) & 0xf] << hexChar[line[i] & 0xf] << " ";
                            }
                            for (; i < 16; ++i) {
                                std::cout << "   ";
                            }
                            std::cout << " ";
                        }
                        {
                            for (util::ui32 i = 0; i < count; ++i) {
                                std::cout << ASCIIchar (line[i]);
                            }
                            std::cout << std::endl;
                        }
                    }

                    void DumpChunk (util::i64 position, util::i64 end, ChunkReader &chunk) {
                        for (util::i64 i = 0, lineCount = (end - position) / 16; i < lineCount; ++i) {
                            util::ui8 line[16];
                            chunk.Read (line, 16);
                            DumpLine (line, 16);
                        }
                        util::ui32 lastLineCharCount = (end - position) % 16;
                        if (lastLineCharCount != 0) {
                            util::ui8 lastLine[16];
                            chunk.Read (lastLine, lastLineCharCount);
                            DumpLine (lastLine, lastLineCharCount);
                        }
                    }
                }

                void StdErrorHandler::HandleUknownChunk (const std::string &module,
                        const std::string &func, ChunkReader &chunk) {
                    std::cout << module << ": UNKNOWN CHUNK: " << func << ", ID: " <<
                        chunk.GetID () << ", LENGTH: " << chunk.GetLength () << std::endl;
                }

                void StdErrorHandler::HandleUnreadChunk (util::i64 position, util::i64 end, ChunkReader &chunk) {
                    std::cout <<
                        "CHUNK TRACE: " << std::endl <<
                        "\tID: " << ChunkNameFromID (chunk.GetID ()) << " (0x" << std::hex << chunk.GetID () << ")" << std::endl <<
                        "\tLENGTH: " << chunk.GetLength () << std::endl <<
                        "\tPOSITION: " << position << std::endl <<
                        "\tEND: " << end << std::endl;
                    DumpChunk (position, end, chunk);
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
