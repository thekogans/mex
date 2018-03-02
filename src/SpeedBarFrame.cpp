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

#include "thekogans/mex/SpeedBarWindow.h"
#include "thekogans/mex/SpeedBarFrame.h"

namespace thekogans {
    namespace mex {

        SpeedBarFrame::SpeedBarFrame (QWidget *parent) :
            //QDockWidget (tr ("Speed Bar"), parent), scrollArea (this) {
            QDockWidget (tr ("Speed Bar"), parent), speedBarWindow (this) {
            setObjectName ("SpeedBarFrame");
            //util::ui8Color color = opengl::DefaultPalette[SpeedBarWindow::defaultPalette[SpeedBarWindow::SB_COLOR_BKGRND]];
            //QPalette palette;
            //palette.setColor (QPalette::Window, QColor (color.r, color.g, color.b));
            //scrollArea.setPalette (palette);
            //scrollArea.setWidget (new SpeedBarWindow (&scrollArea));
            //setWidget (&scrollArea);
            setWidget (&speedBarWindow);
        }

    } // namespace mex
} // namespace thekogans
