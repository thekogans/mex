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

#include "thekogans/mex/3ds/io/editor/Camera.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Camera)

                namespace {
                    const util::f32 lensTable[] = {
                        9.857142f,
                        10.78125f,
                        15.0f,
                        17.0f,
                        24.0f,
                        35.0f,
                        50.0f,
                        85.0f,
                        100.0f,
                        135.0f,
                        200.0f,
                        500.0f,
                        625.0f,
                        800.0f,
                        1000.0f,
                        2500.0f,
                        5000.0f,
                        25000.0f,
                        50000.0f,
                        100000.0f
                    };

                    const util::f32 fovTable[] = {
                        175.0f,
                        160.0f,
                        115.0f,
                        102.0f,
                        84.0f,
                        63.0f,
                        46.0f,
                        28.0f,
                        24.0f,
                        18.0f,
                        12.0f,
                        5.0f,
                        4.0f,
                        3.125f,
                        2.5f,
                        1.0f,
                        0.5f,
                        0.1f,
                        0.05f,
                        0.025f
                    };

                    const util::i32 TABLE_LEN = sizeof (fovTable) / sizeof (fovTable[0]);
                }

                util::f32 Camera::Lens2FOV (util::f32 lens) {
                    if (lens < lensTable[0]) {
                        lens = lensTable[0];
                    }
                    else if (lens > lensTable[TABLE_LEN - 1]) {
                        lens = lensTable[TABLE_LEN - 1];
                    }

                    util::i32 i;
                    for (i = 0; i < TABLE_LEN; ++i) {
                        if (lens < lensTable[i]) {
                            break;
                        }
                    }

                    if (i == TABLE_LEN) {
                        i = TABLE_LEN - 1;
                    }
                    else if (i == 0) {
                        i = 1;
                    }

                    return fovTable[i - 1] + (lens - lensTable[i - 1]) *
                        (fovTable[i] - fovTable[i - 1]) / (lensTable[i] - lensTable[i - 1]);
                }

                util::f32 Camera::FOV2Lens (util::f32 fov) {
                    if (fov < fovTable[TABLE_LEN - 1]) {
                        fov = fovTable[TABLE_LEN - 1];
                    }
                    else if (fov > fovTable[0]) {
                        fov = fovTable[0];
                    }

                    util::i32 i;
                    for (i = 0; i < TABLE_LEN; ++i) {
                        if (fov > fovTable[i]) {
                            break;
                        }
                    }

                    if (i == TABLE_LEN) {
                        i = TABLE_LEN - 1;
                    }
                    else if (i == 0) {
                        i = 1;
                    }

                    return lensTable[i - 1] + (fov - fovTable[i - 1]) *
                        (lensTable[i] - lensTable[i - 1]) / (fovTable[i] - fovTable[i - 1]);
                }

                Camera &Camera::operator = (const Camera &camera) {
                    if (&camera != this) {
                        name = camera.name;
                        position = camera.position;
                        target = camera.target;
                        roll = camera.roll;
                        lens = camera.lens;
                        showCone = camera.showCone;
                        ranges = camera.ranges;
                    }
                    return *this;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
