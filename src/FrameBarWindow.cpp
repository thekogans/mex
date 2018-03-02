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
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include "thekogans/util/Constants.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/StatusBarWindow.h"
#include "thekogans/mex/FrameBarWindow.h"

namespace thekogans {
    namespace mex {

        FrameBarWindow::FrameBarWindow (StatusBarWindow *statusBarWindow_) :
            QWidget (statusBarWindow_), statusBarWindow (statusBarWindow_),
            boxMode (false), modal (false), eventSink (0) {
            assert (core::UI::Instance ().frameBarWindow == 0);
            core::UI::Instance ().frameBarWindow = this;
        }

        void FrameBarWindow::Show (bool show) {
            if (show) {
                statusBarWindow->clearMessage ();
                for (std::size_t i = 0, count = statusBarWindow->statusItems.size (); i < count; ++i) {
                    statusBarWindow->removeWidget (statusBarWindow->statusItems[i].second);
                }
                statusBarWindow->addWidget (this, 1);
                setVisible (true);
                statusBarWindow->repaint ();
            }
            else {
                statusBarWindow->removeWidget (this);
                for (std::size_t i = 0, count = statusBarWindow->statusItems.size (); i < count; ++i) {
                    statusBarWindow->addPermanentWidget (statusBarWindow->statusItems[i].second);
                    statusBarWindow->statusItems[i].second->show ();
                }
            }
        }

        bool FrameBarWindow::GoToFrame (const QPoint &pt) {
            util::i32 insideWidth = GetInsideWidth ();
            util::ui32 frame = util::LERP (
                (util::f32)util::CLAMP (pt.x (), 0, insideWidth) / insideWidth,
                GetStartFrame (), GetEndFrame ());
            if (frame != GetCurrFrame ()) {
                update ();
                if (eventSink != 0) {
                    eventSink->GoToFrame (frame);
                }
                return true;
            }
            return false;
        }

        void FrameBarWindow::paintEvent (QPaintEvent *event) {
            QPainter painter (this);
            painter.setPen (QColor (255, 255, 255));
            painter.setBackground (QColor (128, 128, 128));
            painter.setBackgroundMode (Qt::OpaqueMode);
            painter.drawText (
                GetInsideWidth () * (util::f32)GetCurrFrame () / (util::f32)GetEndFrame (),
                (height () + QFontMetrics (font ()).height ()) * 0.5f, util::ui32Tostring (GetCurrFrame ()).c_str ());
            event->accept ();
        }

        void FrameBarWindow::mousePressEvent (QMouseEvent *event) {
            if (event->button () == Qt::LeftButton && !modal) {
                grabMouse ();
                modal = true;
                boxMode = IsBoxMode ();
                SetBoxMode (true);
                if (!GoToFrame (event->pos ())) {
                    core::command::DrawViewLayoutFinalOperation (*core::Module::GetCurrModule ()).Execute ();
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
                event->accept ();
            }
        }

        void FrameBarWindow::mouseReleaseEvent (QMouseEvent *event) {
            if (event->button () == Qt::LeftButton && modal) {
                releaseMouse ();
                modal = false;
                if (!boxMode) {
                    SetBoxMode (false);
                    core::command::DrawViewLayoutFinalOperation (*core::Module::GetCurrModule ()).Execute ();
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
                event->accept ();
            }
        }

        void FrameBarWindow::mouseMoveEvent (QMouseEvent *event) {
            if (modal) {
                GoToFrame (event->pos ());
                event->accept ();
            }
        }

    } // namespace mex
} // namespace thekogans
