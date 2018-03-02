// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_common.
//
// libthekogans_mex_common is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_common is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_common. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_common_ToolBarZoomWindowEventHandler_h)
#define __thekogans_mex_common_ToolBarZoomWindowEventHandler_h

#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/ToolBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/common/Config.h"

namespace thekogans {
    namespace mex {
        namespace common {

            struct _LIB_THEKOGANS_MEX_COMMON_DECL ToolBarZoomWindowEventHandler :
                    public core::ToolBar::Item::ToolEventHandler {
                struct _LIB_THEKOGANS_MEX_COMMON_DECL Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (ToolBarZoomWindowEventHandlerTool)

                    blas::Bound2 bound;

                    explicit Tool (core::Module &module) :
                        core::Tool (module) {}

                    virtual void SetFocus ();
                    virtual void LButtonDown (
                        const _3ds::opengl::View &view,
                        util::ui32 flags,
                        const blas::Point2 &pt);
                    virtual void LButtonUp (
                        const _3ds::opengl::View &,
                        util::ui32,
                        const blas::Point2 &);
                    virtual void RButtonDown (
                        const _3ds::opengl::View &view,
                        util::ui32 flags,
                        const blas::Point2 &);
                    virtual void RButtonUp (
                        const _3ds::opengl::View &,
                        util::ui32,
                        const blas::Point2 &);
                    virtual void MouseMove (
                        const _3ds::opengl::View &view,
                        util::ui32,
                        const blas::Point2 &pt);
                    virtual void UpdateUI (const _3ds::opengl::View &view);
                } tool;

                explicit ToolBarZoomWindowEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

        } // namespace common
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_common_ToolBarZoomWindowEventHandler_h)
