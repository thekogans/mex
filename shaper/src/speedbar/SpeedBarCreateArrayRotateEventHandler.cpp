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
#include "thekogans/util/Flags.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/ext/DrawingAids.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/Shaper.h"
#include "thekogans/mex/shaper/ui/ArrayRotateDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDC_CREATEARRAYROTATE_0 = "Click on polygon to array rotate\n";
            }

            class SpeedBarCreateArrayRotateEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateArrayRotateEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateArrayRotateEventHandlerTool)

                private:
                    util::ui32 objectsInArray;
                    bool rotate;
                    util::f32 theta;
                    util::f32 theta2;
                    blas::Point2 start;
                    blas::Matrix2 xform;
                    blas::Bound2 bound;
                    blas::Point2 axis;
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;

                public:
                    Tool (core::Module &module) :
                        core::Tool (module),
                        objectsInArray (36),
                        rotate (true) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        core::UI::Instance ()->consoleWindow->Print (IDC_CREATEARRAYROTATE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            theta = 0.0f;
                            theta2 = 0.0f;
                            start = pt;
                            xform = blas::Matrix2::Identity;
                            bezierPolygons.clear ();
                            if (IsSelect () || !Shaper::Instance ()->flags.Test (Shaper::Selected)) {
                                bound = GetPickBound (view, pt);
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
                                    axis = Shaper::Instance ()->flags.Test (Shaper::UseLocalAxis) ?
                                        bound.Center () : core::GetIOProject ().shaper.axis.axis;
                                    core::DrawBound2 (view, bound, xform);
                                    bezierPolygons.push_back (bezierPolygon);
                                }
                            }
                            else {
                                GetSelectedPolygons (bezierPolygons, core::GetIOProject ().shaper.selectMask);
                                if (!bezierPolygons.empty ()) {
                                    bound = GetPolygonsBound (bezierPolygons);
                                    axis = Shaper::Instance ()->flags.Test (Shaper::UseLocalAxis) ?
                                        bound.Center () : core::GetIOProject ().shaper.axis.axis;
                                    core::DrawBound2 (view, bound, xform);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags);
                            core::DrawBound2 (view, bound, xform);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygons.empty ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else if (IsSelect ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                BeginTransaction ();
                                ToggleSelectedPolygons (bezierPolygons);
                                CommitTransaction ();
                            }
                            else {
                                UpdateState (2, flags);
                            }
                        }
                        else if (state == 3) {
                            core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (ArrayRotateDialog (objectsInArray, rotate).exec () == QDialog::Accepted) {
                                util::OwnerVector<_3ds::io::BezierPolygon2> newBezierPolygons;
                                util::f32 delta = theta2 / (objectsInArray - 1);
                                blas::Point2 center = bound.Center ();
                                for (util::ui32 i = 0; i < objectsInArray - 1; ++i) {
                                    if (rotate) {
                                        xform = blas::Matrix2::Translate (-axis) *
                                            blas::Matrix2::RotateZ (util::RAD (delta * (i + 1))) *
                                            blas::Matrix2::Translate (axis);
                                    }
                                    else {
                                        if (Shaper::Instance ()->flags.Test (Shaper::UseLocalAxis)) {
                                            xform = blas::Matrix2::Identity;
                                        }
                                        else {
                                            xform = blas::Matrix2::Translate (
                                                center * blas::Matrix2::RotateZ (util::RAD (delta * (i + 1))));
                                        }
                                    }
                                    for (std::size_t j = 0, count = bezierPolygons.size (); j < count; ++j) {
                                        _3ds::io::BezierPolygon2::UniquePtr bezierPolygon =
                                            *bezierPolygons[j] * xform;
                                        assert (bezierPolygon.get () != 0);
                                        if (bezierPolygon.get () != 0) {
                                            newBezierPolygons.push_back (bezierPolygon.get ());
                                            bezierPolygon.release ();
                                        }
                                    }
                                }
                                if (!newBezierPolygons.empty ()) {
                                    BeginTransaction ();
                                    AddPolygons (newBezierPolygons);
                                    newBezierPolygons.clear ();
                                    CommitTransaction ();
                                }
                            }
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags);
                            core::DrawBound2 (view, bound, xform);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound, xform);
                            blas::Point d1 = view.P2D (pt);
                            blas::Point d2 = view.P2D (start);
                            core::UI::Instance ()->viewLayoutWindow->SetMousePosition (d2);
                            theta += ((util::f32)d1.x - d2.x) * 0.25f;
                            util::CLAMP (theta, -180.0f, 180.0f);
                            theta2 = core::GetIOProject ().drawingAids.useAngleSnap ?
                                _3ds::ext::DrawingAids (core::GetIOProject ().drawingAids).SnapAngle (theta) : theta;
                            xform = blas::Matrix2::Translate (-axis) *
                                blas::Matrix2::RotateZ (util::RAD (theta2)) *
                                blas::Matrix2::Translate (axis);
                            core::DrawBound2 (view, bound, xform);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound, xform);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText ("Angle: %.2f degrees", theta2);
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateArrayRotateEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateArrayRotateEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
