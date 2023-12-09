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
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/ext/Units.h"
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
#include "thekogans/mex/shaper/command/DrawVertexFinalOperation.h"
#include "thekogans/mex/shaper/command/DrawPolygonFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_SELECTVERTEXQUAD_0 = "Select first corner of quad: use CTRL key to deselect\n";
                const char *IDS_SELECTVERTEXQUAD_1 = "Select opposite corner of quad\n";

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
                            if (!bezierPolygon->vertices[pickInfo.vertexIndex].IsSelected (selectMask)) {
                                ExecuteAndAddCommand (
                                    command::Command::SharedPtr (
                                        new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                            *bezierPolygon, pickInfo.vertexIndex,
                                            bezierPolygon->vertices[pickInfo.vertexIndex].flags | selectMask)));
                                if (_3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (selectMask)) {
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
                            }
                        }
                        if (commands.empty ()) {
                            return false;
                        }
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
                            if (bezierPolygon->vertices[pickInfo.vertexIndex].IsSelected (selectMask)) {
                                bool selected = _3ds::ext::BezierPolygon2 (*bezierPolygon).IsSelected (selectMask);
                                ExecuteAndAddCommand (
                                    command::Command::SharedPtr (
                                        new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                            *bezierPolygon, pickInfo.vertexIndex,
                                            bezierPolygon->vertices[pickInfo.vertexIndex].flags & ~selectMask)));
                                if (selected) {
                                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                                        ExecuteAndAddFinalOperation (
                                            command::FinalOperation::SharedPtr (
                                                new DrawPolygonFinalOperation (core::GetIOProject ().shaper.viewLayout[i], *bezierPolygon)));
                                    }
                                }
                                else {
                                    for (util::ui32 i = 0, count = core::GetIOProject ().shaper.viewLayout.GetViewCount (); i < count; ++i) {
                                        ExecuteAndAddFinalOperation (
                                            command::FinalOperation::SharedPtr (
                                                new DrawVertexFinalOperation (core::GetIOProject ().shaper.viewLayout[i],
                                                    *bezierPolygon, pickInfo.vertexIndex)));
                                    }
                                }
                            }
                        }
                        if (commands.empty ()) {
                            return false;
                        }
                        ExecuteAndAddFinalOperation (
                            command::FinalOperation::SharedPtr (
                                new core::command::FlipFramebufferFinalOperation));
                        return true;
                    }
                };

                THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (UnselectVerticesInBoundCommand)
            }

            class SpeedBarSelectVertexQuadEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectVertexQuadEventHandler)

            private:
                // Select a set of vertices which fall within a defined quadrillateral.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarSelectVertexQuadEventHandlerTool)

                private:
                    blas::Bound2 bound;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXQUAD_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bound = pt;
                            core::DrawBound2 (view, bound);
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff);
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXQUAD_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (IsCtrl () ?
                                ExecuteAndAddCommand (
                                    command::Command::SharedPtr (
                                        new UnselectVerticesInBoundCommand (module, bound.Normalize ()))) :
                                ExecuteAndAddCommand (
                                    command::Command::SharedPtr (
                                        new SelectVerticesInBoundCommand (module, bound.Normalize ())))) {
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXQUAD_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (2, flags | ScrollLockOn);
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_SELECTVERTEXQUAD_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound);
                            bound.max = pt;
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectVertexQuadEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectVertexQuadEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
