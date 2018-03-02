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
#include "thekogans/mex/opengl/BlitFramebuffer.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"

namespace thekogans {
    namespace mex {
        namespace core {

            void UI::CreateFramebuffer (const blas::Size &size) {
                framebuffer.reset (new opengl::Framebuffer);
                assert (framebuffer.get () != 0);
                framebuffer->AttachRenderbuffer (GL_COLOR_ATTACHMENT0_EXT,
                    opengl::Renderbuffer::UniquePtr (
                        new opengl::Renderbuffer (GL_RGBA, size)));
                assert (framebuffer->IsValid ());
            }

            void UI::DestroyFramebuffer () {
                framebuffer.reset (0);
                assert (framebuffer.get () == 0);
            }

            void UI::FlipFramebuffer () {
                if (framebuffer.get () != 0) {
                    assert (viewLayoutWindow != 0);
                    viewLayoutWindow->MakeCurrent ();
                    blas::Extents extents = framebuffer->GetExtents ();
                    if (extents != blas::Extents::Empty) {
                        HideCursor hideCursor;
                        opengl::BlitFramebuffer blitFramebuffer (framebuffer->id);
                        if (blitFramebuffer.Blit (blas::Size (blas::Point::Zero, extents))) {
                            // Guard against framebuffer being flipped in
                            // the middle of user/tool interaction (this
                            // took a long time to debug under OS X Cocoa).
                            ViewLayout *viewLayout = viewLayoutWindow->GetViewLayout ();
                            if (viewLayout != 0 && viewLayout->tool != 0) {
                                viewLayout->tool->UpdateUI (
                                    *viewLayout->module.GetOpenGLModule ().GetCurrView ());
                            }
                        }
                    }
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
