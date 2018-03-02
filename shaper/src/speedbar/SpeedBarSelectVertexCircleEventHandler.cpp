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

#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/ext/Units.h"
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
#include "thekogans/mex/shaper/command/DrawVertexFinalOperation.h"
#include "thekogans/mex/shaper/command/DrawPolygonFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_SELECTVERTEXCIRCLE_0 = "Select center of circle: use CTRL key to deselect\n";
                const char *IDS_SELECTVERTEXCIRCLE_1 = "Select radius of circle\n";
            }

            class SpeedBarSelectVertexCircleEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectVertexCircleEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarSelectVertexCircleEventHandlerTool)

                private:
                    blas::Circle circle;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXCIRCLE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            circle.center = pt;
                            circle.radius = 0.0f;
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXCIRCLE_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            bool commitTransaction = false;
                            if (circle.radius != 0.0f) {
                                core::WaitCursor waitCursor;
                                util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                                for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                                    _3ds::io::BezierPolygon2 *bezierPolygon = core::GetIOProject ().shaper.polygons2[i];
                                    assert (bezierPolygon != 0);
                                    if (!_3ds::ext::BezierPolygon2 (*bezierPolygon).IsHidden () &&
                                            !_3ds::ext::BezierPolygon2 (*bezierPolygon).IsFrozen ()) {
                                        for (std::size_t j = 0, count = bezierPolygon->vertices.size (); j < count; ++j) {
                                            if (bezierPolygon->vertices[j].pt.InCircle (circle)) {
                                                if (IsCtrl () && bezierPolygon->vertices[j].IsSelected (selectMask)) {
                                                    bool selected = _3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (selectMask);
                                                    ExecuteAndAddCommand (
                                                        command::Command::UniquePtr (
                                                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                                *bezierPolygon, j, bezierPolygon->vertices[j].flags & ~selectMask)));
                                                    if (selected) {
                                                        for (util::ui32 k = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::UniquePtr (
                                                                    new DrawPolygonFinalOperation (core::GetIOProject ().shaper.viewLayout[k], *bezierPolygon)));
                                                        }
                                                    }
                                                    else {
                                                        for (util::ui32 k = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::UniquePtr (
                                                                    new DrawVertexFinalOperation (core::GetIOProject ().shaper.viewLayout[k],
                                                                        *bezierPolygon, j)));
                                                        }
                                                    }
                                                    commitTransaction = true;
                                                }
                                                else if (!IsCtrl () && !bezierPolygon->vertices[j].IsSelected (selectMask)) {
                                                    ExecuteAndAddCommand (
                                                        command::Command::UniquePtr (
                                                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                                *bezierPolygon, j, bezierPolygon->vertices[j].flags | selectMask)));
                                                    if (_3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (selectMask)) {
                                                        for (util::ui32 k = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::UniquePtr (
                                                                    new DrawPolygonFinalOperation (
                                                                        core::GetIOProject ().shaper.viewLayout[k], *bezierPolygon)));
                                                        }
                                                    }
                                                    else {
                                                        for (util::ui32 k = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::UniquePtr (
                                                                    new DrawVertexFinalOperation (core::GetIOProject ().shaper.viewLayout[k],
                                                                        *bezierPolygon, j)));
                                                        }
                                                    }
                                                    commitTransaction = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if (commitTransaction) {
                                ExecuteAndAddFinalOperation (
                                    command::FinalOperation::UniquePtr (
                                        new core::command::FlipFramebufferFinalOperation));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXCIRCLE_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXCIRCLE_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                            circle.radius = (pt - circle.center).Length ();
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawCircle2 (view, circle, 0.0f,
                                (core::GetIOProject ().shaper.steps + 1) * 4);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectVertexCircleEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectVertexCircleEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
