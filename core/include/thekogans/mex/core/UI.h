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

#if !defined (__thekogans_mex_core_UI_h)
#define __thekogans_mex_core_UI_h

#include <memory>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // defined (TOOLCHAIN_OS_Windows)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QWidget>
#include "thekogans/util/Singleton.h"
#include "thekogans/util/Mutex.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/opengl/Framebuffer.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/Progress.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct MenuBar;
            struct ToolBar;
            struct ViewLayout;
            struct SpeedBar;
            struct StatusBar;

            struct _LIB_THEKOGANS_MEX_CORE_DECL UI : public util::Singleton<UI, util::Mutex> {
                thekogans::mex::opengl::Framebuffer::UniquePtr framebuffer;
                struct Window {
                    virtual ~Window () {}
                    virtual QWidget *GetQWidget () = 0;
                    virtual void Save (const char *keyName) {}
                    virtual void Restore (const char *keyName) {}
                    virtual void Show (bool show) {}
                    virtual void OnIdle () {}
                };
                struct MainFrameWindow : public  Window {
                    virtual bool IsMenuBarWindowVisible () const = 0;
                    virtual void ShowMenuBarWindow (bool show) = 0;
                    virtual bool IsToolBarWindowVisible () const = 0;
                    virtual void ShowToolBarWindow (bool show) = 0;
                    virtual bool IsSpeedBarWindowVisible () const = 0;
                    virtual void ShowSpeedBarWindow (bool show) = 0;
                    virtual bool IsConsoleWindowVisible () const = 0;
                    virtual void ShowConsoleWindow (bool show) = 0;
                    virtual bool IsStatusBarWindowVisible () const = 0;
                    virtual void ShowStatusBarWindow (bool show) = 0;
                } *mainFrameWindow;
                struct MenuBarWindow : public  Window {
                    virtual void SetMenuBar (MenuBar &menuBar) = 0;
                } *menuBarWindow;
                struct ToolBarWindow : public  Window {
                    virtual void SetToolBar (ToolBar &toolBar) = 0;
                } *toolBarWindow;
                struct ViewLayoutWindow : public  Window {
                    virtual void MakeCurrent () = 0;
                    virtual blas::Size GetSize () const = 0;
                    virtual core::ViewLayout *GetViewLayout () const = 0;
                    virtual void SetViewLayout (ViewLayout &viewLayout) = 0;
                    virtual void CaptureMouse () = 0;
                    virtual void ReleaseMouse () = 0;
                    virtual blas::Point GetMousePosition () const = 0;
                    virtual void SetMousePosition (const blas::Point &pt) const = 0;
                } *viewLayoutWindow;
                struct SpeedBarWindow : public  Window {
                    virtual void SetSpeedBar (SpeedBar &speedBar) = 0;
                } *speedBarWindow;
                struct ConsoleWindow : public  Window {
                    virtual void Print (const char *format, ...) = 0;
                } *consoleWindow;
                struct StatusBarWindow : public  Window {
                    virtual void SetStatusBar (StatusBar &statusBar) = 0;
                    virtual _3ds::io::Progress::UniquePtr CreateProgress () = 0;
                } *statusBarWindow;
                struct FrameBarWindow : public  Window {
                    struct EventSink {
                        virtual ~EventSink () {}
                        virtual bool IsVisible () const = 0;
                        virtual util::ui32 GetStartFrame () const = 0;
                        virtual util::ui32 GetEndFrame () const = 0;
                        virtual util::ui32 GetCurrFrame () const = 0;
                        virtual void GoToFrame (util::ui32 frame) = 0;
                        virtual bool IsBoxMode () const = 0;
                        virtual void SetBoxMode (bool boxMode) = 0;
                    };
                    virtual void SetEventSink (EventSink *eventSink) = 0;
                    virtual void SetModal (bool modal) = 0;
                } *frameBarWindow;
				
                UI () :
                    mainFrameWindow (0),
                    menuBarWindow (0),
                    toolBarWindow (0),
					viewLayoutWindow (0),
                    speedBarWindow (0),
                    consoleWindow (0),
					statusBarWindow (0),
                    frameBarWindow (0) {}
				
                void CreateFramebuffer (const blas::Size &size);
                void DestroyFramebuffer ();
                void FlipFramebuffer ();
            };

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_UI_h)
