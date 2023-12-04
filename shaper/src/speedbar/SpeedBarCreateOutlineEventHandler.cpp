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

#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_CREATEOUTLINE_0 = "Select polygon to outline\n";
                const char *IDS_CREATEOUTLINE_1 = "Draw a line for the outline width\n";
            }

            class SpeedBarCreateOutlineEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateOutlineEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateOutlineEventHandlerTool)

                private:
                    // Create an outline of a polygon. If the polygon is open,
                    // the outline is connected to the polygon by it's end vertices,
                    // and the polygon becomes closed.
                    blas::Point2 start;
                    blas::Point2 end;
                    blas::Bound2 bound;
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATEOUTLINE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygons.clear ();
                            if (IsSelect () || !Shaper::Instance ().flags.Test (Shaper::Selected)) {
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
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
                                GetSelectedPolygons (bezierPolygons, core::GetIOProject ().shaper.selectMask);
                                if (!bezierPolygons.empty ()) {
                                    bound = GetPolygonsBound (bezierPolygons);
                                    core::DrawBound2 (view, bound);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            start = pt;
                            end = pt;
                            BeginTransaction ();
                        }
                        else if (state == 4) {
                            UpdateState (5, flags | ScrollLockOn);
                            core::DrawLine2 (view, start, end);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygons.empty ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else if (IsSelect ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                core::DrawBound2 (view, bound);
                                BeginTransaction ();
                                ToggleSelectedPolygons (bezierPolygons);
                                CommitTransaction ();
                            }
                            else {
                                core::DrawBound2 (view, bound);
                                core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                                UpdateState (2, flags | ScrollLockOff | CursorVisible);
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATEOUTLINE_1);
                            }
                        }
                        else if (state == 3) {
                            UpdateState (4, flags | ScrollLockOff);
                        }
                        else if (state == 5) {
                            core::CursorMgr::Instance ().SetPickCursor (
                                core::GetIOProject ().programConfiguration.pickBoxSize);
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            util::f32 width = (end - start).Length ();
                            if (!util::IS_ZERO (width)) {
                                for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                    _3ds::io::BezierPolygon2::UniquePtr bezierPolygon1;
                                    _3ds::io::BezierPolygon2::UniquePtr bezierPolygon2;
                                    // Get the outline.
                                    _3ds::ext::BezierPolygon2 (*bezierPolygons[i]).Outline (
                                        width, bezierPolygon1, bezierPolygon2);
                                    assert (bezierPolygon1.get () != 0);
                                    // Delete the old polygon.
                                    DeletePolygon (GetPolygonIndex (*bezierPolygons[i]), false);
                                    // Add the new polygon(s).
                                    AddPolygon (std::move (bezierPolygon1), false);
                                    if (bezierPolygon2.get () != 0) {
                                        AddPolygon (std::move (bezierPolygon2), false);
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
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEOUTLINE_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            core::CursorMgr::Instance ().SetPickCursor (
                                core::GetIOProject ().programConfiguration.pickBoxSize);
                        }
                        else if (state == 4) {
                            UpdateState (5, flags | ScrollLockOn);
                            core::DrawLine2 (view, start, end);
                            core::CursorMgr::Instance ().SetPickCursor (
                                core::GetIOProject ().programConfiguration.pickBoxSize);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEOUTLINE_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 4) {
                            core::DrawLine2 (view, start, end);
                            end = pt;
                            core::DrawLine2 (view, start, end);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 4) {
                            core::DrawLine2 (view, start, end);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateOutlineEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateOutlineEventHandler, Shaper)

        } // namespace editor
    } // namespace mex
} // namespace thekogans
