// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_editor.
//
// libthekogans_mex_editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_editor. If not, see <http://www.gnu.org/licenses/>.

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QMessageBox>
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/editor/ui/CameraDefinitionDialog.h"

namespace thekogans {
    namespace mex {
        namespace editor {

            CameraDefinitionDialog::CameraDefinitionDialog (
                    core::Module &module_,
                    std::string &name_,
                    util::f32 &roll_,
                    util::f32 &fov_,
                    bool &showCone_,
                    QWidget *parent) :
                    QDialog (parent),
                    module (module_),
                    name (name_),
                    roll (roll_),
                    fov (fov_),
                    showCone (showCone_) {
                setupUi (this);
                cameraNameLineEdit->setText (
                    static_cast<_3ds::ext::Editor &> (
                        module.GetExtModule ()).GetUniqueName (name).c_str ());
                lensRadioButton->setChecked (true);
                lensLineEdit->setText (
                    util::f32Tostring (
                        _3ds::io::Camera::FOV2Lens (fov), "%.2f").c_str ());
                fovLineEdit->setText (util::f32Tostring (fov, "%.2f").c_str ());
                rollLineEdit->setText (util::f32Tostring (roll, "%.2f").c_str ());
                showConeCheckBox->setChecked (showCone);
                connect (calculatePushButton, SIGNAL (clicked (bool)), this, SLOT (OnCalculate (bool)));
                connect (_15mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens15MM (bool)));
                connect (_20mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens20MM (bool)));
                connect (_24mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens24MM (bool)));
                connect (_28mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens28MM (bool)));
                connect (_35mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens35MM (bool)));
                connect (_50mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens50MM (bool)));
                connect (_85mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens85MM (bool)));
                connect (_135mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens135MM (bool)));
                connect (_200mmPushButton, SIGNAL (clicked (bool)), this, SLOT (OnLens200MM (bool)));
            }

            void CameraDefinitionDialog::DoLens (util::f32 lens) {
                lensLineEdit->setText (util::f32Tostring (lens, "%.2f").c_str ());
                fovLineEdit->setText (util::f32Tostring (_3ds::io::Camera::Lens2FOV (lens), "%.2f").c_str ());
            }

            void CameraDefinitionDialog::OnCalculate (bool) {
                if (lensRadioButton->isChecked ()) {
                    std::string lens = lensLineEdit->text ().toStdString ();
                    fovLineEdit->setText (util::f32Tostring (_3ds::io::Camera::Lens2FOV (blas::atoff (lens.c_str ())), "%.2f").c_str ());
                }
                else {
                    std::string fov = fovLineEdit->text ().toStdString ();
                    lensLineEdit->setText (util::f32Tostring (_3ds::io::Camera::FOV2Lens (blas::atoff (fov.c_str ())), "%.2f").c_str ());
                }
            }

            void CameraDefinitionDialog::OnLens15MM (bool) {
                DoLens (15.0f);
            }

            void CameraDefinitionDialog::OnLens20MM (bool) {
                DoLens (20.0f);
            }

            void CameraDefinitionDialog::OnLens24MM (bool) {
                DoLens (24.0f);
            }

            void CameraDefinitionDialog::OnLens28MM (bool) {
                DoLens (28.0f);
            }

            void CameraDefinitionDialog::OnLens35MM (bool) {
                DoLens (35.0f);
            }

            void CameraDefinitionDialog::OnLens50MM (bool) {
                DoLens (50.0f);
            }

            void CameraDefinitionDialog::OnLens85MM (bool) {
                DoLens (85.0f);
            }

            void CameraDefinitionDialog::OnLens135MM (bool) {
                DoLens (135.0f);
            }

            void CameraDefinitionDialog::OnLens200MM (bool) {
                DoLens (200.0f);
            }

            void CameraDefinitionDialog::accept () {
                {
                    std::string text = cameraNameLineEdit->text ().toStdString ();
                    if (text.empty ()) {
                        QMessageBox::warning (this, tr ("Camera Definition"),
                            tr ("Name cannot be blank."), QMessageBox::Ok);
                        return;
                    }
                    if (static_cast<_3ds::ext::Editor &> (module.GetExtModule ()).CheckName (text)) {
                        QMessageBox::warning (this, tr ("Camera Definition"),
                            tr ("Name must be unique."), QMessageBox::Ok);
                        return;
                    }
                    name = text;
                }
                fov = blas::atoff (fovLineEdit->text ().toStdString ().c_str ());
                roll = blas::atoff (rollLineEdit->text ().toStdString ().c_str ());
                showCone = showConeCheckBox->isChecked ();
                QDialog::accept ();
            }

        } // namespace editor
    } // namespace mex
} // namespace thekogans
