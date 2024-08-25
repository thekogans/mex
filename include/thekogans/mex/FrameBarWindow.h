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

#if !defined (__thekogans_mex_FrameBarWindow_h)
#define __thekogans_mex_FrameBarWindow_h

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtGui/QFontMetrics>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStatusBar>
#include "thekogans/util/Types.h"
#include "thekogans/mex/core/UI.h"

namespace thekogans {
    namespace mex {

        struct StatusBarWindow;

        struct FrameBarWindow : public QWidget, public core::UI::FrameBarWindow {
            Q_OBJECT

        private:
            StatusBarWindow *statusBarWindow;
            bool boxMode;
            bool modal;
            EventSink *eventSink;

        public:
            FrameBarWindow (StatusBarWindow *statusBarWindow_);

            virtual QWidget *GetQWidget () {return this;}
            virtual void Show (bool show);

            virtual void Draw () {
                repaint ();
            }
            virtual void SetEventSink (EventSink *eventSink_) {
                eventSink = eventSink_;
            }
            virtual void SetModal (bool modal_) {
                modal = modal_;
                Show (modal);
            }

            inline bool IsVisible () const {
                return eventSink != 0 && eventSink->IsVisible ();
            }
            inline bool IsModal () const {return modal;}

        private:
            inline util::ui32 GetStartFrame () const {
                return eventSink != 0 ? eventSink->GetStartFrame () : 0;
            }
            inline util::ui32 GetEndFrame () const {
                return eventSink != 0 ? eventSink->GetEndFrame () : 0;
            }
            inline util::ui32 GetCurrFrame () const {
                return eventSink != 0 ? eventSink->GetCurrFrame () : 0;
            }
            inline bool IsBoxMode () const {
                return eventSink != 0 && eventSink->IsBoxMode ();
            }
            inline void SetBoxMode (bool boxMode) {
                if (eventSink != 0) {
                    eventSink->SetBoxMode (boxMode);
                }
            }

            bool GoToFrame (const QPoint &pt);
            inline util::i32 GetInsideWidth () const {
                return width () - QFontMetrics (font ()).width (
                    util::ui32Tostring (GetEndFrame ()).c_str ());
            }

        protected:
            virtual void paintEvent (QPaintEvent *event);
            virtual void mousePressEvent (QMouseEvent *event);
            virtual void mouseReleaseEvent (QMouseEvent *event);
            virtual void mouseMoveEvent (QMouseEvent *event);
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_FrameBarWindow_h)
