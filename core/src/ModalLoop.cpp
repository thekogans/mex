// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtGui/QKeyEvent>
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/ModalLoop.h"

namespace thekogans {
    namespace mex {
        namespace core {

            ModalLoop::ModalLoop () : done (false) {
                QCoreApplication::instance ()->installEventFilter (this);
            }

            ModalLoop::~ModalLoop () {
                QCoreApplication::instance ()->removeEventFilter (this);
            }

            void ModalLoop::Run () {
                while (!done) {
                    DoIteration ();
                    QCoreApplication::processEvents ();
                }
            }

            bool ModalLoop::eventFilter (QObject *object, QEvent *event) {
                if (event->type () == QEvent::KeyPress) {
                    QKeyEvent *keyEvent = static_cast<QKeyEvent *> (event);
                    if (keyEvent->key () == Qt::Key_Escape) {
                        done = true;
                    }
                }
                // Filter out mouse and keyboard events
                // to prevent reentrancy issues.
                // FIXME: we might need to revisit this strategy
                // if certain events cause problems down the road.
                //return dynamic_cast<QKeyEvent *> (event) != 0 ||
                //    dynamic_cast<QMouseEvent *> (event) != 0;
                return event->type () == QEvent::KeyPress ||
                    event->type () == QEvent::KeyRelease ||
                    event->type () == QEvent::MouseButtonDblClick ||
                    event->type () == QEvent::MouseButtonPress ||
                    event->type () == QEvent::MouseButtonRelease ||
                    event->type () == QEvent::MouseMove ||
                    event->type () == QEvent::MouseTrackingChange;
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
