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

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Region.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/Shaper.h"
#include "thekogans/mex/shaper/command/DrawPolygonsFinalOperation.h"
#include "thekogans/mex/shaper/command/UnselectPolygonsInRegionCommand.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            bool UnselectPolygonsInRegionCommand::Execute () {
                std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;
                util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                    Shaper::Instance ()->flags.Test (Shaper::SelectPolygonWindow) ?
                        _3ds::ext::BezierPolygon2::PickInfo::PolygonWindow :
                        _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                    core::GetIOProject ().shaper.polygons2, std::move (region),
                    core::GetIOProject ().shaper.steps);
                for (bool found = pickInfo.FindFirst (); found; found = pickInfo.FindNext ()) {
                    _3ds::io::BezierPolygon2 *bezierPolygon =
                        core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                    assert (bezierPolygon != 0);
                    // NOTE: We don't check for selected here because the polygon may
                    // have only some of its vertices selected. In that case, those will
                    // need to be unselected.
                    command::Command::SharedPtr
                        bezierPolygonClearFlagsCommand (
                            new _3ds::io::command::BezierPolygon2ClearFlagsCommand (
                                *bezierPolygon, selectMask));
                    if (bezierPolygonClearFlagsCommand->Execute ()) {
                        commands.push_back (bezierPolygonClearFlagsCommand);
                        bezierPolygons.push_back (bezierPolygon);
                    }
                }
                if (commands.empty ()) {
                    return false;
                }
                assert (!bezierPolygons.empty ());
                for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                    ExecuteAndAddFinalOperation (
                        command::FinalOperation::SharedPtr (
                            new DrawPolygonsFinalOperation (
                                core::GetIOProject ().shaper.viewLayout[i], bezierPolygons)));
                }
                ExecuteAndAddFinalOperation (
                    command::FinalOperation::SharedPtr (
                        new core::command::FlipFramebufferFinalOperation));
                return true;
            }

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (UnselectPolygonsInRegionCommand)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
