// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_lofter.
//
// libthekogans_mex_lofter is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_lofter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_lofter. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_lofter_Module_h)
#define __thekogans_mex_lofter_Module_h

#include "thekogans/mex/3ds/io/Module.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/opengl/lofter/Lofter.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/lofter/Config.h"

namespace thekogans {
    namespace mex {
        namespace lofter {

            struct _LIB_THEKOGANS_MEX_LOFTER_DECL Lofter : public core::Module {
                THEKOGANS_MEX_CORE_DECLARE_MODULE

                virtual _3ds::io::Module &GetIOModule () const {
                    return core::GetIOProject ().lofter;
                }
                virtual _3ds::ext::Module &GetExtModule () const {
                    return core::GetExtProject ().lofter;
                }
                virtual _3ds::opengl::Module &GetOpenGLModule () const {
                    return core::GetOpenGLProject ().lofter;
                }
            };

        } // namespace lofter
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_lofter_Module_h)
