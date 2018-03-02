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

#if !defined (__thekogans_mex_3ds_io_command_Version_h)
#define __thekogans_mex_3ds_io_command_Version_h

#include "thekogans/util/Version.h"
#include "thekogans/mex/3ds/io/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    /// \brief
                    /// Return the compiled 3ds_io_command version.
                    /// \return Compiled 3ds_io_command version.
                    _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL const util::Version &
                        _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_API GetVersion ();

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_Version_h)
