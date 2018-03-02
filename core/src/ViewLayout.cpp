// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#include <cassert>
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/3ds/io/command/BlasSimpleTypeCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/ext/ViewLayout.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/command/SetCurrModuleFinalOperation.h"
#include "thekogans/mex/core/command/DrawViewFinalOperation.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"

namespace thekogans {
    namespace mex {
        namespace core {

            void ViewLayout::SetSize (const blas::Size &size) {
                _3ds::io::command::ViewLayoutSetSizeCommand (
                    module.GetIOModule ().viewLayout, size).Execute ();
                command::DrawViewLayoutFinalOperation (module).Execute ();
                UI::Instance ().FlipFramebuffer ();
            }

            void ViewLayout::SetFocus () {
                if (tool != 0) {
                    tool->SetFocus ();
                }
                else {
                    CursorMgr::Instance ().SetCursor (CursorMgr::ARROW_CURSOR);
                }
            }

            void ViewLayout::KillFocus () {
                if (tool != 0) {
                    tool->KillFocus ();
                }
            }

            namespace {
                void SnapPoint (const _3ds::opengl::View &view, blas::Point2 &pt) {
                    if (view.view.IsUseSnap ()) {
                        const blas::Point3 &snapSpacing =
                            GetIOProject ().drawingAids.snapSpacing * view.xform;
                        util::f32 remainder = fmodf (pt.x, snapSpacing.x);
                        pt.x -= remainder;
                        if (remainder > snapSpacing.x * 0.5f) {
                            pt.x += snapSpacing.x;
                        }
                        remainder = fmodf (pt.y, snapSpacing.y);
                        pt.y -= remainder;
                        if (remainder > snapSpacing.y * 0.5f) {
                            pt.y += snapSpacing.y;
                        }
                    }
                }
            }

            void ViewLayout::LButtonDown (util::ui32 flags, const blas::Point &dp) {
                util::ui16 viewIndex;
                if (capturedViewIndex != util::NIDX16) {
                    assert (capturedViewIndex == module.GetIOModule ().viewLayout.layout.currViewIndex);
                    viewIndex = capturedViewIndex;
                }
                else {
                    viewIndex = _3ds::ext::ViewLayout (module.GetIOModule ().viewLayout).ViewFromPoint (dp);
                }
                if (viewIndex != util::NIDX16) {
                    if (viewIndex != module.GetIOModule ().viewLayout.layout.currViewIndex) {
                        core::HideCursor hideCursor;
                        _3ds::opengl::ViewLayout (module.GetIOModule ().viewLayout).DrawViewHighlight (false);
                        module.GetIOModule ().viewLayout.layout.currViewIndex = viewIndex;
                        _3ds::opengl::ViewLayout (module.GetIOModule ().viewLayout).DrawViewHighlight (true);
                    }
                    else if (tool != 0) {
                        _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                        assert (view.get () != 0);
                        if (view.get () != 0) {
                            blas::Point2 pt = view->D2P (dp);
                            SnapPoint (*view, pt);
                            tool->LButtonDown (*view, flags, pt);
                        }
                    }
                }
            }

            void ViewLayout::LButtonDblClk (util::ui32 flags, const blas::Point &dp) {
                if (tool != 0) {
                    _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        blas::Point2 pt = view->D2P (dp);
                        SnapPoint (*view, pt);
                        tool->LButtonDblClk (*view, flags, pt);
                    }
                }
            }

            void ViewLayout::LButtonUp (util::ui32 flags, const blas::Point &dp) {
                if (tool != 0) {
                    _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        blas::Point2 pt = view->D2P (dp);
                        SnapPoint (*view, pt);
                        tool->LButtonUp (*view, flags, pt);
                    }
                }
            }

