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

#if !defined (__thekogans_mex_3ds_io_command_shaper_BezierPolygonSegmentCommands_h)
#define __thekogans_mex_3ds_io_command_shaper_BezierPolygonSegmentCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Bound.h"
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
                    struct BezierPolygonSegmentLinearCommand : public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonSegmentLinearCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t vertexIndex1;
                        std::size_t vertexIndex2;

                        BezierPolygonSegmentLinearCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t vertexIndex1_,
                                std::size_t vertexIndex2_) :
                                bezierPolygon (bezierPolygon_),
                                vertexIndex1 (vertexIndex1_),
                                vertexIndex2 (vertexIndex2_) {
                            assert (vertexIndex1 <= bezierPolygon.vertices.size ());
                            assert (vertexIndex2 <= bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            ExecuteAndAddCommand (
                                thekogans::mex::command::Command::UniquePtr (
                                    new BezierPolygonVertexSetYellowCommand<T> (
                                        bezierPolygon, vertexIndex1, T::Zero)));
                            ExecuteAndAddCommand (
                                thekogans::mex::command::Command::UniquePtr (
                                    new BezierPolygonVertexSetRedCommand<T> (
                                        bezierPolygon, vertexIndex2, T::Zero)));
                            return true;
                        }
                    };

                    typedef BezierPolygonSegmentLinearCommand<blas::Point2> BezierPolygon2SegmentLinearCommand;
                    typedef BezierPolygonSegmentLinearCommand<blas::Point3> BezierPolygon3SegmentLinearCommand;

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL BezierPolygon2SegmentCurveCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygon2SegmentCurveCommand)

                        io::BezierPolygon2 &bezierPolygon;
                        std::size_t vertexIndex1;
                        std::size_t vertexIndex2;

                        BezierPolygon2SegmentCurveCommand (
                                io::BezierPolygon2 &bezierPolygon_,
                                std::size_t vertexIndex1_,
                                std::size_t vertexIndex2_) :
                                bezierPolygon (bezierPolygon_),
                                vertexIndex1 (vertexIndex1_),
                                vertexIndex2 (vertexIndex2_) {
                            assert (vertexIndex1 < bezierPolygon.vertices.size ());
                            assert (vertexIndex2 < bezierPolygon.vertices.size ());
                            assert (bezierPolygon.vertices.size () > 1);
                        }

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL BezierPolygon2SegmentRefineCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygon2SegmentRefineCommand)

                        io::BezierPolygon2 &bezierPolygon;
                        std::size_t vertexIndex1;
                        std::size_t vertexIndex2;
                        const blas::Bound2 bound;
                        util::ui32 steps;

                        BezierPolygon2SegmentRefineCommand (
                                io::BezierPolygon2 &bezierPolygon_,
                                std::size_t vertexIndex1_,
                                std::size_t vertexIndex2_,
                                const blas::Bound2 bound_,
                                util::ui32 steps_) :
                                bezierPolygon (bezierPolygon_),
                                vertexIndex1 (vertexIndex1_),
                                vertexIndex2 (vertexIndex2_),
                                bound (bound_),
                                steps (steps_) {
                            assert (vertexIndex1 < bezierPolygon.vertices.size ());
                            assert (vertexIndex2 < bezierPolygon.vertices.size ());
                            assert (bezierPolygon.vertices.size () > 1);
                        }

                        virtual bool Execute ();
                    };

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_shaper_BezierPolygonVertexCommands_h)
