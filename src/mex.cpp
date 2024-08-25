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

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>
#include "thekogans/util/Exception.h"
#include "thekogans/util/LoggerMgr.h"
#include "thekogans/util/ConsoleLogger.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/MainWindow.h"

using namespace thekogans;
using namespace thekogans::mex;

int main (int argc, char *argv[]) {
    QApplication application (argc, argv);
    Q_INIT_RESOURCE (mex);
#if defined (THEKOGANS_MEX_CONFIG_Release)
    std::unique_ptr<QSplashScreen> splashScreen (new QSplashScreen);
    assert (splashScreen.get () != 0);
    splashScreen->setPixmap (QPixmap (":/images/splash.jpg"));
    splashScreen->show ();
    splashScreen->showMessage ("Setting up the UI...",
        Qt::AlignBottom | Qt::AlignLeft, Qt::white);
#endif // defined (THEKOGANS_MEX_CONFIG_Release)
    MainWindow mainWindow;
#if defined (THEKOGANS_MEX_CONFIG_Release)
    splashScreen->showMessage ("Loading modules...",
        Qt::AlignBottom | Qt::AlignLeft, Qt::white);
#endif // defined (THEKOGANS_MEX_CONFIG_Release)
    // The environment is now set. Time to load the modules.
    core::Module::LoadModules ("modules.xml");
    core::Module *module = core::Module::GetCurrModule ();
    if (module != 0) {
        module->SetFocus ();
    }
    mainWindow.show ();
#if defined (THEKOGANS_MEX_CONFIG_Release)
    splashScreen->finish (&mainWindow);
    splashScreen.reset ();
#endif // defined (THEKOGANS_MEX_CONFIG_Release)
    THEKOGANS_UTIL_LOG_INIT (
        util::LoggerMgr::Debug,
        util::LoggerMgr::All);
    THEKOGANS_UTIL_LOG_ADD_LOGGER (util::Logger::SharedPtr (new util::ConsoleLogger));
    THEKOGANS_UTIL_IMPLEMENT_LOG_FLUSHER;
    THEKOGANS_UTIL_TRY {
        application.exec ();
    }
    THEKOGANS_UTIL_CATCH_AND_LOG
    return 0;
}
