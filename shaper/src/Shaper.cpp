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

#include "thekogans/mex/shaper/Config.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            THEKOGANS_MEX_CORE_IMPLEMENT_MODULE (Shaper, "2D Shaper")

            Shaper *Shaper::Instance () {
                static Shaper shaper;
                return &shaper;
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

using namespace thekogans::util;
using namespace thekogans::mex;

extern "C" _LIB_THEKOGANS_MEX_SHAPER_DECL const core::Module::ModuleInterface &
        _LIB_THEKOGANS_MEX_SHAPER_API GetModuleInterface (const char *name) {
    struct ShaperInterface : public core::Module::ModuleInterface {
        ShaperInterface () {}
        virtual ui32 GetMajorVersion () const {return 1;}
        virtual ui32 GetMinorVersion () const {return 0;}
        virtual core::Module *GetModule () const {
            return shaper::Shaper::Instance ();
        }
    } static const shaperInterface;
    return shaperInterface;
}
