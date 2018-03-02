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
                const char *IDS_CREATECONNECT_0 = "Select end vertex to connect\n";
                const char *IDS_CREATECONNECT_10 = "Polygon must be open.\n";
                const char *IDS_CREATECONNECT_11 = "Must select end vertex.\n";
                const char *IDS_CREATECONNECT_12 = "Select second end vertex to connect\n";
                const char *IDS_CREATECONNECT_2 = "Offsets: x: %s, y: %s";
            }

            class SpeedBarCreateConnectEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateConnectEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateConnectEventHandlerTool)

                private:
                    _3ds::io::BezierPolygon2 *bezierPolygon1;
                    _3ds::io::BezierPolygon2 *bezierPolygon2;
                    std::size_t vertexIndex1;
                    std::size_t vertexIndex2;
                    blas::Point2 start;
                    blas::Point2 end;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygon1 = 0;
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                IsSelect () ? _3ds::ext::BezierPolygon2::PickInfo::Polygon :
                                _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                BeginTransaction ();
                                bezierPolygon1 =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                if (!IsSelect ()) {
                                    vertexIndex1 = pickInfo.vertexIndex;
                                    assert (vertexIndex1 != util::NIDX);
                                    start = bezierPolygon1->vertices[vertexIndex1].pt;
                                    end = pt;
                                    core::DrawLine2 (view, start, end);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            bezierPolygon2 = 0;
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                bezierPolygon2 =
                                    core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                vertexIndex2 = pickInfo.vertexIndex;
                            }
                            core::DrawLine2 (view, start, end);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygon1 == 0) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                AbortTransaction ();
                            }
                            else if (IsSelect ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;
                                bezierPolygons.push_back (bezierPolygon1);
                                ToggleSelectedPolygons (bezierPolygons);
                                CommitTransaction ();
                            }
                            else if (bezierPolygon1->IsClosed ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                AbortTransaction ();
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_10);
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_0);
                            }
                            else if (vertexIndex1 > 0 && vertexIndex1 < bezierPolygon1->vertices.size () - 1) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                AbortTransaction ();
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_11);
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_0);
                            }
                            else {
                                UpdateState (2, flags | ScrollLockOff | CursorVisible);
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_12);
                            }
                        }
                        else if (state == 3) {
                            if (bezierPolygon2 == 0 ||
                                bezierPolygon1 == bezierPolygon2 ||
                                bezierPolygon2->IsClosed () ||
                                (vertexIndex2 > 0 &&
                                    vertexIndex2 < bezierPolygon2->vertices.size () - 1)) {
                                UpdateState (2, flags | ScrollLockOff | CursorVisible);
                                core::DrawLine2 (view, start, end);
                            }
                            else {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                util::ui32 type;
                                if (vertexIndex1 > 0) {
                                    if (vertexIndex2 > 0) {
                                        type = _3ds::ext::BezierPolygon2::CONNECT_LAST_LAST;
                                    }
                                    else {
                                        type = _3ds::ext::BezierPolygon2::CONNECT_LAST_FIRST;
                                    }
                                }
                                else {
                                    if (vertexIndex2 > 0) {
                                        type = _3ds::ext::BezierPolygon2::CONNECT_FIRST_LAST;
                                    }
                                    else {
                                        type = _3ds::ext::BezierPolygon2::CONNECT_FIRST_FIRST;
                                    }
                                }
                                _3ds::io::BezierPolygon2::UniquePtr bezierPolygon =
                                    _3ds::ext::BezierPolygon2 (*bezierPolygon1).Connect (*bezierPolygon2, type);
                                assert (bezierPolygon.get () != 0);
                                // Delete the old polygons.
                                DeletePolygon (GetPolygonIndex (*bezierPolygon1), false);
                                DeletePolygon (GetPolygonIndex (*bezierPolygon2), false);
                                // Add the new polygon.
                                AddPolygon (std::move (bezierPolygon), false);
                                ExecuteAndAddFinalOperation (
                                    command::FinalOperation::UniquePtr (
                                        new core::command::FlipFramebufferFinalOperation));
                                CommitTransaction ();
                                core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_0);
                            }
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            core::DrawLine2 (view, start, end);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATECONNECT_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawLine2 (view, start, end);
                            end = pt;
                            core::DrawLine2 (view, start, end);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawLine2 (view, start, end);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            blas::Point2 offset = end - start;
                            ui.SetText (IDS_CREATECONNECT_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (offset.x).c_str (),
                                _3ds::ext::Units (core::GetIOProject ().units).Format (offset.y).c_str ());
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateConnectEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateConnectEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
