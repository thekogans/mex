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

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/3ds/opengl/Viewport3D.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/shaper/Util.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            namespace {
                const char *IDS_FREEHANDDRAW_0 = "Draw freehand curve\n";
            }

            class SpeedBarCreateFreehandDrawEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateFreehandDrawEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateFreehandDrawEventHandlerTool)

                private:
                    blas::Polygon2 polygon;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        assert (core::UI::Instance ()->consoleWindow != 0);
                        core::UI::Instance ()->consoleWindow->Print (IDS_FREEHANDDRAW_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            scrollLock = false;
                            polygon = blas::Polygon2 ();
                            polygon.points.push_back (pt);
                            BeginTransaction ();
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            core::DrawPolygon2 (view, polygon);
                            if (!polygon.points.empty ()) {
                                core::WaitCursor waitCursor;
                                AddPolygon (_3ds::ext::FitPolygon (polygon,
                                        core::GetIOProject ().shaper.create.freehandSmoothness / 12.5f));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32, const blas::Point2 &pt) {
                        if (state == 1) {
                            polygon.points.push_back (pt);
                            std::size_t count = polygon.points.size ();
                            assert (count > 1);
                            core::DrawLine2 (view, polygon.points[count - 2], polygon.points[count - 1]);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 1) {
                            core::DrawPolygon2 (view, polygon);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateFreehandDrawEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateFreehandDrawEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
