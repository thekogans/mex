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
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonVertexCommands.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/VertexSelectingTool.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                class SelectVerticesInBoundCommand : public command::CompoundCommand {
                    THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SelectVerticesInBoundCommand)

                private:
                    const core::Module &module;
                    const blas::Bound2 &bound;

                public:
                    SelectVerticesInBoundCommand (const core::Module &module_,
                        const blas::Bound2 &bound_) : module (module_), bound (bound_) {}

                    virtual bool Execute () {
                        util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                        _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                            _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                            core::GetIOProject ().shaper.polygons2,
                            blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                            core::GetIOProject ().shaper.steps);
                        for (bool found = pickInfo.FindFirst (); found; found = pickInfo.FindNext ()) {
                            _3ds::io::BezierPolygon2 *bezierPolygon =
                                core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                            assert (bezierPolygon != 0);
                            const _3ds::io::BezierPolygon2::Vertex &vertex =
                                bezierPolygon->vertices[pickInfo.vertexIndex];
                            if (!vertex.IsSelected (selectMask)) {
                                ExecuteAndAddCommand (
                                    command::Command::SharedPtr (
                                        new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                            *bezierPolygon, pickInfo.vertexIndex, vertex.flags | selectMask)));
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

                THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (SelectVerticesInBoundCommand)

                class UnselectVerticesInBoundCommand : public command::CompoundCommand {
                    THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (UnselectVerticesInBoundCommand)

                private:
                    const core::Module &module;
                    const blas::Bound2 &bound;

                public:
                    UnselectVerticesInBoundCommand (const core::Module &module_,
                        const blas::Bound2 &bound_) : module (module_), bound (bound_) {}

                    virtual bool Execute () {
                        util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                        _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                            _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                            core::GetIOProject ().shaper.polygons2,
                            blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                            core::GetIOProject ().shaper.steps);
                        for (bool found = pickInfo.FindFirst (); found; found = pickInfo.FindNext ()) {
                            _3ds::io::BezierPolygon2 *bezierPolygon =
                                core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                            assert (bezierPolygon != 0);
                            const _3ds::io::BezierPolygon2::Vertex &vertex =
                                bezierPolygon->vertices[pickInfo.vertexIndex];
                            if (vertex.IsSelected (selectMask)) {
                                ExecuteAndAddCommand (
                                    command::Command::SharedPtr (
                                        new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                            *bezierPolygon, pickInfo.vertexIndex, vertex.flags & ~selectMask)));
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

                THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (UnselectVerticesInBoundCommand)
            }

            bool VertexSelectingTool::Select (const _3ds::opengl::View &view,
                const blas::Bound2 &bound, bool unselect) {
                return unselect ?
                    ExecuteAndAddCommand (
                        command::Command::SharedPtr (
                            new UnselectVerticesInBoundCommand (module, bound))) :
                    ExecuteAndAddCommand (
                        command::Command::SharedPtr (
                            new SelectVerticesInBoundCommand (module, bound)));
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
