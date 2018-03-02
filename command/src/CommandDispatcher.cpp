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

#include <memory>
#include <cassert>
#include "thekogans/mex/command/CommandDispatcher.h"

namespace thekogans {
    namespace mex {
        namespace command {

            bool CommandDispatcher::Undo () {
                if (IsTransactionPending ()) {
                    AbortTransaction ();
                }
                if (CanUndo () && retiredTransactions[currentTransaction - 1]->Undo ()) {
                    --currentTransaction;
                    return true;
                }
                return false;
            }

            bool CommandDispatcher::Redo () {
                if (IsTransactionPending ()) {
                    AbortTransaction ();
                }
                if (CanRedo () && retiredTransactions[currentTransaction]->Redo ()) {
                    ++currentTransaction;
                    return true;
                }
                return false;
            }

            bool CommandDispatcher::IsTransactionPending (const std::string &name) const {
                return !activeTransactions.empty () &&
                    (name.empty () || activeTransactions.back ()->name == name);
            }

            bool CommandDispatcher::IsPendingTransactionEmpty () const {
                return activeTransactions.empty () || activeTransactions.back ()->IsEmpty ();
            }

            void CommandDispatcher::BeginTransaction (
                    TransactionFactory::UniquePtr transactionFactory_) {
                transactionFactory = std::move (transactionFactory_);
                Transaction::UniquePtr transaction;
                if (transactionFactory.get () != 0) {
                    transaction = transactionFactory->CreateTransaction ();
                }
                else {
                    // FIXME: Pick a better name.
                    transaction.reset (new Transaction ("thekogans::command::Transaction"));
                }
                assert (transaction.get () != 0);
                activeTransactions.push_back (transaction.get ());
                transaction.release ();
            }

            void CommandDispatcher::CheckpointTransaction () {
                if (IsTransactionPending ()) {
                    CommitTransaction ();
                }
                BeginTransaction (std::move (transactionFactory));
            }

            void CommandDispatcher::RollbackTransaction () {
                if (IsTransactionPending ()) {
                    AbortTransaction ();
                }
                BeginTransaction (std::move (transactionFactory));
            }

            void CommandDispatcher::CommitTransaction () {
                assert (!activeTransactions.empty ());
                if (!activeTransactions.empty ()) {
                    Transaction::UniquePtr transaction (activeTransactions.back ());
                    activeTransactions.pop_back ();
                    if (!transaction->IsEmpty ()) {
                        if (!activeTransactions.empty ()) {
                            assert (transaction->IsUndoable ());
                            assert (!transaction->IsCommitting ());
                            activeTransactions.back ()->AddCommand (
                                Command::UniquePtr (transaction.release ()));
                        }
                        else if (transaction->IsUndoable ()) {
                            if (CanRedo ()) {
                                for (std::size_t i = currentTransaction,
                                        count = retiredTransactions.size (); i < count; ++i) {
                                    delete retiredTransactions[i];
                                }
                                retiredTransactions.erase (
                                    retiredTransactions.begin () + currentTransaction,
                                    retiredTransactions.end ());
                            }
                            transaction->Optimize ();
                            retiredTransactions.push_back (transaction.get ());
                            transaction.release ();
                            ++currentTransaction;
                        }
                        else if (transaction->IsCommitting ()) {
                            retiredTransactions.deleteAndClear ();
                            currentTransaction = 0;
                        }
                    }
                }
            }

            void CommandDispatcher::AbortTransaction () {
                assert (!activeTransactions.empty ());
                if (!activeTransactions.empty ()) {
                    Transaction::UniquePtr transaction (
                        activeTransactions.back ());
                    activeTransactions.pop_back ();
                    transaction->Optimize ();
                    transaction->Undo ();
                }
            }

            bool CommandDispatcher::ExecuteAndAddCommand (
                    Command::UniquePtr command) {
                assert (!activeTransactions.empty ());
                assert (command.get () != 0);
                if (!activeTransactions.empty () && command.get () != 0) {
                    if (command->Execute ()) {
                        activeTransactions.back ()->AddCommand (std::move (command));
                        return true;
                    }
                }
                return false;
            }

            bool CommandDispatcher::ExecuteAndAddFinalOperation (
                    FinalOperation::UniquePtr finalOperation) {
                assert (!activeTransactions.empty ());
                assert (finalOperation.get () != 0);
                if (!activeTransactions.empty () && finalOperation.get () != 0) {
                    if (finalOperation->Execute ()) {
                        activeTransactions.back ()->AddFinalOperation (std::move (finalOperation));
                        return true;
                    }
                }
                return false;
            }

        } // namespace command
    } // namespace mex
} // namespace thekogans
