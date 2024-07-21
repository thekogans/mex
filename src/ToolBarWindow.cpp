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

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtGui/QMouseEvent>
#include "thekogans/mex/3ds/opengl/Image.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/ToolBarWindow.h"

namespace thekogans {
    namespace mex {

        ToolButton::ToolButton (QToolBar *parent, core::ToolBar::Item *item_) :
            QToolButton (parent), item (item_) {
            assert (item->eventHandler != 0);
            setToolTip (QString (item->toolTip.c_str ()));
            setStatusTip (QString (item->statusTip.c_str ()));
            setFocusPolicy (Qt::NoFocus);
            SetImage (*item->image);
        }

        void ToolButton::OnUpdateUI () {
            if (item != 0) {
                struct UI : public core::ToolBar::Item::EventHandler::UI {
                    ToolButton &toolButton;

                    UI (core::ToolBar::Item &item, ToolButton &toolButton_) :
                        core::ToolBar::Item::EventHandler::UI (item), toolButton (toolButton_) {}

                    virtual void SetEnabled (bool enabled) {
                        toolButton.setEnabled (enabled);
                    }
                    virtual void SetCheckable (bool checkable) {
                        toolButton.setCheckable (checkable);
                    }
                    virtual void SetChecked (bool checked) {
                        toolButton.setChecked (checked);
                    }
                    virtual void SetImage () {
                        toolButton.SetImage (*item.image);
                    }
                } ui (*item, *this);
                item->eventHandler->OnUpdateUI (ui);
            }
        }

        void ToolButton::SetImage (_3ds::opengl::Image &image) {
            std::vector<_3ds::opengl::Image::RGBA> &bits = image.bits;
            for (std::size_t i = 0, count = bits.size (); i < count; ++i) {
                // Make background color transparent.
                // FIXME: parameterize the background color.
                if (bits[i].r == 0xc0 && bits[i].g == 0xc0 && bits[i].b == 0xc0) {
                    bits[i].a = 0;
                }
            }
            QImage qImage ((unsigned char *)&bits[0],
                image.width, image.height, QImage::Format_ARGB32);
            setIcon (QIcon (QPixmap::fromImage (qImage.mirrored ())));
        }

        void ToolButton::mousePressEvent (QMouseEvent *event) {
            if (event->button () == Qt::RightButton) {
                QMouseEvent mouseEvent (QEvent::MouseButtonPress, event->pos (),
                    event->globalPos (), Qt::LeftButton, Qt::LeftButton, event->modifiers ());
                QToolButton::mousePressEvent (&mouseEvent);
            }
            else {
                QToolButton::mousePressEvent (event);
            }
        }

        void ToolButton::mouseReleaseEvent (QMouseEvent *event) {
            core::ViewLayout *viewLayout = core::UI::Instance ()->viewLayoutWindow->GetViewLayout ();
            if (viewLayout != 0 && viewLayout->tool != 0) {
                viewLayout->tool->KillFocus ();
            }
            if (event->button () == Qt::RightButton) {
                QMouseEvent mouseEvent (QEvent::MouseButtonRelease, event->pos (),
                    event->globalPos (), Qt::LeftButton, Qt::LeftButton, event->modifiers ());
                QToolButton::mouseReleaseEvent (&mouseEvent);
                item->eventHandler->OnClickRight ();
            }
            else {
                QToolButton::mouseReleaseEvent (event);
                item->eventHandler->OnClickLeft ();
            }
        }

        ToolBarWindow::ToolBarWindow (QWidget *parent) : QToolBar (parent) {
            setObjectName ("ToolBarWindow");
            setFocusPolicy (Qt::NoFocus);
            assert (core::UI::Instance ()->toolBarWindow == 0);
            core::UI::Instance ()->toolBarWindow = this;
        }

        void ToolBarWindow::OnIdle () {
            for (std::size_t i = 0, count = toolButtons.size (); i < count; ++i) {
                toolButtons[i]->OnUpdateUI ();
            }
        }

        void ToolBarWindow::SetToolBar (core::ToolBar &toolBar) {
            clear ();
            toolButtons.clear ();
            setIconSize (QSize (toolBar.width, toolBar.height));
            for (util::ui32 i = 0, count = toolBar.items.size (); i < count; ++i) {
                if (toolBar.items[i] == 0) {
                    addSeparator ();
                }
                else {
                    ToolButton *toolButton = new ToolButton (this, toolBar.items[i]);
                    addWidget (toolButton);
                    toolButtons.push_back (toolButton);
                }
            }
            update ();
        }

    } // namespace mex
} // namespace thekogans
