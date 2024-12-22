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

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QFileDialog>
#include "thekogans/util/Path.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/Shaper.h"
#include "thekogans/mex/shaper/ui/3DDisplayDialog.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            _3DDisplayDialog::_3DDisplayDialog (QWidget *parent) : QDialog (parent) {
                setupUi (this);
                typedef util::OwnerVector<_3ds::io::Mesh>::const_iterator const_iterator;
                for (const_iterator it = core::GetIOProject ().editor.meshes.begin (),
                        end = core::GetIOProject ().editor.meshes.end (); it != end; ++it) {
                    QListWidgetItem *item = new QListWidgetItem;
                    item->setText ((*it)->name.c_str ());
                    objectsListWidget->addItem (item);
                    if (std::find (core::GetIOProject ().shaper.meshes.begin (),
                            core::GetIOProject ().shaper.meshes.end (), (*it)->name) !=
                            core::GetIOProject ().shaper.meshes.end ()) {
                        item->setSelected (true);
                    }
                }
                connect (allPushButton, SIGNAL (clicked (bool)), this, SLOT (OnAll (bool)));
                connect (nonePushButton, SIGNAL (clicked (bool)), this, SLOT (OnNone (bool)));
            }

            void _3DDisplayDialog::OnAll (bool) {
                objectsListWidget->selectAll ();
            }

            void _3DDisplayDialog::OnNone (bool) {
                objectsListWidget->clearSelection ();
            }

            void _3DDisplayDialog::accept () {
                core::GetIOProject ().shaper.meshes.clear ();
                QList<QListWidgetItem *> selectedItems = objectsListWidget->selectedItems ();
                typedef QList<QListWidgetItem *>::const_iterator const_iterator;
                for (const_iterator it = selectedItems.begin (), end = selectedItems.end (); it != end; ++it) {
                    core::GetIOProject ().shaper.meshes.push_back ((*it)->text ().toStdString ());
                }
                core::command::DrawViewLayoutFinalOperation (*Shaper::Instance ()).Execute ();
                core::command::FlipFramebufferFinalOperation ().Execute ();
                QDialog::accept ();
            }

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
