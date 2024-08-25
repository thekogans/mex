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

#include <assert.h>
#include "thekogans/util/Environment.h"
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
#include "thekogans/mex/shaper/ui/ArrayLinearDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            ArrayLinearDialog::ArrayLinearDialog (
                    bool selected_,
                    util::ui32 &objectsInArray_,
                    util::f32 &spacing_,
                    bool &objectSpacing_,
                    bool &centerToCenter_,
                    util::f32 &size_,
                    QWidget *parent) :
                    QDialog (parent),
                    selected (selected_),
                    objectsInArray (objectsInArray_),
                    spacing (spacing_),
                    objectSpacing (objectSpacing_),
                    centerToCenter (centerToCenter_),
                    size (size_) {
                setupUi (this);
                arrayingText->setText (selected ? "Selected" : "Polygon");
                sizeText->setText (util::f32Tostring (size, "%.5f").c_str ());
                totalNumberInArrayLineEdit->setText (util::ui32Tostring (objectsInArray).c_str ());
                objectSpacingRadioButton->setChecked (true);
                objectSpacingLineEdit->setText (util::f32Tostring (spacing, "%.5f").c_str ());
                if (centerToCenter) {
                    centerToCenterRadioButton->setChecked (true);
                }
                else {
                    endToEndRadioButton->setChecked (true);
                }
                OnCalculate (true);
                if (!objectSpacing) {
                    objectSpacingRadioButton->setChecked (false);
                    arrayLengthRadioButton->setChecked (true);
                }
                connect (calculatePushButton, SIGNAL (clicked (bool)), this, SLOT (OnCalculate (bool)));
            }

            void ArrayLinearDialog::OnCalculate (bool) {
                util::ui32 objectsInArray = util::stringToui32 (totalNumberInArrayLineEdit->text ().toStdString ().c_str ());
                if (objectsInArray < 2) {
                    // "Need at least two polygons for array.";
                    return;
                }
                util::f32 size = atof (sizeText->text ().toStdString ().c_str ());
                if (objectSpacingRadioButton->isChecked ()) {
                    util::f32 spacing = atof (objectSpacingLineEdit->text ().toStdString ().c_str ());
                    util::f32 length;
                    if (centerToCenterRadioButton->isChecked ()) {
                        length = size + (objectsInArray - 1) * spacing;
                    }
                    else {
                        length = size * objectsInArray + (objectsInArray - 1) * spacing;
                    }
                    arrayLengthLineEdit->setText (util::f32Tostring (length, "%.5f").c_str ());
                }
                else {
                    util::f32 length = atof (arrayLengthLineEdit->text ().toStdString ().c_str ());
                    util::f32 spacing;
                    if (centerToCenterRadioButton->isChecked ()) {
                        spacing = (length - size) / (objectsInArray - 1);
                        if (spacing < 0.0f) {
                            //"Length too small to fit polygon(s).";
                            return;
                        }
                    }
                    else {
                        spacing = (length - size * objectsInArray) / (objectsInArray - 1);
                        if (spacing < 0.0f) {
                            //"Arrayed polygons will overlap.", "Warning:";
                        }
                    }
                    objectSpacingLineEdit->setText (util::f32Tostring (spacing, "%.5f").c_str ());
                }
            }

            void ArrayLinearDialog::accept () {
                OnCalculate (true);
                util::ui32 objectsInArray_ = util::stringToui32 (totalNumberInArrayLineEdit->text ().toStdString ().c_str ());
                if (objectsInArray_ < 2) {
                    // FIXME: display error message
                    assert (0);
                    return;
                }
                util::f32 spacing_ = atof (objectSpacingLineEdit->text ().toStdString ().c_str ());
                if (objectSpacingRadioButton->isChecked () && spacing < 0.0f) {
                    // FIXME: display error message
                    assert (0);
                    return;
                }
                objectsInArray = objectsInArray_;
                spacing = spacing_;
                objectSpacing = objectSpacingRadioButton->isChecked ();
                centerToCenter = centerToCenterRadioButton->isChecked ();
                QDialog::accept ();
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
