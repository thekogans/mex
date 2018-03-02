// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_command_shaper_ShaperCommands_h)
#define __thekogans_mex_3ds_io_command_shaper_ShaperCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ShaperValidateAfterOpenCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ShaperValidateAfterOpenCommand)

                        io::Shaper &shaper;

                        explicit ShaperValidateAfterOpenCommand (io::Shaper &shaper_) :
                            shaper (shaper_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ShaperValidateBeforeSaveCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ShaperValidateBeforeSaveCommand)

                        io::Shaper &shaper;

                        ShaperValidateBeforeSaveCommand (io::Shaper &shaper_) :
                            shaper (shaper_) {}

                        virtual bool Execute ();
                    };

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_shaper_ShaperCommands_h)
