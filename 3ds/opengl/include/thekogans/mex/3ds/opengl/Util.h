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

#if !defined (__thekogans_mex_3ds_opengl_Util_h)
#define __thekogans_mex_3ds_opengl_Util_h

#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/keyframer/Node.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawLine2 (
                    const blas::Point2 &p1,
                    const blas::Point2 &p2);
                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawLine3 (
                    const blas::Point3 &p1,
                    const blas::Point3 &p2);

                template<typename T>
                void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPolygon (const blas::Polygon<T> &polygon);

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawBox (
                    const ext::View &view,
                    const blas::Bound3 &bound,
                    const blas::Matrix3 &xform);
                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPath (
                    const std::vector<blas::Point3> &path,
                    const std::vector<util::ui32> &keys,
                    const io::Keyframer::Segment &segment);
                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API DrawPathWithColor (
                    const std::vector<blas::Point3> &path,
                    const std::vector<util::ui32> &keys,
                    const io::Keyframer::Segment &segment,
                    const thekogans::mex::opengl::ui8Color &color = DefaultPalette[MEDIUM_GRAY]);

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API Color2glColor (
                    const io::Color &color,
                    util::f32 glColor[],
                    const util::f32 defaultglColor[]);
                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API f32Color2glColor (
                    const thekogans::mex::opengl::f32Color &color,
                    util::f32 glColor[]);

                _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL void _LIB_THEKOGANS_MEX_3DS_OPENGL_API SetupLights (
                    const io::Color &ambientLight,
                    const util::OwnerVector<io::Light> &lights);

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Util_h)
