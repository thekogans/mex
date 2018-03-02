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
#include "thekogans/mex/shaper/ui/ArrayRadialDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            ArrayRadialDialog::ArrayRadialDialog (
                    bool selected_,
                    util::ui32 &objectsInArray_,
                    util::f32 &degrees_,
                    bool &useDegrees_,
                    bool &rotate_,
                    QWidget *parent) :
                    QDialog (parent),
                    selected (selected_),
                    objectsInArray (objectsInArray_),
                    degrees (degrees_),
                    useDegrees (useDegrees_),
                    rotate (rotate_) {
                setupUi (this);
                arraying->setText (selected ? "Selected" : "Polygon");
                totalNumberInArrayLineEdit->setText (util::ui32Tostring (objectsInArray).c_str ());
                degreesRadioButton->setChecked (true);
                degreesLineEdit->setText (util::f32Tostring (degrees, "%.5f").c_str ());
                OnCalculate (true);
                if (!useDegrees) {
                    degreesRadioButton->setChecked (false);
                    arcLengthRadioButton->setChecked (true);
                }
                rotateObjectsCheckBox->setChecked (rotate);
                connect (calculatePushButton, SIGNAL (clicked (bool)), this, SLOT (OnCalculate (bool)));
            }

            void ArrayRadialDialog::OnCalculate (bool) {
                util::ui32 objectsInArray =
                    atoi (totalNumberInArrayLineEdit->text ().toStdString ().c_str ());
                if (objectsInArray < 2) {
                    // "Need at least two polygons for array.";
                    return;
                }
                if (degreesRadioButton->isChecked ()) {
                    util::f32 degrees = atof (degreesLineEdit->text ().toStdString ().c_str ());
                    arcLengthLineEdit->setText (
                        util::f32Tostring ((objectsInArray - 1) * degrees, "%.5f").c_str ());
                }
                else {
                    util::f32 arcLength = atof (arcLengthLineEdit->text ().toStdString ().c_str ());
                    degreesLineEdit->setText (
                        util::f32Tostring (arcLength / (objectsInArray - 1), "%.5f").c_str ());
                }
            }

            void ArrayRadialDialog::accept () {
                OnCalculate (true);
                util::ui32 objectsInArray_ =
                    atoi (totalNumberInArrayLineEdit->text ().toStdString ().c_str ());
                if (objectsInArray_ < 2) {
                    // FIXME: display error message
                    assert (0);
                    return;
                }
                objectsInArray = objectsInArray_;
                degrees = atof (degreesLineEdit->text ().toStdString ().c_str ());;
                useDegrees = degreesRadioButton->isChecked ();
                rotate = rotateObjectsCheckBox->isChecked ();
                QDialog::accept ();
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
