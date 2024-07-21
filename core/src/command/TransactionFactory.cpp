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

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Heap.h"
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/core/command/SetCurrModuleFinalOperation.h"
#include "thekogans/mex/core/command/DrawViewFinalOperation.h"
#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/core/command/TransactionFactory.h"

namespace thekogans {
    namespace mex {
        namespace core {
            namespace command {

                namespace {
                    // To minimize chatter, this transaction optimizes
                    // away redundant redraw final operations.
                    struct Transaction : public thekogans::mex::command::Transaction {
                        THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                        Transaction (
                            const std::string &name,
                            bool undoable,
                            bool committing) :
                            thekogans::mex::command::Transaction (name, undoable, committing) {}

                        virtual void Optimize () {
                            bool sawSetCurrModuleFinalOperation = false;
                            bool sawDrawViewLayoutFinalOperation = false;
                            bool sawFlipFramebufferFinalOperation = false;
                            {
                                for (std::size_t i = finalOperations.size (); i--;) {
                                    if (util::dynamic_refcounted_sharedptr_cast<SetCurrModuleFinalOperation> (
                                            finalOperations[i]) != 0) {
                                        if (!sawSetCurrModuleFinalOperation) {
                                            sawSetCurrModuleFinalOperation = true;
                                        }
                                        else {
                                            finalOperations.erase (finalOperations.begin () + i);
                                        }
                                    }
                                    else if (util::dynamic_refcounted_sharedptr_cast<DrawViewLayoutFinalOperation> (
                                            finalOperations[i]) != 0) {
                                        if (!sawDrawViewLayoutFinalOperation) {
                                            sawDrawViewLayoutFinalOperation = true;
                                        }
                                        else {
                                            finalOperations.erase (finalOperations.begin () + i);
                                        }
                                    }
                                    else if (util::dynamic_refcounted_sharedptr_cast<FlipFramebufferFinalOperation> (
                                            finalOperations[i]) != 0) {
                                        if (!sawFlipFramebufferFinalOperation) {
                                            sawFlipFramebufferFinalOperation = true;
                                        }
                                        else {
                                            finalOperations.erase (finalOperations.begin () + i);
                                        }
                                    }
                                }
                            }
                            {
                                std::vector<thekogans::mex::command::FinalOperation::SharedPtr> drawViewFinalOperations;
                                for (std::size_t i = finalOperations.size (); i--;) {
                                    if (util::dynamic_refcounted_sharedptr_cast<DrawViewFinalOperation> (
                                            finalOperations[i]) != 0) {
                                        if (sawDrawViewLayoutFinalOperation ||
                                                SawDrawViewFinalOperation (finalOperations[i],
                                                    drawViewFinalOperations)) {
                                            finalOperations.erase (finalOperations.begin () + i);
                                        }
                                        else {
                                            drawViewFinalOperations.push_back (finalOperations[i]);
                                        }
                                    }
                                }
                            }
                        }

                    private:
                        bool SawDrawViewFinalOperation (
                                thekogans::mex::command::FinalOperation::SharedPtr finalOperation,
                                const std::vector<thekogans::mex::command::FinalOperation::SharedPtr> &drawViewFinalOperations) {
                            DrawViewFinalOperation::SharedPtr drawViewFinalOperation1 =
                                util::dynamic_refcounted_sharedptr_cast<DrawViewFinalOperation> (finalOperation);
                            assert (drawViewFinalOperation1 != 0);
                            if (drawViewFinalOperation1 != 0) {
                                for (std::size_t i = 0, count = drawViewFinalOperations.size (); i < count; ++i) {
                                    DrawViewFinalOperation::SharedPtr drawViewFinalOperation2 =
                                        util::dynamic_refcounted_sharedptr_cast<DrawViewFinalOperation> (
                                            drawViewFinalOperations[i]);
                                    assert (drawViewFinalOperation2 != 0);
                                    if (drawViewFinalOperation2 != 0 &&
                                            &drawViewFinalOperation1->view == &drawViewFinalOperation2->view) {
                                        return true;
                                    }
                                }
                            }
                            return false;
                        }
                    };

                    THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Transaction)
                }

                thekogans::mex::command::Transaction::SharedPtr TransactionFactory::CreateTransaction () {
                    return thekogans::mex::command::Transaction::SharedPtr (
                        new Transaction (name, undoable, committing));
                }

            } // namespace command
        } // namespace core
    } // namespace mex
} // namespace thekogans
