// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_opengl_Config_h)
#define __thekogans_mex_opengl_Config_h

#if !defined (__cplusplus)
    #error libthekogans_mex_opengl requires C++ compilation (use a .cpp suffix)
#endif // !defined (__cplusplus)

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
    // Renderbuffer
    #define GL_RENDERBUFFER_EXT 0x8D41
    #define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
    typedef void (APIENTRYP PFNGLGENRENDERBUFFERSEXTPROC)(GLsizei n, GLuint *renderbuffers);
    typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSEXTPROC)(GLsizei n, const GLuint *renderbuffers);
    typedef void (APIENTRYP PFNGLBINDRENDERBUFFEREXTPROC)(GLenum target, GLuint renderbuffer);
    typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEEXTPROC)
        (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    #define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
    #define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
    #define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
    #define GL_RENDERBUFFER_RED_SIZE_EXT 0x8D50
    #define GL_RENDERBUFFER_GREEN_SIZE_EXT 0x8D51
    #define GL_RENDERBUFFER_BLUE_SIZE_EXT 0x8D52
    #define GL_RENDERBUFFER_ALPHA_SIZE_EXT 0x8D53
    #define GL_RENDERBUFFER_DEPTH_SIZE_EXT 0x8D54
    #define GL_RENDERBUFFER_STENCIL_SIZE_EXT 0x8D55
    typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)
        (GLenum target, GLenum param, GLint *value);
    // Framebuffer
    #define GL_FRAMEBUFFER_EXT 0x8D40
    #define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
    #define GL_READ_FRAMEBUFFER_EXT 0x8CA8
    #define GL_READ_FRAMEBUFFER_BINDING_EXT 0x8CAA
    #define GL_DRAW_FRAMEBUFFER_EXT 0x8CA9
    #define GL_DRAW_FRAMEBUFFER_BINDING_EXT GL_FRAMEBUFFER_BINDING_EXT
    #define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
    #define GL_DEPTH_ATTACHMENT_EXT 0x8D00
    #define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
    typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSEXTPROC)(GLsizei n, GLuint *framebuffers);
    typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSEXTPROC)(GLsizei n, const GLuint *framebuffers);
    typedef void (APIENTRYP PFNGLBINDFRAMEBUFFEREXTPROC)(GLenum target, GLuint framebuffer);
    typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)(GLuint framebuffer);
    typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)
        (GLenum target, GLenum attachmentPoint, GLenum textureTarget, GLuint textureId, GLint level);
    typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)
        (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    typedef void (APIENTRYP PFNGLBLITFRAMEBUFFEREXTPROC)
        (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
            GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
    #define _LIB_THEKOGANS_MEX_OPENGL_API __stdcall
    #if defined (TOOLCHAIN_TYPE_Shared)
        #if defined (_LIB_THEKOGANS_MEX_OPENGL_BUILD)
            #define _LIB_THEKOGANS_MEX_OPENGL_DECL __declspec (dllexport)
        #else // defined (_LIB_THEKOGANS_MEX_OPENGL_BUILD)
            #define _LIB_THEKOGANS_MEX_OPENGL_DECL __declspec (dllimport)
        #endif // defined (_LIB_THEKOGANS_MEX_OPENGL_BUILD)
    #else // defined (TOOLCHAIN_TYPE_Shared)
        #define _LIB_THEKOGANS_MEX_OPENGL_DECL
    #endif // defined (TOOLCHAIN_TYPE_Shared)
    #pragma warning (disable: 4251)  // using non-exported as public in exported
    #pragma warning (disable: 4786)
#else // defined (TOOLCHAIN_OS_Windows)
    #if defined (TOOLCHAIN_OS_Linux)
        #define GL_GLEXT_PROTOTYPES
        #include <GL/gl.h>
        #include <GL/glu.h>
    #else // defined (TOOLCHAIN_OS_Linux)
        #define GL_GLEXT_PROTOTYPES
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
    #endif // defined (TOOLCHAIN_OS_Linux)
    #define _LIB_THEKOGANS_MEX_OPENGL_API
    #define _LIB_THEKOGANS_MEX_OPENGL_DECL
#endif // defined (TOOLCHAIN_OS_Windows)

#define THEKOGANS_MEX_OPENGL_DISALLOW_COPY_AND_ASSIGN(type)\
private:\
    type (const type &);\
    type &operator = (const type &);

#endif // !defined (__thekogans_mex_opengl_Config_h)
