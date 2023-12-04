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

#if !defined (__thekogans_mex_3ds_ext_shaper_BezierPolygon_h)
#define __thekogans_mex_3ds_ext_shaper_BezierPolygon_h

#include <cassert>
#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Region.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/Circle.h"
#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                template<typename T>
                inline THEKOGANS_UTIL_TYPENAME BezierPolygon<T>::Vertex operator * (
                        const THEKOGANS_UTIL_TYPENAME BezierPolygon<T>::Vertex &vertex,
                        const blas::Matrix<T> &xform) {
                    return THEKOGANS_UTIL_TYPENAME BezierPolygon<T>::Vertex (
                        vertex.pt * xform, xform.XformNormal (vertex.red),
                        xform.XformNormal (vertex.yellow), vertex.flags);
                }

                template<typename T>
                THEKOGANS_UTIL_TYPENAME BezierPolygon<T>::UniquePtr operator * (
                        const BezierPolygon<T> &bezierPolygon,
                        const blas::Matrix<T> &xform) {
                    THEKOGANS_UTIL_TYPENAME BezierPolygon<T>::UniquePtr newBezierPolygon (new BezierPolygon<T>);
                    newBezierPolygon->vertices.resize (bezierPolygon.vertices.size ());
                    for (std::size_t i = 0, count = bezierPolygon.vertices.size (); i < count; ++i) {
                        newBezierPolygon->vertices[i] = bezierPolygon.vertices[i] * xform;
                    }
                    return newBezierPolygon;
                }

            } // namespace io

            namespace ext {

                template<typename T>
                struct BezierPolygon {
                    typedef std::unique_ptr<BezierPolygon<T> > UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (BezierPolygon<T>)

                    const io::BezierPolygon<T> &bezierPolygon;

                    struct VertexInfo {
                        const io::BezierPolygon<T> &bezierPolygon;
                        util::ui32 selectedVertexCount[3];
                        util::ui32 shapeVertexCount;
                        util::ui32 hiddenVertexCount;
                        util::ui32 frozenVertexCount;

                        explicit VertexInfo (const io::BezierPolygon<T> &bezierPolygon_) :
                            bezierPolygon (bezierPolygon_) {}

                        void Build () {
                            selectedVertexCount[0] = 0;
                            selectedVertexCount[1] = 0;
                            selectedVertexCount[2] = 0;
                            shapeVertexCount = 0;
                            hiddenVertexCount = 0;
                            frozenVertexCount = 0;
                            const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                bezierPolygon.vertices;
                            for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                switch (vertices[i].flags & (io::BezierPolygon<T>::Vertex::SelectionSetA |
                                    io::BezierPolygon<T>::Vertex::SelectionSetB |
                                    io::BezierPolygon<T>::Vertex::SelectionSetC)) {
                                    case io::BezierPolygon<T>::Vertex::SelectionSetA |
                                        io::BezierPolygon<T>::Vertex::SelectionSetB |
                                        io::BezierPolygon<T>::Vertex::SelectionSetC:
                                        ++selectedVertexCount[0];
                                        ++selectedVertexCount[1];
                                        ++selectedVertexCount[2];
                                        break;
                                    case io::BezierPolygon<T>::Vertex::SelectionSetA |
                                        io::BezierPolygon<T>::Vertex::SelectionSetB:
                                        ++selectedVertexCount[0];
                                        ++selectedVertexCount[1];
                                        break;
                                    case io::BezierPolygon<T>::Vertex::SelectionSetA |
                                        io::BezierPolygon<T>::Vertex::SelectionSetC:
                                        ++selectedVertexCount[0];
                                        ++selectedVertexCount[2];
                                        break;
                                    case io::BezierPolygon<T>::Vertex::SelectionSetB |
                                        io::BezierPolygon<T>::Vertex::SelectionSetC:
                                        ++selectedVertexCount[1];
                                        ++selectedVertexCount[2];
                                        break;
                                    case io::BezierPolygon<T>::Vertex::SelectionSetA:
                                        ++selectedVertexCount[0];
                                        break;
                                    case io::BezierPolygon<T>::Vertex::SelectionSetB:
                                        ++selectedVertexCount[1];
                                        break;
                                    case io::BezierPolygon<T>::Vertex::SelectionSetC:
                                        ++selectedVertexCount[2];
                                        break;
                                }
                                if (vertices[i].IsShape ()) {
                                    ++shapeVertexCount;
                                }
                                if (vertices[i].IsHidden ()) {
                                    ++hiddenVertexCount;
                                }
                                if (vertices[i].IsFrozen ()) {
                                    ++frozenVertexCount;
                                }
                            }
                        }

                        inline bool IsSelected (util::ui32 selectionSet) const {
                            assert (selectionSet < 3);
                            return selectedVertexCount[selectionSet] == bezierPolygon.vertices.size ();
                        }

                        inline bool IsShape () const {
                            return shapeVertexCount == bezierPolygon.vertices.size ();
                        }

                        inline bool IsHidden () const {
                            return hiddenVertexCount == bezierPolygon.vertices.size ();
                        }

                        inline bool IsFrozen () const {
                            return frozenVertexCount == bezierPolygon.vertices.size ();
                        }

                    private:
                        VertexInfo (const VertexInfo &);
                        VertexInfo &operator = (const VertexInfo &);
                    };

                    struct ArcLength {
                    private:
                        struct CubicPolynomial {
                        private:
                            T C0;
                            T C1;
                            T C2;
                            T C3;

                        public:
                            CubicPolynomial () {}
                            CubicPolynomial (const T &C0_, const T &C1_, const T &C2_, const T &C3_) :
                                C0 (C0_), C1 (C1_), C2 (C2_), C3 (C3_) {}
                            explicit CubicPolynomial (const blas::BezierCubic<T> &bezierCubic) :
                                C0 (bezierCubic.p1), C1 (3.0f * (bezierCubic.p2 - bezierCubic.p1)),
                                C2 (3.0f * (bezierCubic.p1 - 2.0f * bezierCubic.p2 + bezierCubic.p3)),
                                C3 (3.0f * (bezierCubic.p2 - bezierCubic.p3) + bezierCubic.p4 - bezierCubic.p1) {}

                            inline T Position (util::f32 u) const {
                                return C0 + u * (C1 + u * (C2 + u * C3));
                            }
                            inline T Velocity (util::f32 u) const {
                                return C1 + u * (2.0f * C2 + u * 3.0f * C3);
                            }
                            inline T Acceleration (util::f32 u) const {
                                return 2.0f * C2 + u * 6.0f * C3;
                            }
                            inline util::f32 Speed (util::f32 u) const {
                                return Velocity (u).Length ();
                            }
                            util::f32 Length (util::f32 u) const {
                                // Legendre polynomial information for Gaussian quadrature
                                // of speed on domain [0, u], 0 <= u <= 1.
                                const util::i32 degree = 5;
                                static const util::f32 modRoot[degree] = {
                                    // Legendre roots mapped to (root + 1) / 2
                                    0.046910077f,
                                    0.230765345f,
                                    0.5f,
                                    0.769234655f,
                                    0.953089922f
                                };
                                static const util::f32 modCoeff[degree] = {
                                    // Original coefficients divided by 2.
                                    0.118463442f,
                                    0.239314335f,
                                    0.284444444f,
                                    0.239314335f,
                                    0.118463442f
                                };
                                // Need to transform domain [0, u] to [-1, 1].
                                // If 0 <= x <= u and -1 <= t <= 1, then x = u * (t + 1) / 2.
                                util::f32 result = 0.0f;
                                for (util::i32 i = 0; i < degree; ++i) {
                                    result += modCoeff[i] * Speed (u * modRoot[i]);
                                }
                                result *= u;
                                return result;
                            }
                        };

                        std::vector<CubicPolynomial> polygons;
                        std::vector<util::f32> lengths;

                    public:
                        explicit ArcLength (const BezierPolygon<T> &bezierPolygon) {
                            const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                bezierPolygon.vertices;
                            for (std::size_t i = 0, count = vertices.size () - !bezierPolygon.IsClosed (); i < count; ++i) {
                                std::size_t next = (i + 1) % vertices.size ();
                                polygons.push_back (CubicPolynomial (blas::BezierCubic<T> (
                                    vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                                    vertices[next].pt + vertices[next].red, vertices[next].pt)));
                            }
                            ComputeArcLength ();
                        }
                        explicit ArcLength (const blas::BezierCubic<T> &bezierCubic) {
                            polygons.push_back (CubicPolynomial (bezierCubic));
                            ComputeArcLength ();
                        }

                        inline util::f32 GetTotalLength () const {
                            assert (!lengths.empty ());
                            return lengths.back ();
                        }
                        inline util::f32 GetSegmentLength (util::ui32 index) const {
                            assert (index < polygons.size ());
                            return lengths[index + 1] - lengths[index];
                        }
                        inline T Position (util::f32 s) const {
                            std::size_t i;
                            util::f32 u;
                            InvertIntegral (s, i, u);
                            return polygons[i].Position (u);
                        }
                        inline T Velicity (util::f32 s) const {
                            std::size_t i;
                            util::f32 u;
                            InvertIntegral (s, i, u);
                            return polygons[i].Velocity (u);
                        }
                        inline T Acceleration (util::f32 s) const {
                            std::size_t i;
                            util::f32 u;
                            InvertIntegral (s, i, u);
                            return polygons[i].Acceleration (u);
                        }
                        inline util::f32 GetT (util::f32 s) const {
                            std::size_t i;
                            util::f32 u;
                            InvertIntegral (s, i, u);
                            return u;
                        }

                    private:
                        void ComputeArcLength () {
                            lengths.resize (polygons.size () + 1);
                            lengths[0] = 0.0f;
                            for (std::size_t i = 0, count = polygons.size (); i < count; ++i) {
                                lengths[i + 1] = lengths[i] + polygons[i].Length (1.0f);
                            }
                        }
                        void InvertIntegral (util::f32 s, std::size_t &i, util::f32 &u) const {
                            // Clamp s to [0, L] so that t in [tmin, tmax].
                            if (s <= 0.0f) {
                                i = 0;
                                u = 0.0f;
                            }
                            else {
                                if (s >= GetTotalLength ()) {
                                    i = polygons.size () - 1;
                                    u = 1.0f;
                                }
                                else {
                                    // Determine which polynomial corresponds to s.
                                    util::f32 distance;
                                    for (i = 0; i < polygons.size (); ++i) {
                                        if (s <= lengths[i + 1]) {
                                            // Distance along segment.
                                            distance = s - lengths[i];
                                            // Initial guess for inverting the arc length integral.
                                            u = distance / (lengths[i + 1] - lengths[i]);
                                            break;
                                        }
                                    }
                                    // Use Newton's method to invert the arc length integral.
                                    const util::f32 tolerance = 1e-06f;
                                    const util::ui32 maxIterations = 32;
                                    for (util::ui32 iteration = 0; iteration < maxIterations; ++iteration) {
                                        util::f32 difference = polygons[i].Length (u) - distance;
                                        if (fabsf (difference) <= tolerance) {
                                            break;
                                        }
                                        // assert: speed > 0
                                        u -= difference / polygons[i].Speed (u);
                                    }
                                }
                            }
                        }
                    };

                    struct PickInfo {
                    public:
                        enum Type {
                            Vertex,
                            Segment,
                            Polygon,
                            PolygonWindow     // Pick all polygons which lay entirely within a given region.
                        };

                    private:
                        Type type;
                        const std::vector<io::BezierPolygon<T> *> &bezierPolygons;
                        THEKOGANS_UTIL_TYPENAME blas::Region<T>::UniquePtr region;
                        util::ui32 steps;
                        bool frozen;

                    public:
                        std::size_t polygonIndex;
                        std::size_t vertexIndex;

                        PickInfo (
                            Type type_,
                            const std::vector<io::BezierPolygon<T> *> &bezierPolygons_,
                            THEKOGANS_UTIL_TYPENAME blas::Region<T>::UniquePtr region_,
                            util::ui32 steps_,
                            bool frozen_ = false,
                            std::size_t polygonIndex_ = 0) :
                            type (type_),
                            bezierPolygons (bezierPolygons_),
                            region (std::move (region_)),
                            steps (steps_),
                            frozen (frozen_),
                            polygonIndex (polygonIndex_),
                            vertexIndex (0) {}

                        bool FindFirst () {
                            switch (type) {
                                case Vertex: {
                                    return FindVertex ();
                                }
                                case Segment: {
                                    return FindPolygon ();
                                }
                                case Polygon: {
                                    return FindPolygon ();
                                }
                                case PolygonWindow: {
                                    return FindPolygonWindow ();
                                }
                            }
                            return false;
                        }

                        bool FindNext () {
                            switch (type) {
                                case Vertex: {
                                    ++vertexIndex;
                                    return FindVertex ();
                                }
                                case Segment: {
                                    ++vertexIndex;
                                    return FindPolygon ();
                                }
                                case Polygon: {
                                    ++polygonIndex;
                                    vertexIndex = 0;
                                    return FindPolygon ();
                                }
                                case PolygonWindow: {
                                    ++polygonIndex;
                                    vertexIndex = 0;
                                    return FindPolygonWindow ();
                                }
                            }
                            return false;
                        }

                    private:
                        bool FindVertex () {
                            for (std::size_t polygonCount = bezierPolygons.size ();
                                    polygonIndex < polygonCount; ++polygonIndex, vertexIndex = 0) {
                                if (!BezierPolygon<T> (*bezierPolygons[polygonIndex]).IsHidden () &&
                                        (frozen || !BezierPolygon<T> (*bezierPolygons[polygonIndex]).IsFrozen ())) {
                                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                        bezierPolygons[polygonIndex]->vertices;
                                    for (std::size_t vertexCount = vertices.size (); vertexIndex < vertexCount; ++vertexIndex) {
                                        if (region->PointInRegion (vertices[vertexIndex].pt)) {
                                            return true;
                                        }
                                    }
                                }
                            }
                            return false;
                        }

                        bool FindPolygon () {
                            for (std::size_t polygonCount = bezierPolygons.size ();
                                    polygonIndex < polygonCount; ++polygonIndex, vertexIndex = 0) {
                                if (!BezierPolygon<T> (*bezierPolygons[polygonIndex]).IsHidden () &&
                                        (frozen || !BezierPolygon<T> (*bezierPolygons[polygonIndex]).IsFrozen ())) {
                                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                        bezierPolygons[polygonIndex]->vertices;
                                    if (!vertices.empty ()) {
                                        for (std::size_t vertexCount = vertices.size () - 1;
                                                vertexIndex < vertexCount; ++vertexIndex) {
                                            if (SegmentInBound (vertices[vertexIndex], vertices[vertexIndex + 1]) ||
                                                    SegmentCrossBound (vertices[vertexIndex], vertices[vertexIndex + 1])) {
                                                return true;
                                            }
                                        }
                                        if (bezierPolygons[polygonIndex]->IsClosed () &&
                                                (SegmentInBound (vertices.back (), vertices[0]) ||
                                                    SegmentCrossBound (vertices.back (), vertices[0]))) {
                                            return true;
                                        }
                                    }
                                }
                            }
                            return false;
                        }

                        bool FindPolygonWindow () {
                            for (std::size_t polygonCount = bezierPolygons.size ();
                                    polygonIndex < polygonCount; ++polygonIndex, vertexIndex = 0) {
                                if (!BezierPolygon<T> (*bezierPolygons[polygonIndex]).IsHidden () &&
                                        (frozen || !BezierPolygon<T> (*bezierPolygons[polygonIndex]).IsFrozen ())) {
                                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                                        bezierPolygons[polygonIndex]->vertices;
                                    if (!vertices.empty ()) {
                                        std::size_t vertexCount = vertices.size () - 1;
                                        for (; vertexIndex < vertexCount; ++vertexIndex) {
                                            if (!SegmentInBound (vertices[vertexIndex], vertices[vertexIndex + 1])) {
                                                break;
                                            }
                                        }
                                        if (vertexIndex == vertexCount &&
                                                (!bezierPolygons[polygonIndex]->IsClosed () ||
                                                    SegmentInBound (vertices.back (), vertices[0]))) {
                                            return true;
                                        }
                                    }
                                }
                            }
                            return false;
                        }

                        inline bool SegmentInBound (
                                const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &v1,
                                const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &v2) {
                            return region->BezierCubicInRegion (
                                blas::BezierCubic<T> (v1.pt, v1.pt + v1.yellow, v2.pt + v2.red, v2.pt), steps, false);
                        }
                        inline bool SegmentCrossBound (
                                const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &v1,
                                const THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex &v2) {
                            return region->BezierCubicInRegion (
                                blas::BezierCubic<T> (v1.pt, v1.pt + v1.yellow, v2.pt + v2.red, v2.pt), steps, true);
                        }
                    };

                    explicit BezierPolygon (const io::BezierPolygon<T> &bezierPolygon_) :
                        bezierPolygon (bezierPolygon_) {}

                    bool IsSelected (util::ui16 selectMask) const;
                    bool IsShape () const;
                    bool IsHidden () const;
                    bool IsFrozen () const;

                    bool IsClockWise () const;
                    bool IsCircular (util::f32 eps = EPSILON) const;

                    inline std::size_t PreviousVertexIndex (std::size_t vertexIndex) const {
                        assert (!bezierPolygon.vertices.empty ());
                        return vertexIndex > 0 ?
                            vertexIndex - 1 : bezierPolygon.IsClosed () ?
                                bezierPolygon.vertices.size () - 1 : util::NIDX;
                    }
                    inline std::size_t NextVertexIndex (std::size_t vertexIndex) const {
                        assert (!bezierPolygon.vertices.empty ());
                        return vertexIndex < bezierPolygon.vertices.size () - 1 ?
                            vertexIndex + 1 : bezierPolygon.IsClosed () ?
                                0 : util::NIDX;
                    }

                    blas::Polygon<T> ToPolygon (util::ui32 steps,
                        util::ui16 detail = io::Lofter::ModelParams::High,
                        bool optimize = false) const;

                    blas::Bound2 GetBound2 (util::ui32 steps, const blas::Matrix3 &xform) const;
                    blas::Bound<T> GetBound (util::ui32 steps) const;

                    T GetCenterMass () const;

                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr SetFirst (std::size_t vertexIndex) const;

                    enum {
                        CONNECT_FIRST_FIRST,
                        CONNECT_FIRST_LAST,
                        CONNECT_LAST_FIRST,
                        CONNECT_LAST_LAST
                    };

                    // Description:
                    //      Connect two open polygons to create an open polygon.
                    // Parameters:
                    //      bezierPolygon - polygon to connect
                    //      type - CONNECT_FIRST_FIRST, CONNECT_FIRST_LAST,
                    //             CONNECT_LAST_FIRST or CONNECT_LAST_LAST
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr Connect (
                        const io::BezierPolygon<T> &bezierPolygon,
                        util::ui32 type) const;

                    // Description:
                    //      Connect two open polygons to create a closed polygon.
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr PolyConnect (
                        const io::BezierPolygon<T> &bezierPolygon,
                        util::ui32 type) const;

                    // Description:
                    //      Create an outline.
                    // Parameters:
                    //      width - width of outline
                    //      bezierPolygon1 - if the given polygon is open, return the entire outline.
                    //                       if closed, return the outer part of the outline.
                    //      bezierPolygon2 - if the given polygon is open, return 0.
                    //                       if closed, return the inner part of the outline.
                    // Remarks:
                    //      If the polygon is open, creates a closed polygon by
                    //      joining end vertices with that of the outline. If
                    //      the polygon is closed, creates another polygon.
                    void Outline (
                        util::f32 width,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon1,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon2) const;

                    util::f32 GetSegmentBoundIntersection (
                        std::size_t vertexIndex1,
                        std::size_t vertexIndex2,
                        const blas::Bound<T> &bound,
                        util::ui32 steps) const;
                    void DeleteSegment (
                        std::size_t vertexIndex1,
                        std::size_t vertexIndex2,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon1,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon2) const;
                    void BreakSegment (
                        std::size_t vertexIndex1,
                        std::size_t vertexIndex2,
                        util::f32 t,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon1,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon2) const;

                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr Simplify (util::f32 amount) const;
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr Linear () const;
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr Curve () const;
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr Weld (util::f32 eps = EPSILON) const;

                    void CrossHorizontal (
                        util::f32 y,
                        util::ui32 steps,
                        std::vector<util::f32> &intersections) const;
                    void CrossVertical (
                        util::f32 x,
                        util::ui32 steps,
                        std::vector<util::f32> &intersections) const;

                    bool InCircle (const blas::Circle &circle, util::ui32 steps) const;
                    bool CrossCircle (const blas::Circle &circle, util::ui32 steps) const;

                    bool InPolygon (const blas::Polygon<T> &polygon, util::ui32 steps) const;
                    bool CrossPolygon (const blas::Polygon<T> &polygon, util::ui32 steps) const;
                };

                typedef BezierPolygon<blas::Point2> BezierPolygon2;
                typedef BezierPolygon<blas::Point3> BezierPolygon3;

                template<typename T>
                bool BezierPolygon<T>::IsSelected (util::ui16 selectMask) const {
                    assert (util::Flags16 (selectMask).TestAny (
                        io::BezierPolygon<T>::Vertex::SelectionSetA |
                        io::BezierPolygon<T>::Vertex::SelectionSetB |
                        io::BezierPolygon<T>::Vertex::SelectionSetC));
                    assert (!util::Flags16 (selectMask).TestAny (
                        ~(io::BezierPolygon<T>::Vertex::SelectionSetA |
                        io::BezierPolygon<T>::Vertex::SelectionSetB |
                        io::BezierPolygon<T>::Vertex::SelectionSetC)));
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        if (!vertices[i].IsSelected (selectMask)) {
                            return false;
                        }
                    }
                    return true;
                }

                template<typename T>
                bool BezierPolygon<T>::IsShape () const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        if (!vertices[i].IsShape ()) {
                            return false;
                        }
                    }
                    return true;
                }

                template<typename T>
                bool BezierPolygon<T>::IsHidden () const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        if (!vertices[i].IsHidden ()) {
                            return false;
                        }
                    }
                    return true;
                }

                template<typename T>
                bool BezierPolygon<T>::IsFrozen () const {
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                        if (!vertices[i].IsFrozen ()) {
                            return false;
                        }
                    }
                    return true;
                }

                template<typename T>
                blas::Polygon<T> BezierPolygon<T>::ToPolygon (util::ui32 steps, util::ui16 detail, bool optimize) const {
                    blas::Polygon<T> polygon;
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        util::f32 delta = 0.0f;
                        util::ui32 increment = 0;
                        if (detail == io::Lofter::ModelParams::Medium) {
                            delta = 2.0f / (steps + 1);
                            increment = 2;
                        }
                        else if (detail == io::Lofter::ModelParams::High) {
                            delta = 1.0f / (steps + 1);
                            increment = 1;
                        }
                        for (std::size_t i = 0, count = vertices.size () - !bezierPolygon.IsClosed (); i < count; ++i) {
                            if (detail == io::Lofter::ModelParams::Low) {
                                polygon.points.push_back (vertices[i].pt);
                            }
                            else {
                                std::size_t next = (i + 1) % vertices.size ();
                                blas::BezierCubic<T> bezierCubic (vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                                    vertices[next].red + vertices[next].pt, vertices[next].pt);
                                polygon.points.push_back (bezierCubic.p1);
                                util::f32 t = delta;
                                for (util::ui32 j = 1; j < steps + 1; j += increment, t += delta) {
                                    polygon.points.push_back (bezierCubic.GetPoint (t, optimize));
                                }
                            }
                        }
                        if (!bezierPolygon.IsClosed ()) {
                            polygon.points.push_back (vertices.back ().pt);
                        }
                        else {
                            polygon.closed = true;
                        }
                    }
                    return polygon;
                }

                template<typename T>
                blas::Bound2 BezierPolygon<T>::GetBound2 (util::ui32 steps, const blas::Matrix3 &xform) const {
                    blas::Bound2 bound (blas::Bound2::Empty);
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        const util::f32 delta = 1.0f / (steps + 1);
                        for (std::size_t i = 0; i < vertices.size () - 1; ++i) {
                            blas::BezierCubic<T> bezierCubic (
                                vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                                vertices[i + 1].red + vertices[i + 1].pt, vertices[i + 1].pt);
                            util::f32 t = 0.0f;
                            for (util::ui32 j = 0; j <= steps + 1; ++j, t += delta) {
                                bound += blas::Point2 (bezierCubic.GetPoint (t) * xform);
                            }
                        }
                        if (bezierPolygon.IsClosed ()) {
                            blas::BezierCubic<T> bezierCubic (
                                vertices.back ().pt, vertices.back ().pt + vertices.back ().yellow,
                                vertices[0].red + vertices[0].pt, vertices[0].pt);
                            util::f32 t = 0.0f;
                            for (util::ui32 j = 0; j <= steps + 1; ++j, t += delta) {
                                bound += blas::Point2 (bezierCubic.GetPoint (t) * xform);
                            }
                        }
                    }
                    return bound;
                }

                template<typename T>
                blas::Bound<T> BezierPolygon<T>::GetBound (util::ui32 steps) const {
                    blas::Bound<T> bound (blas::Bound<T>::Empty);
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        const util::f32 delta = 1.0f / (steps + 1);
                        for (std::size_t i = 0; i < vertices.size () - 1; ++i) {
                            blas::BezierCubic<T> bezierCubic (
                                vertices[i].pt, vertices[i].pt + vertices[i].yellow,
                                vertices[i + 1].red + vertices[i + 1].pt, vertices[i + 1].pt);
                            util::f32 t = 0.0f;
                            for (util::ui32 j = 0; j <= steps + 1; ++j, t += delta) {
                                bound += bezierCubic.GetPoint (t);
                            }
                        }
                        if (bezierPolygon.IsClosed ()) {
                            blas::BezierCubic<T> bezierCubic (
                                vertices.back ().pt, vertices.back ().pt + vertices.back ().yellow,
                                vertices[0].red + vertices[0].pt, vertices[0].pt);
                            util::f32 t = 0.0f;
                            for (util::ui32 j = 0; j <= steps + 1; ++j, t += delta) {
                                bound += bezierCubic.GetPoint (t);
                            }
                        }
                    }
                    return bound;
                }

                template<typename T>
                T BezierPolygon<T>::GetCenterMass () const {
                    T center = T::Zero;
                    const std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> &vertices =
                        bezierPolygon.vertices;
                    if (!vertices.empty ()) {
                        for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                            center += vertices[i].pt;
                        }
                        center /= (util::f32)vertices.size ();
                    }
                    return center;
                }

                template<typename T>
                THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr BezierPolygon<T>::SetFirst (
                        std::size_t vertexIndex) const {
                    std::size_t count = bezierPolygon.vertices.size ();
                    assert (count > 1);
                    assert (vertexIndex > 0);
                    assert (bezierPolygon.IsClosed () ? vertexIndex < count : vertexIndex == count - 1);
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr newBezierPolygon (
                        new io::BezierPolygon<T> (bezierPolygon));
                    assert (newBezierPolygon.get () != 0);
                    if (newBezierPolygon.get () != 0) {
                        newBezierPolygon->vertices[0].flags &= ~io::BezierPolygon2::Vertex::First;
                        newBezierPolygon->vertices.back ().flags &= ~io::BezierPolygon2::Vertex::Last;
                        if (bezierPolygon.IsClosed ()) {
                            std::vector<THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex> vertices;
                            vertices.resize (vertexIndex);
                            memcpy (&vertices[0], &newBezierPolygon->vertices[0],
                                vertexIndex * sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                            memcpy (&newBezierPolygon->vertices[0], &newBezierPolygon->vertices[vertexIndex],
                                (count - vertexIndex) * sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                            memcpy (&newBezierPolygon->vertices[count - vertexIndex],
                                &vertices[0], vertexIndex * sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                        }
                        else {
                            std::size_t halfCount = count / 2;
                            for (std::size_t i = 0; i < halfCount; ++i) {
                                std::swap (
                                    newBezierPolygon->vertices[i].pt,
                                    newBezierPolygon->vertices[count - i - 1].pt);
                                std::swap (
                                    newBezierPolygon->vertices[i].red,
                                    newBezierPolygon->vertices[count - i - 1].yellow);
                                std::swap (
                                    newBezierPolygon->vertices[i].yellow,
                                    newBezierPolygon->vertices[count - i - 1].red);
                            }
                            if (count & 1) {
                                std::swap (
                                    newBezierPolygon->vertices[halfCount].red,
                                    newBezierPolygon->vertices[halfCount].yellow);
                            }
                        }
                        newBezierPolygon->vertices[0].flags |= io::BezierPolygon2::Vertex::First;
                        newBezierPolygon->vertices.back ().flags |= io::BezierPolygon2::Vertex::Last;
                    }
                    return newBezierPolygon;
                }

                template<typename T>
                THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr BezierPolygon<T>::Connect (
                        const io::BezierPolygon<T> &bezierPolygon2,
                        util::ui32 type) const {
                    assert (!bezierPolygon.IsClosed ());
                    assert (!bezierPolygon2.IsClosed ());
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr newBezierPolygon (
                        new io::BezierPolygon<T> (bezierPolygon));
                    std::size_t count = newBezierPolygon->vertices.size ();
                    newBezierPolygon->vertices.insert (newBezierPolygon->vertices.end (),
                        bezierPolygon2.vertices.begin (), bezierPolygon2.vertices.end ());
                    if (type == CONNECT_FIRST_FIRST || type == CONNECT_FIRST_LAST) {
                        std::size_t halfCount = count / 2;
                        for (std::size_t i = 0; i < halfCount; ++i) {
                            std::swap (
                                newBezierPolygon->vertices[i].pt,
                                newBezierPolygon->vertices[count - i - 1].pt);
                            std::swap (
                                newBezierPolygon->vertices[i].red,
                                newBezierPolygon->vertices[count - i - 1].yellow);
                            std::swap (
                                newBezierPolygon->vertices[i].yellow,
                                newBezierPolygon->vertices[count - i - 1].red);
                        }
                        if (count & 1) {
                            std::swap (
                                newBezierPolygon->vertices[halfCount].red,
                                newBezierPolygon->vertices[halfCount].yellow);
                        }
                    }
                    if (type == CONNECT_FIRST_LAST || type == CONNECT_LAST_LAST) {
                        std::size_t halfCount = bezierPolygon2.vertices.size () / 2;
                        for (std::size_t i = 0, newCount = newBezierPolygon->vertices.size (); i < halfCount; ++i) {
                            std::swap (
                                newBezierPolygon->vertices[count + i].pt,
                                newBezierPolygon->vertices[newCount - i - 1].pt);
                            std::swap (
                                newBezierPolygon->vertices[count + i].red,
                                newBezierPolygon->vertices[newCount - i - 1].yellow);
                            std::swap (
                                newBezierPolygon->vertices[count + i].yellow,
                                newBezierPolygon->vertices[newCount - i - 1].red);
                        }
                        if (bezierPolygon2.vertices.size () & 1) {
                            std::swap (
                                newBezierPolygon->vertices[count + halfCount].red,
                                newBezierPolygon->vertices[count + halfCount].yellow);
                        }
                    }
                    newBezierPolygon->vertices[count - 1].yellow = T::Zero;
                    newBezierPolygon->vertices[count].red = T::Zero;
                    return newBezierPolygon;
                }

                template<typename T>
                THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr BezierPolygon<T>::PolyConnect (
                        const io::BezierPolygon<T> &bezierPolygon,
                        util::ui32 type) const {
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr newBezierPolygon =
                        Connect (bezierPolygon, type);
                    assert (newBezierPolygon.get () != 0);
                    if (newBezierPolygon.get () != 0) {
                        newBezierPolygon->vertices.back ().flags |= io::BezierPolygon<T>::Vertex::Closed;
                    }
                    return newBezierPolygon;
                }

                template<typename T>
                void BezierPolygon<T>::DeleteSegment (
                        std::size_t vertexIndex1,
                        std::size_t vertexIndex2,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon1,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon2) const {
                    assert (vertexIndex1 != vertexIndex2);
                    assert (vertexIndex1 < bezierPolygon.vertices.size ());
                    assert (vertexIndex2 < bezierPolygon.vertices.size ());
                    assert (bezierPolygon.vertices.size () > 1);
                    if (bezierPolygon.IsClosed () || bezierPolygon.vertices.size () > 2) {
                        bezierPolygon1.reset (new io::BezierPolygon<T> (bezierPolygon));
                        assert (bezierPolygon1.get () != 0);
                        if (bezierPolygon.IsClosed ()) {
                            bezierPolygon1->vertices[0].flags &= ~io::BezierPolygon2::Vertex::First;
                            bezierPolygon1->vertices.back ().flags &=
                                ~(io::BezierPolygon2::Vertex::Closed | io::BezierPolygon2::Vertex::Last);
                            bezierPolygon1->vertices[vertexIndex1].yellow = T::Zero;
                            bezierPolygon1->vertices[vertexIndex2].red = T::Zero;
                            if (vertexIndex2 > 0) {
                                std::vector<io::BezierPolygon2::Vertex> tempVertices;
                                tempVertices.insert (tempVertices.begin (),
                                    bezierPolygon1->vertices.begin (),
                                    bezierPolygon1->vertices.begin () + vertexIndex2);
                                memcpy (
                                    &bezierPolygon1->vertices[0],
                                    &bezierPolygon1->vertices[vertexIndex2],
                                    (bezierPolygon1->vertices.size () - vertexIndex2) *
                                    sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                                memcpy (
                                    &bezierPolygon1->vertices[bezierPolygon1->vertices.size () - vertexIndex2],
                                    &tempVertices[0],
                                    vertexIndex2 * sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                            }
                            bezierPolygon1->vertices[0].flags |= io::BezierPolygon2::Vertex::First;
                            bezierPolygon1->vertices.back ().flags |= io::BezierPolygon2::Vertex::Last;
                        }
                        else if (vertexIndex1 == 0) {
                            bezierPolygon1->vertices.erase (bezierPolygon1->vertices.begin ());
                            bezierPolygon1->vertices[0].red = T::Zero;
                            bezierPolygon1->vertices[0].flags |= io::BezierPolygon2::Vertex::First;
                        }
                        else if (vertexIndex2 == bezierPolygon.vertices.size () - 1) {
                            bezierPolygon1->vertices.pop_back ();
                            bezierPolygon1->vertices.back ().yellow = T::Zero;
                            bezierPolygon1->vertices.back ().flags |= io::BezierPolygon2::Vertex::Last;
                        }
                        else {
                            bezierPolygon2.reset (new io::BezierPolygon<T> ());
                            assert (bezierPolygon2.get () != 0);
                            bezierPolygon2->vertices.insert (
                                bezierPolygon2->vertices.begin (),
                                bezierPolygon1->vertices.begin () + vertexIndex2,
                                bezierPolygon1->vertices.end ());
                            bezierPolygon2->vertices[0].flags |= io::BezierPolygon2::Vertex::First;
                            bezierPolygon1->vertices.erase (
                                bezierPolygon1->vertices.begin () + vertexIndex2,
                                bezierPolygon1->vertices.end ());
                            bezierPolygon1->vertices.back ().flags |= io::BezierPolygon2::Vertex::Last;
                        }
                    }
                }

                template<typename T>
                void BezierPolygon<T>::BreakSegment (
                        std::size_t vertexIndex1,
                        std::size_t vertexIndex2,
                        util::f32 t,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon1,
                        THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr &bezierPolygon2) const {
                    assert (vertexIndex1 != vertexIndex2);
                    assert (vertexIndex1 < bezierPolygon.vertices.size ());
                    assert (vertexIndex2 < bezierPolygon.vertices.size ());
                    assert (bezierPolygon.vertices.size () > 1);
                    assert (t >= 0.0f && t <= 1.0f);
                    blas::BezierCubic<T> left;
                    blas::BezierCubic<T> right;
                    blas::BezierCubic<T> (
                        bezierPolygon.vertices[vertexIndex1].pt,
                        bezierPolygon.vertices[vertexIndex1].pt + bezierPolygon.vertices[vertexIndex1].yellow,
                        bezierPolygon.vertices[vertexIndex2].pt + bezierPolygon.vertices[vertexIndex2].red,
                        bezierPolygon.vertices[vertexIndex2].pt).Split (t, left, right, false);
                    if (bezierPolygon.IsClosed ()) {
                        bezierPolygon1.reset (new io::BezierPolygon<T> (bezierPolygon));
                        bezierPolygon1->vertices[vertexIndex1].yellow = left.p2 - left.p1;
                        bezierPolygon1->vertices[vertexIndex2].red = right.p3 - right.p4;
                        bezierPolygon1->vertices.insert (bezierPolygon1->vertices.begin () + ++vertexIndex1,
                            THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex (left.p4, left.p3 - left.p4, T::Zero));
                        bezierPolygon1->vertices.insert (bezierPolygon1->vertices.begin () + ++vertexIndex1,
                            THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex (right.p1, T::Zero, right.p2 - right.p1));
                        bezierPolygon1 = BezierPolygon<T> (*bezierPolygon1.get ()).SetFirst (vertexIndex1);
                        bezierPolygon1->vertices.back ().flags &= ~io::BezierPolygon2::Vertex::Closed;
                    }
                    else {
                        {
                            bezierPolygon1.reset (new io::BezierPolygon<T>);
                            bezierPolygon1->vertices.resize (vertexIndex1 + 2);
                            memcpy (&bezierPolygon1->vertices[0], &bezierPolygon.vertices[0],
                                (vertexIndex1 + 1) * sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                            bezierPolygon1->vertices[vertexIndex1].yellow = left.p2 - left.p1;
                            bezierPolygon1->vertices.back () = THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex (
                                left.p4, left.p3 - left.p4, T::Zero, io::BezierPolygon<T>::Vertex::Last);
                        }
                        {
                            bezierPolygon2.reset (new io::BezierPolygon<T>);
                            bezierPolygon2->vertices.resize (bezierPolygon.vertices.size () - vertexIndex2 + 1);
                            bezierPolygon2->vertices[0] = THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex (
                                right.p1, T::Zero, right.p2 - right.p1, io::BezierPolygon<T>::Vertex::First);
                            memcpy (
                                &bezierPolygon2->vertices[1],
                                &bezierPolygon.vertices[vertexIndex2],
                                (bezierPolygon.vertices.size () - vertexIndex2) *
                                sizeof (THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::Vertex));
                            bezierPolygon2->vertices[1].red = right.p3 - right.p4;
                        }
                    }
                }

                template<typename T>
                THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr BezierPolygon<T>::Weld (util::f32 eps) const {
                    THEKOGANS_UTIL_TYPENAME io::BezierPolygon<T>::UniquePtr newBezierPolygon (new io::BezierPolygon<T>);
                    if (!bezierPolygon.vertices.empty ()) {
                        newBezierPolygon->vertices.push_back (bezierPolygon.vertices[0]);
                        for (std::size_t i = 1, count = bezierPolygon.vertices.size (); i < count; ++i) {
                            if (!bezierPolygon.vertices[i].pt.IsBetweenEq (
                                    newBezierPolygon->vertices.back ().pt, newBezierPolygon->vertices.back ().pt, eps)) {
                                newBezierPolygon->vertices.push_back (bezierPolygon.vertices[i]);
                            }
                            else {
                                newBezierPolygon->vertices.back ().yellow = bezierPolygon.vertices[i].yellow;
                            }
                        }
                        if (bezierPolygon.IsClosed ()) {
                            if (newBezierPolygon->vertices[0].pt.IsBetweenEq (
                                    newBezierPolygon->vertices.back ().pt, newBezierPolygon->vertices.back ().pt, eps)) {
                                newBezierPolygon->vertices[0].red = newBezierPolygon->vertices.back ().red;
                                newBezierPolygon->vertices.erase (newBezierPolygon->vertices.end () - 1);
                            }
                            newBezierPolygon->vertices.back ().flags |=
                                io::BezierPolygon<T>::Vertex::Last | io::BezierPolygon<T>::Vertex::Closed;
                        }
                    }
                    return newBezierPolygon;
                }

                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::IsClockWise () const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::IsCircular (util::f32 eps) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL void BezierPolygon2::Outline (
                    util::f32 width,
                    io::BezierPolygon2::UniquePtr &bezierPolygon1,
                    io::BezierPolygon2::UniquePtr &bezierPolygon2) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL util::f32 BezierPolygon2::GetSegmentBoundIntersection (
                    std::size_t vertexIndex1,
                    std::size_t vertexIndex2,
                    const blas::Bound2 &bound,
                    util::ui32 steps) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr BezierPolygon2::Simplify (
                    util::f32 amount) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr BezierPolygon2::Linear () const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr BezierPolygon2::Curve () const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL void BezierPolygon2::CrossHorizontal (util::f32 y,
                    util::ui32 steps, std::vector<util::f32> &intersections) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL void BezierPolygon2::CrossVertical (util::f32 x,
                    util::ui32 steps,
                    std::vector<util::f32> &intersections) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::InCircle (
                    const blas::Circle &circle,
                    util::ui32 steps) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::CrossCircle (
                    const blas::Circle &circle,
                    util::ui32 steps) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::InPolygon (
                    const blas::Polygon2 &polygon,
                    util::ui32 steps) const;
                template<>
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL bool BezierPolygon2::CrossPolygon (
                    const blas::Polygon2 &polygon,
                    util::ui32 steps) const;

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API FitPolygon (
                    const blas::Polygon2 &polygon,
                    util::f32 error);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Quad (
                    const blas::Bound2 &bound);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Circle (
                    const blas::Point2 &center,
                    util::f32 radius);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Ellipse (
                    const blas::Bound2 &bound);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Ngon (
                    const blas::Point2 &center,
                    util::f32 radius,
                    util::f32 angle,
                    std::size_t vertexCount,
                    bool curved);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Star1 (
                    const blas::Point2 &center,
                    util::f32 radius,
                    util::f32 angle,
                    bool curved);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Star2 (
                    const blas::Point2 &center,
                    util::f32 radius1,
                    util::f32 radius2,
                    util::f32 angle,
                    util::f32 skew,
                    std::size_t vertexCount,
                    bool curved);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Arc (
                    const blas::Point2 &center,
                    util::f32 radius,
                    util::f32 startAngle,
                    util::f32 spanAngle,
                    std::size_t vertexCount,
                    bool clockwise,
                    bool closed);
                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon3::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Helix (
                    const blas::Point3 &center,
                    util::f32 startRadius,
                    util::f32 endRadius,
                    util::f32 height,
                    util::f32 bias,
                    util::ui32 turns,
                    util::f32 degrees,
                    util::ui32 vertexCount,
                    bool clockwise);

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_shaper_BezierPolygon_h)
