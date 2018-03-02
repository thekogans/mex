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

#if !defined (__thekogans_mex_ToolBarWindow_h)
#define __thekogans_mex_ToolBarWindow_h

#include <vector>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QToolButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QAction>
#include "thekogans/mex/3ds/opengl/Image.h"
#include "thekogans/mex/core/ToolBar.h"
#include "thekogans/mex/core/UI.h"

namespace thekogans {
    namespace mex {

        struct ToolButton : public QToolButton {
            Q_OBJECT

        private:
            core::ToolBar::Item *item;

        public:
            ToolButton (QToolBar *parent, core::ToolBar::Item *item_);

            void OnUpdateUI ();
            void SetImage (_3ds::opengl::Image &image);

        protected:
            virtual void mousePressEvent (QMouseEvent *event);
            virtual void mouseReleaseEvent (QMouseEvent *event);
        };

        struct ToolBarWindow : public QToolBar, public core::UI::ToolBarWindow {
            Q_OBJECT

        private:
            std::vector<ToolButton *> toolButtons;

        public:
            ToolBarWindow (QWidget *parent);

            virtual QWidget *GetQWidget () {return this;}
            virtual void OnIdle ();
            virtual void SetToolBar (core::ToolBar &toolBar);
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_ToolBarWindow_h)
