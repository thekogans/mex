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

#if !defined (__thekogans_mex_3ds_io_command_shaper_BezierPolygonVertexCommands_h)
#define __thekogans_mex_3ds_io_command_shaper_BezierPolygonVertexCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/command/Command.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/command/stdVectorCommands.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    template<typename T>
                    struct BezierPolygonVertexSetPtCommand : public thekogans::mex::command::Command {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonVertexSetPtCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t index;
                        T pt;

                        BezierPolygonVertexSetPtCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t index_,
                                const T &pt_) :
                                bezierPolygon (bezierPolygon_),
                                index (index_),
                                pt (pt_) {
                            assert (index < bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            return Redo ();
                        }
                        virtual bool Undo () {
                            return Redo ();
                        }
                        virtual bool Redo () {
                            std::swap (bezierPolygon.vertices[index].pt, pt);
                            return true;
                        }
                    };

                    typedef BezierPolygonVertexSetPtCommand<blas::Point2> BezierPolygon2VertexSetPtCommand;
                    typedef BezierPolygonVertexSetPtCommand<blas::Point3> BezierPolygon3VertexSetPtCommand;

                    template<typename T>
                    struct BezierPolygonVertexSetRedCommand : public thekogans::mex::command::Command {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonVertexSetRedCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t index;
                        T red;

                        BezierPolygonVertexSetRedCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t index_,
                                const T &red_) :
                                bezierPolygon (bezierPolygon_),
                                index (index_),
                                red (red_) {
                            assert (index < bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            return Redo ();
                        }
                        virtual bool Undo () {
                            return Redo ();
                        }
                        virtual bool Redo () {
                            std::swap (bezierPolygon.vertices[index].red, red);
                            return true;
                        }
                    };

                    typedef BezierPolygonVertexSetRedCommand<blas::Point2> BezierPolygon2VertexSetRedCommand;
                    typedef BezierPolygonVertexSetRedCommand<blas::Point3> BezierPolygon3VertexSetRedCommand;

                    template<typename T>
                    struct BezierPolygonVertexSetYellowCommand : public thekogans::mex::command::Command {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonVertexSetYellowCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t index;
                        T yellow;

                        BezierPolygonVertexSetYellowCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t index_,
                                const T &yellow_) :
                                bezierPolygon (bezierPolygon_),
                                index (index_),
                                yellow (yellow_) {
                            assert (index < bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            return Redo ();
                        }
                        virtual bool Undo () {
                            return Redo ();
                        }
                        virtual bool Redo () {
                            std::swap (bezierPolygon.vertices[index].yellow, yellow);
                            return true;
                        }
                    };

                    typedef BezierPolygonVertexSetYellowCommand<blas::Point2> BezierPolygon2VertexSetYellowCommand;
                    typedef BezierPolygonVertexSetYellowCommand<blas::Point3> BezierPolygon3VertexSetYellowCommand;

                    template<typename T>
                    struct BezierPolygonVertexSetFlagsCommand : public thekogans::mex::command::Command {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonVertexSetFlagsCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t index;
                        util::ui16 flags;

                        BezierPolygonVertexSetFlagsCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t index_,
                                util::ui16 flags_) :
                                bezierPolygon (bezierPolygon_),
                                index (index_),
                                flags (flags_) {
                            assert (index < bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            return Redo ();
                        }
                        virtual bool Undo () {
                            return Redo ();
                        }
                        virtual bool Redo () {
                            std::swap (bezierPolygon.vertices[index].flags, flags);
                            return true;
                        }
                    };

                    typedef BezierPolygonVertexSetFlagsCommand<blas::Point2> BezierPolygon2VertexSetFlagsCommand;
                    typedef BezierPolygonVertexSetFlagsCommand<blas::Point3> BezierPolygon3VertexSetFlagsCommand;

                    template<typename T>
                    struct BezierPolygonInsertVertexCommand : public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonInsertVertexCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t index;
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex vertex;

                        BezierPolygonInsertVertexCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t index_,
                                const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &vertex_) :
                                bezierPolygon (bezierPolygon_),
                                index (index_),
                                vertex (vertex_) {
                            assert (index <= bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            ExecuteAndAddCommand (
                                thekogans::mex::command::Command::UniquePtr (
                                    new thekogans::mex::command::stdVectorinsertCommand<
                                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> (
                                        bezierPolygon.vertices, index, vertex)));
                            if (index == 0) {
                                if (!bezierPolygon.vertices[0].IsFirst ()) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                0,
                                                bezierPolygon.vertices[0].flags |
                                                io::BezierPolygon<T>::Vertex::First)));
                                }
                                if (bezierPolygon.vertices.size () > 1 &&
                                        bezierPolygon.vertices[1].IsFirst ()) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                1,
                                                bezierPolygon.vertices[1].flags &
                                                ~io::BezierPolygon<T>::Vertex::First)));
                                }
                            }
                            if (index == bezierPolygon.vertices.size () - 1) {
                                if (!bezierPolygon.vertices[index].IsLast ()) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                index,
                                                bezierPolygon.vertices[index].flags |
                                                io::BezierPolygon<T>::Vertex::Last)));
                                }
                                if (bezierPolygon.vertices.size () > 1 &&
                                        bezierPolygon.vertices[index - 1].IsLast ()) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                index - 1,
                                                bezierPolygon.vertices[index - 1].flags &
                                                ~io::BezierPolygon<T>::Vertex::Last)));
                                }
                                if (bezierPolygon.vertices[index - 1].IsClosed ()) {
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                index - 1,
                                                bezierPolygon.vertices[index - 1].flags &
                                                ~io::BezierPolygon<T>::Vertex::Closed)));
                                    if (!bezierPolygon.vertices[index].IsClosed ()) {
                                        ExecuteAndAddCommand (
                                            thekogans::mex::command::Command::UniquePtr (
                                                new BezierPolygonVertexSetFlagsCommand<T> (
                                                    bezierPolygon,
                                                    index,
                                                    bezierPolygon.vertices[index].flags |
                                                    io::BezierPolygon<T>::Vertex::Closed)));
                                    }
                                }
                            }
                            return true;
                        }
                    };

                    typedef BezierPolygonInsertVertexCommand<blas::Point2> BezierPolygon2InsertVertexCommand;
                    typedef BezierPolygonInsertVertexCommand<blas::Point3> BezierPolygon3InsertVertexCommand;

                    template<typename T>
                    struct BezierPolygonDeleteVertexCommand : public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonDeleteVertexCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        std::size_t index;

                        BezierPolygonDeleteVertexCommand (
                                io::BezierPolygon<T> &bezierPolygon_,
                                std::size_t index_) :
                                bezierPolygon (bezierPolygon_),
                                index (index_) {
                            assert (index < bezierPolygon.vertices.size ());
                        }

                        virtual bool Execute () {
                            bool closed = bezierPolygon.vertices[index].IsClosed ();
                            ExecuteAndAddCommand (
                                thekogans::mex::command::Command::UniquePtr (
                                    new thekogans::mex::command::stdVectoreraseCommand<
                                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> (
                                            bezierPolygon.vertices, index)));
                            if (!bezierPolygon.vertices.empty ()) {
                                if (index == 0) {
                                    assert (!bezierPolygon.vertices[0].IsFirst ());
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                0,
                                                bezierPolygon.vertices[0].flags |
                                                io::BezierPolygon<T>::Vertex::First)));
                                }
                                if (index == bezierPolygon.vertices.size ()) {
                                    assert (!bezierPolygon.vertices[index - 1].IsLast ());
                                    ExecuteAndAddCommand (
                                        thekogans::mex::command::Command::UniquePtr (
                                            new BezierPolygonVertexSetFlagsCommand<T> (
                                                bezierPolygon,
                                                index - 1,
                                                bezierPolygon.vertices[index - 1].flags |
                                                io::BezierPolygon<T>::Vertex::Last)));
                                    if (closed) {
                                        assert (!bezierPolygon.vertices[index - 1].IsClosed ());
                                        ExecuteAndAddCommand (
                                            thekogans::mex::command::Command::UniquePtr (
                                                new BezierPolygonVertexSetFlagsCommand<T> (
                                                    bezierPolygon,
                                                    index - 1,
                                                    bezierPolygon.vertices[index - 1].flags |
                                                    io::BezierPolygon<T>::Vertex::Closed)));
                                    }
                                }
                            }
                            return true;
                        }
                    };

                    typedef BezierPolygonDeleteVertexCommand<blas::Point2> BezierPolygon2DeleteVertexCommand;
                    typedef BezierPolygonDeleteVertexCommand<blas::Point3> BezierPolygon3DeleteVertexCommand;

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_shaper_BezierPolygonVertexCommands_h)
