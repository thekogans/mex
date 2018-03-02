// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_Icons_h)
#define __thekogans_mex_3ds_opengl_Icons_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                enum {
                    AXIS,
                    HOOK,
                    LOFTER_PATH = HOOK,
                    LIGHT_HEAD,
                    CAMERA_HEAD,
                    BOX_3x3,
                    BOX_4x4,
                    BOX_5x5,
                    BOX_9x9,
                    TICK_3x3,
                    TICK_5x5,
                    TICK_7x7
                };

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL util::ui32 _LIB_THEKOGANS_MEX_3DS_OPENGL_API GetTickIcon (
                    util::ui32 tickSize);

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawIcon (
                    util::ui32 icon, const blas::Point3 &position);

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Icons_h)
