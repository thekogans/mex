// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_Config_h)
#define __thekogans_mex_3ds_opengl_Config_h

#if !defined (__cplusplus)
    #error libthekogans_mex_3ds_opengl requires C++ compilation (use a .cpp suffix)
#endif // !defined (__cplusplus)

#include "thekogans/util/Environment.h"

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (_WINDOWS_)
        #if !defined (WIN32_LEAN_AND_MEAN)
            #define WIN32_LEAN_AND_MEAN
        #endif // !defined (WIN32_LEAN_AND_MEAN)
        #if !defined (NOMINMAX)
            #define NOMINMAX
        #endif // !defined (NOMINMAX)
        #include <windows.h>
    #endif // !defined (_WINDOWS_)
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include "thekogans/mex/opengl/glext.h"
    extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
    extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
    extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
    extern PFNGLISBUFFERARBPROC glIsBufferARB;
    extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
    extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
    extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
    extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
    extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
    extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
    extern PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;
    #define _LIB_THEKOGANS_MEX_3DS_OPENGL_API __stdcall
    #if defined (THEKOGANS_MEX_3DS_OPENGL_TYPE_Shared)
        #if defined (_LIB_THEKOGANS_MEX_3DS_OPENGL_BUILD)
            #define _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL __declspec (dllexport)
        #else // defined (_LIB_THEKOGANS_MEX_3DS_OPENGL_BUILD)
            #define _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL __declspec (dllimport)
        #endif // defined (_LIB_THEKOGANS_MEX_3DS_OPENGL_BUILD)
    #else // defined (THEKOGANS_MEX_3DS_OPENGL_TYPE_Shared)
        #define _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL
    #endif // defined (THEKOGANS_MEX_3DS_OPENGL_TYPE_Shared)
    #pragma warning (disable: 4251)  // using non-exported as public in exported
    #pragma warning (disable: 4786)
#else // defined (TOOLCHAIN_OS_Windows)
    #if defined (TOOLCHAIN_OS_Linux)
        #include <GL/gl.h>
        #include <GL/glu.h>
    #else // defined (TOOLCHAIN_OS_Linux)
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
    #endif // defined (TOOLCHAIN_OS_Linux)
    #define _LIB_THEKOGANS_MEX_3DS_OPENGL_API
    #define _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL
#endif // defined (TOOLCHAIN_OS_Windows)

#endif // !defined (__thekogans_mex_3ds_opengl_Config_h)
