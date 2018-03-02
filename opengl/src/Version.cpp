// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/opengl/Version.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            _LIB_THEKOGANS_MEX_OPENGL_DECL const util::Version & _LIB_THEKOGANS_MEX_OPENGL_API GetVersion () {
                static const util::Version version (
                    THEKOGANS_MEX_OPENGL_MAJOR_VERSION,
                    THEKOGANS_MEX_OPENGL_MINOR_VERSION,
                    THEKOGANS_MEX_OPENGL_PATCH_VERSION);
                return version;
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
