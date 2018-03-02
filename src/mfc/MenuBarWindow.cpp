#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <vector>
#include "thekogans/mex/3ds/mex/core/Module.h"
#include "MenuBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                namespace {
                    const UINT ID_MENUBAR_FIRST = 21401;
                    const UINT ID_MENUBAR_LAST = 22400;
                }

                void MenuBarWindow::SetMenuBar (core::MenuBar &menuBar_) {
                    // One time initialization of Modules menu.
                    // It doesn't really belong here, but its the
                    // only place we can be sure modules have been
                    // loaded.
                    // FIXME: this really doesn't belong here.
                    if (modules.children.empty ()) {
                        struct ModuleEventHandler : public core::MenuBar::Item::CommandEventHandler {
                            util::ui32 moduleIndex;
                            ModuleEventHandler (util::ui32 moduleIndex_) : moduleIndex (moduleIndex_) {}
                            virtual void OnSetFocus () {
                                core::Module::SetCurrModule (moduleIndex);
                            }
                            virtual void OnUpdateUI (UI &ui) {
                                ui.SetChecked (core::Module::currModule == moduleIndex);
                            }
                        };
                        for (std::size_t i = 0, count = core::Module::modules.size (); i < count; ++i) {
                            new core::MenuBar::Item (core::MenuBar::Item::TYPE_ITEM,
                                core::Module::modules[i].second->GetName (), "",
                                new ModuleEventHandler (i), &modules);
                        }
                    }
                    // Reset internal state.
                    {
                        menuBar = &menuBar_;
                        items.clear ();
                    }
                    // Create module menu.
                    {
                        CMenu menu;
                        menu.CreateMenu ();
                        UINT id = ID_MENUBAR_FIRST;
                        for (std::size_t i = 0, count = menuBar->root.children.size (); i < count; ++i) {
                            AddMenuItem (menu, menuBar->root.children[i], id);
                        }
                        DestroyMenu (LoadMenu (menu.Detach (), 0));
                    }
                }

                void MenuBarWindow::AddMenuItem (CMenu &menu, core::MenuBar::Item *item, UINT &id) {
                    switch (item->type) {
                        case core::MenuBar::Item::TYPE_POPUP: {
                            CMenu popup;
                            popup.CreatePopupMenu ();
                            if (!item->children.empty ()) {
                                for (std::size_t i = 0, count = item->children.size (); i < count; ++i) {
                                    AddMenuItem (popup, item->children[i], id);
                                }
                            }
                            else {
                                for (std::size_t i = 0, count = modules.children.size (); i < count; ++i) {
                                    popup.AppendMenu (MF_STRING, id++, modules.children[i]->text.c_str ());
                                    items.push_back (modules.children[i]);
                                }
                            }
                            menu.AppendMenu (MF_POPUP, (UINT)popup.Detach (), item->text.c_str ());
                            break;
                        }
                        case core::MenuBar::Item::TYPE_ITEM:
                            menu.AppendMenu (MF_STRING, id++, item->text.c_str ());
                            items.push_back (item);
                            break;
                        case core::MenuBar::Item::TYPE_SEPARATOR:
                            menu.AppendMenu (MF_SEPARATOR);
                            break;
                    }
                }

                int MenuBarWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CMenuBar::OnCreate (lpCreateStruct) == -1) {
                        return -1;
                    }
                    assert (core::UI::Instance ().menuBarWindow == 0);
                    core::UI::Instance ().menuBarWindow = this;
                    return 0;
                }

                void MenuBarWindow::OnCommand (UINT nID) {
                    assert (nID >= ID_MENUBAR_FIRST);
                    nID -= ID_MENUBAR_FIRST;
                    assert (nID < items.size ());
                    assert (items[nID]->eventHandler != 0);
                    if (items[nID]->eventHandler != 0) {
                        items[nID]->eventHandler->OnSetFocus ();
                    }
                }

                void MenuBarWindow::OnUpdateCommandUI (CCmdUI *pCmdUI) {
                    assert (pCmdUI->m_nID >= ID_MENUBAR_FIRST);
                    UINT nID = pCmdUI->m_nID - ID_MENUBAR_FIRST;
                    assert (nID < items.size ());
                    assert (items[nID]->eventHandler != 0);
                    if (items[nID]->eventHandler != 0) {
                        struct UI : public core::MenuBar::Item::EventHandler::UI {
                            CCmdUI *pCmdUI;
                            UI (core::MenuBar::Item &item, CCmdUI *pCmdUI_) :
                                core::MenuBar::Item::EventHandler::UI (item), pCmdUI (pCmdUI_) {}
                            virtual void SetEnabled (bool enabled) {
                                pCmdUI->Enable (enabled ? TRUE : FALSE);
                            }
                            virtual void SetCheckable (bool checked) {}
                            virtual void SetChecked (bool checked) {
                                pCmdUI->SetCheck (checked ? TRUE : FALSE);
                            }
                        } ui (*items[nID], pCmdUI);
                        items[nID]->eventHandler->OnUpdateUI (ui);
                    }
                }

                BEGIN_MESSAGE_MAP (MenuBarWindow, thekogans::win::mfc::CMenuBar)
                    ON_WM_CREATE ()
                    ON_COMMAND_RANGE (ID_MENUBAR_FIRST, ID_MENUBAR_LAST, OnCommand)
                    ON_UPDATE_COMMAND_UI_RANGE (ID_MENUBAR_FIRST, ID_MENUBAR_LAST, OnUpdateCommandUI)
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
