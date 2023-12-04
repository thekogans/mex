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
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/3ds/ext/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_MODIFYPOLYGONMIRROR_0 = "Select polygon to mirror, [Tab] = change cursor [Shift] = clone\n";
                const char *IDS_MODIFYPOLYGONMIRROR_2 = "Mirror @: x: %s, y: %s";
            }

            class SpeedBarModifyPolygonMirrorEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifyPolygonMirrorEventHandler)

            private:
                // Mirror polygon or a selection set of polygons.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarModifyPolygonMirrorEventHandlerTool)

                private:
                    blas::Point2 start;
                    blas::Matrix2 xform;
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;

                public:
                    Tool (core::Module &module) : core::Tool (module) {
                        cursor = core::CursorMgr::FOURWAY_CURSOR;
                    }

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (cursor);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_MODIFYPOLYGONMIRROR_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            start = pt;
                            xform = blas::Matrix2::Translate (-start) *
                                (cursor == core::CursorMgr::HORIZONTAL_CURSOR ? blas::Matrix2::MirrorX :
                                    cursor == core::CursorMgr::VERTICAL_CURSOR ? blas::Matrix2::MirrorY :
                                    blas::Matrix2::MirrorXY) *
                                blas::Matrix2::Translate (start);
                            bezierPolygons.clear ();
                            if (IsSelect () || !Shaper::Instance ().flags.Test (Shaper::Selected)) {
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                    core::GetIOProject ().shaper.steps);
                                if (pickInfo.FindFirst ()) {
                                    BeginTransaction ();
                                    _3ds::io::BezierPolygon2 *bezierPolygon =
                                        core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex];
                                    bezierPolygons.push_back (bezierPolygon);
                                    DrawPolygons2 (view, bezierPolygons, xform);
                                }
                            }
                            else {
                                GetSelectedPolygons (bezierPolygons, core::GetIOProject ().shaper.selectMask);
                                if (!bezierPolygons.empty ()) {
                                    BeginTransaction ();
                                    DrawPolygons2 (view, bezierPolygons, xform);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            DrawPolygons2 (view, bezierPolygons, xform);
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
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (IsClone ()) {
                                util::OwnerVector<_3ds::io::BezierPolygon2> newBezierPolygons;
                                for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                    _3ds::io::BezierPolygon2::UniquePtr bezierPolygon =
                                        *bezierPolygons[i] * xform;
                                    assert (bezierPolygon.get () != 0);
                                    if (bezierPolygon.get () != 0) {
                                        _3ds::io::command::BezierPolygon2ClearFlagsCommand (
                                            *bezierPolygon.get (),
                                            _3ds::io::BezierPolygon2::Vertex::SelectionSetA |
                                            _3ds::io::BezierPolygon2::Vertex::SelectionSetB |
                                            _3ds::io::BezierPolygon2::Vertex::SelectionSetC |
                                            _3ds::io::BezierPolygon2::Vertex::Shape |
                                            _3ds::io::BezierPolygon2::Vertex::Hidden |
                                            _3ds::io::BezierPolygon2::Vertex::Frozen).Execute ();
                                        newBezierPolygons.push_back (bezierPolygon.get ());
                                        bezierPolygon.release ();
                                    }
                                }
                                AddPolygons (newBezierPolygons);
                                newBezierPolygons.clear ();
                            }
                            else {
                                ErasePolygons (bezierPolygons, false);
                                for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                    ExecuteAndAddCommand (
                                        command::Command::SharedPtr (
                                            new _3ds::ext::command::BezierPolygon2XformCommand (
                                                *bezierPolygons[i], xform)));
                                }
                                DrawPolygons (bezierPolygons);
                            }
                            CommitTransaction ();
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            DrawPolygons2 (view, bezierPolygons, xform);
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
                            DrawPolygons2 (view, bezierPolygons, xform);
                            switch (cursor) {
                                case core::CursorMgr::FOURWAY_CURSOR:
                                    xform = blas::Matrix2::Translate (-start) *
                                        blas::Matrix2::MirrorXY *
                                        blas::Matrix2::Translate (pt);
                                    break;
                                case core::CursorMgr::HORIZONTAL_CURSOR:
                                    xform = blas::Matrix2::Translate (-start) *
                                        blas::Matrix2::MirrorX *
                                        blas::Matrix2::Translate (blas::Point2 (pt.x, start.y));
                                    break;
                                case core::CursorMgr::VERTICAL_CURSOR:
                                    xform = blas::Matrix2::Translate (-start) *
                                        blas::Matrix2::MirrorY *
                                        blas::Matrix2::Translate (blas::Point2 (start.x, pt.y));
                                    break;
                            }
                            DrawPolygons2 (view, bezierPolygons, xform);
                        }
                    }

                    virtual void KeyDown (const _3ds::opengl::View &view, util::ui32 key, util::ui32 repeatCount, util::ui32 flags) {
                        if (key == Qt::Key_Tab) {
                            if (state == 2) {
                                DrawPolygons2 (view, bezierPolygons, xform);
                                RollbackTransaction ();
                                core::UI::Instance ().viewLayoutWindow->SetMousePosition (view.P2D (start));
                                xform = blas::Matrix2::Translate (-start) *
                                    (cursor == core::CursorMgr::HORIZONTAL_CURSOR ? blas::Matrix2::MirrorX :
                                        cursor == core::CursorMgr::VERTICAL_CURSOR ? blas::Matrix2::MirrorY : blas::Matrix2::MirrorXY) *
                                    blas::Matrix2::Translate (start);
                                DrawPolygons2 (view, bezierPolygons, xform);
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
                            DrawPolygons2 (view, bezierPolygons, xform);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_MODIFYPOLYGONMIRROR_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (xform.t.x).c_str (),
                                _3ds::ext::Units (core::GetIOProject ().units).Format (xform.t.y).c_str ());
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarModifyPolygonMirrorEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarModifyPolygonMirrorEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
