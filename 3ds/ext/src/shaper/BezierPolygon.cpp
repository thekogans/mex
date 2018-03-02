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

#include <vector>
#include "thekogans/mex/blas/Line.h"
#include "thekogans/mex/blas/Polygon.h"
#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                namespace {
                    // Use least-squares method to find Bezier control points for region.
                    blas::BezierCubic2 GenerateBezierCubic (
                            const blas::Polygon2 &polygon,
                            std::size_t first,
                            std::size_t last,
                            const std::vector<util::f32> &uPrime,
                            const blas::Point2 &tHat1,
                            const blas::Point2 &tHat2) {
                        std::size_t nPts = last - first + 1;
                        // Precomputed rhs for eqn.
                        // Compute the A's.
                        std::vector<blas::Point2> A;
                        A.resize (nPts * 2);
                        {
                            blas::Point2 *a = &A[0];
                            for (std::size_t i = 0; i < nPts; ++i) {
                                *a++ = tHat1 * blas::B1 (uPrime[i]);
                                *a++ = tHat2 * blas::B2 (uPrime[i]);
                            }
                        }
                        // Create the C and X matrices.
                        util::f32 C[2][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
                        util::f32 X[2] = {0.0f, 0.0f};
                        {
                            for (std::size_t i = 0; i < nPts; ++i) {
                                C[0][0] += blas::Dot (A[i * 2], A[i * 2]);
                                C[0][1] += blas::Dot (A[i * 2], A[i * 2 + 1]);
                                //C[1][0] += blas::Dot (A[i * 2], A[i * 2 + 1]);
                                C[1][0] = C[0][1];
                                C[1][1] += blas::Dot (A[i * 2 + 1], A[i * 2 + 1]);
                                blas::Point2 tmp (
                                    polygon[first + i] -
                                    polygon[first] * blas::B0 (uPrime[i]) -
                                    polygon[first] * blas::B1 (uPrime[i]) -
                                    polygon[last] * blas::B2 (uPrime[i]) -
                                    polygon[last] * blas::B3 (uPrime[i]));
                                X[0] += blas::Dot (A[i * 2], tmp);
                                X[1] += blas::Dot (A[i * 2 + 1], tmp);
                            }
                        }
                        // Compute the determinants of C and X.
                        util::f32 det_C0_C1 = C[0][0] * C[1][1] - C[1][0] * C[0][1];
                        util::f32 det_C0_X = C[0][0] * X[1] - C[0][1] * X[0];
                        util::f32 det_X_C1 = X[0] * C[1][1] - X[1] * C[0][1];
                        // Finally, derive alpha values
                        if (det_C0_C1 == 0.0f) {
                            det_C0_C1 = (C[0][0] * C[1][1]) * 10e-12f;
                        }
                        util::f32 alpha_l = det_X_C1 / det_C0_C1;
                        util::f32 alpha_r = det_C0_X / det_C0_C1;
                        // If alpha negative, use the Wu/Barsky heuristic (see text).
                        if (alpha_l < 0.0f || alpha_r < 0.0f) {
                            util::f32 distance = (polygon[last] - polygon[first]).Length () / 3.0f;
                            return blas::BezierCubic2 (polygon[first], polygon[first] + tHat1 * distance,
                                polygon[last] + tHat2 * distance, polygon[last]);
                        }
                        // First and last control points of the Bezier
                        // curve are positioned exactly at the first and
                        // last data points Control points 1 and 2 are
                        // positioned an alpha distance out on the
                        // tangent vectors, left and right, respectively.
                        return blas::BezierCubic2 (polygon[first], polygon[first] + tHat1 * alpha_l,
                            polygon[last] + tHat2 * alpha_r, polygon[last]);
                    }

                    // Use Newton-Raphson iteration to find better root.
                    util::f32 NewtonRaphsonRootFind (
                            const blas::BezierCubic2 &Q,
                            const blas::Point2 &P,
                            util::f32 u) {
                        // Compute Q(u)
                        blas::Point2 Q_u = Q.GetPoint (u);
                        // Generate control vertices for Q'.
                        blas::BezierCubic2 Q1 (
                            (Q.p2 - Q.p1) * 3.0f,
                            (Q.p3 - Q.p2) * 3.0f,
                            (Q.p4 - Q.p3) * 3.0f,
                            blas::Point2::Zero);
                        // Generate control vertices for Q''
                        blas::BezierCubic2 Q2 (
                            (Q1.p2 - Q1.p1) * 2.0f,
                            (Q1.p3 - Q1.p2) * 2.0f,
                            blas::Point2::Zero,
                            blas::Point2::Zero);
                        // Compute Q'(u) and Q''(u).
                        blas::Point2 Q1_u = Q1.GetPoint (u);
                        blas::Point2 Q2_u = Q2.GetPoint (u);
                        // Compute f(u) / f'(u).
                        util::f32 numerator =
                            (Q_u.x - P.x) * Q1_u.x +
                            (Q_u.y - P.y) * Q1_u.y;
                        util::f32 denominator =
                            Q1_u.x * Q1_u.x +
                            Q1_u.y * Q1_u.y +
                            (Q_u.x - P.x) * Q2_u.x +
                            (Q_u.y - P.y) * Q2_u.y;
                        // u = u - f(u) / f'(u)
                        return denominator != 0.0f ? u - numerator / denominator : u;
                    }

                    // Given set of points and their parameterization, try to find
                    // a better parameterization.
                    void Reparameterize (
                            const blas::Polygon2 &polygon,
                            std::size_t first,
                            std::size_t last,
                            const std::vector<util::f32> &u,
                            const blas::BezierCubic2 &bezierCubic,
                            std::vector<util::f32> &uPrime) {
                        uPrime.resize (last - first + 1);
                        for (std::size_t i = first; i <= last; ++i) {
                            uPrime[i - first] = NewtonRaphsonRootFind (bezierCubic, polygon[i], u[i - first]);
                        }
                    }

                    // Assign parameter values to digitized points
                    // using relative distances between points.
                    void ChordLengthParameterize (
                            const blas::Polygon2 &polygon,
                            std::size_t first,
                            std::size_t last,
                            std::vector<util::f32> &u) {
                        u.resize (last - first + 1);
                        u[0] = 0.0f;
                        {
                            for (std::size_t i = first + 1; i <= last; ++i) {
                                u[i - first] = u[i - first - 1] + (polygon[i] - polygon[i - 1]).Length ();
                            }
                        }
                        if (u[last - first] != 0.0f) {
                            for (std::size_t i = first + 1; i <= last; ++i) {
                                u[i - first] /= u[last - first];
                            }
                        }
                    }

                    // Find the maximum squared distance of digitized points to fitted
                    // curve.
                    util::f32 ComputeMaxError (
                            const blas::Polygon2 &polygon,
                            std::size_t first,
                            std::size_t last,
                            const blas::BezierCubic2 &bezierCubic,
                            const std::vector<util::f32> &u,
                            std::size_t &splitPoint) {
                        util::f32 maxDistance = 0.0f;
                        splitPoint = (last - first + 1) / 2;
                        for (std::size_t i = first + 1; i < last; ++i) {
                            blas::Point2 pt = bezierCubic.GetPoint (u[i - first]) - polygon[i];
                            util::f32 distance = blas::Dot (pt, pt);
                            if (maxDistance <= distance) {
                                maxDistance = distance;
                                splitPoint = i;
                            }
                        }
                        return maxDistance;
                    }

                    void FitCubic (
                            io::BezierPolygon2 &bezierPolygon,
                            const blas::Polygon2 &polygon,
                            std::size_t first,
                            std::size_t last,
                            const blas::Point2 &tHat1,
                            const blas::Point2 &tHat2,
                            util::f32 error) {
                        // Use heuristic if region only has two points in it.
                        if (last - first + 1 == 2) {
                            util::f32 distance = (polygon[last] - polygon[first]).Length () / 3.0f;
                            bezierPolygon.vertices.push_back (
                                io::BezierPolygon2::Vertex (polygon[first],
                                    blas::Point2::Zero, tHat1 * distance));
                            bezierPolygon.vertices.push_back (
                                io::BezierPolygon2::Vertex (polygon[last],
                                    tHat2 * distance, blas::Point2::Zero));
                        }
                        else {
                            // Parameterize points, and attempt to fit curve.
                            std::vector<util::f32> u;
                            ChordLengthParameterize (polygon, first, last, u);
                            blas::BezierCubic2 bezierCubic = GenerateBezierCubic (
                                polygon, first, last, u, tHat1, tHat2);
                            // Find max deviation of points to fitted curve.
                            std::size_t splitPoint;
                            util::f32 maxError = ComputeMaxError (
                                polygon, first, last, bezierCubic, u, splitPoint);
                            if (maxError < error) {
                                bezierPolygon.vertices.push_back (
                                    io::BezierPolygon2::Vertex (
                                        bezierCubic.p1,
                                        blas::Point2::Zero,
                                        bezierCubic.p2 - bezierCubic.p1));
                                bezierPolygon.vertices.push_back (
                                    io::BezierPolygon2::Vertex (
                                        bezierCubic.p4,
                                        bezierCubic.p3 - bezierCubic.p4,
                                        blas::Point2::Zero));
                            }
                            else {
                                // If error not too large, try some reparameterization
                                // and iteration.
                                if (maxError < error * error) {
                                    const util::ui32 MAX_ITERATIONS = 4;
                                    for (util::ui32 i = 0; i < MAX_ITERATIONS; ++i) {
                                        std::vector<util::f32> uPrime;
                                        Reparameterize (polygon, first, last, u, bezierCubic, uPrime);
                                        bezierCubic = GenerateBezierCubic (
                                            polygon, first, last, uPrime, tHat1, tHat2);
                                        maxError = ComputeMaxError (
                                            polygon, first, last, bezierCubic, uPrime, splitPoint);
                                        if (maxError < error) {
                                            bezierPolygon.vertices.push_back (
                                                io::BezierPolygon2::Vertex (
                                                    bezierCubic.p1,
                                                    blas::Point2::Zero,
                                                    bezierCubic.p2 - bezierCubic.p1));
                                            bezierPolygon.vertices.push_back (
                                                io::BezierPolygon2::Vertex (
                                                    bezierCubic.p4,
                                                    bezierCubic.p3 - bezierCubic.p4,
                                                    blas::Point2::Zero));
                                            return;
                                        }
                                        u = uPrime;
                                    }
                                }
                                // Fitting failed, split at max error point and fit recursively.
                                blas::Point2 tHatCenter (polygon.ComputeCenterTangent (splitPoint));
                                FitCubic (bezierPolygon, polygon, first, splitPoint, tHat1, tHatCenter, error);
                                FitCubic (bezierPolygon, polygon, splitPoint, last, -tHatCenter, tHat2, error);
                            }
                        }
                    }

                    void WeldVertices (io::BezierPolygon2 &bezierPolygon, util::f32 eps) {
                        // FIXME: implement
                        return;
                        for (std::size_t i = 0; i < bezierPolygon.vertices.size (); ++i) {
                            for (std::size_t j = i + 1; j < bezierPolygon.vertices.size ();) {
                                if (!bezierPolygon.vertices[i].pt.IsBetweenEq (
                                        bezierPolygon.vertices[j].pt, bezierPolygon.vertices[j].pt, eps)) {
                                    break;
                                }
                                bezierPolygon.vertices[i].yellow = bezierPolygon.vertices[j].yellow;
                                bezierPolygon.vertices.erase (bezierPolygon.vertices.begin () + j);
                            }
                        }
                        if (bezierPolygon.IsClosed () && bezierPolygon.vertices.back ().pt.IsBetweenEq (
                                bezierPolygon.vertices[0].pt, bezierPolygon.vertices[0].pt, eps)) {
                            bezierPolygon.vertices[0].red = bezierPolygon.vertices.back ().red;
                            bezierPolygon.vertices.erase (bezierPolygon.vertices.begin () +
                                bezierPolygon.vertices.size () - 1);
                        }
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API FitPolygon (
                        const blas::Polygon2 &polygon, util::f32 error) {
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    if (polygon.points.size () > 1) {
                        FitCubic (*bezierPolygon.get (), polygon, 0,
                            polygon.points.size () - 1, polygon.ComputeLeftTangent (0),
                            polygon.ComputeRightTangent (polygon.points.size () - 1), error);
                        WeldVertices (*bezierPolygon.get (), util::EPSILON);
                        if (!bezierPolygon->vertices.empty ()) {
                            bezierPolygon->vertices[0].flags = io::BezierPolygon2::Vertex::First;
                            bezierPolygon->vertices.back ().flags = io::BezierPolygon2::Vertex::Last;
                        }
                    }
                    else {
                        // FIXME: implement
                        assert (0);
                    }
                    return bezierPolygon;
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Quad (
                        const blas::Bound2 &bound) {
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    bezierPolygon->vertices.resize (4);
                    bezierPolygon->vertices[0] = io::BezierPolygon2::Vertex (
                        blas::Point2 (bound.min.x, bound.min.y), blas::Point2::Zero, blas::Point2::Zero,
                        io::BezierPolygon2::Vertex::First);
                    bezierPolygon->vertices[1] = io::BezierPolygon2::Vertex (
                        blas::Point2 (bound.max.x, bound.min.y));
                    bezierPolygon->vertices[2] = io::BezierPolygon2::Vertex (
                        blas::Point2 (bound.max.x, bound.max.y));
                    bezierPolygon->vertices[3] = io::BezierPolygon2::Vertex (
                        blas::Point2 (bound.min.x, bound.max.y), blas::Point2::Zero, blas::Point2::Zero,
                        io::BezierPolygon2::Vertex::Last | io::BezierPolygon2::Vertex::Closed);
                    return bezierPolygon;
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Circle (
                        const blas::Point2 &center, util::f32 radius) {
                    return Arc (center, radius, 0.0f, util::TWOPI, 4, false, true);
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Ellipse (
                        const blas::Bound2 &bound) {
                    const util::f32 k = 0.5522847498308f;
                    util::f32 a = fabsf (bound.max.x - bound.min.x) * 0.5f;
                    util::f32 b = fabsf (bound.max.y - bound.min.y) * 0.5f;
                    blas::Point2 center = bound.Center ();
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    bezierPolygon->vertices.resize (4);
                    bezierPolygon->vertices[0] = io::BezierPolygon2::Vertex (
                        blas::Point2 (0.0f, -b) + center, blas::Point2 (-a * k, 0.0f), blas::Point2 (a * k, 0.0f),
                        io::BezierPolygon2::Vertex::First);
                    bezierPolygon->vertices[1] = io::BezierPolygon2::Vertex (
                        blas::Point2 (a, 0.0f) + center, blas::Point2 (0.0f, -b * k), blas::Point2 (0.0f, b * k));
                    bezierPolygon->vertices[2] = io::BezierPolygon2::Vertex (
                        blas::Point2 (0.0f, b) + center, blas::Point2 (a * k, 0.0f), blas::Point2 (-a * k, 0.0f));
                    bezierPolygon->vertices[3] = io::BezierPolygon2::Vertex (
                        blas::Point2 (-a, 0.0f) + center, blas::Point2 (0.0f, b * k), blas::Point2 (0.0f, -b * k),
                        io::BezierPolygon2::Vertex::Last | io::BezierPolygon2::Vertex::Closed);
                    return bezierPolygon;
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Ngon (
                        const blas::Point2 &center,
                        util::f32 radius,
                        util::f32 angle,
                        std::size_t vertexCount,
                        bool curved) {
                    if (curved) {
                        return Arc (center, radius, angle, util::TWOPI, vertexCount, false, true);
                    }
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    bezierPolygon->vertices.resize (vertexCount);
                    util::f32 dangle = util::TWOPI / vertexCount;
                    for (std::size_t i = 0; i < vertexCount; ++i) {
                        bezierPolygon->vertices[i] = io::BezierPolygon2::Vertex (
                            blas::Point2 (center.x + radius * cosf (angle + i * dangle),
                            center.y + radius * (util::f32)sin (angle + i * dangle)));
                    }
                    bezierPolygon->vertices[0].flags = io::BezierPolygon2::Vertex::First;
                    bezierPolygon->vertices.back ().flags = io::BezierPolygon2::Vertex::Last | io::BezierPolygon2::Vertex::Closed;
                    return bezierPolygon;
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Star1 (
                        const blas::Point2 &center,
                        util::f32 radius,
                        util::f32 angle,
                        bool curved) {
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    bezierPolygon->vertices.resize (10);
                    util::f32 dangle = util::TWOPI / 5;
                    for (std::size_t i = 0; i < 5; ++i) {
                        bezierPolygon->vertices[i * 2] = io::BezierPolygon2::Vertex (
                            blas::Point2 (
                                center.x + radius * cosf (angle + i * dangle),
                                center.y + radius * sinf (angle + i * dangle)));
                    }
                    blas::Point2 pt;
                    blas::Line2 (bezierPolygon->vertices[0].pt, bezierPolygon->vertices[4].pt).CrossLine (
                        blas::Line2 (bezierPolygon->vertices[2].pt, bezierPolygon->vertices[8].pt), pt);
                    bezierPolygon->vertices[1] = io::BezierPolygon2::Vertex (pt);
                    blas::Line2 (bezierPolygon->vertices[0].pt, bezierPolygon->vertices[4].pt).CrossLine (
                        blas::Line2 (bezierPolygon->vertices[2].pt, bezierPolygon->vertices[6].pt), pt);
                    bezierPolygon->vertices[3] = io::BezierPolygon2::Vertex (pt);
                    blas::Line2 (bezierPolygon->vertices[4].pt, bezierPolygon->vertices[8].pt).CrossLine (
                        blas::Line2 (bezierPolygon->vertices[2].pt, bezierPolygon->vertices[6].pt), pt);
                    bezierPolygon->vertices[5] = io::BezierPolygon2::Vertex (pt);
                    blas::Line2 (bezierPolygon->vertices[4].pt, bezierPolygon->vertices[8].pt).CrossLine (
                        blas::Line2 (bezierPolygon->vertices[0].pt, bezierPolygon->vertices[6].pt), pt);
                    bezierPolygon->vertices[7] = io::BezierPolygon2::Vertex (pt);
                    blas::Line2 (bezierPolygon->vertices[0].pt, bezierPolygon->vertices[6].pt).CrossLine (
                        blas::Line2 (bezierPolygon->vertices[2].pt, bezierPolygon->vertices[8].pt), pt);
                    bezierPolygon->vertices[9] = io::BezierPolygon2::Vertex (pt);
                    bezierPolygon->vertices[0].flags = io::BezierPolygon2::Vertex::First;
                    bezierPolygon->vertices.back ().flags =
                        io::BezierPolygon2::Vertex::Last |
                        io::BezierPolygon2::Vertex::Closed;
                    return curved ? BezierPolygon2 (*bezierPolygon).Curve () : std::move (bezierPolygon);
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Star2 (
                        const blas::Point2 &center,
                        util::f32 radius1,
                        util::f32 radius2,
                        util::f32 angle,
                        util::f32 skew,
                        std::size_t vertexCount,
                        bool curved) {
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    bezierPolygon->vertices.resize (vertexCount * 2);
                    util::f32 dangle = util::TWOPI / vertexCount;
                    util::f32 halfDangle = dangle * 0.5f;
                    for (std::size_t i = 0; i < vertexCount; ++i) {
                        bezierPolygon->vertices[i * 2] = io::BezierPolygon2::Vertex (
                            blas::Point2 (center.x + radius1 * cosf (angle + i * dangle + skew),
                            center.y + radius1 * sinf (angle + i * dangle + skew)));
                        bezierPolygon->vertices[i * 2 + 1] = io::BezierPolygon2::Vertex (
                            blas::Point2 (center.x + radius2 * cosf (angle + i * dangle + halfDangle),
                            center.y + radius2 * sinf (angle + i * dangle + halfDangle)));
                    }
                    bezierPolygon->vertices[0].flags = io::BezierPolygon2::Vertex::First;
                    bezierPolygon->vertices.back ().flags =
                        io::BezierPolygon2::Vertex::Last |
                        io::BezierPolygon2::Vertex::Closed;
                    return curved ? BezierPolygon2 (*bezierPolygon).Curve () : std::move (bezierPolygon);
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Arc (
                        const blas::Point2 &center,
                        util::f32 radius,
                        util::f32 startAngle,
                        util::f32 spanAngle,
                        std::size_t vertexCount,
                        bool clockwise,
                        bool closed) {
                    io::BezierPolygon2::UniquePtr bezierPolygon (new io::BezierPolygon2 ());
                    bezierPolygon->vertices.resize (vertexCount);
                    util::f32 delta = spanAngle / (vertexCount - !closed);
                    if (clockwise) {
                        delta *= -1.0f;
                    }
                    for (std::size_t i = 0, count = vertexCount - !closed; i < count; ++i) {
                        blas::BezierCubic2 bezierCubic = blas::Arc (center, radius, startAngle + i * delta, delta);
                        bezierPolygon->vertices[i].pt = bezierCubic.p1;
                        bezierPolygon->vertices[i].yellow = bezierCubic.p2 - bezierCubic.p1;
                        bezierPolygon->vertices[(i + 1) % vertexCount].red = bezierCubic.p3 - bezierCubic.p4;
                        if (!closed) {
                            if (i == 0) {
                                bezierPolygon->vertices[i].red = blas::Point2::Zero;
                            }
                            else if (i == vertexCount - 2) {
                                bezierPolygon->vertices[i + 1].pt = bezierCubic.p4;
                                bezierPolygon->vertices[i + 1].yellow = blas::Point2::Zero;
                            }
                        }
                    }
                    bezierPolygon->vertices[0].flags = io::BezierPolygon2::Vertex::First;
                    bezierPolygon->vertices.back ().flags = io::BezierPolygon2::Vertex::Last;
                    if (closed) {
                        bezierPolygon->vertices.back ().flags |= io::BezierPolygon2::Vertex::Closed;
                    }
                    return bezierPolygon;
                }

                namespace {
                    inline util::f32 BiasPercent (util::f32 bias, util::f32 pct) {
                        return
                            bias > 0.0f ? 1.0f - powf (1.0f - pct, bias * 9.0f + 1.0f):
                            bias < 0.0f ? powf (pct, -bias * 9.0f + 1.0f) : pct;
                    }
                }

                _LIB_THEKOGANS_MEX_3DS_EXT_DECL io::BezierPolygon3::UniquePtr _LIB_THEKOGANS_MEX_3DS_EXT_API Helix (
                        const blas::Point3 &center,
                        util::f32 startRadius,
                        util::f32 endRadius,
                        util::f32 height,
                        util::f32 bias,
                        std::size_t turns,
                        util::f32 degrees,
                        std::size_t vertexCount,
                        bool clockwise) {
                    io::BezierPolygon3::UniquePtr bezierPolygon (new io::BezierPolygon3 ());
                    bezierPolygon->vertices.resize (vertexCount);
                    util::f32 z = 0.0f, dz = height / (vertexCount - 1);
                    util::f32 angle = 0.0f, dangle = degrees * turns / (vertexCount - 1);
                    util::f32 r = startRadius, dr = (endRadius - startRadius) / (vertexCount - 1);
                    if (clockwise) {
                        dangle *= -1;
                    }
                    for (std::size_t i = 0; i < vertexCount - 1; ++i, r += dr, angle += dangle, z += dz) {
                        blas::BezierCubic2 arc = blas::Arc (blas::Point2::Zero, r, angle, dangle);
                    #if defined (DONT_USE_BIAS)
                        blas::Point3 p1 (arc.p1, z);
                        blas::Point3 p2 (arc.p1, z + 1.0f / 3.0f * dz);
                        blas::Point3 p3 (arc.p1, z + 2.0f / 3.0f * dz);
                        blas::Point3 p4 (arc.p1, z + dz);
                    #else // defined (DONT_USE_BIAS)
                        blas::Point3 p1 (arc.p1, height * BiasPercent (bias, z / height));
                        blas::Point3 p2 (arc.p2, height * BiasPercent (bias, (z + 1.0f / 3.0f * dz) / height));
                        blas::Point3 p3 (arc.p3, height * BiasPercent (bias, (z + 2.0f / 3.0f * dz) / height));
                        blas::Point3 p4 (arc.p4, height * BiasPercent (bias, (z + dz) / height));
                    #endif // defined (DONT_USE_BIAS)
                        bezierPolygon->vertices[i].pt = p1;
                        bezierPolygon->vertices[i].yellow = p2 - p1;
                        bezierPolygon->vertices[i + 1].red = p3 - p4;
                        if (i == 0) {
                            bezierPolygon->vertices[i].red = blas::Point3::Zero;
                        }
                        else if (i == vertexCount - 2) {
                            bezierPolygon->vertices[i + 1].pt = p4;
                            bezierPolygon->vertices[i + 1].yellow = blas::Point3::Zero;
                        }
                    }
                    bezierPolygon->vertices[0].flags = io::BezierPolygon2::Vertex::First;
                    bezierPolygon->vertices.back ().flags = io::BezierPolygon2::Vertex::Last;
                    return bezierPolygon;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
