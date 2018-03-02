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

#include "thekogans/mex/command/SimpleTypeCommands.h"

namespace thekogans {
    namespace mex {
        namespace command {

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (boolSetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i8SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui8SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i16SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui16SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i32SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui32SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i64SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui64SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f32SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f64SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (GUIDSetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (FractionSetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (VariantSetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (Flags16SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (Flags32SetCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (Flags64SetCommand, 1024)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (boolSwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i8SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui8SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i16SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui16SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i32SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui32SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i64SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui64SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f32SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f64SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (GUIDSwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (FractionSwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (VariantSwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (Flags16SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (Flags32SwapCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (Flags64SwapCommand, 1024)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i8IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui8IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i16IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui16IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i32IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui32IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i64IncCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui64IncCommand, 1024)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i8AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui8AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i16AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui16AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i32AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui32AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i64AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui64AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f32AddCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f64AddCommand, 1024)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i8DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui8DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i16DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui16DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i32DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui32DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i64DecCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui64DecCommand, 1024)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i8SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui8SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i16SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui16SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i32SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui32SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (i64SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ui64SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f32SubCommand, 1024)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (f64SubCommand, 1024)

        } // namespace command
    } // namespace mex
} // namespace thekogans
