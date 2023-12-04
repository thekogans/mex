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

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"
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
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_MODIFYSEGMENTADJUST_0 = "Select segment on polygon for spline adjust\n";
            }

            class SpeedBarModifySegmentAdjustEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifySegmentAdjustEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarModifySegmentAdjustEventHandlerTool)

                private:
                    blas::Point2 start;
                    blas::Matrix2 yellow1;
                    blas::Matrix2 red1;
                    blas::Matrix2 yellow2;
                    blas::Matrix2 red2;
                    std::size_t vertexIndex1;
                    std::size_t vertexIndex2;
                    _3ds::io::BezierPolygon2 *bezierPolygon;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_MODIFYSEGMENTADJUST_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            start =  pt;
                            red1 = blas::Matrix2::Identity;
                            yellow1 = blas::Matrix2::Identity;
                            red2 = blas::Matrix2::Identity;
                            yellow2 = blas::Matrix2::Identity;
                            bezierPolygon = 0;
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Segment,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                bezierPolygon =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                assert (bezierPolygon != 0);
                                vertexIndex1 = pickInfo.vertexIndex;
                                vertexIndex2 = _3ds::ext::BezierPolygon2 (*bezierPolygon).NextVertexIndex (vertexIndex1);
                                BeginTransaction ();
                                EraseSegmentSegments (*bezierPolygon, vertexIndex1, vertexIndex2);
                                HideSegmentSegments (*bezierPolygon, vertexIndex1, vertexIndex2);
                                BezierPolygon (*bezierPolygon).DrawSegmentSegments (view,
                                    vertexIndex1, red1, yellow1, vertexIndex2, red2, yellow2);
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags);
                            BezierPolygon (*bezierPolygon).DrawSegmentSegments (view,
                                vertexIndex1, red1, yellow1, vertexIndex2, red2, yellow2);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygon == 0) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else {
                                UpdateState (2, flags);
                            }
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            ExecuteAndAddCommand (
                                command::Command::SharedPtr (
                                    new _3ds::io::command::BezierPolygon2VertexSetRedCommand (
                                        *bezierPolygon, vertexIndex1,
                                        bezierPolygon->vertices[vertexIndex1].red * red1)));
                            ExecuteAndAddCommand (
                                command::Command::SharedPtr (
                                    new _3ds::io::command::BezierPolygon2VertexSetYellowCommand (
                                        *bezierPolygon, vertexIndex1,
                                        bezierPolygon->vertices[vertexIndex1].yellow * yellow1)));
                            ExecuteAndAddCommand (
                                command::Command::SharedPtr (
                                    new _3ds::io::command::BezierPolygon2VertexSetRedCommand (
                                        *bezierPolygon, vertexIndex2,
                                        bezierPolygon->vertices[vertexIndex2].red * red2)));
                            ExecuteAndAddCommand (
                                command::Command::SharedPtr (
                                    new _3ds::io::command::BezierPolygon2VertexSetYellowCommand (
                                        *bezierPolygon, vertexIndex2,
                                        bezierPolygon->vertices[vertexIndex2].yellow * yellow2)));
                            UnhideSegmentSegments (*bezierPolygon, vertexIndex1, vertexIndex2);
                            DrawSegmentSegments (*bezierPolygon, vertexIndex1, vertexIndex2, true);
                            CommitTransaction ();
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags);
                            BezierPolygon (*bezierPolygon).DrawSegmentSegments (view,
                                vertexIndex1, red1, yellow1, vertexIndex2, red2, yellow2);
                            AbortTransaction ();
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            BezierPolygon (*bezierPolygon).DrawSegmentSegments (view,
                                vertexIndex1, red1, yellow1, vertexIndex2, red2, yellow2);
                            core::UI::Instance ().viewLayoutWindow->SetMousePosition (view.P2D (start));
                            if (bezierPolygon->vertices[vertexIndex1].yellow.IsZero ()) {
                                yellow1 *= blas::Matrix2::Translate (pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    red1 *= blas::Matrix2::Translate (start - pt);
                                }
                            }
                            else {
                                blas::Point2 t1 = bezierPolygon->vertices[vertexIndex1].yellow * yellow1;
                                yellow1 *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    red1 *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                }
                            }
                            if (bezierPolygon->vertices[vertexIndex2].red.IsZero ()) {
                                red2 *= blas::Matrix2::Translate (pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    yellow2 *= blas::Matrix2::Translate (start - pt);
                                }
                            }
                            else {
                                blas::Point2 t1 = bezierPolygon->vertices[vertexIndex2].red * red2;
                                red2 *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    yellow2 *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                }
                            }
                            BezierPolygon (*bezierPolygon).DrawSegmentSegments (view,
                                vertexIndex1, red1, yellow1, vertexIndex2, red2, yellow2);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            BezierPolygon (*bezierPolygon).DrawSegmentSegments (view,
                                vertexIndex1, red1, yellow1, vertexIndex2, red2, yellow2);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarModifySegmentAdjustEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifySegmentAdjustEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
