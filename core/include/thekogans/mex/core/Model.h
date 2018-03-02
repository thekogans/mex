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

#if !defined (__thekogans_mex_core_Model_h)
#define __thekogans_mex_core_Model_h

#include <memory>
#include <string>
#include "thekogans/mex/command/CommandDispatcher.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/Project.h"
#include "thekogans/mex/3ds/opengl/Project.h"
#include "thekogans/mex/3ds/io/command/ProjectCommands.h"
#include "thekogans/mex/3ds/ext/command/ProjectCommands.h"
#include "thekogans/mex/3ds/opengl/command/ProjectCommands.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            _LIB_THEKOGANS_MEX_CORE_DECL thekogans::mex::command::CommandDispatcher &
                _LIB_THEKOGANS_MEX_CORE_API GetCommandDispatcher ();

            struct _LIB_THEKOGANS_MEX_CORE_DECL Model {
                _3ds::io::Project ioProject;
                _3ds::ext::Project extProject;
                _3ds::opengl::Project openglProject;

                Model () :
                    extProject (ioProject),
                    openglProject (extProject) {}
                virtual ~Model () {}

                inline _3ds::io::Project &GetIOProject () {
                    return ioProject;
                }
                inline _3ds::ext::Project &GetExtProject () {
                    return extProject;
                }
                inline _3ds::opengl::Project &GetOpenGLProject () {
                    return openglProject;
                }
            };

            _LIB_THEKOGANS_MEX_CORE_DECL Model &
                _LIB_THEKOGANS_MEX_CORE_API GetModel ();
            _LIB_THEKOGANS_MEX_CORE_DECL _3ds::io::Project &
                _LIB_THEKOGANS_MEX_CORE_API GetIOProject ();
            _LIB_THEKOGANS_MEX_CORE_DECL _3ds::ext::Project &
                _LIB_THEKOGANS_MEX_CORE_API GetExtProject ();
            _LIB_THEKOGANS_MEX_CORE_DECL _3ds::opengl::Project &
                _LIB_THEKOGANS_MEX_CORE_API GetOpenGLProject ();

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_Model_h)
