// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl_command.
//
// libthekogans_mex_3ds_opengl_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_command_shaper_ShaperCommands_h)
#define __thekogans_mex_3ds_opengl_command_shaper_ShaperCommands_h

#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/opengl/shaper/Shaper.h"
#include "thekogans/mex/3ds/opengl/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {
                namespace command {

                    struct _LIB_THEKOGANS_MEX_3DS_OPENGL_COMMAND_DECL ShaperValidateAfterOpenCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ShaperValidateAfterOpenCommand)

                        opengl::Shaper &shaper;
                        opengl::Shaper newShaper;

                        explicit ShaperValidateAfterOpenCommand (opengl::Shaper &shaper_) :
                            shaper (shaper_), newShaper (shaper.extShaper ()) {}

                        virtual bool Execute ();
                    };

                } // namespace command
            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_command_shaper_ShaperCommands_h)
