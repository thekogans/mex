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

#if !defined (__thekogans_mex_3ds_io_command_Config_h)
#define __thekogans_mex_3ds_io_command_Config_h

#if !defined (__cplusplus)
    #error libthekogans_mex_3ds_io_command requires C++ compilation (use a .cpp suffix)
#endif // !defined (__cplusplus)

#include "thekogans/util/Environment.h"

#if defined (TOOLCHAIN_OS_Windows)
    #define _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_API __stdcall
    #if defined (THEKOGANS_MEX_3DS_IO_COMMAND_TYPE_Shared)
        #if defined (_LIB_THEKOGANS_MEX_3DS_IO_COMMAND_BUILD)
            #define _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL __declspec (dllexport)
        #else // defined (_LIB_THEKOGANS_MEX_3DS_IO_COMMAND_BUILD)
            #define _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL __declspec (dllimport)
        #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_COMMAND_BUILD)
    #else // defined (THEKOGANS_MEX_3DS_IO_COMMAND_TYPE_Shared)
        #define _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL
    #endif // defined (THEKOGANS_MEX_3DS_IO_COMMAND_TYPE_Shared)
    #pragma warning (disable: 4251)  // using non-exported as public in exported
    #pragma warning (disable: 4786)
#else // defined (TOOLCHAIN_OS_Windows)
    #define _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_API
    #define _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL
#endif // defined (TOOLCHAIN_OS_Windows)

#endif // !defined (__thekogans_mex_3ds_io_command_Config_h)
