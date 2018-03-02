// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include "thekogans/util/Path.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/shaper/ui/TextDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            void ChooseFontDialog (QWidget *parent) {
                std::string filters = "TrueType Font Files (*.TTF)";
                filters += ";;";
                filters += "Postscript Type 1 Font Files (*.PFB)";
                QString fileName = QFileDialog::getOpenFileName (parent, "Open File",
                    core::GetIOProject ().shaper.create.fontsPath.c_str (), filters.c_str ());
                if (!fileName.isEmpty ()) {
                    std::string filePath = fileName.toStdString ();
                    util::Path path (filePath);
                    core::GetIOProject ().shaper.create.fontsPath = path.GetDirectory ();
                    core::GetIOProject ().shaper.create.fontFile = path.GetFullFileName ();
                }
            }

            TextDialog::TextDialog (QWidget *parent) : QDialog (parent) {
                setupUi (this);
                textLineEdit->setText (core::GetIOProject ().shaper.create.text.c_str ());
                connect (fontPushButton, SIGNAL (clicked (bool)), this, SLOT (OnFont (bool)));
            }

            void TextDialog::OnFont (bool) {
                ChooseFontDialog (this);
            }

            void TextDialog::accept () {
                core::GetIOProject ().shaper.create.text = textLineEdit->text ().toStdString ();
                QDialog::accept ();
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
