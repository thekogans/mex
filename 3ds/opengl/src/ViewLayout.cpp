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

#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ClearColor.h"
#include "thekogans/mex/opengl/LineWidth.h"
#include "thekogans/mex/opengl/DrawBuffer.h"
#include "thekogans/mex/opengl/Viewport2D.h"
#include "thekogans/mex/opengl/Util.h"
#include "thekogans/mex/3ds/ext/ViewLayout.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/ViewLayout.h"
#include "thekogans/mex/3ds/opengl/Util.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                void ViewLayout::DrawViewHighlight (bool draw) const {
                    Color color (DefaultPalette[draw ? WHITE : BLACK]);
                    {
                        DrawBuffer drawBuffer (GL_BACK);
                        DrawViewHighlightHelper ();
                    }
                    {
                        DrawBuffer drawBuffer (GL_FRONT);
                        DrawViewHighlightHelper ();
                    }
                }

                void ViewLayout::Draw (const Module &module,
                        const std::vector<blas::Point2> &jitterTable) const {
                    switch (viewLayout.layout.currStyle) {
                        case io::ViewLayout::Layout::Single: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::TwoVert: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 2);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::TwoHorz: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 2);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::Four: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 2);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 2);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        y.GetViewBorderStart (1),
                                        x.GetViewBorderSize (1),
                                        y.GetViewBorderSize (1));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        y.GetViewFrameStart (1),
                                        x.GetViewFrameSize (1),
                                        y.GetViewFrameSize (1));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 3:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            module.GetView (viewLayout[3])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeLeft: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 2);
                                const ext::ViewLayout::Dimension yLeft (
                                    viewLayout.size.y, viewLayout.size.height, 2);
                                const ext::ViewLayout::Dimension yRight (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (1),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (1));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (1),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (1));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (0));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeBottom: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension xTop (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                const ext::ViewLayout::Dimension xBottom (
                                    viewLayout.size.x, viewLayout.size.width, 2);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 2);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        xTop.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (1),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (1),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        xTop.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (1),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (1),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 2:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeRight: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 2);
                                const ext::ViewLayout::Dimension yLeft (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                const ext::ViewLayout::Dimension yRight (
                                    viewLayout.size.y, viewLayout.size.height, 2);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (1),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (1));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (1),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (1));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (0));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (1));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeTop: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension xTop (
                                    viewLayout.size.x, viewLayout.size.width, 2);
                                const ext::ViewLayout::Dimension xBottom (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 2);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        xTop.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xTop.GetViewBorderStart (1),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (1),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        xTop.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xTop.GetViewFrameStart (1),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (1),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 2:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeVert: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 3);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (2),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (2),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (2),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (2),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (2),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (2),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeHorz: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 3);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (2),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (2));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (2),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (2));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (2),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (2));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            break;
                        }
                        case io::ViewLayout::Layout::FourLeft: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (1, 3), util::Fraction (2, 3);
                                coverage.push_back (util::Fraction (1, 3));
                                coverage.push_back (util::Fraction (2, 3));
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 2, coverage);
                                const ext::ViewLayout::Dimension yLeft (
                                    viewLayout.size.y, viewLayout.size.height, 3);
                                const ext::ViewLayout::Dimension yRight (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (2),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (2));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (1),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (1));
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (2),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (2));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (1),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (1));
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (2),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (2));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (1));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (0));
                                            break;
                                        case 3:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            module.GetView (viewLayout[3])->Draw (module, jitterTable);
                            break;
                        }
                    #if EXTENDED_VIEW_LAYOUT
                        case io::ViewLayout::Layout::FourBottom: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension xTop (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                const ext::ViewLayout::Dimension xBottom (
                                    viewLayout.size.x, viewLayout.size.width, 3);
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (1, 3), util::Fraction (2, 3);
                                coverage.push_back (util::Fraction (1, 3));
                                coverage.push_back (util::Fraction (2, 3));
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 2, coverage);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        xTop.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (1),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (1),
                                        y.GetViewBorderSize (0));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (2),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (2),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        xTop.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (1),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (1),
                                        y.GetViewFrameSize (0));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (2),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (2),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 2:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (0));
                                            break;
                                        case 3:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (2),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (2),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            module.GetView (viewLayout[3])->Draw (module, jitterTable);
                            break;
                        }
                    #endif // EXTENDED_VIEW_LAYOUT
                        case io::ViewLayout::Layout::FourRight: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (2, 3), util::Fraction (1, 3);
                                coverage.push_back (util::Fraction (2, 3));
                                coverage.push_back (util::Fraction (1, 3));
                                const ext::ViewLayout::Dimension x (
                                    viewLayout.size.x, viewLayout.size.width, 2, coverage);
                                const ext::ViewLayout::Dimension yLeft (
                                    viewLayout.size.y, viewLayout.size.height, 1);
                                const ext::ViewLayout::Dimension yRight (
                                    viewLayout.size.y, viewLayout.size.height, 3);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        x.GetViewBorderStart (0),
                                        yLeft.GetViewBorderStart (0),
                                        x.GetViewBorderSize (0),
                                        yLeft.GetViewBorderSize (0));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (2),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (2));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (1),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (1));
                                    Rectangle (
                                        x.GetViewBorderStart (1),
                                        yRight.GetViewBorderStart (0),
                                        x.GetViewBorderSize (1),
                                        yRight.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        x.GetViewFrameStart (0),
                                        yLeft.GetViewFrameStart (0),
                                        x.GetViewFrameSize (0),
                                        yLeft.GetViewFrameSize (0));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (2),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (2));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (1),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (1));
                                    Rectangle (
                                        x.GetViewFrameStart (1),
                                        yRight.GetViewFrameStart (0),
                                        x.GetViewFrameSize (1),
                                        yRight.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                x.GetViewHighlightStart (0),
                                                yLeft.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (0),
                                                yLeft.GetViewHighlightSize (0));
                                            break;
                                        case 1:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (2),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (2));
                                            break;
                                        case 2:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (1),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (1));
                                            break;
                                        case 3:
                                            Rectangle (
                                                x.GetViewHighlightStart (1),
                                                yRight.GetViewHighlightStart (0),
                                                x.GetViewHighlightSize (1),
                                                yRight.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            module.GetView (viewLayout[3])->Draw (module, jitterTable);
                            break;
                        }
                    #if EXTENDED_VIEW_LAYOUT
                        case io::ViewLayout::Layout::FourTop: {
                            {
                                // Switch to 2D
                                Viewport2D viewport2D (viewLayout.size);
                                // Clear surface
                                {
                                    ClearColor clearColor (DefaultPalette[BLACK]);
                                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                                }
                                const ext::ViewLayout::Dimension xTop (
                                    viewLayout.size.x, viewLayout.size.width, 3);
                                const ext::ViewLayout::Dimension xBottom (
                                    viewLayout.size.x, viewLayout.size.width, 1);
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (2, 3), util::Fraction (1, 3);
                                coverage.push_back (util::Fraction (2, 3));
                                coverage.push_back (util::Fraction (1, 3));
                                const ext::ViewLayout::Dimension y (
                                    viewLayout.size.y, viewLayout.size.height, 2, coverage);
                                // Draw view borders.
                                {
                                    Color color (DefaultPalette[BLACK]);
                                    LineWidth lineWidth (3.0f);
                                    Rectangle (
                                        xTop.GetViewBorderStart (0),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (0),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xTop.GetViewBorderStart (1),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (1),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xTop.GetViewBorderStart (2),
                                        y.GetViewBorderStart (1),
                                        xTop.GetViewBorderSize (2),
                                        y.GetViewBorderSize (1));
                                    Rectangle (
                                        xBottom.GetViewBorderStart (0),
                                        y.GetViewBorderStart (0),
                                        xBottom.GetViewBorderSize (0),
                                        y.GetViewBorderSize (0));
                                }
                                // Draw view frames.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    Rectangle (
                                        xTop.GetViewFrameStart (0),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (0),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xTop.GetViewFrameStart (1),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (1),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xTop.GetViewFrameStart (2),
                                        y.GetViewFrameStart (1),
                                        xTop.GetViewFrameSize (2),
                                        y.GetViewFrameSize (1));
                                    Rectangle (
                                        xBottom.GetViewFrameStart (0),
                                        y.GetViewFrameStart (0),
                                        xBottom.GetViewFrameSize (0),
                                        y.GetViewFrameSize (0));
                                }
                                // Highlight current view.
                                {
                                    Color color (DefaultPalette[LIGHT_GRAY]);
                                    switch (viewLayout.layout.currViewIndex) {
                                        case 0:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 1:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (1),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (1),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 2:
                                            Rectangle (
                                                xTop.GetViewHighlightStart (2),
                                                y.GetViewHighlightStart (1),
                                                xTop.GetViewHighlightSize (2),
                                                y.GetViewHighlightSize (1));
                                            break;
                                        case 3:
                                            Rectangle (
                                                xBottom.GetViewHighlightStart (0),
                                                y.GetViewHighlightStart (0),
                                                xBottom.GetViewHighlightSize (0),
                                                y.GetViewHighlightSize (0));
                                            break;
                                    }
                                }
                            }
                            // Draw views.
                            module.GetView (viewLayout[0])->Draw (module, jitterTable);
                            module.GetView (viewLayout[1])->Draw (module, jitterTable);
                            module.GetView (viewLayout[2])->Draw (module, jitterTable);
                            module.GetView (viewLayout[3])->Draw (module, jitterTable);
                            break;
                        }
                    #endif // EXTENDED_VIEW_LAYOUT
                        default:
                            // FIXME: throw something appropriate
                            assert (0);
                            break;
                    }
                }

                void ViewLayout::DrawViewHighlightHelper () const {
                    // Switch to 2D
                    Viewport2D viewport2D (viewLayout.size);
                    switch (viewLayout.layout.currStyle) {
                        case io::ViewLayout::Layout::Single: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            Rectangle (
                                x.GetViewHighlightStart (0),
                                y.GetViewHighlightStart (0),
                                x.GetViewHighlightSize (0),
                                y.GetViewHighlightSize (0));
                            break;
                        }
                        case io::ViewLayout::Layout::TwoVert: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 2);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::TwoHorz: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 2);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::Four: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 2);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 2);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 3:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeLeft: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 2);
                            const ext::ViewLayout::Dimension yLeft (
                                viewLayout.size.y, viewLayout.size.height, 2);
                            const ext::ViewLayout::Dimension yRight (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (0));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeBottom: {
                            const ext::ViewLayout::Dimension xTop (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            const ext::ViewLayout::Dimension xBottom (
                                viewLayout.size.x, viewLayout.size.width, 2);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 2);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 2:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeRight: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 2);
                            const ext::ViewLayout::Dimension yLeft (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            const ext::ViewLayout::Dimension yRight (
                                viewLayout.size.y, viewLayout.size.height, 2);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (0));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (1));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeTop: {
                            const ext::ViewLayout::Dimension xTop (
                                viewLayout.size.x, viewLayout.size.width, 2);
                            const ext::ViewLayout::Dimension xBottom (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 2);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 2:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeVert: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 3);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (2),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (2),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::ThreeHorz: {
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 3);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (2),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (2));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                        case io::ViewLayout::Layout::FourLeft: {
                            ext::ViewLayout::Dimension::Coverage coverage;
                            //coverage += util::Fraction (1, 3), util::Fraction (2, 3);
                            coverage.push_back (util::Fraction (1, 3));
                            coverage.push_back (util::Fraction (2, 3));
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 2, coverage);
                            const ext::ViewLayout::Dimension yLeft (
                                viewLayout.size.y, viewLayout.size.height, 3);
                            const ext::ViewLayout::Dimension yRight (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (2),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (2));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (1));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (0));
                                    break;
                                case 3:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                    #if EXTENDED_VIEW_LAYOUT
                        case io::ViewLayout::Layout::FourBottom: {
                            const ext::ViewLayout::Dimension xTop (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            const ext::ViewLayout::Dimension xBottom (
                                viewLayout.size.x, viewLayout.size.width, 3);
                            ext::ViewLayout::Dimension::Coverage coverage;
                            //coverage += util::Fraction (1, 3), util::Fraction (2, 3);
                            coverage.push_back (util::Fraction (1, 3));
                            coverage.push_back (util::Fraction (2, 3));
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 2, coverage);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 2:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (0));
                                    break;
                                case 3:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (2),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (2),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                    #endif // EXTENDED_VIEW_LAYOUT
                        case io::ViewLayout::Layout::FourRight: {
                            ext::ViewLayout::Dimension::Coverage coverage;
                            //coverage += util::Fraction (2, 3), util::Fraction (1, 3);
                            coverage.push_back (util::Fraction (2, 3));
                            coverage.push_back (util::Fraction (1, 3));
                            const ext::ViewLayout::Dimension x (
                                viewLayout.size.x, viewLayout.size.width, 2, coverage);
                            const ext::ViewLayout::Dimension yLeft (
                                viewLayout.size.y, viewLayout.size.height, 1);
                            const ext::ViewLayout::Dimension yRight (
                                viewLayout.size.y, viewLayout.size.height, 3);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        x.GetViewHighlightStart (0),
                                        yLeft.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (0),
                                        yLeft.GetViewHighlightSize (0));
                                    break;
                                case 1:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (2),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (2));
                                    break;
                                case 2:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (1),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (1));
                                    break;
                                case 3:
                                    Rectangle (
                                        x.GetViewHighlightStart (1),
                                        yRight.GetViewHighlightStart (0),
                                        x.GetViewHighlightSize (1),
                                        yRight.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                    #if EXTENDED_VIEW_LAYOUT
                        case io::ViewLayout::Layout::FourTop: {
                            const ext::ViewLayout::Dimension xTop (
                                viewLayout.size.x, viewLayout.size.width, 3);
                            const ext::ViewLayout::Dimension xBottom (
                                viewLayout.size.x, viewLayout.size.width, 1);
                            ext::ViewLayout::Dimension::Coverage coverage;
                            //coverage += util::Fraction (2, 3), util::Fraction (1, 3);
                            coverage.push_back (util::Fraction (2, 3));
                            coverage.push_back (util::Fraction (1, 3));
                            const ext::ViewLayout::Dimension y (
                                viewLayout.size.y, viewLayout.size.height, 2, coverage);
                            switch (viewLayout.layout.currViewIndex) {
                                case 0:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 1:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (1),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (1),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 2:
                                    Rectangle (
                                        xTop.GetViewHighlightStart (2),
                                        y.GetViewHighlightStart (1),
                                        xTop.GetViewHighlightSize (2),
                                        y.GetViewHighlightSize (1));
                                    break;
                                case 3:
                                    Rectangle (
                                        xBottom.GetViewHighlightStart (0),
                                        y.GetViewHighlightStart (0),
                                        xBottom.GetViewHighlightSize (0),
                                        y.GetViewHighlightSize (0));
                                    break;
                            }
                            break;
                        }
                    #endif // EXTENDED_VIEW_LAYOUT
                        default:
                            // FIXME: throw something appropriate
                            assert (0);
                            break;
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
