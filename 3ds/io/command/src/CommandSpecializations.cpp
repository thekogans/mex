// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/util/Heap.h"
#include "thekogans/mex/3ds/io/command/CommandSpecializations.h"

namespace thekogans {
    namespace mex {
        namespace command {

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ViewSetCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ViewLayoutlayoutSetCommand, 128)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioProjectComplexTypeSwapCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioShaperComplexTypeSwapCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioLofterComplexTypeSwapCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioEditorComplexTypeSwapCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioKeyframerComplexTypeSwapCommand, 128)
            //THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioMaterialsEditorComplexTypeSwapCommand, 128)
            //THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (ioVideoPostComplexTypeSwapCommand, 128)

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (stdVectorViewpush_backCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (stdVectorViewassignCommand, 128)
            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (stdVectorViewclearCommand, 128)

        } // namespace command
    } // namespace mex
} // namespace thekogans
