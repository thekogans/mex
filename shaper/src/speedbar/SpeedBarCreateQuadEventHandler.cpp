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
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/Units.h"
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
                const char *IDS_CREATEQUAD_0 = "Place one corner of quadrilateral:\n";
                const char *IDS_CREATEQUAD_1 = "Place opposite corner of quadrilateral:\n";
                const char *IDS_CREATEQUAD_21 = "Size: %s\n";
                const char *IDS_CREATEQUAD_22 = "[%s %s]\n";
            }

            class SpeedBarCreateQuadEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateQuadEventHandler)

            private:
                // Create a quadrilateral. Hold down the CONTROL
                // key to create a square.
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateQuadEventHandlerTool)

                private:
                    util::f32 oldx;
                    blas::Bound2 bound;
                    util::ui32 square;

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATEQUAD_0);
                    }

                    virtual void LButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            oldx = pt.x;
                            bound = pt;
                            square = IsCtrl ();
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            if (square) {
                                bound.max.x += pt.x - oldx;
                                bound.max.y += oldx - pt.x;
                            }
                            else {
                                bound.max = pt;
                            }
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void LButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff);
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEQUAD_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            bound = bound.Normalize ();
                            if (bound.min != bound.max) {
                                core::WaitCursor waitCursor;
                                AddPolygon (_3ds::ext::Quad (bound));
                                CommitTransaction ();
                            }
                            else {
                                AbortTransaction ();
                            }
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEQUAD_0);
                        }
                    }

                    virtual void RButtonDown (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            if (square) {
                                bound.max.x += pt.x - oldx;
                                bound.max.y += oldx - pt.x;
                            }
                            else {
                                bound.max = pt;
                            }
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void RButtonUp (const _3ds::opengl::View &view, util::ui32 flags, const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATEQUAD_0);
                        }
                    }

                    virtual void MouseMove (const _3ds::opengl::View &view, util::ui32, const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound);
                            if (square) {
                                bound.max.x += pt.x - oldx;
                                bound.max.y += oldx - pt.x;
                                oldx = pt.x;
                            }
                            else {
                                bound.max = pt;
                            }
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawBound2 (view, bound);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            if (square) {
                                ui.SetText (IDS_CREATEQUAD_21,
                                    _3ds::ext::Units (core::GetIOProject ().units).Format (
                                        bound.max.x - bound.min.x).c_str ());
                            }
                            else {
                                ui.SetText (IDS_CREATEQUAD_22,
                                    _3ds::ext::Units (core::GetIOProject ().units).Format (
                                        sqrtf ((bound.max.x - bound.min.x) * (bound.max.x - bound.min.x))).c_str (),
                                    _3ds::ext::Units (core::GetIOProject ().units).Format (
                                        sqrtf ((bound.max.y - bound.min.y) * (bound.max.y - bound.min.y))).c_str ());
                            }
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateQuadEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateQuadEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
