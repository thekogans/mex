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

#if !defined (__thekogans_mex_3ds_io_editor_Camera_h)
#define __thekogans_mex_3ds_io_editor_Camera_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Camera {
                    typedef std::unique_ptr<Camera> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Camera)

                    std::string name;
                    blas::Point3 position;
                    blas::Point3 target;
                    util::f32 roll;
                    util::f32 lens;
                    bool showCone;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Ranges {
                        util::f32 hither;
                        util::f32 yon;

                        Ranges () :
                            hither (0.0f),
                            yon (1000.0f) {}
                        Ranges (
                            util::f32 hither_,
                            util::f32 yon_) :
                            hither (hither_),
                            yon (yon_) {}
                        Ranges (const Ranges &ranges) :
                            hither (ranges.hither),
                            yon (ranges.yon) {}

                        inline Ranges &operator = (const Ranges &ranges) {
                            hither = ranges.hither;
                            yon = ranges.yon;
                            return *this;
                        }

                        inline void Swap (Ranges &ranges) {
                            std::swap (hither, ranges.hither);
                            std::swap (yon, ranges.yon);
                        }
                    } ranges;

                    static util::f32 Lens2FOV (util::f32 lens);
                    static util::f32 FOV2Lens (util::f32 fov);

                    Camera (const std::string &name_ = std::string ()) :
                        name (name_),
                        position (blas::Point3::Zero),
                        target (blas::Point3::One),
                        roll (0.0f),
                        lens (48.0f),
                        showCone (false) {}
                    Camera (
                        const std::string &name_,
                        const blas::Point3 &position_,
                        const blas::Point3 &target_,
                        util::f32 roll_,
                        util::f32 lens_,
                        bool showCone_,
                        const Ranges &ranges_) :
                        name (name_),
                        position (position_),
                        target (target_),
                        roll (roll_),
                        lens (lens_),
                        showCone (showCone_),
                        ranges (ranges_) {}
                    Camera (const Camera &camera) :
                        name (camera.name),
                        position (camera.position),
                        target (camera.target),
                        roll (camera.roll),
                        lens (camera.lens),
                        showCone (camera.showCone),
                        ranges (camera.ranges) {}

                    Camera &operator = (const Camera &camera);

                    inline void Swap (Camera &camera) {
                        std::swap (name, camera.name);
                        std::swap (position, camera.position);
                        std::swap (target, camera.target);
                        std::swap (roll, camera.roll);
                        std::swap (lens, camera.lens);
                        std::swap (showCone, camera.showCone);
                        ranges.Swap (camera.ranges);
                    }
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_editor_Camera_h)
