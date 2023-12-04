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
#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BoundRegion.h"
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
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_CREATECLOSE_0 = "Select polygon to close\n";
            }

            class SpeedBarCreateCloseEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateCloseEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateCloseEventHandlerTool)

                private:
                    blas::Bound2 bound;
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATECLOSE_0);
                    }

                    virtual void LButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygons.clear ();
                            if (IsSelect () || !Shaper::Instance ().flags.Test (Shaper::Selected)) {
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (
                                        new blas::BoundRegion2 (GetPickBound (view, pt))),
                                    core::GetIOProject ().shaper.steps);
                                if (pickInfo.FindFirst ()) {
                                    _3ds::io::BezierPolygon2 *bezierPolygon =
                                        core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                    bound = _3ds::ext::BezierPolygon2 (*bezierPolygon).GetBound (
                                        core::GetIOProject ().shaper.steps);
                                    core::DrawBound2 (view, bound);
                                    bezierPolygons.push_back (bezierPolygon);
                                }
                            }
                            else {
                                GetSelectedPolygons (bezierPolygons,
                                    core::GetIOProject ().shaper.selectMask);
                                if (!bezierPolygons.empty ()) {
                                    bound = GetPolygonsBound (bezierPolygons);
                                    core::DrawBound2 (view, bound);
                                }
                            }
                        }
                    }

                    virtual void LButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (!bezierPolygons.empty ()) {
                                core::DrawBound2 (view, bound);
                                if (IsSelect ()) {
                                    BeginTransaction ();
                                    ToggleSelectedPolygons (bezierPolygons);
                                    CommitTransaction ();
                                }
                                else {
                                    BeginTransaction ();
                                    for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                        if (!bezierPolygons[i]->IsClosed ()) {
                                            ExecuteAndAddCommand (
                                                command::Command::SharedPtr (
                                                    new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                        *bezierPolygons[i],
                                                        bezierPolygons[i]->vertices.size () - 1,
                                                        bezierPolygons[i]->vertices.back ().flags |
                                                        _3ds::io::BezierPolygon2::Vertex::Closed)));
                                            DrawSegment (*bezierPolygons[i],
                                                bezierPolygons[i]->vertices.size () - 1, 0, false);
                                        }
                                    }
                                    if (!IsPendingTransactionEmpty ()) {
                                        ExecuteAndAddFinalOperation (
                                            command::FinalOperation::SharedPtr (
                                                new core::command::FlipFramebufferFinalOperation));
                                        CommitTransaction ();
                                    }
                                    else {
                                        AbortTransaction ();
                                    }
                                }
                            }
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateCloseEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (
                SpeedBarCreateCloseEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
