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

#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/core/command/TransactionFactory.h"
#include "thekogans/mex/core/command/DrawViewFinalOperation.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/common/toolbar/ToolBarZoomOutEventHandler.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace common {

            void ToolBarZoomOutEventHandler::OnClickLeft () {
                core::GetCommandDispatcher ().BeginTransaction (
                    TransactionFactory::SharedPtr (
                        new core::command::TransactionFactory (
                            "ToolBarZoomOutViewEventHandler")));
                _3ds::io::ViewLayout &viewLayout = module.GetIOModule ().viewLayout;
                _3ds::io::View &view = viewLayout[viewLayout.layout.currViewIndex];
                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                    Command::SharedPtr (
                        new _3ds::io::command::ViewZoomOutCommand (view)));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::SharedPtr (
                        new core::command::DrawViewFinalOperation (module, view)));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::SharedPtr (
                        new core::command::FlipFramebufferFinalOperation));
                core::GetCommandDispatcher ().CommitTransaction ();
            }

            void ToolBarZoomOutEventHandler::OnClickRight () {
                core::GetCommandDispatcher ().BeginTransaction (
                    TransactionFactory::SharedPtr (
                        new core::command::TransactionFactory (
                            "ToolBarZoomOutViewLayoutEventHandler")));
                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                    Command::SharedPtr (
                        new _3ds::io::command::ViewLayoutZoomOutCommand (
                            module.GetIOModule ().viewLayout)));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::SharedPtr (
                        new core::command::DrawViewLayoutFinalOperation (module)));
                core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                    FinalOperation::SharedPtr (
                        new core::command::FlipFramebufferFinalOperation));
                core::GetCommandDispatcher ().CommitTransaction ();
            }
        };

    } // namespace mex
} // namespace thekogans
