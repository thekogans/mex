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
#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/PolygonSelectingTool.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                class SelectPolygonsInBoundCommand : public command::CompoundCommand {
                    THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SelectPolygonsInBoundCommand)

                private:
                    const core::Module &module;
                    const blas::Bound2 &bound;

                public:
                    SelectPolygonsInBoundCommand (const core::Module &module_,
                        const blas::Bound2 &bound_) : module (module_), bound (bound_) {}

                    virtual bool Execute () {
                        util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                        _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                            Shaper::Instance ().flags.Test (Shaper::SelectPolygonWindow) ?
                                _3ds::ext::BezierPolygon2::PickInfo::PolygonWindow :
                                _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                            core::GetIOProject ().shaper.polygons2,
                            blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                            core::GetIOProject ().shaper.steps);
                        for (bool found = pickInfo.FindFirst (); found; found = pickInfo.FindNext()) {
                            _3ds::io::BezierPolygon2 *bezierPolygon =
                                core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                            assert (bezierPolygon != 0);
                            if (!_3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (selectMask)) {
                                command::Command::SharedPtr
                                    bezierPolygonSetFlagsCommand (
                                        new _3ds::io::command::BezierPolygon2SetFlagsCommand (
                                            *bezierPolygon, selectMask));
                                if (bezierPolygonSetFlagsCommand->Execute ()) {
                                    commands.push_back (bezierPolygonSetFlagsCommand);
                                }
                            }
                        }
                        if (commands.empty ()) {
                            return false;
                        }
                        // FIXME: replace with DrawPolygonsFinalOperation.
                        ExecuteAndAddFinalOperation (
                            command::FinalOperation::SharedPtr (
                                new core::command::DrawViewLayoutFinalOperation (module)));
                        ExecuteAndAddFinalOperation (
                            command::FinalOperation::SharedPtr (
                                new core::command::FlipFramebufferFinalOperation));
                        return true;
                    }
                };

                THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (SelectPolygonsInBoundCommand)

                class UnselectPolygonsInBoundCommand : public command::CompoundCommand {
                    THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (UnselectPolygonsInBoundCommand)

                private:
                    const core::Module &module;
                    const blas::Bound2 &bound;

                public:
                    UnselectPolygonsInBoundCommand (const core::Module &module_,
                        const blas::Bound2 &bound_) : module (module_), bound (bound_) {}

                    virtual bool Execute () {
                        util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                        _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                            Shaper::Instance ().flags.Test (Shaper::SelectPolygonWindow) ?
                                _3ds::ext::BezierPolygon2::PickInfo::PolygonWindow :
                                _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                            core::GetIOProject ().shaper.polygons2,
                            blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                            core::GetIOProject ().shaper.steps);
                        for (bool found = pickInfo.FindFirst (); found; found = pickInfo.FindNext()) {
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
                            }
                        }
                        if (commands.empty ()) {
                            return false;
                        }
                        // FIXME: replace with DrawPolygonsFinalOperation.
                        ExecuteAndAddFinalOperation (
                            command::FinalOperation::SharedPtr (
                                new core::command::DrawViewLayoutFinalOperation (module)));
                        ExecuteAndAddFinalOperation (
                            command::FinalOperation::SharedPtr (
                                new core::command::FlipFramebufferFinalOperation));
                        return true;
                    }
                };

                THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (UnselectPolygonsInBoundCommand)
            }

            bool PolygonSelectingTool::Select (
                    const _3ds::opengl::View &view,
                    const blas::Bound2 &bound,
                    bool unselect) {
                return unselect ?
                    ExecuteAndAddCommand (
                        command::Command::SharedPtr (
                            new UnselectPolygonsInBoundCommand (module, bound.Normalize ()))) :
                    ExecuteAndAddCommand (
                        command::Command::SharedPtr (
                            new SelectPolygonsInBoundCommand (module, bound.Normalize ())));
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