            void ViewLayout::RButtonDown (util::ui32 flags, const blas::Point &dp) {
                if (tool != 0) {
                    _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        blas::Point2 pt = view->D2P (dp);
                        SnapPoint (*view, pt);
                        tool->RButtonDown (*view, flags, pt);
                    }
                }
            }

            void ViewLayout::RButtonUp (util::ui32 flags, const blas::Point &dp) {
                if (tool != 0) {
                    _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        blas::Point2 pt = view->D2P (dp);
                        SnapPoint (*view, pt);
                        tool->RButtonUp (*view, flags, pt);
                    }
                }
            }

            void ViewLayout::MouseMove (util::ui32 flags, const blas::Point &dp) {
                if (dp.x != lastPt.x || dp.y != lastPt.y) {
                    lastPt = dp;
                    if (tool != 0) {
                        ScrollView (lastPt);
                        _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                        assert (view.get () != 0);
                        if (view.get () != 0) {
                            blas::Point2 pt = view->D2P (lastPt);
                            SnapPoint (*view, pt);
                            tool->MouseMove (*view, flags, pt);
                        }
                    }
                }
            }

            void ViewLayout::KeyDown (util::ui32 key, util::ui32 repeatCount, util::ui32 flags) {
                if (tool != 0) {
                    _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        tool->KeyDown (*view, key, repeatCount, flags);
                    }
                }
            }

            void ViewLayout::KeyUp (util::ui32 key, util::ui32 repeatCount, util::ui32 flags) {
                if (tool != 0) {
                    _3ds::opengl::View::UniquePtr view (module.GetOpenGLModule ().GetCurrView ());
                    assert (view.get () != 0);
                    if (view.get () != 0) {
                        tool->KeyUp (*view, key, repeatCount, flags);
                    }
                }
            }

            void ViewLayout::SetTool (Tool *tool_) {
                if (tool != 0) {
                    tool->KillFocus ();
                }
                tool = tool_;
                if (tool != 0) {
                    tool->SetFocus ();
                }
            }

            void ViewLayout::CaptureView () {
                capturedViewIndex = module.GetIOModule ().viewLayout.layout.currViewIndex;
                assert (UI::Instance ().viewLayoutWindow != 0);
                UI::Instance ().viewLayoutWindow->CaptureMouse ();
            }

            void ViewLayout::ReleaseView () {
                capturedViewIndex = util::NIDX16;
                assert (UI::Instance ().viewLayoutWindow != 0);
                UI::Instance ().viewLayoutWindow->ReleaseMouse ();
            }

            void ViewLayout::ScrollView (blas::Point &dp) {
                // Views can only scroll when captured.
                if (capturedViewIndex != util::NIDX16) {
                    // Views can only be captured by tools.
                    assert (tool != 0);
                    if (tool != 0 && !tool->IsScrollLock ()) {
                        _3ds::io::ViewLayout &viewLayout = module.GetIOModule ().viewLayout;
                        _3ds::io::View &ioView = viewLayout[viewLayout.layout.currViewIndex];
                        const blas::Size &size = ioView.size;
                        if (!dp.InSize (size)) {
                            _3ds::opengl::View::UniquePtr openglView (
                                module.GetOpenGLModule ().GetView (ioView));
                            assert (openglView.get () != 0);
                            if (openglView.get () != 0) {
                                blas::Point3 worldOrigin = ioView.worldOrigin * openglView->xform;
                                if (dp.x < size.x) {
                                    worldOrigin.x -= 0.25f * openglView->viewBound.Extents ().x;
                                    dp.x = size.x + size.width / 4;
                                }
                                else if (dp.x > size.Right ()) {
                                    worldOrigin.x += 0.25f * openglView->viewBound.Extents ().x;
                                    dp.x = size.x + size.width * 3 / 4;
                                }
                                if (dp.y < size.y) {
                                    worldOrigin.y -= 0.25f * openglView->viewBound.Extents ().y;
                                    dp.y = size.y + size.height / 4;
                                }
                                else if (dp.y > size.Top ()) {
                                    worldOrigin.y += 0.25f * openglView->viewBound.Extents ().y;
                                    dp.y = size.y + size.height * 3 / 4;
                                }
                                UI::Instance ().viewLayoutWindow->SetMousePosition (dp);
                                // Since we're doing this on behalf of the active tool, it
                                // had better started a transaction. This assert will help
                                // debug the broken tools.
                                assert (GetCommandDispatcher ().IsTransactionPending (tool->GetName ()));
                                GetCommandDispatcher ().ExecuteAndAddCommand (
                                    thekogans::mex::command::Command::UniquePtr (
                                        new thekogans::mex::command::Point3SetCommand (
                                            ioView.worldOrigin, worldOrigin * openglView->ixform)));
                                GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                                    thekogans::mex::command::FinalOperation::UniquePtr (
                                        new command::SetCurrModuleFinalOperation (Module::currModule)));
                                GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                                    thekogans::mex::command::FinalOperation::UniquePtr (
                                        new command::DrawViewFinalOperation (module, ioView)));
                                GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                                    thekogans::mex::command::FinalOperation::UniquePtr (
                                        new command::FlipFramebufferFinalOperation));
                            }
                        }
                    }
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
