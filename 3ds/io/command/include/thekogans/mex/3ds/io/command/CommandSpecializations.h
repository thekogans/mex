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

#if !defined (__thekogans_mex_3ds_io_command_CommandSpecializations_h)
#define __thekogans_mex_3ds_io_command_CommandSpecializations_h

#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/command/ComplexTypeCommands.h"
#include "thekogans/mex/command/stdVectorCommands.h"
#include "thekogans/mex/command/OwnerVectorCommands.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
//#include "thekogans/mex/3ds/io/materialseditor/MaterialsEditor.h"
//#include "thekogans/mex/3ds/io/videopost/VideoPost.h"

namespace thekogans {
    namespace mex {
        namespace command {

            typedef SimpleTypeSetCommand<mex::_3ds::io::View> ViewSetCommand;
            typedef SimpleTypeSetCommand<mex::_3ds::io::ViewLayout::Layout> ViewLayoutlayoutSetCommand;

            typedef ComplexTypeSwapCommand<mex::_3ds::io::Project> ioProjectComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<mex::_3ds::io::Shaper> ioShaperComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<mex::_3ds::io::Lofter> ioLofterComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<mex::_3ds::io::Editor> ioEditorComplexTypeSwapCommand;
            typedef ComplexTypeSwapCommand<mex::_3ds::io::Keyframer> ioKeyframerComplexTypeSwapCommand;
            //typedef ComplexTypeSwapCommand<mex::_3ds::io::MaterialsEditor> ioMaterialsEditorComplexTypeSwapCommand;
            //typedef ComplexTypeSwapCommand<mex::_3ds::io::VideoPost> ioVideoPostComplexTypeSwapCommand;

            typedef stdVectorpush_backCommand<mex::_3ds::io::View> stdVectorViewpush_backCommand;
            typedef stdVectorassignCommand<mex::_3ds::io::View> stdVectorViewassignCommand;
            typedef stdVectorclearCommand<mex::_3ds::io::View> stdVectorViewclearCommand;

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_CommandSpecializations_h)
