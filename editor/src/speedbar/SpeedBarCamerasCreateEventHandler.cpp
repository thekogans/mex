// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_editor.
//
// libthekogans_mex_editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_editor. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/editor/ui/CameraDefinitionDialog.h"

namespace thekogans {
    namespace mex {
        namespace editor {

            namespace {
                const char *CONSOLE_1 = "Cameras Create: Place camera\n";
                const char *CONSOLE_2 = "Cameras Create: Now place camera's target\n";
            }

            class SpeedBarCamerasCreateEventHandler :
                    public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (
                    SpeedBarCamerasCreateEventHandler)

            private:

                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCamerasCreateEventHandlerTool)

                private:
                    std::string name;
                    blas::Point2 position;
                    blas::Point2 target;
                    util::f32 roll;
                    util::f32 fov;
                    bool showCone;

                public:
                    explicit Tool (core::Module &module) :
                        core::Tool (module),
                        name ("Camera"),
                        roll (0.0f),
                        fov (48.0f),
                        showCone (false) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ()->SetCursor (core::CursorMgr::CROSS_CURSOR);
                        //GetEditor ()->HideCameras (false);
                        assert (core::UI::Instance ()->consoleWindow != 0);
                        core::UI::Instance ()->consoleWindow->Print (CONSOLE_1);
                    }

                    virtual void LButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            position = pt;
                            target = pt;
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            target = pt;
                            core::DrawArrow2 (view, position, target);
                        }
                    }

                    virtual void LButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff | CursorVisible);
                            core::UI::Instance ()->consoleWindow->Print (CONSOLE_2);
                        }
                        else if (state == 3) {
                            core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            CameraDefinitionDialog cameraDefinitionDialog (
                                module, name, roll, fov, showCone);
                            if (cameraDefinitionDialog.exec () == QDialog::Accepted) {
                                // FIXME: implement
                                assert (0);
                            }
                            core::UI::Instance ()->consoleWindow->Print (CONSOLE_1);
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
                            UpdateState (3, flags | ScrollLockOn | CursorHidden);
                            target = pt;
                            core::DrawArrow2 (view, position, target);
                        }
                    }

                    virtual void RButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            core::UI::Instance ()->consoleWindow->Print (CONSOLE_1);
                        }
                    }

                    virtual void MouseMove (
                            const _3ds::opengl::View &view,
                            util::ui32,
                            const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawArrow2 (view, position, target);
                            target = pt;
                            core::DrawArrow2 (view, position, target);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawArrow2 (view, position, target);
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCamerasCreateEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool),
                    tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (
                SpeedBarCamerasCreateEventHandler,
                Editor)

        } // namespace editor
    } // namespace mex
} // namespace thekogans
