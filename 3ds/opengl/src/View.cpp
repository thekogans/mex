// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/opengl/Util.h"
#include "thekogans/mex/opengl/Font.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Scissor.h"
#include "thekogans/mex/opengl/ClearColor.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/Viewport2D.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Module.h"
#include "thekogans/mex/3ds/opengl/View.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (View)

                void View::Draw (const Module &module, const std::vector<blas::Point2> &jitterTable) const {
                    Enable scissorTest (GL_SCISSOR_TEST, true);
                    Scissor scissor (view.size);
                    if (!jitterTable.empty ()) {
                        glClear (GL_ACCUM_BUFFER_BIT);
                        for (std::size_t i = 0, count = jitterTable.size (); i < count; ++i) {
                            DrawHelper (module, jitterTable[i]);
                            glAccum (GL_ACCUM, 1.0f / count);
                        }
                        glAccum (GL_RETURN, 1.0f);
                    }
                    else {
                        DrawHelper (module);
                    }
                }

                void View::DrawHelper (const Module &module, const blas::Point2 &jitter) const {
                    {
                        // Switch to 2D
                        Viewport2D viewport2D (view.size);
                        // Clear surface.
                        ClearColor clearColor (DefaultPalette[MEDIUM_GRAY]);
                        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        // Draw view name.
                        Color color (DefaultPalette[WHITE]);
                        // Font font (FontMgr::Instance ().GetSystemFont ());
                        // assert (font.font != 0);
                        // font.DrawText (view.size.x + 1, view.size.Top () - font.font->GetHeight () - 1,
                        //     GetName (view), view.size);
                    }
                    if (!view.IsNone () && view.IsEnabled ()) {
                        // Setup model-view, projection, and viewport.
                        SetupView setupView (*this, jitter);
                        if (view.IsUseGrid ()) {
                            // FIXME: implement
                            //grid.Draw ();
                        }
                        if (view.IsSafeFrame ()) {
                            // FIXME: implement
                        }
                        if (view.IsBackground ()) {
                            // FIXME: implement
                        }
                        module.Draw (*this);
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
