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

#if !defined (__thekogans_mex_3ds_io_Tape_h)
#define __thekogans_mex_3ds_io_Tape_h

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                template<typename T>
                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Tape {
                    bool visible;
                    bool vsnap;
                    T start;
                    T end;

                    Tape () :
                        visible (false),
                        vsnap (false),
                        start (T::Zero),
                        end (T::X * 100.0f) {}

                    inline void Swap (Tape<T> &tape) {
                        std::swap (visible, tape.visible);
                        std::swap (vsnap, tape.vsnap);
                        std::swap (start, tape.start);
                        std::swap (end, tape.end);
                    }
                };

                typedef Tape<blas::Point2> Tape2;
                typedef Tape<blas::Point3> Tape3;

                template<typename T>
                inline ChunkReader &operator >> (ChunkReader &chunk, Tape<T> &tape) {
                    chunk >> tape.visible >> tape.vsnap >> tape.start >> tape.end;
                    return chunk;
                }

                template<typename T>
                inline ChunkWriter &operator << (ChunkWriter &chunk, const Tape<T> &tape) {
                    chunk << tape.visible << tape.vsnap << tape.start << tape.end;
                    return chunk;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Tape_h)
