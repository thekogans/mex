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

#if !defined (__thekogans_mex_3ds_io_Axis_h)
#define __thekogans_mex_3ds_io_Axis_h

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
                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Axis {
                    bool visible;
                    T axis;

                    Axis () :
                        visible (false),
                        axis (T::Zero) {}

                    inline void Swap (Axis<T> &axis_) {
                        std::swap (visible, axis_.visible);
                        std::swap (axis, axis_.axis);
                    }
                };

                typedef Axis<blas::Point2> Axis2;
                typedef Axis<blas::Point3> Axis3;

                template<typename T>
                inline ChunkReader &operator >> (ChunkReader &chunk, Axis<T> &axis) {
                    chunk >> axis.visible >> axis.axis;
                    return chunk;
                }

                template<typename T>
                inline ChunkWriter &operator << (ChunkWriter &chunk, const Axis<T> &axis) {
                    chunk << axis.visible << axis.axis;
                    return chunk;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Axis_h)
