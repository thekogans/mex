// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_shaper_Shaper_h)
#define __thekogans_mex_shaper_Shaper_h

#include <vector>
#include <string>
#include "thekogans/util/Flags.h"
#include "thekogans/mex/3ds/io/Module.h"
#include "thekogans/mex/3ds/ext/shaper/Shaper.h"
#include "thekogans/mex/3ds/opengl/shaper/Shaper.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/shaper/Config.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct _LIB_THEKOGANS_MEX_SHAPER_DECL Shaper : public core::Module {
                THEKOGANS_MEX_CORE_DECLARE_MODULE

                enum {
                    UseLocalAxis = 1,
                    Selected = 2,
                    SelectPolygonWindow = 4,
                    ShapeAssignWindow = 8,
                    DisplayHidePolygonWindow = 16,
                    DisplayFreezePolygonWindow = 32
                };
                util::Flags32 flags;

                Shaper () : flags (SelectPolygonWindow | ShapeAssignWindow | DisplayHidePolygonWindow | DisplayFreezePolygonWindow) {}

                virtual _3ds::io::Module &GetIOModule () const {return core::GetIOProject ().shaper;}
                virtual _3ds::ext::Module &GetExtModule () const {return core::GetExtProject ().shaper;}
                virtual _3ds::opengl::Module &GetOpenGLModule () const {return core::GetOpenGLProject ().shaper;}

                static Shaper *Instance ();
            };

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_shaper_Shaper_h)
