// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#include <cassert>
#include "thekogans/mex/3ds/opengl/Icons.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                namespace {
                    // *         *
                    //  *       *
                    //   *     *
                    //    *   *
                    //
                    //      *
                    //
                    //    *   *
                    //   *     *
                    //  *       *
                    // *         *
                    const GLubyte axis[] = {
                        0x80, 0x20,
                        0x40, 0x40,
                        0x20, 0x80,
                        0x11, 0x00,
                        0x00, 0x00,
                        0x04, 0x00,
                        0x00, 0x00,
                        0x11, 0x00,
                        0x20, 0x80,
                        0x40, 0x40,
                        0x80, 0x20
                    };

                    //      *
                    //      *
                    //      *
                    //      *
                    //
                    // **** * ****
                    //
                    //      *
                    //      *
                    //      *
                    //      *
                    const GLubyte lofterPath[] = {
                        0x04, 0x00,
                        0x04, 0x00,
                        0x04, 0x00,
                        0x04, 0x00,
                        0x00, 0x00,
                        0xf5, 0xe0,
                        0x00, 0x00,
                        0x04, 0x00,
                        0x04, 0x00,
                        0x04, 0x00,
                        0x04, 0x00
                    };

                    // *   *   *
                    //  *  *  *
                    //   * * *
                    //    ***
                    // *********
                    //    ***
                    //   * * *
                    //  *  *  *
                    // *   *   *
                    const GLubyte lightHead[] = {
                        0x88, 0x80,
                        0x49, 0x00,
                        0x2a, 0x00,
                        0x1c, 0x00,
                        0xff, 0x80,
                        0x1c, 0x00,
                        0x2a, 0x00,
                        0x49, 0x00,
                        0x88, 0x80
                    };

                    //      **
                    //      **
                    //    ******
                    //   *      *
                    //   *      *
                    // ***  **  ***
                    // ***  **  ***
                    //   *      *
                    //   *      *
                    //    ******
                    //      **
                    //      **
                    const GLubyte cameraHead[] = {
                        0x06, 0x00,
                        0x06, 0x00,
                        0x1f, 0x80,
                        0x20, 0x40,
                        0x20, 0x40,
                        0xe6, 0x70,
                        0xe6, 0x70,
                        0x20, 0x40,
                        0x20, 0x40,
                        0x1f, 0x80,
                        0x06, 0x00,
                        0x06, 0x00
                    };

                    // ***
                    // * *
                    // ***
                    const GLubyte box3x3[] = {
                        0xe0,
                        0xa0,
                        0xe0
                    };

                    // ****
                    // *  *
                    // *  *
                    // ****
                    const GLubyte box4x4[] = {
                        0xf0,
                        0x90,
                        0x90,
                        0xf0
                    };

                    // *****
                    // *   *
                    // *   *
                    // *   *
                    // *****
                    const GLubyte box5x5[] = {
                        0xf8,
                        0x88,
                        0x88,
                        0x88,
                        0xf8
                    };

                    // *********
                    // *       *
                    // *       *
                    // *       *
                    // *       *
                    // *       *
                    // *       *
                    // *       *
                    // *********
                    const GLubyte box9x9[] = {
                        0xff, 0x80,
                        0x80, 0x80,
                        0x80, 0x80,
                        0x80, 0x80,
                        0x80, 0x80,
                        0x80, 0x80,
                        0x80, 0x80,
                        0x80, 0x80,
                        0xff, 0x80,
                    };

                    //  *
                    // ***
                    //  *
                    const GLubyte tick3x3[] = {
                        0x40,
                        0xe0,
                        0x40
                    };

                    //   *
                    //   *
                    // *****
                    //   *
                    //   *
                    const GLubyte tick5x5[] = {
                        0x20,
                        0x20,
                        0xf8,
                        0x20,
                        0x20
                    };

                    //    *
                    //    *
                    //    *
                    // *******
                    //    *
                    //    *
                    //    *
                    const GLubyte tick7x7[] = {
                        0x10,
                        0x10,
                        0x10,
                        0xfe,
                        0x10,
                        0x10,
                        0x10
                    };

                    struct Icon {
                        util::ui32 width;
                        util::ui32 height;
                        util::f32 offsetx;
                        util::f32 offsety;
                        const GLubyte *bitmap;
                    } const icons[] = {
                        {11, 11, 5.0f, 5.0f, axis},
                        {11, 11, 5.0f, 5.0f, lofterPath},
                        {9, 9, 4.0f, 4.0f, lightHead},
                        {12, 12, 6.0f, 6.0f, cameraHead},
                        {3, 3, 1.0f, 1.0f, box3x3},
                        {4, 4, 2.0f, 2.0f, box4x4},
                        {5, 5, 2.0f, 2.0f, box5x5},
                        {9, 9, 4.0f, 4.0f, box9x9},
                        {3, 3, 1.0f, 1.0f, tick3x3},
                        {5, 5, 2.0f, 2.0f, tick5x5},
                        {7, 7, 3.0f, 3.0f, tick7x7}
                    };
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL util::ui32 _LIB_THEKOGANS_MEX_3DS_OPENGL_API GetTickIcon (
                        util::ui32 tickSize) {
                    switch (tickSize) {
                        case 3:
                            return TICK_3x3;
                        case 5:
                            return TICK_5x5;
                        case 7:
                            return TICK_7x7;
                    }
                    // FIXME: add a few more tick icons.
                    assert (0);
                    return TICK_5x5;
                }

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawIcon (
                        util::ui32 icon,
                        const blas::Point3 &position) {
                    assert (icon >= AXIS && icon <= TICK_7x7);
                    glRasterPos3f (position.x, position.y, position.z);
                    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
                    glBitmap (icons[icon].width, icons[icon].height, icons[icon].offsetx,
                        icons[icon].offsety, 0.0f, 0.0f, icons[icon].bitmap);
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
