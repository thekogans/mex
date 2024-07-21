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

#if !defined (__thekogans_mex_core_command_FlipFramebufferFinalOperation_h)
#define __thekogans_mex_core_command_FlipFramebufferFinalOperation_h

#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/core/UI.h"

namespace thekogans {
    namespace mex {
        namespace core {
            namespace command {

                struct _LIB_THEKOGANS_MEX_CORE_DECL FlipFramebufferFinalOperation :
                        public thekogans::mex::command::FinalOperation {
                    THEKOGANS_MEX_COMMAND_DECLARE_FINAL_OPERATION (FlipFramebufferFinalOperation)

                    virtual bool Execute () {
                        if (UI::Instance ()->framebuffer.get () != 0) {
                            UI::Instance ()->FlipFramebuffer ();
                            return true;
                        }
                        return false;
                    }
                };

            } // namespace command
        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_command_FlipFramebufferFinalOperation_h)
