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
#include "thekogans/mex/command/CommandDispatcher.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonVertexCommands.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommandSpecializations.h"
#include "thekogans/mex/3ds/opengl/Viewport3D.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/Util.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            // Create line. This tool has the following properties:
            //
            //      1. You can create a new polygon by clicking
            //          on an empty spot in the viewport.
            //      2. You can insert vertices in to an existing
            //          polygon by clicking on it.
            //      3. You can add vertices to the end of an
            //          open polygon by clicking on it's end vertex.
            //      4. You can close an open polygon by clicking
            //          on it's end vertex and then clicking on it's
            //          start vertex.
            //      5. You can connect two open polygons by clicking
            //          on their end vertices.
            //      6. As you create a vertex, you can adjust it's
            //          curvature by holding the left button down,
            //          and moving the mouse.
            //      7. Right click to stop the operation.

            namespace {
                const char *IDS_CREATELINE_0 = "Click to create or insert Line vertex:\n";
            }

            class SpeedBarCreateLineEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateLineEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateLineEventHandlerTool)

                private:
                    blas::Point2 start;
                    blas::Matrix2 red;
                    blas::Matrix2 yellow;
                    bool selected;
                    std::size_t polygonIndex;
                    std::size_t vertexIndex;
                    std::size_t startVertexCount;
                    blas::Matrix2 xform;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATELINE_0);
                    }

                    virtual void LButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            BeginTransaction ();
                            start = pt;
                            red = blas::Matrix2::Identity;
                            yellow = blas::Matrix2::Identity;
                            selected = false;
                            blas::Bound2 bound = GetPickBound (view, pt);
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                // Clicked on existing vertex.
                                polygonIndex = pickInfo.polygonIndex;
                                vertexIndex = pickInfo.vertexIndex;
                                startVertexCount = ioBezierPolygon ().vertices.size ();
                                selected = _3ds::ext::BezierPolygon2 (ioBezierPolygon ()).IsSelected (
                                    core::GetIOProject ().shaper.selectMask);
                                UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            }
                            else {
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Segment,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (new blas::BoundRegion2 (bound)),
                                    core::GetIOProject ().shaper.steps);
                                if (pickInfo.FindFirst ()) {
                                    // Clicked on existing segment.
                                    polygonIndex = pickInfo.polygonIndex;
                                    vertexIndex = pickInfo.vertexIndex;
                                    startVertexCount = ioBezierPolygon ().vertices.size ();
                                    selected = _3ds::ext::BezierPolygon2 (ioBezierPolygon ()).IsSelected (
                                        core::GetIOProject ().shaper.selectMask);
                                    UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                                }
                                else {
                                    // Clicked on empty space.
                                    polygonIndex = core::GetIOProject ().shaper.polygons2.size ();
                                    vertexIndex = 0;
                                    startVertexCount = 1;
                                    {
                                        _3ds::io::BezierPolygon2::UniquePtr bezierPolygon (
                                            new _3ds::io::BezierPolygon2);
                                        assert (bezierPolygon.get () != 0);
                                        bezierPolygon->vertices.push_back (
                                            _3ds::io::BezierPolygon2::Vertex (
                                                pt, blas::Point2::Zero, blas::Point2::Zero,
                                                _3ds::io::BezierPolygon2::Vertex::Last |
                                                _3ds::io::BezierPolygon2::Vertex::First));
                                        AddPolygon (std::move (bezierPolygon));
                                    }
                                    UpdateState (3, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                                }
                            }
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            start = pt;
                            red = blas::Matrix2::Identity;
                            yellow = blas::Matrix2::Identity;
                            if (!ioBezierPolygon ().IsClosed () &&
                                ioBezierPolygon ().vertices[vertexIndex].IsLast ()) {
                                // Original click was on last vertex of open polygon.
                                _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                    _3ds::ext::BezierPolygon2::PickInfo::Vertex,
                                    core::GetIOProject ().shaper.polygons2,
                                    blas::Region2::UniquePtr (
                                        new blas::BoundRegion2 (GetPickBound (view, pt))),
                                    core::GetIOProject ().shaper.steps);
                                if (pickInfo.FindFirst ()) {
                                    if (polygonIndex == pickInfo.polygonIndex &&
                                        ioBezierPolygon ().vertices[pickInfo.vertexIndex].IsFirst ()) {
                                        // This click is on first vertex of same polygon. Close it.
                                        state = 5;
                                        BezierPolygon (
                                            ioBezierPolygon ()).DrawVertexSegments (
                                                view, vertexIndex, xform);
                                        UnhideVertexSegments (ioBezierPolygon (), vertexIndex);
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new _3ds::io::command::BezierPolygon2DeleteVertexCommand (
                                                    ioBezierPolygon (), vertexIndex--)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new _3ds::io::command::BezierPolygon2VertexSetFlagsCommand (
                                                    ioBezierPolygon (), vertexIndex,
                                                    ioBezierPolygon ().vertices[vertexIndex].flags |
                                                    _3ds::io::BezierPolygon2::Vertex::Closed)));
                                        if (selected) {
                                            DrawPolygon (ioBezierPolygon ());
                                        }
                                        else {
                                            std::size_t nextVertexIndex =
                                                _3ds::ext::BezierPolygon2 (
                                                    ioBezierPolygon ()).NextVertexIndex (vertexIndex);
                                            if (nextVertexIndex != util::NIDX) {
                                                DrawSegment (ioBezierPolygon (), vertexIndex, nextVertexIndex);
                                            }
                                        }
                                        CommitTransaction ();
                                    }
                                    else if (polygonIndex != pickInfo.polygonIndex &&
                                        !ioBezierPolygon (pickInfo.polygonIndex).IsClosed () &&
                                        (ioBezierPolygon (
                                            pickInfo.polygonIndex).vertices[pickInfo.vertexIndex].IsFirst () ||
                                            ioBezierPolygon (pickInfo.polygonIndex).vertices[pickInfo.vertexIndex].IsLast ())) {
                                        // This click is on first or last vertex of another
                                        // open polygon. Connect them.
                                        state = 5;
                                        BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                            view, vertexIndex, xform);
                                        UnhideVertexSegments (ioBezierPolygon (), vertexIndex);
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new _3ds::io::command::BezierPolygon2DeleteVertexCommand (
                                                    ioBezierPolygon (), vertexIndex)));
                                        _3ds::io::BezierPolygon2::UniquePtr newBezierPolygon =
                                            _3ds::ext::BezierPolygon2 (ioBezierPolygon ()).Connect (
                                                ioBezierPolygon (pickInfo.polygonIndex), pickInfo.vertexIndex ?
                                                _3ds::ext::BezierPolygon2::CONNECT_LAST_LAST :
                                                _3ds::ext::BezierPolygon2::CONNECT_LAST_FIRST);
                                        assert (newBezierPolygon.get () != 0);
                                        if (newBezierPolygon.get () != 0) {
                                            std::vector<std::size_t> polygonIndices;
                                            polygonIndices.push_back (polygonIndex);
                                            polygonIndices.push_back (pickInfo.polygonIndex);
                                            DeletePolygons (polygonIndices, false);
                                            AddPolygon (std::move (newBezierPolygon));
                                            CommitTransaction ();
                                        }
                                        else {
                                            AbortTransaction ();
                                        }
                                    }
                                }
                            }
                            if (state != 5) {
                                BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                    view, vertexIndex, xform);
                                ExecuteAndAddCommand (
                                    command::Command::UniquePtr (
                                        new _3ds::io::command::BezierPolygon2VertexSetPtCommand (
                                            ioBezierPolygon (), vertexIndex,
                                            ioBezierPolygon ().vertices[vertexIndex].pt * xform)));
                                BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                    view, vertexIndex, red, yellow);
                            }
                        }
                    }

                    virtual void LButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 1) {
                            start = pt;
                            xform = blas::Matrix2::Identity;
                            std::size_t nextVertexIndex =
                                _3ds::ext::BezierPolygon2 (ioBezierPolygon ()).NextVertexIndex (vertexIndex);
                            if (nextVertexIndex != util::NIDX) {
                                EraseSegment (ioBezierPolygon (), vertexIndex, nextVertexIndex);
                            }
                            ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2InsertVertexCommand (
                                        ioBezierPolygon (),
                                        ++vertexIndex,
                                        _3ds::io::BezierPolygon2::Vertex (pt))));
                            HideVertexSegments (ioBezierPolygon (), vertexIndex);
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                view, vertexIndex, xform);
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                        }
                        else if (state == 3) {
                            start = pt;
                            xform = blas::Matrix2::Identity;
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                view, vertexIndex, red, yellow);
                            ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2VertexSetRedCommand (
                                        ioBezierPolygon (), vertexIndex,
                                        ioBezierPolygon ().vertices[vertexIndex].red * red)));
                            ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2VertexSetYellowCommand (
                                        ioBezierPolygon (), vertexIndex,
                                        ioBezierPolygon ().vertices[vertexIndex].yellow * yellow)));
                            UnhideVertexSegments (ioBezierPolygon (), vertexIndex);
                            std::size_t previousVertexIndex =
                                _3ds::ext::BezierPolygon2 (ioBezierPolygon ()).PreviousVertexIndex (vertexIndex);
                            if (previousVertexIndex != util::NIDX) {
                                DrawSegment (ioBezierPolygon (), previousVertexIndex, vertexIndex);
                            }
                            ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2InsertVertexCommand (
                                        ioBezierPolygon (),
                                        ++vertexIndex,
                                        _3ds::io::BezierPolygon2::Vertex (pt))));
                            HideVertexSegments (ioBezierPolygon (), vertexIndex);
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (view, vertexIndex, xform);
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                        }
                        else if (state == 5) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
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
                            UpdateState (5, flags | ScrollLockOn | CursorHidden);
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (view, vertexIndex, xform);
                            UnhideVertexSegments (ioBezierPolygon (), vertexIndex);
                            ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2DeleteVertexCommand (
                                        ioBezierPolygon (), vertexIndex--)));
                            if (selected) {
                                DrawPolygon (ioBezierPolygon ());
                            }
                            else {
                                std::size_t nextVertexIndex =
                                    _3ds::ext::BezierPolygon2 (ioBezierPolygon ()).NextVertexIndex (vertexIndex);
                                if (nextVertexIndex != util::NIDX) {
                                    DrawSegment (ioBezierPolygon (), vertexIndex, nextVertexIndex);
                                }
                            }
                            if (ioBezierPolygon ().vertices.size () > startVertexCount) {
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                        }
                    }

                    virtual void RButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                        }
                    }

                    virtual void MouseMove (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 2) {
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                view, vertexIndex, xform);
                            xform = blas::Matrix2::Translate (pt - start);
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                view, vertexIndex, xform);
                        }
                        else if (state == 3) {
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                view, vertexIndex, red, yellow);
                            core::UI::Instance ().viewLayoutWindow->SetMousePosition (view.P2D (start));
                            if (ioBezierPolygon ().vertices[vertexIndex].yellow.IsZero ()) {
                                yellow *= blas::Matrix2::Translate (pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    red *= blas::Matrix2::Translate (start - pt);
                                }
                            }
                            else {
                                blas::Point2 t1 = ioBezierPolygon ().vertices[vertexIndex].yellow * yellow;
                                yellow *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                if (!util::Flags32 (flags).Test (core::FLAG_CTRL)) {
                                    red *= blas::Matrix2::Aim2D (t1, t1 + pt - start);
                                }
                            }
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (
                                view, vertexIndex, red, yellow);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            BezierPolygon (ioBezierPolygon ()).DrawVertexSegments (view, vertexIndex, xform);
                        }
                    }

                private:
                    inline _3ds::io::BezierPolygon2 &ioBezierPolygon (
                            std::size_t polygonIndex_ = -1) const {
                        return *core::GetIOProject ().shaper.polygons2[
                            polygonIndex_ == -1 ? polygonIndex : polygonIndex_];
                    }
                } tool;

            public:
                explicit SpeedBarCreateLineEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (
                SpeedBarCreateLineEventHandler, Shaper)

        } // namespace editor
    } // namespace mex
} // namespace thekogans
