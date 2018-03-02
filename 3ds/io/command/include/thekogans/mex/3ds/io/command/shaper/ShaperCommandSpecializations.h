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

#if !defined (__thekogans_mex_3ds_io_command_shaper_ShaperCommandSpecializations_h)
#define __thekogans_mex_3ds_io_command_shaper_ShaperCommandSpecializations_h

#include "thekogans/mex/command/stdVectorCommands.h"
#include "thekogans/mex/command/OwnerVectorCommands.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace command {

            typedef OwnerVectorpush_backCommand<_3ds::io::BezierPolygon2> OwnerVectorBezierPolygon2push_backCommand;
            typedef OwnerVectorpush_backCommand<_3ds::io::BezierPolygon3> OwnerVectorBezierPolygon3push_backCommand;
            typedef OwnerVectoreraseCommand<_3ds::io::BezierPolygon2> OwnerVectorBezierPolygon2eraseCommand;
            typedef OwnerVectoreraseCommand<_3ds::io::BezierPolygon3> OwnerVectorBezierPolygon3eraseCommand;
            typedef OwnerVectorclearCommand<_3ds::io::BezierPolygon2> OwnerVectorBezierPolygon2clearCommand;
            typedef OwnerVectorclearCommand<_3ds::io::BezierPolygon3> OwnerVectorBezierPolygon3clearCommand;

            typedef stdVectorpush_backCommand<_3ds::io::BezierPolygon2::Vertex> stdVectorBezierPolygon2Vertexpush_backCommand;
            typedef stdVectorpush_backCommand<_3ds::io::BezierPolygon3::Vertex> stdVectorBezierPolygon3Vertexpush_backCommand;
            typedef stdVectorassignCommand<_3ds::io::BezierPolygon2::Vertex> stdVectorBezierPolygon2VertexassignCommand;
            typedef stdVectorassignCommand<_3ds::io::BezierPolygon3::Vertex> stdVectorBezierPolygon3VertexassignCommand;
            typedef stdVectorinsertCommand<_3ds::io::BezierPolygon2::Vertex> stdVectorBezierPolygon2VertexinsertCommand;
            typedef stdVectorinsertCommand<_3ds::io::BezierPolygon3::Vertex> stdVectorBezierPolygon3VertexinsertCommand;
            typedef stdVectoreraseCommand<_3ds::io::BezierPolygon2::Vertex> stdVectorBezierPolygon2VertexeraseCommand;
            typedef stdVectoreraseCommand<_3ds::io::BezierPolygon3::Vertex> stdVectorBezierPolygon3VertexeraseCommand;

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_shaper_ShaperCommandSpecializations_h)
