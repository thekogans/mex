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

#if !defined (__thekogans_mex_shaper_command_SelectPolygonsInRegionCommand_h)
#define __thekogans_mex_shaper_command_SelectPolygonsInRegionCommand_h

#include "thekogans/mex/blas/Region.h"
#include "thekogans/mex/command/CompoundCommand.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct _LIB_THEKOGANS_MEX_SHAPER_DECL SelectPolygonsInRegionCommand :
                    public command::CompoundCommand {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SelectPolygonsInRegionCommand)

            private:
                blas::Region2::UniquePtr region;

            public:
                SelectPolygonsInRegionCommand (blas::Region2::UniquePtr region_) :
                    region (std::move (region_)) {}

                virtual bool Execute ();
            };

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_shaper_command_SelectPolygonsInRegionCommand_h)
