// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/io/editor/Light.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Light)
                THEKOGANS_UTIL_IMPLEMENT_HEAP (Light::Spot)

                // FIXME: move to ext?
                bool Light::Spot::Cone::CalcPoints (
                        const blas::Point3 &position,
                        const blas::Point3 &target,
                        util::f32 fov,
                        util::f32 roll,
                        std::vector<blas::Point3> &points) const {
                    blas::Point3 pt = position - target;
                    util::f32 x = pt.Length () * tanf (util::RAD (fov) * 0.5f);
                    if (util::IS_ZERO (x)) {
                        return false;
                    }
                    std::vector<blas::Point3> points_;
                    util::ui32 count;
                    if (type == Rectangular) {
                        // Generate rectangle.
                        count = 4;
                        util::f32 y = x;
                        if (aspect < 1.0f) {
                            y *= aspect;
                        }
                        else if (aspect > 1.0f) {
                            x /= aspect;
                        }
                        points_.push_back (blas::Point3 (-x, y, 0.0f));
                        points_.push_back (blas::Point3 (x, y, 0.0f));
                        points_.push_back (blas::Point3 (x, -y, 0.0f));
                        points_.push_back (blas::Point3 (-x, -y, 0.0f));
                    }
                    else {
                        // Generate circle.
                        count = 16;
                        blas::Matrix3 xform = blas::Matrix3::RotateZ (TWOPI / count);
                        points_.push_back (blas::Point3 (x, 0.0f, 0.0f));
                        for (util::ui32 i = 0; i < 15; ++i) {
                            points_.push_back (points_.back () * xform);
                        }
                    }
                    // Transform cone base to world space.
                    blas::Matrix3 xform = (blas::Matrix3::Translate (-target) *
                        blas::Matrix3::AimZ (pt) * blas::Matrix3::RotateZ (util::RAD (roll))).Invert ();
                    for (util::ui32 i = 0; i < count; ++i) {
                        points_[i] *= xform;
                    }
                    points.swap (points_);
                    return true;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
