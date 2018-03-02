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
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/BoundRegion.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/shaper/Util.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_SELECTPOLYGONSINGLE_0 = "Select polygon to select/deselect\n";
            }

            class SpeedBarSelectPolygonSingleEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectPolygonSingleEventHandler)

            private:
                // Select a single polygon.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarSelectPolygonSingleEventHandlerTool)

                private:
                    std::vector<_3ds::io::BezierPolygon2 *> bezierPolygons;
                    blas::Bound2 bound;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetPickCursor (
                            core::GetIOProject ().programConfiguration.pickBoxSize);
                        assert (core::UI::Instance ().consoleWindow != 0);
                        core::UI::Instance ().consoleWindow->Print (IDS_SELECTPOLYGONSINGLE_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            bezierPolygons.clear ();
                            _3ds::ext::BezierPolygon2::PickInfo pickInfo (
                                _3ds::ext::BezierPolygon2::PickInfo::Polygon,
                                core::GetIOProject ().shaper.polygons2,
                                blas::Region2::UniquePtr (new blas::BoundRegion2 (GetPickBound (view, pt))),
                                core::GetIOProject ().shaper.steps);
                            if (pickInfo.FindFirst ()) {
                                bezierPolygons.push_back (core::GetIOProject ().shaper.polygons2[pickInfo.polygonIndex]);
                                bound = GetPolygonsBound (bezierPolygons);
                                core::DrawBound2 (view, bound);
                            }
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            if (!bezierPolygons.empty ()) {
                                core::DrawBound2 (view, bound);
                                BeginTransaction ();
                                ToggleSelectedPolygons (bezierPolygons);
                                CommitTransaction ();
                            }
                        }
                    }
                } tool;

            public:
                explicit SpeedBarSelectPolygonSingleEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarSelectPolygonSingleEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
