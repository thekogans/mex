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

#if !defined (__thekogans_mex_3ds_io_command_ViewLayoutCommands_h)
#define __thekogans_mex_3ds_io_command_ViewLayoutCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewLayoutSetSizeCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewLayoutSetSizeCommand)

                        io::ViewLayout &viewLayout;
                        blas::Size size;

                        ViewLayoutSetSizeCommand (
                            io::ViewLayout &viewLayout_,
                            const blas::Size &size_) :
                            viewLayout (viewLayout_),
                            size (size_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewLayoutSetCurrViewCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewLayoutSetCurrViewCommand)

                        io::ViewLayout &viewLayout;
                        util::ui16 currViewIndex;

                        ViewLayoutSetCurrViewCommand (
                            io::ViewLayout &viewLayout_,
                            util::ui16 currViewIndex_) :
                            viewLayout (viewLayout_),
                            currViewIndex (currViewIndex_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewLayoutToggleMaximizedCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewLayoutToggleMaximizedCommand)

                        io::ViewLayout &viewLayout;

                        explicit ViewLayoutToggleMaximizedCommand (
                            io::ViewLayout &viewLayout_) :
                            viewLayout (viewLayout_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewLayoutZoomInCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewLayoutZoomInCommand)

                        io::ViewLayout &viewLayout;

                        explicit ViewLayoutZoomInCommand (io::ViewLayout &viewLayout_) :
                            viewLayout (viewLayout_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewLayoutZoomOutCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewLayoutZoomOutCommand)

                        io::ViewLayout &viewLayout;

                        explicit ViewLayoutZoomOutCommand (io::ViewLayout &viewLayout_) :
                            viewLayout (viewLayout_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewLayoutZoomBoundCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewLayoutZoomBoundCommand)

                        io::ViewLayout &viewLayout;
                        blas::Bound2 bound;

                        ViewLayoutZoomBoundCommand (
                            io::ViewLayout &viewLayout_,
                            const blas::Bound2 &bound_) :
                            viewLayout (viewLayout_),
                            bound (bound_) {}

                        virtual bool Execute ();
                    };

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_ViewLayoutCommands_h)
