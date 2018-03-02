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

#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonVertexCommands.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2VertexSetPtCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3VertexSetPtCommand)

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2VertexSetRedCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3VertexSetRedCommand)

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2VertexSetYellowCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3VertexSetYellowCommand)

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2VertexSetFlagsCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3VertexSetFlagsCommand)

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2InsertVertexCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3InsertVertexCommand)

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon2DeleteVertexCommand)
                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_T (BezierPolygon3DeleteVertexCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
