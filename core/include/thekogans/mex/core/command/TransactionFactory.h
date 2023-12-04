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

#if !defined (__thekogans_mex_core_command_TransactionFactory_h)
#define __thekogans_mex_core_command_TransactionFactory_h

#include <memory>
#include <string>
#include "thekogans/mex/command/Transaction.h"
#include "thekogans/mex/command/TransactionFactory.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {
            namespace command {

                struct _LIB_THEKOGANS_MEX_CORE_DECL TransactionFactory :
                        public thekogans::mex::command::TransactionFactory {
                    std::string name;
                    bool undoable;
                    bool committing;

                    TransactionFactory (
                        const char *name_,
                        bool undoable_ = true,
                        bool committing_ = false) :
                        name (name_),
                        undoable (undoable_),
                        committing (committing_) {}

                    virtual thekogans::mex::command::Transaction::SharedPtr CreateTransaction ();
                };

            } // namespace command
        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_command_TransactionFactory_h)
