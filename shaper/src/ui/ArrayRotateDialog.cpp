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

#include <cassert>
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
#include "thekogans/util/Types.h"
#include "thekogans/util/StringUtils.h"
#include "thekogans/mex/shaper/ui/ArrayRotateDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            ArrayRotateDialog::ArrayRotateDialog (
                    util::ui32 &objectsInArray_,
                    bool &rotate_,
                    QWidget *parent) :
                    QDialog (parent),
                    objectsInArray (objectsInArray_),
                    rotate (rotate_) {
                setupUi (this);
                totalNumberInArrayLineEdit->setText (
                    util::ui32Tostring (objectsInArray).c_str ());
                rotateObjectsCheckBox->setChecked (rotate);
            }

            void ArrayRotateDialog::accept () {
                util::ui32 objectsInArray_ =
                    util::stringToui32 (totalNumberInArrayLineEdit->text ().toStdString ().c_str ());
                if (objectsInArray_ < 2) {
                    // FIXME: display error message
                    assert (0);
                    return;
                }
                objectsInArray = objectsInArray_;
                rotate = rotateObjectsCheckBox->isChecked ();
                QDialog::accept ();
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
