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

#include "thekogans/util/Constants.h"
#include "thekogans/util/Path.h"
#include "thekogans/util/SystemInfo.h"
#include "thekogans/util/DynamicLibrary.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/XercesUtils.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Module.h"

namespace thekogans {
    namespace mex {
        namespace core {

            std::vector<Module::ModuleInfo> Module::modules;
            std::size_t Module::currModule = util::NIDX;

            const util::ui32 Module::ModuleInterface::MAJOR_VERSION = 1;
            const util::ui32 Module::ModuleInterface::MINOR_VERSION = 0;

            const util::ui32 Module::PluginInterface::MAJOR_VERSION = 1;
            const util::ui32 Module::PluginInterface::MINOR_VERSION = 0;

            Module *Module::GetModuleByName (const std::string &name) {
                for (std::size_t i = 0, count = modules.size (); i < count; ++i) {
                    if (modules[i].first == name) {
                        assert (modules[i].second != nullptr);
                        return modules[i].second;
                    }
                }
                return nullptr;
            }

            void Module::SetCurrModule (util::ui32 currModule_) {
                assert (currModule_ < modules.size ());
                Module *module = GetCurrModule ();
                if (module != nullptr) {
                    module->KillFocus ();
                }
                currModule = currModule_;
                module = GetCurrModule ();
                if (module != nullptr) {
                    module->SetFocus ();
                }
            }

            namespace {
                void ParsePlugin (
                        const XERCES_CPP_NAMESPACE::DOMNode &node,
                        Module &module) {
                    const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes = node.getAttributes ();
                    std::string path = util::GetAttributeValue ("path", attributes);
                    util::DynamicLibrary dynamicLibrary;
                    THEKOGANS_UTIL_TRY {
                        dynamicLibrary.Load (
                            util::MakePath (
                                util::SystemInfo::Instance ()->GetProcessStartDirectory (),
                                path));
                        typedef Module::PluginInterface &
                            (_LIB_THEKOGANS_MEX_CORE_API *GetPluginInterfaceProc) ();
                        GetPluginInterfaceProc GetPluginInterface = (GetPluginInterfaceProc)
                            dynamicLibrary.GetProc ("GetPluginInterface");
                        assert (GetPluginInterface != nullptr);
                        const Module::PluginInterface &pluginInterface = GetPluginInterface ();
                        if (pluginInterface.GetMajorVersion () !=
                                Module::PluginInterface::MAJOR_VERSION) {
                            dynamicLibrary.Unload ();
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION ("%s", "Major version mismatch.");
                        }
                        if (pluginInterface.GetMinorVersion () !=
                                Module::PluginInterface::MINOR_VERSION) {
                            assert (UI::Instance ()->consoleWindow != nullptr);
                            UI::Instance ()->consoleWindow->Print (
                                "'%s': *** WARNING ***, plugin '%s' minor version mismatch (%u - %u).\n",
                                module.GetName (),
                                path.c_str (),
                                pluginInterface.GetMinorVersion (),
                                Module::PluginInterface::MINOR_VERSION);
                        }
                        pluginInterface.LoadPlugins (module);
                    }
                    THEKOGANS_UTIL_CATCH (util::Exception) {
                        assert (UI::Instance ()->consoleWindow != nullptr);
                        UI::Instance ()->consoleWindow->Print (
                            "'%s': Unable to load plugin '%s' (%s).\n",
                            module.GetName (),
                            path.c_str (),
                            exception.what ());
                    }
                }

