// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext_command.
//
// libthekogans_mex_3ds_ext_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_ext_command_shaper_BezierPolygonCommands_h)
#define __thekogans_mex_3ds_ext_command_shaper_BezierPolygonCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Matrix.h"
#include "thekogans/mex/command/Command.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/command/stdVectorCommands.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/ext/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {
                namespace command {

                    template<typename T>
                    struct BezierPolygonXformCommand : public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (BezierPolygonXformCommand)

                        io::BezierPolygon<T> &bezierPolygon;
                        const blas::Matrix<T> xform;

                        BezierPolygonXformCommand (
                            io::BezierPolygon<T> &bezierPolygon_,
                            const blas::Matrix<T> &xform_) :
                            bezierPolygon (bezierPolygon_),
                            xform (xform_) {}

                        virtual bool Execute () {
                            std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                bezierPolygon.vertices;
                            for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                ExecuteAndAddCommand (thekogans::mex::command::Command::UniquePtr (
                                    new thekogans::mex::command::stdVectorassignCommand<
                                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> (
                                            vertices, i, vertices[i] * xform)));
                            }
                            return true;
                        }
                    };

                    typedef BezierPolygonXformCommand<blas::Point2> BezierPolygon2XformCommand;
                    typedef BezierPolygonXformCommand<blas::Point3> BezierPolygon3XformCommand;

                } // namespace command
            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_command_shaper_BezierPolygonCommands_h)
