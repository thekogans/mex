// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/core/ToolBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/common/Config.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct ToolBarSelectionSetCEventHandler : public core::ToolBar::Item::ButtonEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarSelectionSetCEventHandler)

                core::Module &module;

                explicit ToolBarSelectionSetCEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnClickLeft () {
                    core::GetIOProject ().shaper.selectMask = _3ds::io::BezierPolygon2::Vertex::SelectionSetC;
                    core::GetIOProject ().shaper.unselectMask = ~_3ds::io::BezierPolygon2::Vertex::SelectionSetC;
                    core::command::DrawViewLayoutFinalOperation (module).Execute ();
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
                virtual bool IsChecked () const {
                    return core::GetIOProject ().shaper.selectMask == _3ds::io::BezierPolygon2::Vertex::SelectionSetC;
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_TOOLBAR_ITEM_EVENT_HANDLER (ToolBarSelectionSetCEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
