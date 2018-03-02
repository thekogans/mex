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
#include <QtCore/QEvent>
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/MenuBarWindow.h"

namespace thekogans {
    namespace mex {

        MenuItem::MenuItem (QWidget *parent, core::MenuBar::Item *item_) :
            QAction (parent), item (item_) {
            if (item->type == core::MenuBar::Item::TYPE_SEPARATOR) {
                setSeparator (true);
            }
            else {
                connect (this, SIGNAL (triggered (bool)), this, SLOT (trigger ()));
                setText (item->text.c_str ());
                setStatusTip (item->statusTip.c_str ());
            }
        }

        void MenuItem::OnUpdateUI () {
            if (item != 0 && item->eventHandler != 0) {
                struct UI : public core::MenuBar::Item::EventHandler::UI {
                    MenuItem *menuItem;

                    UI (core::MenuBar::Item &item, MenuItem *menuItem_) :
                        core::MenuBar::Item::EventHandler::UI (item), menuItem (menuItem_) {}

                    virtual void SetEnabled (bool enabled) {
                        menuItem->setEnabled (enabled);
                    }
                    virtual void SetCheckable (bool checkable) {
                        menuItem->setCheckable (checkable);
                    }
                    virtual void SetChecked (bool checked) {
                        menuItem->setChecked (checked);
                    }
                } ui (*item, this);
                item->eventHandler->OnUpdateUI (ui);
            }
        }

        void MenuItem::trigger () {
            core::ViewLayout *viewLayout = core::UI::Instance ().viewLayoutWindow->GetViewLayout ();
            if (viewLayout != 0 && viewLayout->tool != 0) {
                viewLayout->tool->KillFocus ();
            }
            assert (item->eventHandler != 0);
            item->eventHandler->OnSetFocus ();
        }

        MenuBarWindow::MenuBarWindow (QWidget *parent) : QMenuBar (parent) {
            setObjectName ("MenuBarWindow");
            assert (core::UI::Instance ().menuBarWindow == 0);
            core::UI::Instance ().menuBarWindow = this;
        }

        void MenuBarWindow::OnIdle () {
            for (std::size_t i = 0, count = menuItems.size (); i < count; ++i) {
                menuItems[i]->OnUpdateUI ();
            }
        }

        void MenuBarWindow::SetMenuBar (core::MenuBar &menuBar) {
            // One time initialization of Modules menu.
            // It doesn't really belong here, but it's the
            // only place we can be sure modules have been
            // loaded.
            // FIXME: this really doesn't belong here.
            if (modules.children.empty ()) {
                struct ModuleEventHandler : public core::MenuBar::Item::ButtonEventHandler {
                    util::ui32 moduleIndex;
                    ModuleEventHandler (util::ui32 moduleIndex_) : moduleIndex (moduleIndex_) {}
                    virtual void OnSetFocus () {
                        core::Module::SetCurrModule (moduleIndex);
                    }
                    virtual bool IsChecked () const {
                        return core::Module::currModule == moduleIndex;
                    }
                };
                for (std::size_t i = 0, count = core::Module::modules.size (); i < count; ++i) {
                    new core::MenuBar::Item (core::MenuBar::Item::TYPE_ITEM,
                        core::Module::modules[i].second->GetName (), "", "",
                        new ModuleEventHandler (i), &modules);
                }
            }
            // Reset internal state.
            clear ();
            menuItems.clear ();
            // Create module menu.
            for (std::size_t i = 0, count = menuBar.root.children.size (); i < count; ++i) {
                AddMenuItem (0, menuBar.root.children[i]);
            }
            update ();
        }

        void MenuBarWindow::AddMenuItem (QMenu *parent, core::MenuBar::Item *item) {
            switch (item->type) {
                case core::MenuBar::Item::TYPE_POPUP: {
                    if (parent == 0) {
                        parent = addMenu (tr (item->text.c_str ()));
                    }
                    else {
                        parent = parent->addMenu (tr (item->text.c_str ()));
                    }
                    for (std::size_t i = 0, count = item->children.size (); i < count; ++i) {
                        AddMenuItem (parent, item->children[i]);
                    }
                    break;
                }
                case core::MenuBar::Item::TYPE_ITEM:
                case core::MenuBar::Item::TYPE_SEPARATOR:
                    if (item->modules == "yes") {
                        for (std::size_t i = 0, count = modules.children.size (); i < count; ++i) {
                            AddMenuItem (parent, modules.children[i]);
                        }
                    }
                    else {
                        MenuItem::UniquePtr menuItem (new MenuItem (parent, item));
                        assert (menuItem.get () != 0);
                        parent->addAction (menuItem.get ());
                        menuItems.push_back (menuItem.get ());
                        menuItem.release ();
                    }
                    break;
            }
        }

    } // namespace mex
} // namespace thekogans
