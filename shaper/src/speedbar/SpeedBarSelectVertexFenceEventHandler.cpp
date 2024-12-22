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

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
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
                const char *IDS_SELECTVERTEXFENCE_0 =
                    "Select first point of fence: CTRL to deselect, SPACE to close\n";
                const char *IDS_SELECTVERTEXFENCE_1 =
                    "Select points of fenced area. Hit SPACE to close\n";
            }

            class SpeedBarSelectVertexFenceEventHandler :
                    public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (
                    SpeedBarSelectVertexFenceEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (
                        SpeedBarSelectVertexFenceEventHandlerTool)

                private:
                    blas::Point2 last;
                    blas::Polygon2 polygon;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ()->consoleWindow->Print (IDS_SELECTVERTEXFENCE_0);
                    }

                    virtual void LButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (IsEvenState ()) {
                            if (state == 0) {
                                UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                                polygon.points.clear ();
                                polygon.closed = false;
                                BeginTransaction ();
                            }
                            else if (state == 2) {
                                UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            }
                            polygon.points.push_back (pt);
                            last = pt;
                        }
                    }

                    virtual void LButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ()->consoleWindow->Print (IDS_SELECTVERTEXFENCE_1);
                        }
                        else if (state == 3) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                        }
                    }

                    virtual void RButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawPolygon2 (view, polygon);
                            core::DrawLine2 (view, polygon.points.back (), last);
                        }
                    }

                    virtual void RButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ()->consoleWindow->Print (IDS_SELECTVERTEXFENCE_0);
                        }
                    }

                    virtual void MouseMove (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawLine2 (view, polygon.points.back (), last);
                            last = pt;
                            core::DrawLine2 (view, polygon.points.back (), last);
                        }
                    }

                    virtual void KeyDown (
                            const _3ds::opengl::View &view,
                            util::ui32 key,
                            util::ui32 repeatCount,
                            util::ui32 flags) {
                        if (key == Qt::Key_Space && state == 2) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            core::DrawPolygon2 (view, polygon);
                            core::DrawLine2 (view, polygon.points.back (), last);
                            bool commitTransaction = false;
                            if (polygon.points.size () > 2) {
                                polygon.closed = true;
                                core::WaitCursor waitCursor;
                                util::ui16 selectMask = core::GetIOProject ().shaper.selectMask;
                                for (std::size_t i = 0,
                                        count = core::GetIOProject ().shaper.polygons2.size ();
                                        i < count; ++i) {
                                    _3ds::io::BezierPolygon2 *bezierPolygon =
                                        core::GetIOProject ().shaper.polygons2[i];
                                    assert (bezierPolygon != 0);
                                    if (!_3ds::ext::BezierPolygon2 (*bezierPolygon).IsHidden () &&
                                            !_3ds::ext::BezierPolygon2 (*bezierPolygon).IsFrozen ()) {
                                        for (std::size_t j = 0,
                                                count = bezierPolygon->vertices.size ();
                                                j < count; ++j) {
                                            if (bezierPolygon->vertices[j].pt.InPolygon (polygon)) {
                                                if (IsCtrl () && bezierPolygon->vertices[j].IsSelected (
                                                        selectMask)) {
                                                    bool selected =
                                                        _3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (
                                                            selectMask);
                                                    ExecuteAndAddCommand (
                                                        command::Command::SharedPtr (
                                                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                                *bezierPolygon,
                                                                j,
                                                                bezierPolygon->vertices[j].flags & ~selectMask)));
                                                    if (selected) {
                                                        for (util::ui32 k = 0,
                                                                count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                                                                k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::SharedPtr (
                                                                    new DrawPolygonFinalOperation (
                                                                        core::GetIOProject ().shaper.viewLayout[k],
                                                                        *bezierPolygon)));
                                                        }
                                                    }
                                                    else {
                                                        for (util::ui32 k = 0,
                                                                count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                                                                k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::SharedPtr (
                                                                    new DrawVertexFinalOperation (
                                                                        core::GetIOProject ().shaper.viewLayout[k],
                                                                        *bezierPolygon, j)));
                                                        }
                                                    }
                                                    commitTransaction = true;
                                                }
                                                else if (!IsCtrl () &&
                                                        !bezierPolygon->vertices[j].IsSelected (selectMask)) {
                                                    ExecuteAndAddCommand (
                                                        command::Command::SharedPtr (
                                                            new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                                *bezierPolygon,
                                                                j,
                                                                bezierPolygon->vertices[j].flags | selectMask)));
                                                    if (_3ds::ext::BezierPolygon2 (
                                                            *bezierPolygon).IsSelected (selectMask)) {
                                                        for (util::ui32 k = 0,
                                                                count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                                                                k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::SharedPtr (
                                                                    new DrawPolygonFinalOperation (
                                                                        core::GetIOProject ().shaper.viewLayout[k],
                                                                        *bezierPolygon)));
                                                        }
                                                    }
                                                    else {
                                                        for (util::ui32 k = 0,
                                                                count = core::GetIOProject ().shaper.viewLayout.GetViewCount ();
                                                                k < count; ++k) {
                                                            ExecuteAndAddFinalOperation (
                                                                command::FinalOperation::SharedPtr (
                                                                    new DrawVertexFinalOperation (
                                                                        core::GetIOProject ().shaper.viewLayout[k],
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
                                    command::FinalOperation::SharedPtr (
                                        new core::command::FlipFramebufferFinalOperation));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ()->consoleWindow->Print (IDS_SELECTVERTEXFENCE_0);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawPolygon2 (view, polygon);
                            core::DrawLine2 (view, polygon.points.back (), last);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectVertexFenceEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (
                SpeedBarSelectVertexFenceEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
