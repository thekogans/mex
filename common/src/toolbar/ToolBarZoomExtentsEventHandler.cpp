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

#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/core/command/TransactionFactory.h"
#include "thekogans/mex/core/command/DrawViewFinalOperation.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomExtentsEventHandler.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace common {

            void ToolBarZoomExtentsEventHandler::OnClickLeft () {
                core::GetCommandDispatcher ().BeginTransaction (
                    TransactionFactory::UniquePtr (
                        new core::command::TransactionFactory (
                            "ToolBarZoomExtentsViewEventHandler")));
                _3ds::io::ViewLayout &viewLayout = module.GetIOModule ().viewLayout;
                _3ds::io::View &view = viewLayout[viewLayout.layout.currViewIndex];
                _3ds::ext::View::UniquePtr extView (module.GetExtModule ().GetView (view));
                assert (extView.get () != 0);
                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                    Command::UniquePtr (
                        new _3ds::io::command::ViewZoomBoundCommand (view,
                            module.GetExtModule ().GetViewBound (*extView))));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::UniquePtr (
                        new core::command::DrawViewFinalOperation (module, view)));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::UniquePtr (
                        new core::command::FlipFramebufferFinalOperation));
                core::GetCommandDispatcher ().CommitTransaction ();
            }

            void ToolBarZoomExtentsEventHandler::OnClickRight () {
                core::GetCommandDispatcher ().BeginTransaction (
                    TransactionFactory::UniquePtr (
                        new core::command::TransactionFactory (
                            "ToolBarZoomExtentsViewLayoutEventHandler")));
                _3ds::io::ViewLayout &viewLayout = module.GetIOModule ().viewLayout;
                for (util::ui32 i = 0, count = viewLayout.GetViewCount (); i < count; ++i) {
                    _3ds::io::View &view = viewLayout[i];
                    _3ds::ext::View::UniquePtr extView (module.GetExtModule ().GetView (view));
                    assert (extView.get () != 0);
                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                        Command::UniquePtr (
                            new _3ds::io::command::ViewZoomBoundCommand (
                                view, module.GetExtModule ().GetViewBound (*extView))));
                }
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::UniquePtr (
                        new core::command::DrawViewLayoutFinalOperation (module)));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::UniquePtr (
                        new core::command::FlipFramebufferFinalOperation));
                core::GetCommandDispatcher ().CommitTransaction ();
            }

        } // namespace common
    } // namespace mex
} // namespace thekogans
