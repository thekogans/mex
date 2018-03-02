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

#if !defined (__thekogans_mex_core_Module_h)
#define __thekogans_mex_core_Module_h

#include <string>
#include <map>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/Module.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/Module.h"
#include "thekogans/mex/3ds/opengl/Module.h"
#include "thekogans/mex/core/Config.h"
#include "thekogans/mex/core/MenuBar.h"
#include "thekogans/mex/core/ToolBar.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/StatusBar.h"
#include "thekogans/mex/core/Accelerators.h"
#include "thekogans/mex/core/Filters.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct _LIB_THEKOGANS_MEX_CORE_DECL Module {
                MenuBar::UniquePtr menuBar;
                ToolBar::UniquePtr toolBar;
                ViewLayout::UniquePtr viewLayout;
                SpeedBar::UniquePtr speedBar;
                StatusBar::UniquePtr statusBar;
                Accelerators::UniquePtr accelerators;
                Filters::UniquePtr filters;

                typedef std::pair<std::string, Module *> ModuleInfo;
                static std::vector<ModuleInfo> modules;
                static std::size_t currModule;
                static Module *GetCurrModule () {
                    return currModule < modules.size () ? modules[currModule].second : 0;
                }
                static Module *GetModuleByName (const std::string &name);
                static void SetCurrModule (util::ui32 currModule_);

                struct ModuleInterface {
                    virtual ~ModuleInterface () {}
                    static const util::ui32 MAJOR_VERSION;
                    static const util::ui32 MINOR_VERSION;
                    virtual util::ui32 GetMajorVersion () const = 0;
                    virtual util::ui32 GetMinorVersion () const = 0;
                    virtual Module *GetModule () const = 0;
                };
                static void LoadModules (const std::string &path);

                struct PluginInterface {
                    virtual ~PluginInterface () {}
                    static const util::ui32 MAJOR_VERSION;
                    static const util::ui32 MINOR_VERSION;
                    virtual util::ui32 GetMajorVersion () const = 0;
                    virtual util::ui32 GetMinorVersion () const = 0;
                    virtual void LoadPlugins (Module &) const = 0;
                };

                virtual ~Module () {}

                void LoadUI (
                    const std::string &menuBarPath,
                    const std::string &toolBarPath,
                    const std::string &speedBarPath,
                    const std::string &statusBarPath,
                    const std::string &acceleratorsPath,
                    const std::string &filtersPath);

                virtual void SetFocus ();
                virtual void KillFocus ();

                virtual _3ds::io::Module &GetIOModule () const = 0;
                virtual _3ds::ext::Module &GetExtModule () const = 0;
                virtual _3ds::opengl::Module &GetOpenGLModule () const = 0;

                virtual const char *GetType () const = 0;
                virtual const char *GetName () const = 0;

                bool TranslateAccelerator (
                    util::ui32 ch,
                    util::ui32 flags);
            };

            #define THEKOGANS_MEX_CORE_DECLARE_MODULE\
            public:\
                static const char * const TYPE;\
                static const char * const NAME;\
                virtual const char *GetType () const {return TYPE;}\
                virtual const char *GetName () const {return NAME;}

            #define THEKOGANS_MEX_CORE_IMPLEMENT_MODULE(type, name)\
                const char * const type::TYPE = #type;\
                const char * const type::NAME = name;

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_Module_h)
