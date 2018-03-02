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

#include "thekogans/mex/common/filters/FiltersLFTEventHandler.h"
#include "thekogans/mex/common/filters/FiltersPRJEventHandler.h"
#include "thekogans/mex/common/filters/ImplementCommonFiltersItemEventHandler.h"
#include "thekogans/mex/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace lofter {

            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_FILTERS_ITEM_EVENT_HANDLER (FiltersLFTEventHandler, Lofter)
            THEKOGANS_MEX_CORE_IMPLEMENT_COMMON_FILTERS_ITEM_EVENT_HANDLER (FiltersPRJEventHandler, Lofter)

        } // namespace lofter
    } // namespace mex
} // namespace thekogans
