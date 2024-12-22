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
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommandSpecializations.h"
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
#include "thekogans/mex/shaper/ui/ArrayLinearDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDC_CREATEARRAYLINEAR_0 = "Click on polygon to array linearly\n";
            }

            class SpeedBarCreateArrayLinearEventHandler :
                    public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (
                    SpeedBarCreateArrayLinearEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateArrayLinearEventHandlerTool)

                private:
                    util::ui32 objectsInArray;
                    util::f32 spacing;
                    bool objectSpacing;
                    bool centerToCenter;
                    blas::Bound2 bound;
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;

                public:
                    Tool (core::Module &module) :
                            core::Tool (module),
                            objectsInArray (10),
                            spacing (10.0f),
                            objectSpacing (true),
                            centerToCenter (false) {
                        cursor = core::CursorMgr::UP_CURSOR;
                    }

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (cursor);
                        core::UI::Instance ()->consoleWindow->Print (IDC_CREATEARRAYLINEAR_0);
                    }

                    virtual void LButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygons.clear ();
                            if (IsSelect () || !Shaper::Instance ()->flags.Test (Shaper::Selected)) {
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (
                                        new blas::BoundRegion2 (GetPickBound (view, pt))),
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
                                GetSelectedPolygons (
                                    bezierPolygons,
                                    core::GetIOProject ().shaper.selectMask);
                                if (!bezierPolygons.empty ()) {
                                    bound = GetPolygonsBound (bezierPolygons);
                                    core::DrawBound2 (view, bound);
                                }
                            }
                        }
                    }

                    virtual void LButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 1) {
                            if (bezierPolygons.empty ()) {
                                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            }
                            else {
                                core::DrawBound2 (view, bound);
                                if (IsSelect ()) {
                                    UpdateState (
                                        0,
                                        flags | ScrollLockOff | CursorVisible | ViewReleased);
                                    BeginTransaction ();
                                    ToggleSelectedPolygons (bezierPolygons);
                                    CommitTransaction ();
                                }
                                else {
                                    core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                                    UpdateState (
                                        0,
                                        flags | ScrollLockOff | CursorVisible | ViewReleased);
                                    util::f32 size;
                                    if (cursor == core::CursorMgr::UP_CURSOR ||
                                        cursor == core::CursorMgr::DOWN_CURSOR) {
                                        size = bound.max.y - bound.min.y;
                                    }
                                    else {
                                        size = bound.max.x - bound.min.x;
                                    }
                                    if (ArrayLinearDialog (
                                            Shaper::Instance ()->flags.Test (Shaper::Selected),
                                            objectsInArray,
                                            spacing,
                                            objectSpacing,
                                            centerToCenter,
                                            size).exec () == QDialog::Accepted) {
                                        util::f32 length = spacing;
                                        if (!centerToCenter) {
                                            length += size;
                                        }
                                        blas::Matrix2 xform;
                                        switch (cursor) {
                                            case core::CursorMgr::UP_CURSOR:
                                                xform = blas::Matrix2::Translate (blas::Point2 (0.0f, length));
                                                break;
                                            case core::CursorMgr::DOWN_CURSOR:
                                                xform = blas::Matrix2::Translate (blas::Point2 (0.0f, -length));
                                                break;
                                            case core::CursorMgr::LEFT_CURSOR:
                                                xform = blas::Matrix2::Translate (blas::Point2 (-length, 0.0f));
                                                break;
                                            case core::CursorMgr::RIGHT_CURSOR:
                                                xform = blas::Matrix2::Translate (blas::Point2 (length, 0.0f));
                                                break;
                                        }
                                        util::OwnerVector<_3ds::io::BezierPolygon2> newBezierPolygons;
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
                                        if (!newBezierPolygons.empty ()) {
                                            BeginTransaction ();
                                            AddPolygons (newBezierPolygons);
                                            newBezierPolygons.clear ();
                                            CommitTransaction ();
                                        }
                                    }
                                }
                            }
                        }
                    }

                    virtual void KeyDown (
                            const _3ds::opengl::View &view,
                            util::ui32 key,
                            util::ui32 repeatCount,
                            util::ui32 flags) {
                        if (key == Qt::Key_Tab) {
                            switch (cursor) {
                                case core::CursorMgr::UP_CURSOR:
                                    cursor = core::CursorMgr::RIGHT_CURSOR;
                                    break;
                                case core::CursorMgr::DOWN_CURSOR:
                                    cursor = core::CursorMgr::LEFT_CURSOR;
                                    break;
                                case core::CursorMgr::LEFT_CURSOR:
                                    cursor = core::CursorMgr::UP_CURSOR;
                                    break;
                                case core::CursorMgr::RIGHT_CURSOR:
                                    cursor = core::CursorMgr::DOWN_CURSOR;
                                    break;
                            }
                            core::CursorMgr::Instance ()->SetCursor (cursor);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateArrayLinearEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool),
                    tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (
                SpeedBarCreateArrayLinearEventHandler,
                Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
