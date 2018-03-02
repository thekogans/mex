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

#include <algorithm>
#include <cassert>
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                Lofter::Shape::Shape (const io::Lofter::Shape &shape_, util::ui32 steps,
                    util::ui16 datail, bool optimize) : shape (shape_) {
                    io::BezierPolygon2 polygon;
                    typedef io::BezierPolygon3::Vertex BezierVertex3;
                    typedef std::vector<BezierVertex3>::const_iterator const_iterator;
                    for (const_iterator vertex = shape.polygons.vertices.begin (),
                            endVertex = shape.polygons.vertices.end ();
                        vertex != endVertex; ++vertex) {
                        // Even though 3ds stores shapes as 3d splines (x, y, z), we assume
                        // they come from the 2d shaper (x, y). A number of algorithms we
                        // use assume splines are 2d. We verify this assumption here. (Note
                        // if one of these asserts ever fires, we'll need to rethink our
                        // approach).
                        assert (vertex->pt.z == 0.0f);
                        assert (vertex->red.z == 0.0f);
                        assert (vertex->yellow.z == 0.0f);
                        polygon.vertices.push_back (io::BezierPolygon2::Vertex (
                            blas::Point2 (vertex->pt.x, vertex->pt.y),
                            blas::Point2 (vertex->red.x, vertex->red.y),
                            blas::Point2 (vertex->yellow.x, vertex->yellow.y),
                            vertex->flags));
                        if (vertex->IsLast ()) {
                            assert (!polygon.vertices.empty ());
                            AddPolygon (BezierPolygon2 (polygon).ToPolygon (steps, datail, optimize));
                            polygon.vertices.clear ();
                        }
                    }
                }

                // Verify that the shape contains one outer loop
                // enclosing a number of inner loops. None of the
                // loops should intersect self or others.
                bool Lofter::Shape::IsValid () const {
                    if (polygons.empty ()) {
                        return false;
                    }
                    std::vector<blas::Polygon2 *> tempPolygons;
                    {
                        for (std::size_t i = 0, polygonCount = polygons.size (); i < polygonCount; ++i) {
                            if (polygons[i]->outer->points.size () < 2 || !polygons[i]->outer->closed ||
                                polygons[i]->outer->CrossSelf ()) {
                                return false;
                            }
                            tempPolygons.push_back (polygons[i]->outer.get ());
                            for (std::size_t j = 0, innerCount = polygons[i]->inner.size (); j < innerCount; ++j) {
                                if (polygons[i]->inner[j]->points.size () < 2 ||
                                    !polygons[i]->inner[j]->closed || polygons[i]->inner[j]->CrossSelf ()) {
                                    return false;
                                }
                                tempPolygons.push_back (polygons[i]->inner[j]);
                            }
                        }
                    }
                    {
                        for (std::size_t i = 0, outerCount = tempPolygons.size () - 1; i < outerCount; ++i) {
                            for (std::size_t j = i + 1, innerCount = tempPolygons.size (); j < innerCount; ++j) {
                                if (tempPolygons[i]->CrossPolygon (*tempPolygons[j])) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                }

                // Verify that two shapes contain the same number of loops,
                // with the same number of vertices in each corresponding loop.
                // FIXME: This is used to validate the model in Lofter::MakeMesh.
                // This test is necessary but not sufficient as it does not take
                // shape optimization in to account.
                bool Lofter::Shape::IsSimilar (const Lofter::Shape &other) const {
                    if (polygons.size () != other.polygons.size ()) {
                        return false;
                    }
                    for (std::size_t i = 0, polygonCount = polygons.size (); i < polygonCount; ++i) {
                        if (polygons[i]->outer->points.size () != other.polygons[i]->outer->points.size () ||
                            polygons[i]->inner.size () != other.polygons[i]->inner.size ()) {
                            return false;
                        }
                        for (std::size_t j = 0, innerCount = polygons[i]->inner.size (); j < innerCount; ++j) {
                            if (polygons[i]->inner[j]->points.size () !=
                                    other.polygons[i]->inner[j]->points.size ()) {
                                return false;
                            }
                        }
                    }
                    return true;
                }

                // Interpolate one shape in to another.
                void Lofter::Shape::Lerp (const Lofter::Shape &other, util::f32 t) {
                    // vectorize
                    for (std::size_t i = 0, polygonCount = polygons.size (); i < polygonCount; ++i) {
                        {
                            for (std::size_t j = 0,
                                    outerVertexCount = polygons[i]->outer->points.size ();
                                    j < outerVertexCount; ++j) {
                                polygons[i]->outer->points[j] +=
                                    t * (other.polygons[i]->outer->points[j] -
                                        polygons[i]->outer->points[j]);
                            }
                        }
                        {
                            for (std::size_t j = 0,
                                    innerCount = polygons[i]->inner.size ();
                                    j < innerCount; ++j) {
                                for (std::size_t k = 0,
                                        innerVertexCount = polygons[i]->inner[j]->points.size ();
                                        k < innerVertexCount; ++k) {
                                    polygons[i]->inner[j]->points[k] +=
                                        t * (other.polygons[i]->inner[j]->points[k] -
                                            polygons[i]->inner[j]->points[k]);
                                }
                            }
                        }
                    }
                }

                // Create an outline of a shape (used in bevel deformation).
                void Lofter::Shape::Outline (util::f32 width) {
                    // vectorize
                    for (std::size_t i = 0, polygonCount = polygons.size (); i < polygonCount; ++i) {
                        *polygons[i]->outer = polygons[i]->outer->Outline (width);
                        for (std::size_t j = 0, innerCount = polygons[i]->inner.size (); j < innerCount; ++j) {
                            *polygons[i]->inner[j] = polygons[i]->inner[j]->Outline (width);
                        }
                    }
                }

                blas::Bound2 Lofter::Shape::GetBound2 (const blas::Matrix3 &xform) const {
                    blas::Bound2 bound (blas::Bound2::Empty);
                    // vectorize
                    for (std::size_t i = 0, polygonCount = polygons.size (); i < polygonCount; ++i) {
                        bound += polygons[i]->outer->GetBound2 (xform);
                        for (std::size_t j = 0, innerCount = polygons[i]->inner.size (); j < innerCount; ++j) {
                            bound += polygons[i]->inner[j]->GetBound2 (xform);
                        }
                    }
                    return bound;
                }

                // This routine is only called from the ctor. We care about exception safety,
                // but we don't care about preserving transactional semantics. If this routine
                // throws, construction of the entire shape will be aborted.
                void Lofter::Shape::AddPolygon (const blas::Polygon2 &polygon) {
                    if (!polygon.points.empty ()) {
                        // Because shape polygons must not overlap, the new polygon can
                        // be in one of three places; outside all, inside one or enclosing
                        // one or more.
                        for (std::size_t i = polygons.size (); i--;) {
                            // See if an outer contains it.
                            if (polygon.points[0].InPolygon (*polygons[i]->outer)) {
                                // It does, scan the children to see if one of them contains it
                                for (std::size_t j = polygons[i]->inner.size (); j--;) {
                                    // One does, make it a new outer.
                                    if (polygon.points[0].InPolygon (*polygons[i]->inner[j])) {
                                        Polygon::UniquePtr shapePolygon (new Polygon (polygon));
                                        if (shapePolygon->outer->IsClockWise ()) {
                                            std::reverse (shapePolygon->outer->points.begin (),
                                                shapePolygon->outer->points.end ());
                                        }
                                        polygons.push_back (shapePolygon.get ());
                                        shapePolygon.release ();
                                        return;
                                    }
                                    // It contains one of the children.
                                    else if (polygons[i]->inner[j]->points[0].InPolygon (polygon)) {
                                        // Liberate this child.
                                        std::reverse (polygons[i]->inner[j]->points.begin (),
                                            polygons[i]->inner[j]->points.end ());
                                        Polygon::UniquePtr shapePolygon (
                                            new Polygon (*polygons[i]->inner[j]));
                                        polygons.push_back (shapePolygon.get ());
                                        shapePolygon.release ();
                                        polygons[i]->inner.erase (polygons[i]->inner.begin () + j);
                                    }
                                }
                                // No child contains it, make it a child of the outer.
                                blas::Polygon2::UniquePtr shapePolygon (new blas::Polygon2 (polygon));
                                if (!shapePolygon->IsClockWise ()) {
                                    std::reverse (shapePolygon->points.begin (), shapePolygon->points.end ());
                                }
                                polygons[i]->inner.push_back (shapePolygon.get ());
                                shapePolygon.release ();
                                return;
                            }
                            // See if it contains an outer.
                            else if (polygons[i]->outer->points[0].InPolygon (polygon)) {
                                // It does, check to see if there are others it contains.
                                Polygon::UniquePtr shapePolygon (new Polygon (polygon));
                                if (shapePolygon->outer->IsClockWise ()) {
                                    std::reverse (shapePolygon->outer->points.begin (),
                                        shapePolygon->outer->points.end ());
                                }
                                do {
                                    if (polygons[i]->outer->points[0].InPolygon (polygon)) {
                                        // Make the outer it's child
                                        std::reverse (polygons[i]->outer->points.begin (),
                                            polygons[i]->outer->points.end ());
                                        shapePolygon->inner.push_back (polygons[i]->outer.get ());
                                        polygons[i]->outer.release ();
                                        // Liberate the outers children.
                                        for (std::size_t j = 0,
                                                innerCount = polygons[i]->inner.size ();
                                                j < innerCount; ++j) {
                                            std::reverse (polygons[i]->inner[j]->points.begin (),
                                                polygons[i]->inner[j]->points.end ());
                                            Polygon::UniquePtr shapePolygon (
                                                new Polygon (*polygons[i]->inner[j]));
                                            polygons.push_back (shapePolygon.get ());
                                            shapePolygon.release ();
                                        }
                                        // Delete the outer.
                                        delete polygons[i];
                                        polygons.erase (polygons.begin () + i);
                                    }
                                } while (i--);
                                polygons.push_back (shapePolygon.get ());
                                shapePolygon.release ();
                                return;
                            }
                        }
                        // It contains nothing and nothing contains it.
                        // Add it as another outer polygon in the shape.
                        Polygon::UniquePtr shapePolygon (new Polygon (polygon));
                        if (shapePolygon->outer->IsClockWise ()) {
                            std::reverse (shapePolygon->outer->points.begin (),
                                shapePolygon->outer->points.end ());
                        }
                        polygons.push_back (shapePolygon.get ());
                        shapePolygon.release ();
                    }
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
