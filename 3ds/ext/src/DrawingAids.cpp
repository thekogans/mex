// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#include <cstdio>
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/3ds/ext/DrawingAids.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                util::f32 DrawingAids::SnapAngle (util::f32 theta) const {
                    util::f32 remainder = (util::f32)fmod (theta, drawingAids.angleSnap);
                    theta -= remainder;
                    if (remainder > drawingAids.angleSnap * 0.5f) {
                        theta += drawingAids.angleSnap;
                    }
                    return theta;
                }

                void DrawingAids::GetRadiusAndAngle (const blas::Point2 &center,
                    const blas::Point2 &pt, util::f32 &radius, util::f32 &angle) {
                    util::f32 x = pt.x - center.x;
                    util::f32 y = pt.y - center.y;
                    radius = blas::Point2 (x, y).Length ();
                    angle = (util::f32)atan2 (x, y);
                    if (drawingAids.useAngleSnap) {
                        angle = util::RAD (SnapAngle (util::DEG (angle)));
                    }
                }
            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
