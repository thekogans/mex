// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_command.
//
// libthekogans_mex_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_command_CommandDispatcher_h)
#define __thekogans_mex_command_CommandDispatcher_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/mex/command/Config.h"
#include "thekogans/mex/command/Command.h"
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/command/Transaction.h"
#include "thekogans/mex/command/TransactionFactory.h"

namespace thekogans {
    namespace mex {
        namespace command {

            struct _LIB_THEKOGANS_MEX_COMMAND_DECL CommandDispatcher {
            protected:
                TransactionFactory::SharedPtr transactionFactory;
                std::vector<Transaction::SharedPtr> activeTransactions;
                std::vector<Transaction::SharedPtr> retiredTransactions;
                std::size_t currentTransaction;

            public:
                CommandDispatcher () :
                    currentTransaction (0) {}

                bool Undo ();
                bool Redo ();

                inline bool CanUndo () const {
                    return currentTransaction > 0;
                }
                inline bool CanRedo () const {
                    return currentTransaction < retiredTransactions.size ();
                }

                bool IsTransactionPending (const std::string &name = std::string ()) const;
                bool IsPendingTransactionEmpty () const;
                // Transactions can nest.
                void BeginTransaction (
                    TransactionFactory::SharedPtr transactionFactory_ = TransactionFactory::SharedPtr ());
                void CheckpointTransaction ();
                void RollbackTransaction ();
                void CommitTransaction ();
                void AbortTransaction ();

                // NOTE: These methods are only available between BeginTransaction/CommitTransaction calls.
                // NOTE: The Return value/Parameter idiom used throughout thekogans* is:
                //      - raw pointer = owned by callee
                //      - std::unique_ptr = owned by caller
                bool ExecuteAndAddCommand (Command::SharedPtr command);
                bool ExecuteAndAddFinalOperation (FinalOperation::SharedPtr finalOperation);
            };

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_command_CommandDispatcher_h)
