// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_common.
//
// libthekogans_mex_common is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_common is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_common. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_common_FileOpenCommand_h)
#define __thekogans_mex_common_FileOpenCommand_h

#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/common/Config.h"

namespace thekogans {
    namespace mex {
        namespace common {

            struct _LIB_THEKOGANS_MEX_COMMON_DECL FileOpenCommand :
                    public thekogans::mex::command::CompoundCommand {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (FileOpenCommand)

            private:
                core::Module &module;

            public:
                explicit FileOpenCommand (core::Module &module_) :
                    module (module_) {}

                virtual bool Execute ();
            };

        } // namespace common
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_common_FileOpenCommand_h)
