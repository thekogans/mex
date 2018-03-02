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

#if !defined (__thekogans_mex_StatusBarWindow_h)
#define __thekogans_mex_StatusBarWindow_h

#include <vector>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/core/StatusBar.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/FrameBarWindow.h"

namespace thekogans {
    namespace mex {

        struct StatusBarWindow : public QStatusBar, public core::UI::StatusBarWindow {
            Q_OBJECT

        private:
            typedef std::pair<core::StatusBar::Item *, QLabel *> StatusItem;
            std::vector<StatusItem> statusItems;
            FrameBarWindow frameBarWindow;

            friend struct FrameBarWindow;

        public:
            StatusBarWindow (QWidget *parent);

            virtual QWidget *GetQWidget () {return this;}
            virtual void OnIdle ();
            virtual void SetStatusBar (core::StatusBar &statusBar);
            virtual _3ds::io::Progress::UniquePtr CreateProgress ();

        protected:
            virtual void mouseMoveEvent (QMouseEvent *event);
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_StatusBarWindow_h)
