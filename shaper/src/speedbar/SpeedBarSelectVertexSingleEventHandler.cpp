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

#include <vector>
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonVertexCommands.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/command/DrawVertexFinalOperation.h"
#include "thekogans/mex/shaper/command/DrawPolygonFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_SELECTVERTEXSINGLE_0 = "Select vertex to select/deselect\n";
            }

            class SpeedBarSelectVertexSingleEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectVertexSingleEventHandler)

            private:
                // Select a single vertex.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarSelectVertexSingleEventHandlerTool)

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ()->consoleWindow != 0);
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTVERTEXSINGLE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                _3ds::io::BezierPolygon2 *bezierPolygon =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                assert (bezierPolygon != 0);
                                bool selected = _3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (
                                    core::GetIOProject ().shaper.selectMask);
                                BeginTransaction ();
                                if (bezierPolygon->vertices[pickInfo.vertexIndex].IsSelected (
                                        core::GetIOProject ().shaper.selectMask)) {
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                *bezierPolygon, pickInfo.vertexIndex,
                                                bezierPolygon->vertices[pickInfo.vertexIndex].flags &
                                                ~core::GetIOProject ().shaper.selectMask)));
                                }
                                else {
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                *bezierPolygon, pickInfo.vertexIndex,
                                                bezierPolygon->vertices[pickInfo.vertexIndex].flags |
                                                core::GetIOProject ().shaper.selectMask)));
                                }
                                if (selected || _3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (
                                        core::GetIOProject ().shaper.selectMask)) {
                                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                                        ExecuteAndAddFinalOperation (
                                            command::FinalOperation::SharedPtr (
                                                new DrawPolygonFinalOperation (
                                                    core::GetIOProject ().shaper.viewLayout[i], *bezierPolygon)));
                                    }
                                }
                                else {
                                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                                        ExecuteAndAddFinalOperation (
                                            command::FinalOperation::SharedPtr (
                                                new DrawVertexFinalOperation (
                                                    core::GetIOProject ().shaper.viewLayout[i],
                                                    *bezierPolygon, pickInfo.vertexIndex)));
                                    }
                                }
                                ExecuteAndAddFinalOperation (
                                    command::FinalOperation::SharedPtr (
                                        new core::command::FlipFramebufferFinalOperation));
                                CommitTransaction ();
                            }
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectVertexSingleEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectVertexSingleEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
