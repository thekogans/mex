// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_Config_h)
#define __thekogans_mex_3ds_io_Config_h

#if !defined (__cplusplus)
    #error libthekogans_mex_3ds_io requires C++ compilation (use a .cpp suffix)
#endif // !defined (__cplusplus)

#if defined (TOOLCHAIN_OS_Windows)
    #define _LIB_THEKOGANS_MEX_3DS_IO_API __stdcall
    #if defined (TOOLCHAIN_TYPE_Shared)
        #if defined (_LIB_THEKOGANS_MEX_3DS_IO_BUILD)
            #define _LIB_THEKOGANS_MEX_3DS_IO_DECL __declspec (dllexport)
        #else // defined (_LIB_THEKOGANS_MEX_3DS_IO_BUILD)
            #define _LIB_THEKOGANS_MEX_3DS_IO_DECL __declspec (dllimport)
        #endif // defined (_LIB_THEKOGANS_MEX_3DS_IO_BUILD)
    #else // defined (TOOLCHAIN_TYPE_Shared)
        #define _LIB_THEKOGANS_MEX_3DS_IO_DECL
    #endif // defined (TOOLCHAIN_TYPE_Shared)
    #pragma warning (disable: 4251)  // using non-exported as public in exported
    #pragma warning (disable: 4275)
    #pragma warning (disable: 4786)
#else // defined (TOOLCHAIN_OS_Windows)
    #define _LIB_THEKOGANS_MEX_3DS_IO_API
    #define _LIB_THEKOGANS_MEX_3DS_IO_DECL
#endif // defined (TOOLCHAIN_OS_Windows)

// Uncomment this line if you want to receive detailed
// ChunkReader/ChunkWriter exception information. By
// default the library will throw generic Stream
// ReadError, WriteError, TellError and SeekError. By
// uncommenting this line, ChunkReader/ChunkWriter will
// catch these generic Stream errors and will repackage
// them in more detailed ChunkReader::Error and
// ChunkWriter::Error. The price for this extra level
// of detail of course is performance.
//#define _LIB_THEKOGANS_MEX_3DS_IO_HANDLE_STREAM_EXCEPTIONS

#endif // !defined (__thekogans_mex_3ds_io_Config_h)
