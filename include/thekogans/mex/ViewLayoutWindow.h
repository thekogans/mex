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

#if !defined (__thekogans_mex_ViewLayoutWindow_h)
#define __thekogans_mex_ViewLayoutWindow_h

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtOpenGL/QGLWidget>
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/ViewLayout.h"

namespace thekogans {
    namespace mex {

        struct ViewLayoutWindow :
                public QGLWidget,
                public core::UI::ViewLayoutWindow {
            Q_OBJECT

        private:
            core::ViewLayout *viewLayout;
            bool inViewLayout;

        public:
            ViewLayoutWindow (QWidget *parent = 0);
            ~ViewLayoutWindow ();

            virtual QWidget *GetQWidget () {
                return this;
            }
            virtual void OnIdle ();
            virtual void MakeCurrent () {
                makeCurrent ();
            }
            virtual blas::Size GetSize () const;
            virtual core::ViewLayout *GetViewLayout () const {
                return viewLayout;
            }
            virtual void SetViewLayout (core::ViewLayout &viewLayout);
            virtual void CaptureMouse () {
                grabMouse ();
            }
            virtual void ReleaseMouse () {
                releaseMouse ();
            }
            virtual blas::Point GetMousePosition () const {
                return Win2GL (mapFromGlobal (QCursor::pos ()));
            }
            virtual void SetMousePosition (const blas::Point &pt) const {
                QCursor::setPos (mapToGlobal (QPoint (pt.x, HeightInPixels () - pt.y) / devicePixelRatio ()));
            }

        protected:
            virtual void resizeGL (
                int width,
                int height);
            virtual void paintGL ();
            virtual void mousePressEvent (QMouseEvent *event);
            virtual void mouseReleaseEvent (QMouseEvent *event);
            virtual void mouseDoubleClickEvent (QMouseEvent *event);
            virtual void mouseMoveEvent (QMouseEvent *event);

        public:
            // These are public because they are forwarded from MainWindow.
            virtual void keyPressEvent (QKeyEvent *event);
            virtual void keyReleaseEvent (QKeyEvent *event);

        private:
            bool IsCursorInCurrView () const;
            inline int WidthInPixels () const {
                return width () * devicePixelRatio ();
            }
            inline int HeightInPixels () const {
                return height () * devicePixelRatio ();
            }
            blas::Point Win2GL (const QPoint &pt) const;
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ViewLayoutWindow_h)
