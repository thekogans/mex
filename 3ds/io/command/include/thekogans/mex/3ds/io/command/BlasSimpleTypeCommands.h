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

#if !defined (__thekogans_mex_3ds_io_command_BlasSimpleTypeCommands_h)
#define __thekogans_mex_3ds_io_command_BlasSimpleTypeCommands_h

#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/Ellipse.h"
#include "thekogans/mex/blas/EulerAngles.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Point4.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/Quaternion.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Triangle.h"

namespace thekogans {
    namespace mex {
        namespace command {

            typedef SimpleTypeSetCommand<blas::AngleAxis> AngleAxisSetCommand;
            typedef SimpleTypeSetCommand<blas::BezierCubic2> BezierCubic2SetCommand;
            typedef SimpleTypeSetCommand<blas::BezierCubic3> BezierCubic3SetCommand;
            typedef SimpleTypeSetCommand<blas::Bound2> Bound2SetCommand;
            typedef SimpleTypeSetCommand<blas::Bound3> Bound3SetCommand;
            typedef SimpleTypeSetCommand<blas::Circle> CircleSetCommand;
            typedef SimpleTypeSetCommand<blas::Ellipse> EllipseSetCommand;
            typedef SimpleTypeSetCommand<blas::EulerAngles> EulerAnglesSetCommand;
            typedef SimpleTypeSetCommand<blas::Line2> Line2SetCommand;
            typedef SimpleTypeSetCommand<blas::Line3> Line3SetCommand;
            typedef SimpleTypeSetCommand<blas::Matrix2> Matrix2SetCommand;
            typedef SimpleTypeSetCommand<blas::Matrix3> Matrix3SetCommand;
            typedef SimpleTypeSetCommand<blas::Matrix4> Matrix4SetCommand;
            typedef SimpleTypeSetCommand<blas::Point> PointSetCommand;
            typedef SimpleTypeSetCommand<blas::Point2> Point2SetCommand;
            typedef SimpleTypeSetCommand<blas::Point3> Point3SetCommand;
            typedef SimpleTypeSetCommand<blas::Point4> Point4SetCommand;
            typedef SimpleTypeSetCommand<blas::Polygon2> Polygon2SetCommand;
            typedef SimpleTypeSetCommand<blas::Polygon3> Polygon3SetCommand;
            typedef SimpleTypeSetCommand<blas::Quaternion> QuaternionSetCommand;
            typedef SimpleTypeSetCommand<blas::Rect> RectSetCommand;
            typedef SimpleTypeSetCommand<blas::Size> SizeSetCommand;
            typedef SimpleTypeSetCommand<blas::Triangle> TriangleSetCommand;

            typedef SimpleTypeSwapCommand<blas::AngleAxis> AngleAxisSwapCommand;
            typedef SimpleTypeSwapCommand<blas::BezierCubic2> BezierCubic2SwapCommand;
            typedef SimpleTypeSwapCommand<blas::BezierCubic3> BezierCubic3SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Bound2> Bound2SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Bound3> Bound3SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Circle> CircleSwapCommand;
            typedef SimpleTypeSwapCommand<blas::Ellipse> EllipseSwapCommand;
            typedef SimpleTypeSwapCommand<blas::EulerAngles> EulerAnglesSwapCommand;
            typedef SimpleTypeSwapCommand<blas::Line2> Line2SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Line3> Line3SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Matrix2> Matrix2SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Matrix3> Matrix3SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Matrix4> Matrix4SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Point> PointSwapCommand;
            typedef SimpleTypeSwapCommand<blas::Point2> Point2SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Point3> Point3SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Point4> Point4SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Polygon2> Polygon2SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Polygon3> Polygon3SwapCommand;
            typedef SimpleTypeSwapCommand<blas::Quaternion> QuaternionSwapCommand;
            typedef SimpleTypeSwapCommand<blas::Rect> RectSwapCommand;
            typedef SimpleTypeSwapCommand<blas::Size> SizeSwapCommand;
            typedef SimpleTypeSwapCommand<blas::Triangle> TriangleSwapCommand;

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_BlasSimpleTypeCommands_h)
