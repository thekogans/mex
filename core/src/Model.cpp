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

#include "thekogans/mex/core/Model.h"

namespace thekogans {
    namespace mex {
        namespace core {

            _LIB_THEKOGANS_MEX_CORE_DECL thekogans::mex::command::CommandDispatcher &
                    _LIB_THEKOGANS_MEX_CORE_API GetCommandDispatcher () {
                static thekogans::mex::command::CommandDispatcher commandDispatcher;
                return commandDispatcher;
            }

            _LIB_THEKOGANS_MEX_CORE_DECL Model &
                    _LIB_THEKOGANS_MEX_CORE_API GetModel () {
                struct GlobalModel : public Model {
                    GlobalModel () {
                        _3ds::io::command::ProjectValidateAfterOpenCommand (ioProject).Execute ();
                        _3ds::ext::command::ProjectValidateAfterOpenCommand (extProject).Execute ();
                        _3ds::opengl::command::ProjectValidateAfterOpenCommand (openglProject).Execute ();
                    }
                } static globalModel;
                return globalModel;
            }

            _LIB_THEKOGANS_MEX_CORE_DECL _3ds::io::Project &
                    _LIB_THEKOGANS_MEX_CORE_API GetIOProject () {
                return GetModel ().GetIOProject ();
            }

            _LIB_THEKOGANS_MEX_CORE_DECL _3ds::ext::Project &
                    _LIB_THEKOGANS_MEX_CORE_API GetExtProject () {
                return GetModel ().GetExtProject ();
            }

            _LIB_THEKOGANS_MEX_CORE_DECL _3ds::opengl::Project &
                    _LIB_THEKOGANS_MEX_CORE_API GetOpenGLProject () {
                return GetModel ().GetOpenGLProject ();
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