                void ParseModule (const XERCES_CPP_NAMESPACE::DOMNode &node) {
                    const XERCES_CPP_NAMESPACE::DOMNamedNodeMap *attributes = node.getAttributes ();
                    std::string name = util::GetAttributeValue ("name", attributes);
                    std::string path = util::GetAttributeValue ("path", attributes);
                    std::string menuBarPath = util::GetAttributeValue ("menuBarPath", attributes);
                    std::string toolBarPath = util::GetAttributeValue ("toolBarPath", attributes);
                    std::string speedBarPath = util::GetAttributeValue ("speedBarPath", attributes);
                    std::string statusBarPath = util::GetAttributeValue ("statusBarPath", attributes);
                    std::string acceleratorsPath =
                        util::GetAttributeValue ("acceleratorsPath", attributes);
                    std::string filtersPath = util::GetAttributeValue ("filtersPath", attributes);
                    std::string _default = util::GetAttributeValue ("default", attributes);
                    util::DynamicLibrary dynamicLibrary;
                    THEKOGANS_UTIL_TRY {
                        dynamicLibrary.Load (
                            util::MakePath (
                                util::SystemInfo::Instance ()->GetProcessStartDirectory (),
                                path));
                        typedef const Module::ModuleInterface &
                            (_LIB_THEKOGANS_MEX_CORE_API *GetModuleInterfaceProc) (const char *name);
                        GetModuleInterfaceProc GetModuleInterface = (GetModuleInterfaceProc)
                            dynamicLibrary.GetProc ("GetModuleInterface");
                        assert (GetModuleInterface != nullptr);
                        const Module::ModuleInterface &moduleInterface =
                            GetModuleInterface (name.c_str ());
                        if (moduleInterface.GetMajorVersion () !=
                                Module::ModuleInterface::MAJOR_VERSION) {
                            dynamicLibrary.Unload ();
                            THEKOGANS_UTIL_THROW_STRING_EXCEPTION ("%s", "Major version mismatch.");
                        }
                        if (moduleInterface.GetMinorVersion () !=
                                Module::ModuleInterface::MINOR_VERSION) {
                            assert (UI::Instance ()->consoleWindow != nullptr);
                            UI::Instance ()->consoleWindow->Print (
                                "*** WARNING ***, module '%s' minor version mismatch (%u - %u).\n",
                                path.c_str (),
                                moduleInterface.GetMinorVersion (),
                                Module::ModuleInterface::MINOR_VERSION);
                        }
                        Module *module = moduleInterface.GetModule ();
                        assert (module != nullptr);
                        if (module != nullptr) {
                            if (_default == "true") {
                                Module::currModule = Module::modules.size ();
                            }
                            Module::modules.push_back (Module::ModuleInfo (name, module));
                            const XERCES_CPP_NAMESPACE::DOMNodeList *children = node.getChildNodes ();
                            for (std::size_t i = 0, count = children->getLength (); i < count; ++i) {
                                const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                                if (child.getNodeType () ==
                                        XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                                        util::XMLChTostring (child.getNodeName ()) == "plugin") {
                                    ParsePlugin (child, *module);
                                }
                            }
                            // Now that plugins had a chance to load, and register
                            // themselves, load the module UI. In the process this
                            // should hookup all event handlers.
                            module->LoadUI (
                                menuBarPath,
                                toolBarPath,
                                speedBarPath,
                                statusBarPath,
                                acceleratorsPath,
                                filtersPath);
                        }
                    }
                    THEKOGANS_UTIL_CATCH (util::Exception) {
                        assert (UI::Instance ()->consoleWindow != nullptr);
                        UI::Instance ()->consoleWindow->Print (
                            "Unable to load module '%s' (%s).\n",
                            path.c_str (),
                            exception.what ());
                    }
                }
            }

            void Module::LoadModules (const std::string &path) {
                util::XercesPlatformInit xercesPlatformInit;
                util::XercesErrorHandler errorHandler;
                XERCES_CPP_NAMESPACE::XercesDOMParser parser;
                parser.setErrorHandler (&errorHandler);
                THEKOGANS_UTIL_TRY {
                    parser.parse (path.c_str ());
                    const XERCES_CPP_NAMESPACE::DOMNodeList *children =
                        parser.getDocument ()->getDocumentElement ()->getChildNodes ();
                    for (std::size_t i = 0, count = children->getLength (); i < count; ++i) {
                        const XERCES_CPP_NAMESPACE::DOMNode &child = *children->item (i);
                        if (child.getNodeType () == XERCES_CPP_NAMESPACE::DOMNode::ELEMENT_NODE &&
                                util::XMLChTostring (child.getNodeName ()) == "module") {
                            ParseModule (child);
                        }
                    }
                }
                THEKOGANS_UTIL_CATCH (util::Exception) {
                    assert (UI::Instance ()->consoleWindow != nullptr);
                    UI::Instance ()->consoleWindow->Print (
                        "Module: Unable to load '%s'.\n\t%s\n",
                        path.c_str (),
                        exception.what ());
                }
                // Guard against default not being set.
                if (Module::currModule == util::NIDX && !Module::modules.empty ()) {
                    Module::currModule = 0;
                }
            }

            void Module::LoadUI (
                    const std::string &menuBarPath,
                    const std::string &toolBarPath,
                    const std::string &speedBarPath,
                    const std::string &statusBarPath,
                    const std::string &acceleratorsPath,
                    const std::string &filtersPath) {
                menuBar.reset (new core::MenuBar (menuBarPath, *this));
                toolBar.reset (new core::ToolBar (toolBarPath, *this));
                viewLayout.reset (new core::ViewLayout (*this));
                speedBar.reset (new core::SpeedBar (speedBarPath, *this));
                statusBar.reset (new core::StatusBar (statusBarPath, *this));
                accelerators.reset (new core::Accelerators (acceleratorsPath, *this));
                filters.reset (new core::Filters (filtersPath, *this));
            }

            void Module::SetFocus () {
                UI::Instance ()->menuBarWindow->SetMenuBar (*menuBar);
                UI::Instance ()->toolBarWindow->SetToolBar (*toolBar);
                UI::Instance ()->viewLayoutWindow->SetViewLayout (*viewLayout);
                UI::Instance ()->speedBarWindow->SetSpeedBar (*speedBar);
                UI::Instance ()->statusBarWindow->SetStatusBar (*statusBar);
            }

            void Module::KillFocus () {
                viewLayout->KillFocus ();
            }

            bool Module::TranslateAccelerator (
                    util::ui32 ch,
                    util::ui32 flags) {
                if (accelerators.get () != nullptr) {
                    const std::vector<Accelerators::Item *> &items = accelerators->GetItems ();
                    for (std::size_t i = 0, count = items.size (); i < count; ++i) {
                        if (items[i]->HandleEvent (ch, flags)) {
                            return true;
                        }
                    }
                }
                return false;
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
