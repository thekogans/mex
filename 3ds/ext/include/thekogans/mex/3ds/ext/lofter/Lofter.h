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

#if !defined (__thekogans_mex_3ds_ext_lofter_Lofter_h)
#define __thekogans_mex_3ds_ext_lofter_Lofter_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/Module.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Lofter : public Module {
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Shape {
                        typedef std::unique_ptr<Shape> UniquePtr;

                        const io::Lofter::Shape &shape;
                        struct Polygon {
                            typedef std::unique_ptr<Polygon> UniquePtr;

                            // FIXME: Replace these with blas::BezierPolygon2
                            // This will consolidate the drawing code in opengl.
                            blas::Polygon2::UniquePtr outer;
                            util::OwnerVector<blas::Polygon2> inner;

                            explicit Polygon (const blas::Polygon2 &polygon) :
                                outer (new blas::Polygon2 (polygon)) {}
                            Polygon (const Polygon &polygon) :
                                outer (polygon.outer.get () ? new blas::Polygon2 (*polygon.outer) : 0),
                                inner (polygon.inner) {}

                            inline Polygon &operator = (const Polygon &polygon) {
                                if (&polygon != this) {
                                    outer.reset (new blas::Polygon2 (*polygon.outer));
                                    inner = polygon.inner;
                                }
                                return *this;
                            }
                        };
                        util::OwnerVector<Polygon> polygons;

                        Shape (
                            const io::Lofter::Shape &shape_,
                            util::ui32 steps,
                            util::ui16 datail,
                            bool optimize);
                        Shape (const Shape &other) :
                            shape (other.shape),
                            polygons (other.polygons) {}

                        bool IsValid () const;
                        bool IsSimilar (const Shape &other) const;
                        void Lerp (const Shape &other, util::f32 t);
                        void Outline (util::f32 width);
                        blas::Bound2 GetBound2 (const blas::Matrix3 &xform) const;

                    private:
                        Shape &operator = (const Shape &);
                        void AddPolygon (const blas::Polygon2 &polygon);
                    };
                    util::OwnerVector<Shape> shapes;
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL ScaleDeformation {
                        typedef std::unique_ptr<ScaleDeformation> UniquePtr;
                        const io::Lofter::ScaleDeformation &deformation;
                        io::BezierPolygon2 xzCurve;
                        io::BezierPolygon2 yzCurve;

                        explicit ScaleDeformation (const io::Lofter::ScaleDeformation &deformation_);

                        // Description:
                        //      Calculate the intersection value of polygon with grid.
                        // Return:
                        //      First intersection with deformation curve.
                        // Parameters:
                        //      t - horizontal parameter specifying distance from
                        //          bottom to top (t = 0.0 == bottom, t = 1.0 == top).
                        //      intersections - returned intersections.
                        // Remarks:
                        //      The intersection is only guaranteed for 0.0 <= t <= 1.0.
                        inline util::f32 GetXZValue (util::f32 t) const {
                            return GetValue (t, xzCurve, deformation.xzCurve.steps);
                        }
                        inline util::f32 GetYZValue (util::f32 t) const {
                            return GetValue (t, yzCurve, deformation.yzCurve.steps);
                        }

                    private:
                        util::f32 GetValue (util::f32 t, const io::BezierPolygon2 &curve, util::ui16 steps) const;
                    };
                    ScaleDeformation::UniquePtr scaleDeformation;
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL TwistDeformation {
                        typedef std::unique_ptr<TwistDeformation> UniquePtr;
                        const io::Lofter::TwistDeformation &deformation;
                        io::BezierPolygon2 xzCurve;

                        explicit TwistDeformation (const io::Lofter::TwistDeformation &deformation_);

                        // Description:
                        //      Calculate the intersection value of polygon with grid.
                        // Return:
                        //      First intersection with deformation curve.
                        // Parameters:
                        //      t - horizontal parameter specifying distance from
                        //          bottom to top (t = 0.0 == bottom, t = 1.0 == top).
                        //      intersections - returned intersections.
                        // Remarks:
                        //      The intersection is only guaranteed for 0.0 <= t <= 1.0.
                        util::f32 GetValue (util::f32 t) const;
                    };
                    TwistDeformation::UniquePtr twistDeformation;
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL TeeterDeformation {
                        typedef std::unique_ptr<TeeterDeformation> UniquePtr;
                        const io::Lofter::TeeterDeformation &deformation;
                        io::BezierPolygon2 xzCurve;
                        io::BezierPolygon2 yzCurve;

                        explicit TeeterDeformation (const io::Lofter::TeeterDeformation &deformation_);

                        // Description:
                        //      Calculate the intersection value of polygon with grid.
                        // Return:
                        //      First intersection with deformation curve.
                        // Parameters:
                        //      t - horizontal parameter specifying distance from
                        //          bottom to top (t = 0.0 == bottom, t = 1.0 == top).
                        //      intersections - returned intersections.
                        // Remarks:
                        //      The intersection is only guaranteed for 0.0 <= t <= 1.0.
                        inline util::f32 GetXZValue (util::f32 t) const {
                            return GetValue (t, xzCurve, deformation.xzCurve.steps);
                        }
                        inline util::f32 GetYZValue (util::f32 t) const {
                            return GetValue (t, yzCurve, deformation.yzCurve.steps);
                        }

                    private:
                        util::f32 GetValue (util::f32 t, const io::BezierPolygon2 &curve, util::ui16 steps) const;
                    };
                    TeeterDeformation::UniquePtr teeterDeformation;
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL BevelDeformation {
                        typedef std::unique_ptr<BevelDeformation> UniquePtr;
                        const io::Lofter::BevelDeformation &deformation;
                        io::BezierPolygon2 xzCurve;

                        explicit BevelDeformation (const io::Lofter::BevelDeformation &deformation_);

                        // Description:
                        //      Calculate the intersection value of polygon with grid.
                        // Return:
                        //      First intersection with deformation curve.
                        // Parameters:
                        //      t - horizontal parameter specifying distance from
                        //          bottom to top (t = 0.0 == bottom, t = 1.0 == top).
                        //      intersections - returned intersections.
                        // Remarks:
                        //      The intersection is only guaranteed for 0.0 <= t <= 1.0.
                        util::f32 GetValue (util::f32 t) const;
                    };
                    BevelDeformation::UniquePtr bevelDeformation;
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL FitDeformation : public io::Lofter::FitDeformation {
                        typedef std::unique_ptr<FitDeformation> UniquePtr;
                        const io::Lofter::FitDeformation &deformation;
                        io::BezierPolygon2 xzCurve;
                        blas::Bound2 xzBound;
                        io::BezierPolygon2 yzCurve;
                        blas::Bound2 yzBound;

                        explicit FitDeformation (const io::Lofter::FitDeformation &deformation_);

                        // Description:
                        //      Calculate the intersection value of polygon with grid.
                        // Return:
                        //      First intersection with deformation curve.
                        // Parameters:
                        //      t - horizontal parameter specifying distance from
                        //          bottom to top (t = 0.0 == bottom, t = 1.0 == top).
                        //      intersections - returned intersections.
                        // Remarks:
                        //      The intersection is only guaranteed for 0.0 <= t <= 1.0.
                        void GetXZValues (util::f32 t, std::vector<util::f32> &intersections) const;
                        void GetYZValues (util::f32 t, std::vector<util::f32> &intersections) const;
                    };
                    FitDeformation::UniquePtr fitDeformation;

                    explicit Lofter (const io::Lofter &lofter);
                    explicit Lofter (Project &project) :
                        Module (project) {}

                    virtual View::UniquePtr GetView (const io::View &view) const;
                    virtual View::UniquePtr GetCurrView () const;
                    virtual blas::Bound2 GetViewBound (const View &view) const;

                    const Shape *GetCurrShape () const;

                    struct NoShapes {};
                    struct InvalidShape {};
                    struct DissimilarShape {};
                    struct InvalidPath {};
                    struct InvalidParams {};
                    struct InvalidFitX {};
                    struct InvalidFitY {};
                    io::Mesh::UniquePtr MakeMesh (const std::string &name) const;

                    void Swap (Lofter &lofter);

                private:
                    Lofter (const Lofter &);
                    Lofter &operator = (const Lofter &);
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_lofter_Lofter_h)
