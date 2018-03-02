// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext_command.
//
// libthekogans_mex_3ds_ext_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_ext_command_lofter_LofterCommands_h)
#define __thekogans_mex_3ds_ext_command_lofter_LofterCommands_h

#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/ext/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {
                namespace command {

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_COMMAND_DECL LofterValidateAfterOpenCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (LofterValidateAfterOpenCommand)

                        ext::Lofter &lofter;
                        ext::Lofter newLofter;

                        explicit LofterValidateAfterOpenCommand (ext::Lofter &lofter_) :
                            lofter (lofter_), newLofter (lofter.ioLofter ()) {}

                        virtual bool Execute ();
                    };

                } // namespace command
            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_command_lofter_LofterCommands_h)
