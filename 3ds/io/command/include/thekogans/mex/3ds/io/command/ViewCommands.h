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

#if !defined (__thekogans_mex_3ds_io_command_ViewCommands_h)
#define __thekogans_mex_3ds_io_command_ViewCommands_h

#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewZoomInCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewZoomInCommand)

                        io::View &view;

                        explicit ViewZoomInCommand (io::View &view_) :
                            view (view_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewZoomOutCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewZoomOutCommand)

                        io::View &view;

                        explicit ViewZoomOutCommand (io::View &view_) :
                            view (view_) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL ViewZoomBoundCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (ViewZoomBoundCommand)

                        io::View &view;
                        blas::Bound2 bound;

                        ViewZoomBoundCommand (
                            io::View &view_,
                            const blas::Bound2 &bound_) :
                            view (view_),
                            bound (bound_) {}

                        virtual bool Execute ();
                    };

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_ViewCommands_h)
