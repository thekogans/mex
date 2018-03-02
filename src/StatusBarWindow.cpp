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

#include <cstdarg>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtCore/QCoreApplication>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include "thekogans/mex/StatusBarWindow.h"

#if defined (TOOLCHAIN_OS_Windows) && (_MSC_VER <= 1200)
    #define vsnprintf _vsnprintf
#endif // defined (TOOLCHAIN_OS_Windows) && (_MSC_VER <= 1200)

namespace thekogans {
    namespace mex {

        StatusBarWindow::StatusBarWindow (QWidget *parent) :
                QStatusBar (parent), frameBarWindow (this) {
            setObjectName ("StatusBarWindow");
            setMouseTracking (true);
            setFocusPolicy (Qt::NoFocus);
            frameBarWindow.setVisible (false);
            assert (core::UI::Instance ().statusBarWindow == 0);
            core::UI::Instance ().statusBarWindow = this;
        }

        void StatusBarWindow::OnIdle () {
            if (frameBarWindow.IsVisible () && frameBarWindow.isVisible ()) {
                if (!frameBarWindow.IsModal () &&
                    !QRect (0, 0, width (), height ()).contains (mapFromGlobal (QCursor::pos ()))) {
                    frameBarWindow.Show (false);
                }
            }
            else {
                class UI : public core::StatusBar::Item::EventHandler::UI {
                private:
                    QLabel &label;

                public:
                    UI (core::StatusBar::Item &item, QLabel &label_) :
                        core::StatusBar::Item::EventHandler::UI (item), label (label_) {}
                    virtual void SetText (const char *format, ...) {
                        assert (format != 0);
                        // FIXME: Use std::ostringstream for formatting.
                        char str[512];
                        {
                            va_list argptr;
                            va_start (argptr, format);
                            vsnprintf (str, 512, format, argptr);
                            va_end (argptr);
                        }
                        label.setText (str);
                    }
                };
                for (std::size_t i = 0, count = statusItems.size (); i < count; ++i) {
                    assert (statusItems[i].first->eventHandler != 0);
                    UI ui (*statusItems[i].first, *statusItems[i].second);
                    statusItems[i].first->eventHandler->OnUpdateUI (ui);
                }
            }
        }

        void StatusBarWindow::SetStatusBar (core::StatusBar &statusBar) {
            {
                for (std::size_t i = 0, count = statusItems.size (); i < count; ++i) {
                    removeWidget (statusItems[i].second);
                    delete statusItems[i].second;
                }
                statusItems.clear ();
            }
            {
                std::vector<core::StatusBar::Item *> &items = statusBar.items;
                for (std::size_t i = 0, count = items.size (); i < count; ++i) {
                    QLabel *label = new QLabel (this);
                    // FIXME: * 3 is a hack.
                    label->setFixedWidth (items[i]->width * 3);
                    label->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Ignored);
                    addPermanentWidget (label);
                    statusItems.push_back (StatusItem (items[i], label));
                }
            }
            update ();
        }

        _3ds::io::Progress::UniquePtr StatusBarWindow::CreateProgress () {
            class Progress : public _3ds::io::Progress {
            private:
                QStatusBar *statusBar;
                std::unique_ptr<QProgressBar> progressBar;

            public:
                explicit Progress (QStatusBar *statusBar_) :
                        statusBar (statusBar_), progressBar (new QProgressBar (statusBar)) {
                    assert (progressBar.get () != 0);
                    progressBar->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Ignored);
                    statusBar->insertWidget (0, progressBar.get (), 1);
                }
                virtual ~Progress () {
                    statusBar->removeWidget (progressBar.get ());
                }

                virtual void SetTotal (util::ui32 total) {
                    progressBar->setRange (0, total);
                }
                virtual void SetPosition (util::ui32 position) {
                    progressBar->setValue (position);
                    progressBar->repaint ();
                    QCoreApplication::processEvents ();
                }
            };
            return _3ds::io::Progress::UniquePtr (new Progress (this));
        }

        void StatusBarWindow::mouseMoveEvent (QMouseEvent *event) {
            if (frameBarWindow.IsVisible ()) {
                frameBarWindow.Show (true);
                event->accept ();
            }
        }

    } // namespace mex
} // namespace thekogans
