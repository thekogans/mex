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
#include "thekogans/util/OwnerVector.h"
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
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/Shaper.h"
#include "thekogans/mex/shaper/command/DrawVertexFinalOperation.h"
#include "thekogans/mex/shaper/command/DrawPolygonFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_MODIFYVERTEXADJUST_0 = "Select vertex for spline adjust\n";
            }

            class SpeedBarModifyVertexAdjustEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifyVertexAdjustEventHandler)

            private:
                // Adjust vertex curvature.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarModifyVertexAdjustEventHandlerTool)

                private:
                    blas::Point2 start;
                    blas::Matrix2 red;
                    blas::Matrix2 yellow;
                    util::OwnerVector<BezierPolygonVertex> bezierPolygonVertices;
                    std::size_t referenceVertexIndex;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_MODIFYVERTEXADJUST_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            start = pt;
                            red = blas::Matrix2::Identity;
                            yellow = blas::Matrix2::Identity;
                            bezierPolygonVertices.deleteAndClear ();
                            referenceVertexIndex = util::NIDX;
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                _3ds::io::BezierPolygon2 *bezierPolygon =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                assert (bezierPolygon != 0);
                                if (IsSelect ()) {
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
                                        for (util::ui32 i = 0,
                                                count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                                                i < count; ++i) {
                                            ExecuteAndAddFinalOperation (
                                                command::FinalOperation::SharedPtr (
                                                    new DrawPolygonFinalOperation (
                                                        core::GetIOProject ().shaper.viewLayout[i], *bezierPolygon)));
                                        }
                                    }
                                    else {
                                        for (util::ui32 i = 0,
                                                count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                                                i < count; ++i) {
                                            ExecuteAndAddFinalOperation (
                                                command::FinalOperation::SharedPtr (
                                                    new DrawVertexFinalOperation (
                                                        core::GetIOProject ().shaper.viewLayout[i],
                                                        *bezierPolygon,
                                                        pickInfo.vertexIndex)));
                                        }
                                    }
                                    ExecuteAndAddFinalOperation (
                                        command::FinalOperation::SharedPtr (
                                            new core::command::FlipFramebufferFinalOperation));
                                    CommitTransaction ();
                                }
                                else {
                                    if (Shaper::Instance ().flags.Test (Shaper::Selected)) {
                                        GetSelectedVertices (
                                            bezierPolygonVertices, core::GetIOProject ().shaper.selectMask);
                                        referenceVertexIndex = FindReferenceVertex (
                                            bezierPolygonVertices,
                                            *bezierPolygon,
                                            pickInfo.vertexIndex,
                                            core::GetIOProject ().shaper.selectMask);
                                        assert (referenceVertexIndex != util::NIDX);
                                    }
                                    else {
                                        bezierPolygonVertices.push_back (
                                            new BezierPolygonVertex (*bezierPolygon, pickInfo.vertexIndex));
                                        referenceVertexIndex = 0;
                                    }
                                    BeginTransaction ();
                                    EraseVertexSegments (bezierPolygonVertices);
                                    HideVertexSegments (bezierPolygonVertices);
                                    DrawVertexSegments2 (view, bezierPolygonVertices, red, yellow);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags);
                            DrawVertexSegments2 (view, bezierPolygonVertices, red, yellow);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygonVertices.empty ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else {
                                UpdateState (2, flags);
                            }
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            XformVertices (bezierPolygonVertices, red, yellow);
                            UnhideVertexSegments (bezierPolygonVertices);
                            DrawVertexSegments (bezierPolygonVertices);
                            bezierPolygonVertices.deleteAndClear ();
                            CommitTransaction ();
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags);
                            DrawVertexSegments2 (view, bezierPolygonVertices, red, yellow);
                            bezierPolygonVertices.deleteAndClear ();
                            AbortTransaction ();
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            DrawVertexSegments2 (view, bezierPolygonVertices, red, yellow);
                            core::UI::Instance ().viewLayoutWindow->SetMousePosition (view.P2D (start));
                            if (bezierPolygonVertices[referenceVertexIndex]->bezierPolygon.vertices[
                                    bezierPolygonVertices[referenceVertexIndex]->vertexIndex].yellow.IsZero ()) {
                                yellow *= blas::Matrix2::Translate (pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    red *= blas::Matrix2::Translate (start - pt);
                                }
                            }
                            else {
                                blas::Point2 t1 = bezierPolygonVertices[referenceVertexIndex]->bezierPolygon.vertices[
                                    bezierPolygonVertices[referenceVertexIndex]->vertexIndex].yellow * yellow;
                                yellow *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    red *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                }
                            }
                            DrawVertexSegments2 (view, bezierPolygonVertices, red, yellow);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            DrawVertexSegments2 (view, bezierPolygonVertices, red, yellow);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarModifyVertexAdjustEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifyVertexAdjustEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
