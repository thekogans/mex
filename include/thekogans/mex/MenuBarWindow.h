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

#if !defined (__thekogans_mex_MenuBarWindow_h)
#define __thekogans_mex_MenuBarWindow_h

#include <memory>
#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/core/MenuBar.h"
#include "thekogans/mex/core/UI.h"

namespace thekogans {
    namespace mex {

        struct MenuItem : public QAction {
            typedef std::unique_ptr<MenuItem> UniquePtr;

            Q_OBJECT

        private:
            core::MenuBar::Item *item;

        public:
            MenuItem (QWidget *parent, core::MenuBar::Item *item_);

            void OnUpdateUI ();

        private slots:
            void trigger ();
        };

        struct MenuBarWindow : public QMenuBar, public core::UI::MenuBarWindow {
            Q_OBJECT

        private:
            core::MenuBar::Item modules;
            std::vector<MenuItem *> menuItems;

        public:
            MenuBarWindow (QWidget *parent);

            virtual QWidget *GetQWidget () {return this;}
            virtual void OnIdle ();
            virtual void SetMenuBar (core::MenuBar &menuBar);

        private:
            void AddMenuItem (QMenu *parent, core::MenuBar::Item *item);
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_MenuBarWindow_h)
