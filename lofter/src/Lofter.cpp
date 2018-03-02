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

#include "thekogans/mex/lofter/Config.h"
#include "thekogans/mex/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace lofter {

            THEKOGANS_MEX_CORE_IMPLEMENT_MODULE (Lofter, "3D Lofter")

        } // namespace lofter
    } // namespace mex
} // namespace thekogans

using namespace thekogans::util;
using namespace thekogans::mex;

extern "C" _LIB_THEKOGANS_MEX_LOFTER_DECL const core::Module::ModuleInterface &
    _LIB_THEKOGANS_MEX_LOFTER_API GetModuleInterface (const char *name) {
    struct LofterInterface : public core::Module::ModuleInterface {
        LofterInterface () {}
        virtual ui32 GetMajorVersion () const {return 1;}
        virtual ui32 GetMinorVersion () const {return 0;}
        virtual core::Module *GetModule () const {
            static lofter::Lofter lofter;
            return &lofter;
        }
    } static const lofterInterface;
    return lofterInterface;
}
