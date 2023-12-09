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

#if !defined (__thekogans_mex_3ds_io_ChunkWriter_h)
#define __thekogans_mex_3ds_io_ChunkWriter_h

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/FixedString.h"
#include "thekogans/mex/3ds/io/Color.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct ChunkWriter {
                protected:
                    util::File &stream;
                    util::ui16 id;
                    util::ui32 length;
                    util::i64 start;
                    util::i64 end;

                public:
                    struct Exception : public util::Exception {
                        enum Type {
                            Write,
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
                            util::Exception(file, function, line, buildTime, 0, std::string()),
                            type (type_),
                            id (id_),
                            length (length_),
                            start (start_),
                            end (end_) {}
                    };

                    ChunkWriter (util::File &stream_, util::ui16 id_) :
                        stream (stream_), id (id_) {Start (id);}
                    ~ChunkWriter () {End ();}

                    inline std::size_t Write (const void *buffer, util::ui32 count) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            return stream.Write (buffer, count);
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const std::string &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << (const char *)value.c_str ();
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const bool &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            util::ui16 value_ = value ? 1 : 0;
                            stream << value_;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::i8 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::ui8 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::i16 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::ui16 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::i32 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::ui32 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::i64 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::ui64 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::f32 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (util::f64 value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::Size &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::Point &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::Point2 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::Point3 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::AngleAxis &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::Matrix3 &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const blas::Rect &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    template<typename T>
                    inline ChunkWriter &operator << (const blas::Bound<T> &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const FixedString &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const thekogans::mex::opengl::ui8Color &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                    inline ChunkWriter &operator << (const thekogans::mex::opengl::f32Color &value) {
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        THEKOGANS_UTIL_TRY {
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                            stream << value;
                            return *this;
                    #if defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                        }
                        THEKOGANS_UTIL_CATCH (std::exception) {
                            throw Exception (__FILE__, __FUNCTION__, __LINE__, __DATE__ " " __TIME__,
                                Exception::Write, id, length, start, end);
                        }
                    #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS)
                    }

                private:
                    void Start (util::ui16 id_);
                    void End ();
                };

                template<typename T>
                ChunkWriter &operator << (ChunkWriter &chunk, const std::vector<T> &value) {
                    util::ui32 count = value.size ();
                    chunk << count;
                    for (util::ui32 i = 0; i < count; ++i) {
                        chunk << value[i];
                    }
                    return chunk;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_ChunkWriter_h)
