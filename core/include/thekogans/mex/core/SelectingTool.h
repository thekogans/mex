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

#if !defined (__thekogans_mex_core_SelectingTool_h)
#define __thekogans_mex_core_SelectingTool_h

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #include "thekogans/util/os/windows/WindowsHeader.h"
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QWidget>
#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/core/Config.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/Tool.h"

namespace thekogans {
    namespace mex {
        namespace core {

            // Selecting tool.
            //
            // Use this class to derive a geometry selecting/manipulating tool.
            // Most tools which manipulate geometry should allow the user to
            // create and manipulate a selection set. This class defines base
            // functionality for performing window (rubber band) type selection.
            // The tool can be toggled between selecting and manipulating modes
            // by calling ToggleSelecting (). This way the user does not have to
            // choose a selecting tool to create a selection set and then choose
            // another tool to manipulate it.

            struct _LIB_THEKOGANS_MEX_CORE_DECL SelectingTool : public Tool {
            protected:
                bool selecting;         // Internal flag signaling that the
                                        // tool is in (window) selecting mode.

                const QCursor *cursor;  // Editor's current cursor. Durring
                                        // window selection, we replace Editor
                                        // cursor with CROSS_CURSOR. We save the
                                        // old cursor here for restoration.

                blas::Bound2 bound;     // Selection progress bound.

                bool unselect;          // CTRL key was held down durring
                                        // LButtonDown processing. Do unselect.

            public:
                explicit SelectingTool (Module &module) :
                    Tool (module) {}

                void ToggleSelecting ();

                // Description:
                //      Called by the framework when the tool looses focus,
                virtual void KillFocus ();

                // Description:
                //      Called by the framework in response to WM_LBUTTONDOWN message.
                // Parameters:
                //      view - view in which the message occured.
                //      flags - various key flags (FLAG_CTRL...).
                //      pt - point on view plane. To transform the point to world
                //          space, call view->V2W (pt).
                virtual void LButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_LBUTTONUP message.
                // Parameters:
                //      See LButtonDown.
                virtual void LButtonUp (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_RBUTTONDOWN message.
                // Parameters:
                //      See LButtonDown.
                virtual void RButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_RBUTTONUP message.
                // Parameters:
                //      See LButtonDown.
                virtual void RButtonUp (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_MOUSEMOVED message.
                // Parameters:
                //      See LButtonDown.
                virtual void MouseMove (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by a view after it has scrolled and redrawn the scene.
                virtual void UpdateUI (const _3ds::opengl::View &view);

                virtual bool Select (
                    const _3ds::opengl::View &view,
                    const blas::Bound2 &bound,
                    bool unselect) = 0;
            };

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_SelectingTool_h)
