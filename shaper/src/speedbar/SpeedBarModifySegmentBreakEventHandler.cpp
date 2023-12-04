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
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
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
                const char *IDS_MODIFYSEGMENTBREAK_0 = "Select segment to break\n";
            }

            class SpeedBarModifySegmentBreakEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifySegmentBreakEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarModifySegmentBreakEventHandlerTool)

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_MODIFYSEGMENTBREAK_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            blas::Bound2 bound = GetPickBound (view, pt);
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Segment,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                _3ds::io::BezierPolygon2 *bezierPolygon =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                assert (bezierPolygon != 0);
                                std::size_t vertexIndex1 = pickInfo.vertexIndex;
                                assert (vertexIndex1 != util::NIDX);
                                std::size_t vertexIndex2 =
                                    _3ds::ext::BezierPolygon2 (*bezierPolygon).NextVertexIndex (vertexIndex1);
                                assert (vertexIndex2 != util::NIDX);
                                BeginTransaction ();
                                _3ds::io::BezierPolygon2::UniquePtr bezierPolygon1;
                                _3ds::io::BezierPolygon2::UniquePtr bezierPolygon2;
                                _3ds::ext::BezierPolygon2 (*bezierPolygon).BreakSegment (vertexIndex1, vertexIndex2,
                                    _3ds::ext::BezierPolygon2 (*bezierPolygon).GetSegmentBoundIntersection (
                                        vertexIndex1, vertexIndex2, bound, core::GetIOProject ().shaper.steps),
                                    bezierPolygon1, bezierPolygon2);
                                assert (bezierPolygon1.get () != 0);
                                DeletePolygon (pickInfo.polygonIndex, false);
                                AddPolygon (std::move (bezierPolygon1), false);
                                if (bezierPolygon2.get () == 0) {
                                    AddPolygon (std::move (bezierPolygon2), false);
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
                explicit SpeedBarModifySegmentBreakEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifySegmentBreakEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
