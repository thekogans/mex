// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of thekogans_mex.
//
// thekogans_mex is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// thekogans_mex is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with thekogans_mex. If not, see <http://www.gnu.org/licenses/>.

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtGui/QCursor>
#include <QtGui/QResizeEvent>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/Util.h"
#include "thekogans/mex/ViewLayoutWindow.h"

namespace thekogans {
    namespace mex {

        namespace {
            struct InViewLayoutSetter {
                bool &inViewLayout;
                explicit InViewLayoutSetter (bool &inViewLayout_) :
                        inViewLayout (inViewLayout_) {
                    assert (!inViewLayout);
                    inViewLayout = true;
                }
                ~InViewLayoutSetter () {
                    assert (inViewLayout);
                    inViewLayout = false;
                }
            };
        }

        ViewLayoutWindow::ViewLayoutWindow (QWidget *parent) :
                QGLWidget (
                    QGLFormat (
                        QGL::Rgba |
                        QGL::DirectRendering |
                        QGL::SingleBuffer |
                        QGL::NoDepthBuffer |
                        QGL::NoAlphaChannel |
                        QGL::NoAccumBuffer |
                        QGL::NoStencilBuffer |
                        QGL::NoStereoBuffers |
                        QGL::NoOverlay |
                        QGL::NoSampleBuffers),
                    parent),
                viewLayout (0),
                inViewLayout (false) {
            setAutoBufferSwap (false);
            setMouseTracking (true);
            setFocusPolicy (Qt::StrongFocus);
            assert (core::UI::Instance ().viewLayoutWindow == 0);
            core::UI::Instance ().viewLayoutWindow = this;
        }

        ViewLayoutWindow::~ViewLayoutWindow () {
            makeCurrent ();
            core::UI::Instance ().DestroyFramebuffer ();
        }

        void ViewLayoutWindow::OnIdle () {
            if (!isActiveWindow () ||
                    !GetMousePosition ().InSize (
                        blas::Size (0, 0, WidthInPixels (), HeightInPixels ()))) {
                makeCurrent ();
                core::CursorMgr::Instance ().OnMouseMove (blas::Point (-1, -1));
            }
            else if (core::CursorMgr::Instance ().IsCursorVisible ()) {
                if (IsCursorInCurrView ()) {
                    setCursor (*core::CursorMgr::Instance ().GetCurrCursor ());
                }
                else {
                    setCursor (
                        *core::CursorMgr::Instance ().GetCursor (
                            core::CursorMgr::ARROW_CURSOR));
                }
            }
            else {
                setCursor (QCursor (Qt::BlankCursor));
            }
        }

        blas::Size ViewLayoutWindow::GetSize () const {
            return blas::Size (0, 0, WidthInPixels (), HeightInPixels ());
        }

        void ViewLayoutWindow::SetViewLayout (core::ViewLayout &viewLayout_) {
            makeCurrent ();
            if (viewLayout != 0) {
                viewLayout->KillFocus ();
            }
            viewLayout = &viewLayout_;
            if (viewLayout != 0) {
                viewLayout->SetSize (GetSize ());
                viewLayout->SetFocus ();
            }
        }

        void ViewLayoutWindow::resizeGL (
                int width,
                int height) {
            if (width > 0 && height > 0) {
                blas::Size size (0, 0, width, height);
                core::UI::Instance ().CreateFramebuffer (size);
                if (viewLayout != 0) {
                    viewLayout->SetSize (size);
                }
            }
        }

        void ViewLayoutWindow::paintGL () {
            core::UI::Instance ().FlipFramebuffer ();
        }

        void ViewLayoutWindow::mousePressEvent (QMouseEvent *event) {
            event->accept ();
            if (!inViewLayout) {
                InViewLayoutSetter inViewLayoutSetter (inViewLayout);
                if (viewLayout != 0) {
                    makeCurrent ();
                    if (event->button () == Qt::LeftButton) {
                        viewLayout->LButtonDown (GetControlKeyState (event->modifiers ()),
                            Win2GL (event->pos ()));
                    }
                    else if (event->button () == Qt::RightButton) {
                        viewLayout->RButtonDown (GetControlKeyState (event->modifiers ()),
                            Win2GL (event->pos ()));
                    }
                }
            }
        }

        void ViewLayoutWindow::mouseReleaseEvent (QMouseEvent *event) {
            event->accept ();
            if (!inViewLayout) {
                InViewLayoutSetter inViewLayoutSetter (inViewLayout);
                if (viewLayout != 0) {
                    makeCurrent ();
                    if (event->button () == Qt::LeftButton) {
                        viewLayout->LButtonUp (GetControlKeyState (event->modifiers ()),
                            Win2GL (event->pos ()));
                    }
                    else if (event->button () == Qt::RightButton) {
                        viewLayout->RButtonUp (GetControlKeyState (event->modifiers ()),
                            Win2GL (event->pos ()));
                    }
                }
            }
        }

        void ViewLayoutWindow::mouseDoubleClickEvent (QMouseEvent *event) {
            event->accept ();
            if (!inViewLayout) {
                InViewLayoutSetter inViewLayoutSetter (inViewLayout);
                if (viewLayout != 0) {
                    makeCurrent ();
                    if (event->button () == Qt::LeftButton) {
                        viewLayout->LButtonDblClk (GetControlKeyState (event->modifiers ()),
                            Win2GL (event->pos ()));
                    }
                }
            }
        }

        void ViewLayoutWindow::mouseMoveEvent (QMouseEvent *event) {
            event->accept ();
            if (!inViewLayout) {
                InViewLayoutSetter inViewLayoutSetter (inViewLayout);
                makeCurrent ();
                blas::Point pt = Win2GL (event->pos ());
                core::CursorMgr::Instance ().OnMouseMove (pt);
                if (viewLayout != 0) {
                    viewLayout->MouseMove (GetControlKeyState (event->modifiers ()), pt);
                }
            }
        }

        void ViewLayoutWindow::keyPressEvent (QKeyEvent *event) {
            if (!inViewLayout) {
                InViewLayoutSetter inViewLayoutSetter (inViewLayout);
                if (viewLayout != 0 && !viewLayout->module.TranslateAccelerator (
                        tolower (event->key ()), GetControlKeyState (event->modifiers ()))) {
                    makeCurrent ();
                    viewLayout->KeyDown (event->key (), event->count (),
                        GetControlKeyState (event->modifiers ()));
                }
            }
        }

        void ViewLayoutWindow::keyReleaseEvent (QKeyEvent *event) {
            if (!inViewLayout) {
                InViewLayoutSetter inViewLayoutSetter (inViewLayout);
                if (viewLayout != 0) {
                    makeCurrent ();
                    viewLayout->KeyUp (event->key (), event->count (),
                        GetControlKeyState (event->modifiers ()));
                }
            }
        }

        bool ViewLayoutWindow::IsCursorInCurrView () const {
            core::Module *module = core::Module::GetCurrModule ();
            if (module != 0) {
                return GetMousePosition ().InSize (
                    module->GetIOModule ().viewLayout.GetCurrView ().size);
            }
            return false;
        }

        blas::Point ViewLayoutWindow::Win2GL (const QPoint &pt) const {
            return blas::Point (pt.x (), height () - pt.y ()) * devicePixelRatio ();
        }

    } // namespace mex
} // namespace thekogans
