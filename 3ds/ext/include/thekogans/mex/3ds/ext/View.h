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

#if !defined (__thekogans_mex_3ds_ext_View_h)
#define __thekogans_mex_3ds_ext_View_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/io/editor/Camera.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL View {
                    typedef std::unique_ptr<View> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (View)

                    static const util::f32 ShapeWidth;
                    static const util::f32 OrthographicWidth;
                    static const util::f32 PerspectiveWidth;

                    const io::View &view;
                    // *** IMPORTANT ***
                    // The order of these declarations must not be disturbed.
                    // Their initialization is dependent on strict ordering.
                    // *****************
                    // World to view matrix.
                    const blas::Matrix3 xform;
                    // View to world matrix.
                    const blas::Matrix3 ixform;
                    // View origin.
                    const blas::Point2 viewOrigin;
                    // Transformed/Projected/Clipped coords to device coords.
                    const util::f32 scale;
                    const util::f32 oneOverScale;
                    const blas::Point2 offset;
                    // View clip bound.
                    const blas::Bound2 viewBound;
                    // Near and far clip planes.
                    const util::f32 nearPlane;
                    const util::f32 farPlane;

                    struct BadViewType {};

                    explicit View (const io::View &view_);
                    View (const io::View &view_, const io::Light &light);
                    View (const io::View &view_, const io::Camera &camera);
                    virtual ~View () {}

                    static const char *GetName (const io::View &view);
                    static util::f32 GetHorizontalAngle (const io::View &view);
                    static util::f32 GetVerticalAngle (const io::View &view);

                    bool W2D (const blas::Point3 &w, blas::Point &d) const;
                    bool W2D (const blas::Line3 &w, blas::Line &d) const;

                    inline blas::Point3 D2W (blas::Point &d) const {
                        return D2P (d) * ixform;
                    }
                    inline blas::Line3 D2W (blas::Line &d) const {
                        return blas::Line3 (D2P (d.start) * ixform, D2P (d.end) * ixform);
                    }

                    // Projected and clipped to device.
                    inline blas::Point P2D (const blas::Point2 &p) const {
                        return blas::Point (
                            util::ROUND (p.x * scale + offset.x),
                            util::ROUND (p.y * scale + offset.y));
                    }
                    // Device to projected.
                    inline blas::Point2 D2P (const blas::Point &d) const {
                        return blas::Point2 (
                            (d.x - offset.x) * oneOverScale,
                            (d.y - offset.y) * oneOverScale);
                    }
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_View_h)
