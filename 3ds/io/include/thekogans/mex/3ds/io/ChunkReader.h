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

#if !defined (__thekogans_mex_3ds_io_ChunkReader_h)
#define __thekogans_mex_3ds_io_ChunkReader_h

#include <cstdio>
#include "thekogans/util/Types.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/Progress.h"
#include "thekogans/mex/3ds/io/FixedString.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/io/ErrorHandler.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct ChunkReader {
                protected:
                    util::File &file;
                    Progress *progress;
                    ErrorHandler *errorHandler;
                    util::ui16 id;
                    util::ui32 length;
                    util::i64 start;
                    util::i64 end;
                    bool rewind;

                public:
                    struct Exception : public util::Exception {
                        enum Type {
                            Read,
                            Tell,
                            Seek
                        } type;
                        util::ui16 id;
                        util::ui32 length;
                        util::i64 start;
                        util::i64 end;

                        Exception (
                            const char *file,
                            const char *function,
                            util::ui32 line,
                            const char *buildTime,
                            Type type_,
                            util::ui16 id_,
                            util::ui32 length_,
                            util::i64 start_,
                            util::i64 end_) :
                            util::Exception (file, function, line, buildTime, 0, std::string ()),
                            type (type_),
                            id (id_),
                            length (length_),
                            start (start_),
                            end (end_) {}
                    };

                    ChunkReader (
                        util::File &file_,
                        Progress *progress_ = 0,
                        ErrorHandler *errorHandler_ = 0);
                    ChunkReader (const ChunkReader &chunk);
                    ~ChunkReader ();

                    inline void HandleUknownChunk (
                            const std::string &module,
                            const std::string &func) {
                        if (errorHandler != 0) {
                            errorHandler->HandleUknownChunk (module, func, *this);
                        }
                    }

                    inline util::ui16 GetID () const {return id;}
                    inline util::ui32 GetLength () const {return length;}
                    inline util::i64 GetStart () const {return start;}
                    inline bool IsEnd () const {return Tell () >= end;}

                    inline util::i64 Tell () const {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            return file.Tell ();
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Tell, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline util::i64 Seek (util::i64 offs, util::i32 origin) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            return file.Seek (offs, origin);
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Seek, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline void Rewind () {
                        Seek (start, SEEK_SET);
                        rewind = true;
                    }

                    inline std::size_t Read (void *buffer, util::ui32 count) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            return file.Read (buffer, count);
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (std::string &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    // 3D Studio stores bools as util::ui16 values with 0 = false.
                    inline ChunkReader &operator >> (bool &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            util::ui16 value_;
                            file >> value_;
                            value = value_ != 0;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::i8 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::ui8 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::i16 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::ui16 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::i32 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::ui32 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (util::f32 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::Size &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::Point &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::Point2 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::Point3 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::AngleAxis &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::Matrix3 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (blas::Rect &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    template<typename T>
                    inline ChunkReader &operator >> (blas::Bound<T> &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (FixedString &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (thekogans::mex::opengl::ui8Color &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkReader &operator >> (thekogans::mex::opengl::f32Color &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            file >> value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Read, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                private:
                    void Start ();
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_ChunkReader_h)
