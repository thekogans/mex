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

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_CREATEPOLYCONNECT_0 = "Select first polygon to connect.\n";
                const char *IDS_CREATEPOLYCONNECT_10 = "Polygon must be open.\n";
                const char *IDS_CREATEPOLYCONNECT_11 = "Can't operate on same polygon.\n";
                const char *IDS_CREATEPOLYCONNECT_12 = "Select second polygon to connect. [SHIFT] = connect first vertex to first vertex.\n";
            }

            class SpeedBarCreatePolyConnectEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreatePolyConnectEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreatePolyConnectEventHandlerTool)

                private:
                    _3ds::io::BezierPolygon2 *bezierPolygon1;
                    blas::Bound2 bound1;
                    _3ds::io::BezierPolygon2 *bezierPolygon2;
                    blas::Bound2 bound2;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ()->consoleWindow != 0);
                        core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygon1 = 0;
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                bezierPolygon1 =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                bound1 = _3ds::ext::BezierPolygon2 (*bezierPolygon1).GetBound (
                                    core::GetIOProject ().shaper.steps);
                                core::DrawBound2 (view, bound1);
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygon2 = 0;
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                bezierPolygon2 =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                bound2 = _3ds::ext::BezierPolygon2 (*bezierPolygon2).GetBound (
                                    core::GetIOProject ().shaper.steps);
                                if (bezierPolygon1 != bezierPolygon2) {
                                    core::DrawBound2 (view, bound1);
                                }
                                core::DrawBound2 (view, bound2);
                            }
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygon1 == 0) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else if (IsSelect ()) {
                                core::DrawBound2 (view, bound1);
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;
                                bezierPolygons.push_back (bezierPolygon1);
                                BeginTransaction ();
                                ToggleSelectedPolygons (bezierPolygons);
                                CommitTransaction ();
                            }
                            else if (bezierPolygon1->IsClosed ()) {
                                core::DrawBound2 (view, bound1);
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_10);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_0);
                            }
                            else {
                                core::DrawBound2 (view, bound1);
                                UpdateState (2, flags | ScrollLockOff | CursorVisible);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_12);
                            }
                        }
                        else if (state == 3) {
                            if (bezierPolygon2 == 0) {
                                UpdateState (2, flags);
                            }
                            else if (bezierPolygon1 == bezierPolygon2) {
                                UpdateState (2, flags);
                                core::DrawBound2 (view, bound2);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_11);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_12);
                            }
                            else if (bezierPolygon2->IsClosed ()) {
                                UpdateState (2, flags);
                                core::DrawBound2 (view, bound1);
                                core::DrawBound2 (view, bound2);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_10);
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_12);
                            }
                            else {
                                core::DrawBound2 (view, bound1);
                                core::DrawBound2 (view, bound2);
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                _3ds::io::BezierPolygon2::UniquePtr bezierPolygon =
                                    _3ds::ext::BezierPolygon2 (*bezierPolygon1).PolyConnect (
                                        *bezierPolygon2, IsShift () ?
                                            _3ds::ext::BezierPolygon2::CONNECT_FIRST_FIRST :
                                            _3ds::ext::BezierPolygon2::CONNECT_FIRST_LAST);
                                assert (bezierPolygon.get () != 0);
                                BeginTransaction ();
                                // Delete the old polygons.
                                DeletePolygon (GetPolygonIndex (*bezierPolygon1), false);
                                DeletePolygon (GetPolygonIndex (*bezierPolygon2), false);
                                // Add the new polygon.
                                AddPolygon (std::move (bezierPolygon), false);
                                ExecuteAndAddFinalOperation (
                                    command::FinalOperation::SharedPtr (
                                        new core::command::FlipFramebufferFinalOperation));
                                CommitTransaction ();
                                core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_0);
                            }
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            core::UI::Instance ()->consoleWindow->Print (IDS_CREATEPOLYCONNECT_0);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreatePolyConnectEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreatePolyConnectEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
