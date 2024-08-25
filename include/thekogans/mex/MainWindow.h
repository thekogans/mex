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

#if !defined (__thekogans_mex_MainWindow_h)
#define __thekogans_mex_MainWindow_h

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/MenuBarWindow.h"
#include "thekogans/mex/ToolBarWindow.h"
#include "thekogans/mex/ViewLayoutWindow.h"
#include "thekogans/mex/SpeedBarFrame.h"
#include "thekogans/mex/ConsoleFrame.h"
#include "thekogans/mex/StatusBarWindow.h"

namespace thekogans {
    namespace mex {

        struct MainWindow : public QMainWindow, public core::UI::MainFrameWindow {
            Q_OBJECT

        private:
            MenuBarWindow menuBarWindow;
            ToolBarWindow toolBarWindow;
            ViewLayoutWindow viewLayoutWindow;
            SpeedBarFrame speedBarFrame;
            ConsoleFrame consoleFrame;
            StatusBarWindow statusBarWindow;
            QTimer *idleTimer;

        public:
            MainWindow ();

            virtual QWidget *GetQWidget () {
                return this;
            }
            virtual void Save (const char *keyName);
            virtual void Restore (const char *keyName);
            virtual bool IsMenuBarWindowVisible () const {
                return menuBarWindow.isVisible ();
            }
            virtual void ShowMenuBarWindow (bool show) {
                menuBarWindow.setVisible (show);
            }
            virtual bool IsToolBarWindowVisible () const {
                return toolBarWindow.isVisible ();
            }
            virtual void ShowToolBarWindow (bool show) {
                toolBarWindow.setVisible (show);
            }
            virtual bool IsSpeedBarWindowVisible () const {
                return speedBarFrame.isVisible ();
            }
            virtual void ShowSpeedBarWindow (bool show) {
                speedBarFrame.setVisible (show);
            }
            virtual bool IsConsoleWindowVisible () const {
                return consoleFrame.isVisible ();
            }
            virtual void ShowConsoleWindow (bool show) {
                consoleFrame.setVisible (show);
            }
            virtual bool IsStatusBarWindowVisible () const {
                return statusBarWindow.isVisible ();
            }
            virtual void ShowStatusBarWindow (bool show) {
                statusBarWindow.setVisible (show);
            }

        protected:
            virtual void closeEvent (QCloseEvent *event);

        private slots:
            void timerEvent (QTimerEvent *event);
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_MainWindow_h)
