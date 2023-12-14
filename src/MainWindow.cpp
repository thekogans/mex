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

#include <string>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>
#include "thekogans/util/Path.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/Util.h"
#include "thekogans/mex/MainWindow.h"

namespace thekogans {
    namespace mex {

        MainWindow::MainWindow () :
                menuBarWindow (this),
                toolBarWindow (this),
                viewLayoutWindow (this),
                speedBarFrame (this),
                consoleFrame (this),
                statusBarWindow (this) {
            setContextMenuPolicy (Qt::NoContextMenu);
            setMenuBar (&menuBarWindow);
            addToolBar (Qt::TopToolBarArea, &toolBarWindow);
            setCentralWidget (&viewLayoutWindow);
            speedBarFrame.setAllowedAreas (Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
            addDockWidget (Qt::RightDockWidgetArea, &speedBarFrame);
            consoleFrame.setAllowedAreas (Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
            addDockWidget (Qt::BottomDockWidgetArea, &consoleFrame);
            setStatusBar (&statusBarWindow);
            // Each one of the above created UI components
            // initializes its coresponding member in core::UI.
            // This is just a sanity check.
            assert (core::UI::Instance ().menuBarWindow != 0);
            assert (core::UI::Instance ().toolBarWindow != 0);
            assert (core::UI::Instance ().viewLayoutWindow != 0);
            assert (core::UI::Instance ().speedBarWindow != 0);
            assert (core::UI::Instance ().consoleWindow != 0);
            assert (core::UI::Instance ().statusBarWindow != 0);
            assert (core::UI::Instance ().frameBarWindow != 0);
            Restore ("MainWindow");
            core::UI::Instance ().menuBarWindow->Restore ("MenuBarWindow");
            core::UI::Instance ().toolBarWindow->Restore ("ToolBarWindow");
            core::UI::Instance ().viewLayoutWindow->Restore ("ViewLayoutWindow");
            core::UI::Instance ().speedBarWindow->Restore ("SpeedBarWindow");
            core::UI::Instance ().consoleWindow->Restore ("ConsoleWindow");
            core::UI::Instance ().statusBarWindow->Restore ("StatusBarWindow");
            core::UI::Instance ().frameBarWindow->Restore ("FrameBarWindow");
            assert (core::UI::Instance ().mainFrameWindow == 0);
            core::UI::Instance ().mainFrameWindow = this;
            // Idle processing timer.
            startTimer (0);
            // Accelerator filter.
            QCoreApplication::instance ()->installEventFilter (this);
        }

        void MainWindow::Save (const char *keyName) {
            QSettings settings ("thekogans.net", "mex");
            settings.beginGroup (keyName);
            settings.setValue ("pos", pos ());
            settings.setValue ("size", size ());
            settings.setValue ("state", saveState ());
            settings.endGroup ();
        }

        void MainWindow::Restore (const char *keyName) {
            QSettings settings ("thekogans.net", "mex");
            settings.beginGroup (keyName);
            move (settings.value ("pos", QSize (50, 50)).toPoint ());
            resize (settings.value ("size", QSize (1024, 768)).toSize ());
            restoreState (settings.value ("state").toByteArray ());
            settings.endGroup ();
        }

        void MainWindow::closeEvent (QCloseEvent *event) {
            bool dirty = false;
            std::string message = "Unsaved changes exist in the following modules:\n\n";
            for (std::size_t i = 0, count = core::Module::modules.size (); i < count; ++i) {
                if (core::GetCommandDispatcher ().CanUndo ()) {
                    message += core::Module::modules[i].second->GetName ();
                    message += "\n";
                    dirty = true;
                }
            }
            if (dirty) {
                message += "\nAre you sure you want to quit?";
                core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                if (QMessageBox::warning (this, "MEX", message.c_str (),
                        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
                    event->ignore ();
                    return;
                }
            }
            core::UI::Instance ().menuBarWindow->Save ("MenuBarWindow");
            core::UI::Instance ().toolBarWindow->Save ("ToolBarWindow");
            core::UI::Instance ().viewLayoutWindow->Save ("ViewLayoutWindow");
            core::UI::Instance ().speedBarWindow->Save ("SpeedBarWindow");
            core::UI::Instance ().consoleWindow->Save ("ConsoleWindow");
            core::UI::Instance ().statusBarWindow->Save ("StatusBarWindow");
            core::UI::Instance ().frameBarWindow->Save ("FrameBarWindow");
            Save ("MainWindow");
            event->accept ();
        }

        void MainWindow::timerEvent (QTimerEvent *event) {
            event->accept ();
            // Idle processing is done in steps to
            // allow the ui to be as responsive as
            // posible.
            enum {
                SET_TITLE,
                MENUBAR,
                TOOLBAR,
                VIEWLAYOUT,
                SPEEDBAR,
                CONSOLE,
                STATUSBAR,
                LAST_IDLE_JOB
            };
            static util::ui32 step = SET_TITLE;
            switch (step) {
                case SET_TITLE: {
                    core::Module *module = core::Module::GetCurrModule ();
                    if (module != 0) {
                        std::string path = util::MakePath (
                            module->GetIOModule ().openFileDirectory, module->GetIOModule ().openFileName);
                        if (path.empty ()) {
                            path = "Untitled";
                        }
                        if (core::GetCommandDispatcher ().CanUndo ()) {
                            path += " *";
                        }
                        setWindowTitle (("MEX - [" + path + "]").c_str ());
                    }
                    break;
                }
                case MENUBAR:
                    core::UI::Instance ().menuBarWindow->OnIdle ();
                    break;
                case TOOLBAR:
                    core::UI::Instance ().toolBarWindow->OnIdle ();
                    break;
                case VIEWLAYOUT:
                    core::UI::Instance ().viewLayoutWindow->OnIdle ();
                    break;
                case SPEEDBAR:
                    core::UI::Instance ().speedBarWindow->OnIdle ();
                    break;
                case CONSOLE:
                    core::UI::Instance ().consoleWindow->OnIdle ();
                    break;
                case STATUSBAR:
                    core::UI::Instance ().statusBarWindow->OnIdle ();
                    break;
            }
            if (++step == LAST_IDLE_JOB) {
                step = SET_TITLE;
            }
        }

    } // namespace mex
} // namespace thekogans
