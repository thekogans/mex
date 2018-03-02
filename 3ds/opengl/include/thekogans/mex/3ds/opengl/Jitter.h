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

#if !defined (__thekogans_mex_3ds_opengl_Jitter_h)
#define __thekogans_mex_3ds_opengl_Jitter_h

#include "thekogans/mex/blas/Point2.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                enum JitterType {
                    JITTER_TYPE_2 = 2,
                    JITTER_TYPE_3 = 3,
                    JITTER_TYPE_4 = 4,
                    JITTER_TYPE_5 = 5,
                    JITTER_TYPE_6 = 6,
                    JITTER_TYPE_8 = 8,
                    JITTER_TYPE_9 = 9,
                    JITTER_TYPE_12 = 12,
                    JITTER_TYPE_16 = 16
                };

                // These jitter tables came straight out of OpenGL Programmers Guide (chapter 10).
                const blas::Point2 jitter2[] = {
                    blas::Point2 (0.25f, 0.75f),
                    blas::Point2 (0.75f, 0.25f)
                };

                const blas::Point2 jitter3[] = {
                    blas::Point2 (0.5033922635f, 0.8317967229f),
                    blas::Point2 (0.7806016275f, 0.2504380877f),
                    blas::Point2 (0.2261828938f, 0.4131553612f)
                };

                const blas::Point2 jitter4[] = {
                    blas::Point2 (0.375f, 0.25f),
                    blas::Point2 (0.125f, 0.75f),
                    blas::Point2 (0.875f, 0.25f),
                    blas::Point2 (0.625f, 0.75f)
                };

                const blas::Point2 jitter5[] = {
                    blas::Point2 (0.5f, 0.5f),
                    blas::Point2 (0.3f, 0.1f),
                    blas::Point2 (0.7f, 0.9f),
                    blas::Point2 (0.9f, 0.3f),
                    blas::Point2 (0.1f, 0.7f)
                };

                const blas::Point2 jitter6[] = {
                    blas::Point2 (0.4646464646f, 0.4646464646f),
                    blas::Point2 (0.1313131313f, 0.7979797979f),
                    blas::Point2 (0.5353535353f, 0.8686868686f),
                    blas::Point2 (0.8686868686f, 0.5353535353f),
                    blas::Point2 (0.7979797979f, 0.1313131313f),
                    blas::Point2 (0.2020202020f, 0.2020202020f)
                };

                const blas::Point2 jitter8[] = {
                    blas::Point2 (0.5625f, 0.4375f),
                    blas::Point2 (0.0625f, 0.9375f),
                    blas::Point2 (0.3125f, 0.6875f),
                    blas::Point2 (0.6875f, 0.8125f),
                    blas::Point2 (0.8125f, 0.1875f),
                    blas::Point2 (0.9375f, 0.5625f),
                    blas::Point2 (0.4375f, 0.0625f),
                    blas::Point2 (0.1875f, 0.3125f)
                };

                const blas::Point2 jitter9[] = {
                    blas::Point2 (0.5f, 0.5f),
                    blas::Point2 (0.1666666666f, 0.9444444444f),
                    blas::Point2 (0.5f, 0.1666666666f),
                    blas::Point2 (0.5f, 0.8333333333f),
                    blas::Point2 (0.1666666666f, 0.2777777777f),
                    blas::Point2 (0.8333333333f, 0.3888888888f),
                    blas::Point2 (0.1666666666f, 0.6111111111f),
                    blas::Point2 (0.8333333333f, 0.7222222222f),
                    blas::Point2 (0.8333333333f, 0.0555555555f)
                };

                const blas::Point2 jitter12[] = {
                    blas::Point2 (0.4166666666f, 0.625f),
                    blas::Point2 (0.9166666666f, 0.875f),
                    blas::Point2 (0.25f, 0.375f),
                    blas::Point2 (0.4166666666f, 0.125f),
                    blas::Point2 (0.75f, 0.125f),
                    blas::Point2 (0.0833333333f, 0.125f),
                    blas::Point2 (0.75f, 0.625f),
                    blas::Point2 (0.25f, 0.875f),
                    blas::Point2 (0.5833333333f, 0.375f),
                    blas::Point2 (0.9166666666f, 0.375f),
                    blas::Point2 (0.0833333333f, 0.625f),
                    blas::Point2 (0.583333333f, 0.875f)
                };

                const blas::Point2 jitter16[] = {
                    blas::Point2 (0.375f, 0.4375f),
                    blas::Point2 (0.625f, 0.0625f),
                    blas::Point2 (0.875f, 0.1875f),
                    blas::Point2 (0.125f, 0.0625f),
                    blas::Point2 (0.375f, 0.6875f),
                    blas::Point2 (0.875f, 0.4375f),
                    blas::Point2 (0.625f, 0.5625f),
                    blas::Point2 (0.375f, 0.9375f),
                    blas::Point2 (0.625f, 0.3125f),
                    blas::Point2 (0.125f, 0.5625f),
                    blas::Point2 (0.125f, 0.8125f),
                    blas::Point2 (0.375f, 0.1875f),
                    blas::Point2 (0.875f, 0.9375f),
                    blas::Point2 (0.875f, 0.6875f),
                    blas::Point2 (0.125f, 0.3125f),
                    blas::Point2 (0.625f, 0.8125f)
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Jitter_h)
