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

#include "thekogans/mex/opengl/BindFramebuffer.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Scissor.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/shaper/BezierPolygon.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/shaper/command/ErasePolygonCommand.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ErasePolygonCommand)

            bool ErasePolygonCommand::Undo () {
                if (view.IsShape () && view.IsEnabled ()) {
                    _3ds::opengl::View::UniquePtr openglView =
                        core::GetOpenGLProject ().shaper.GetView (view);
                    assert (openglView.get () != 0);
                    if (openglView.get () != 0) {
                        core::UI::Instance ().viewLayoutWindow->MakeCurrent ();
                        opengl::BindFramebuffer bindFramebuffer (
                            GL_FRAMEBUFFER_EXT, core::UI::Instance ().framebuffer->id);
                        opengl::Enable scissorTest (GL_SCISSOR_TEST, true);
                        opengl::Scissor scissor (view.size);
                        // FIXME: jitter?
                        _3ds::opengl::SetupView setupView (*openglView);
                        util::ui32 flags = _3ds::opengl::BezierPolygon2::ShowVertices;
                        if (core::GetIOProject ().shaper.showFirst) {
                            flags |= _3ds::opengl::BezierPolygon2::ShowFirst;
                        }
                        _3ds::opengl::BezierPolygon2 (bezierPolygon, flags, core::GetIOProject ().shaper.steps,
                            _3ds::io::Lofter::ModelParams::High, _3ds::io::BezierPolygon2::Vertex::GetSelectionSet (
                                core::GetIOProject ().shaper.selectMask)).Draw (*openglView);
                    }
                }
                return true;
            }

            bool ErasePolygonCommand::Redo () {
                if (view.IsShape () && view.IsEnabled ()) {
                    _3ds::opengl::View::UniquePtr openglView =
                        core::GetOpenGLProject ().shaper.GetView (view);
                    assert (openglView.get () != 0);
                    if (openglView.get () != 0) {
                        core::UI::Instance ().viewLayoutWindow->MakeCurrent ();
                        opengl::BindFramebuffer bindFramebuffer (
                            GL_FRAMEBUFFER_EXT, core::UI::Instance ().framebuffer->id);
                        opengl::Enable scissorTest (GL_SCISSOR_TEST, true);
                        opengl::Scissor scissor (view.size);
                        // FIXME: jitter?
                        _3ds::opengl::SetupView setupView (*openglView);
                        _3ds::opengl::BezierPolygon2 (bezierPolygon, _3ds::opengl::BezierPolygon2::ShowVertices,
                            core::GetIOProject ().shaper.steps, _3ds::io::Lofter::ModelParams::High,
                            _3ds::io::BezierPolygon2::Vertex::GetSelectionSet (
                                core::GetIOProject ().shaper.selectMask)).DrawWithColor (
                                    *openglView, _3ds::opengl::DefaultPalette[_3ds::opengl::MEDIUM_GRAY]);
                    }
                }
                return true;
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
