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

#if !defined (__thekogans_mex_3ds_io_command_shaper_BezierPolygonCommands_h)
#define __thekogans_mex_3ds_io_command_shaper_BezierPolygonCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/command/Command.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/Config.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonVertexCommands.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    template<typename T>
                    struct BezierPolygonSetFlagsCommand : public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonSetFlagsCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        util::ui16 flags;

                        BezierPolygonSetFlagsCommand (
                            io::BezierPolygon<T> &bezierPolygon_,
                            util::ui16 flags_) :
                            bezierPolygon (bezierPolygon_),
                            flags (flags_) {}

                        virtual bool Execute () {
                            std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                bezierPolygon.vertices;
                            for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                if (!util::Flags16 (vertices[i].flags).Test (flags)) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::SharedPtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon, i, vertices[i].flags | flags)));
                                }
                            }
                            return !commands.empty ();
                        }
                    };

                    typedef BezierPolygonSetFlagsCommand<blas::Point2> BezierPolygon2SetFlagsCommand;
                    typedef BezierPolygonSetFlagsCommand<blas::Point3> BezierPolygon3SetFlagsCommand;

                    template<typename T>
                    struct BezierPolygonClearFlagsCommand : public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonClearFlagsCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        util::ui16 flags;

                        BezierPolygonClearFlagsCommand (
                            io::BezierPolygon<T> &bezierPolygon_,
                            util::ui16 flags_) :
                            bezierPolygon (bezierPolygon_),
                            flags (flags_) {}

                        virtual bool Execute () {
                            std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                bezierPolygon.vertices;
                            for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                if (util::Flags16 (vertices[i].flags).TestAny (flags)) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::SharedPtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon, i, vertices[i].flags & ~flags)));
                                }
                            }
                            return !commands.empty ();
                        }
                    };

                    typedef BezierPolygonClearFlagsCommand<blas::Point2> BezierPolygon2ClearFlagsCommand;
                    typedef BezierPolygonClearFlagsCommand<blas::Point3> BezierPolygon3ClearFlagsCommand;

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_shaper_BezierPolygonCommands_h)
