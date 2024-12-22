// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_keyframer.
//
// libthekogans_mex_keyframer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_keyframer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_keyframer. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/opengl/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/core/ToolBar.h"
#include "thekogans/mex/core/ModalLoop.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace keyframer {

            struct ToolBarFirstEventHandler : public core::ToolBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFirstEventHandler)

                core::Module &module;

                explicit ToolBarFirstEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnClickLeft () {
                    _3ds::io::Keyframer &ioKeyframer = static_cast<_3ds::io::Keyframer &> (module.GetIOModule ());
                    _3ds::io::Keyframer::Segment &segment = ioKeyframer.segment;
                    if (segment.currFrame != segment.startFrame) {
                        segment.currFrame = segment.startFrame;
                        _3ds::ext::Keyframer &extKeyframer = static_cast<_3ds::ext::Keyframer &> (module.GetExtModule ());
                        extKeyframer.SetCurrentFrame ();
                        _3ds::opengl::Keyframer &openglKeyframer = static_cast<_3ds::opengl::Keyframer &> (module.GetOpenGLModule ());
                        openglKeyframer.SetCurrentFrame ();
                        core::command::DrawViewLayoutFinalOperation (module).Execute ();
                        core::command::FlipFramebufferFinalOperation ().Execute ();
                    }
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarFirstEventHandler, Keyframer)

            struct ToolBarPrevEventHandler : public core::ToolBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPrevEventHandler)

                core::Module &module;

                explicit ToolBarPrevEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnClickLeft () {
                    _3ds::io::Keyframer &ioKeyframer = static_cast<_3ds::io::Keyframer &> (module.GetIOModule ());
                    _3ds::io::Keyframer::Segment &segment = ioKeyframer.segment;
                    if (segment.currFrame == segment.startFrame) {
                        segment.currFrame = segment.endFrame;
                    }
                    else {
                        --segment.currFrame;
                    }
                    _3ds::ext::Keyframer &extKeyframer = static_cast<_3ds::ext::Keyframer &> (module.GetExtModule ());
                    extKeyframer.SetCurrentFrame ();
                    _3ds::opengl::Keyframer &openglKeyframer = static_cast<_3ds::opengl::Keyframer &> (module.GetOpenGLModule ());
                    openglKeyframer.SetCurrentFrame ();
                    core::command::DrawViewLayoutFinalOperation (module).Execute ();
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPrevEventHandler, Keyframer)

            struct ToolBarNextEventHandler : public core::ToolBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarNextEventHandler)

                core::Module &module;

                explicit ToolBarNextEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnClickLeft () {
                    _3ds::io::Keyframer &ioKeyframer = static_cast<_3ds::io::Keyframer &> (module.GetIOModule ());
                    _3ds::io::Keyframer::Segment &segment = ioKeyframer.segment;
                    if (segment.currFrame == segment.endFrame) {
                        segment.currFrame = segment.startFrame;
                    }
                    else {
                        ++segment.currFrame;
                    }
                    _3ds::ext::Keyframer &extKeyframer = static_cast<_3ds::ext::Keyframer &> (module.GetExtModule ());
                    extKeyframer.SetCurrentFrame ();
                    _3ds::opengl::Keyframer &openglKeyframer = static_cast<_3ds::opengl::Keyframer &> (module.GetOpenGLModule ());
                    openglKeyframer.SetCurrentFrame ();
                    core::command::DrawViewLayoutFinalOperation (module).Execute ();
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarNextEventHandler, Keyframer)

            struct ToolBarPlayEventHandler : public core::ToolBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPlayEventHandler)

                core::Module &module;

                explicit ToolBarPlayEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnClickLeft () {
                    assert (core::UI::Instance ()->viewLayoutWindow != 0);
                    assert (core::UI::Instance ()->frameBarWindow != 0);
                    struct ModalLoop : public core::ModalLoop {
                        core::Module &module;
                        _3ds::io::Keyframer &ioKeyframer;
                        _3ds::io::Keyframer::Segment &segment;
                        bool maximized;
                        bool cursorVisible;
                        explicit ModalLoop (core::Module &module_) : module (module_),
                                                                     ioKeyframer (static_cast<_3ds::io::Keyframer &> (module.GetIOModule ())),
                                                                     segment (ioKeyframer.segment),
                                                                     maximized (ioKeyframer.viewLayout.layout.maximized),
                                                                     cursorVisible (core::CursorMgr::Instance ()->IsCursorVisible ()) {
                            if (!maximized) {
                                _3ds::io::command::ViewLayoutToggleMaximizedCommand (ioKeyframer.viewLayout).Execute ();
                                core::UI::Instance ()->viewLayoutWindow->SetViewLayout (*module.viewLayout);
                            }
                            if (cursorVisible) {
                                core::CursorMgr::Instance ()->ShowCursor (false);
                            }
                            core::UI::Instance ()->frameBarWindow->SetModal (true);
                        }
                        ~ModalLoop () {
                            core::UI::Instance ()->frameBarWindow->SetModal (false);
                            if (cursorVisible) {
                                core::CursorMgr::Instance ()->ShowCursor (true);
                            }
                            if (!maximized) {
                                _3ds::io::command::ViewLayoutToggleMaximizedCommand (ioKeyframer.viewLayout).Execute ();
                                core::UI::Instance ()->viewLayoutWindow->SetViewLayout (*module.viewLayout);
                            }
                        }
                        virtual void DoIteration () {
                            if (segment.currFrame == segment.endFrame) {
                                segment.currFrame = segment.startFrame;
                            }
                            else {
                                ++segment.currFrame;
                            }
                            _3ds::ext::Keyframer &extKeyframer = static_cast<_3ds::ext::Keyframer &> (module.GetExtModule ());
                            extKeyframer.SetCurrentFrame ();
                            _3ds::opengl::Keyframer &openglKeyframer = static_cast<_3ds::opengl::Keyframer &> (module.GetOpenGLModule ());
                            openglKeyframer.SetCurrentFrame ();
                            core::command::DrawViewLayoutFinalOperation (module).Execute ();
                            core::command::FlipFramebufferFinalOperation ().Execute ();
                        }
                    } modalLoop (module);
                    modalLoop.Run ();
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarPlayEventHandler, Keyframer)

            struct ToolBarLastEventHandler : public core::ToolBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarLastEventHandler)

                core::Module &module;

                explicit ToolBarLastEventHandler (core::Module &module_) : module (module_) {}

                virtual void OnClickLeft () {
                    _3ds::io::Keyframer &ioKeyframer = static_cast<_3ds::io::Keyframer &> (module.GetIOModule ());
                    _3ds::io::Keyframer::Segment &segment = ioKeyframer.segment;
                    if (segment.currFrame != segment.endFrame) {
                        segment.currFrame = segment.endFrame;
                        _3ds::ext::Keyframer &extKeyframer = static_cast<_3ds::ext::Keyframer &> (module.GetExtModule ());
                        extKeyframer.SetCurrentFrame ();
                        _3ds::opengl::Keyframer &openglKeyframer = static_cast<_3ds::opengl::Keyframer &> (module.GetOpenGLModule ());
                        openglKeyframer.SetCurrentFrame ();
                        core::command::DrawViewLayoutFinalOperation (module).Execute ();
                        core::command::FlipFramebufferFinalOperation ().Execute ();
                    }
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarLastEventHandler, Keyframer)

        } // namespace keyframer
    } // namespace mex
} // namespace thekogans
