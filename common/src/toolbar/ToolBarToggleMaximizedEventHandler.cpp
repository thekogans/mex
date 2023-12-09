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

#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/TransactionFactory.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/common/toolbar/ToolBarToggleMaximizedEventHandler.h"

using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace common {

            namespace {
                class ToggleMaximizedCommand : public CompoundCommand {
                    THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ToggleMaximizedCommand)

                private:
                    core::Module &module;

                public:
                    explicit ToggleMaximizedCommand (core::Module &module_) :
                        module (module_) {}

                    virtual bool Execute () {
                        ExecuteAndAddCommand (
                            Command::SharedPtr (
                                new _3ds::io::command::ViewLayoutToggleMaximizedCommand (
                                    module.GetIOModule ().viewLayout)));
                        ExecuteAndAddFinalOperation (
                            FinalOperation::SharedPtr (
                                new core::command::DrawViewLayoutFinalOperation (module)));
                        ExecuteAndAddFinalOperation (
                            FinalOperation::SharedPtr (
                                new core::command::FlipFramebufferFinalOperation));
                        return true;
                    }
                };

                THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ToggleMaximizedCommand)
            }

            void ToolBarToggleMaximizedEventHandler::OnClickLeft () {
                core::GetCommandDispatcher ().BeginTransaction (
                    TransactionFactory::SharedPtr (
                        new core::command::TransactionFactory (
                            "ToolBarToggleMaximizedEventHandler")));
                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                    Command::SharedPtr (
                        new ToggleMaximizedCommand (module)));
                core::GetCommandDispatcher ().CommitTransaction ();
            }

        } // namespace common
    } // namespace mex
} // namespace thekogans
