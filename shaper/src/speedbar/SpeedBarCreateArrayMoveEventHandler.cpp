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
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommandSpecializations.h"
#include "thekogans/mex/3ds/ext/Units.h"
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
#include "thekogans/mex/shaper/ui/ArrayMoveDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_CREATEARRAYMOVE_0 = "Click on polygon to array move\n";
                const char *IDS_CREATEARRAYMOVE_2 = "Offsets: x: %s, y: %s";
            }

            class SpeedBarCreateArrayMoveEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateArrayMoveEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateArrayMoveEventHandlerTool)

                private:
                    util::ui32 objectsInArray;
                    blas::Point2 start;
                    blas::Matrix2 xform;
                    blas::Bound2 bound;
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;

                public:
                    Tool (core::Module &module) : core::Tool (module), objectsInArray (10) {
                        cursor = core::CursorMgr::FOURWAY_CURSOR;
                    }

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (cursor);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATEARRAYMOVE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            start = pt;
                            xform = blas::Matrix2::Identity;
                            bezierPolygons.clear ();
                            if (IsSelect () || !Shaper::Instance ().flags.Test (Shaper::Selected)) {
                                bound = GetPickBound (view, pt);
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                                    core::GetIOProject ().shaper.steps);
                                if (pickInfo.FindFirst ()) {
                                    BeginTransaction ();
                                    _3ds::io::BezierPolygon2 *bezierPolygon =
                                        core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                    bound = _3ds::ext::BezierPolygon2 (*bezierPolygon).GetBound (
                                        core::GetIOProject ().shaper.steps);
                                    core::DrawBound2 (view, bound * xform);
                                    bezierPolygons.push_back (bezierPolygon);
                                }
                            }
                            else {
                                GetSelectedPolygons (bezierPolygons, core::GetIOProject ().shaper.selectMask);
                                if (!bezierPolygons.empty ()) {
                                    BeginTransaction ();
                                    bound = GetPolygonsBound (bezierPolygons);
                                    core::DrawBound2 (view, bound * xform);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            core::DrawBound2 (view, bound * xform);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygons.empty ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else if (IsSelect ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                                ToggleSelectedPolygons (bezierPolygons);
                                CommitTransaction ();
                            }
                            else {
                                UpdateState (2, flags | ScrollLockOff);
                            }
                        }
                        else if (state == 3) {
                            core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            util::OwnerVector<_3ds::io::BezierPolygon2> newBezierPolygons;
                            if (ArrayMoveDialog (objectsInArray).exec () == QDialog::Accepted) {
                                for (util::ui32 i = 0; i < objectsInArray - 1; ++i) {
                                    for (std::size_t j = 0, count = bezierPolygons.size (); j < count; ++j) {
                                        _3ds::io::BezierPolygon2::UniquePtr bezierPolygon;
                                        if (i == 0) {
                                            bezierPolygon = *bezierPolygons[j] * xform;
                                        }
                                        else {
                                            bezierPolygon =
                                                *newBezierPolygons[(i - 1) * bezierPolygons.size () + j] * xform;
                                        }
                                        assert (bezierPolygon.get () != 0);
                                        if (bezierPolygon.get () != 0) {
                                            newBezierPolygons.push_back (bezierPolygon.get ());
                                            bezierPolygon.release ();
                                        }
                                    }
                                }
                            }
                            if (!newBezierPolygons.empty ()) {
                                AddPolygons (newBezierPolygons);
                                newBezierPolygons.clear ();
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            core::DrawBound2 (view, bound * xform);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound * xform);
                            switch (cursor) {
                                case core::CursorMgr::FOURWAY_CURSOR:
                                    xform = blas::Matrix2::Translate (pt - start);
                                    break;
                                case core::CursorMgr::HORIZONTAL_CURSOR:
                                    xform = blas::Matrix2::Translate (blas::Point2 (pt.x - start.x, 0.0f));
                                    break;
                                case core::CursorMgr::VERTICAL_CURSOR:
                                    xform = blas::Matrix2::Translate (blas::Point2 (0.0f, pt.y - start.y));
                                    break;
                            }
                            core::DrawBound2 (view, bound * xform);
                        }
                    }

                    virtual void KeyDown (const _3ds::opengl::View &view, util::ui32 key,
                            util::ui32 repeatCount, util::ui32 flags) {
                        if (key == Qt::Key_Tab) {
                            if (state == 2) {
                                core::DrawBound2 (view, bound * xform);
                                RollbackTransaction ();
                                core::UI::Instance ().viewLayoutWindow->SetMousePosition (view.P2D (start));
                                xform = blas::Matrix2::Identity;
                                core::DrawBound2 (view, bound * xform);
                            }
                            switch (cursor) {
                                case core::CursorMgr::FOURWAY_CURSOR:
                                    cursor = core::CursorMgr::HORIZONTAL_CURSOR;
                                    break;
                                case core::CursorMgr::HORIZONTAL_CURSOR:
                                    cursor = core::CursorMgr::VERTICAL_CURSOR;
                                    break;
                                case core::CursorMgr::VERTICAL_CURSOR:
                                    cursor = core::CursorMgr::FOURWAY_CURSOR;
                                    break;
                            }
                            core::CursorMgr::Instance ().SetCursor (cursor);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound * xform);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_CREATEARRAYMOVE_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (xform.t.x).c_str (),
                                _3ds::ext::Units (core::GetIOProject ().units).Format (xform.t.y).c_str ());
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateArrayMoveEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateArrayMoveEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
