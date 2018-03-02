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

#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/opengl/ModelView.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/opengl/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (Lofter)

                namespace {
                    const util::ui32 PathColors[BezierPolygon3::COLOR_COUNT] = {
                        CYAN, CYAN, CYAN, CYAN,
                        BLUE, BLUE, BLUE, BLUE,
                        BLUE, BLUE, BLUE, BLUE
                    };
                }

                void Lofter::Path::Draw (const View &view) const {
                    if (view.view.IsShape ()) {
                        Color color (DefaultPalette[WHITE]);
                        DrawIcon (LOFTER_PATH, blas::Point3::Zero);
                    }
                    else {
                        assert (!path.polygon.vertices.empty ());
                        BezierPolygon3 (
                            path.polygon,
                            BezierPolygon3::ShowVertices |
                            BezierPolygon3::ShowSteps |
                            BezierPolygon3::Optimize,
                            path.steps,
                            io::Lofter::ModelParams::High,
                            0,
                            PathColors).Draw (view);
                        Color color (DefaultPalette[WHITE]);
                        util::ui32 vidx = path.level / (path.steps + 1);
                        util::ui32 step = path.level % (path.steps + 1);
                        if (step != 0) {
                            const std::vector<io::BezierPolygon3::Vertex> vertices =
                                path.polygon.vertices;
                            std::size_t next = (vidx + 1) % vertices.size ();
                            blas::BezierCubic3 bezierCubic (
                                vertices[vidx].pt,
                                vertices[vidx].pt + vertices[vidx].yellow,
                                vertices[next].red + vertices[next].pt,
                                vertices[next].pt);
                            DrawIcon (
                                GetTickIcon (7),
                                bezierCubic.GetPoint ((util::f32)step / (path.steps + 1)));
                        }
                        else {
                            DrawIcon (GetTickIcon (7), path.polygon.vertices[vidx].pt);
                        }
                    }
                }

                Lofter::Lofter (const ext::Lofter &lofter) : Module (lofter) {
                    // FIXME: implement
                }

                View::UniquePtr Lofter::GetView (const io::View &view) const {
                    assert (view.IsNone () || view.IsOrthographic () || view.IsShape ());
                    return View::UniquePtr (new View (view));
                }

                View::UniquePtr Lofter::GetCurrView () const {
                    return GetView (ioLofter ().viewLayout.GetCurrView ());
                }

                void Lofter::Draw (const View &view) const {
                    Path (ioLofter ().path).Draw (view);
                    if (view.view.IsShape ()) {
                        const ext::Lofter::Shape *shape = extLofter ().GetCurrShape ();
                        if (shape != 0) {
                            Color color (DefaultPalette[WHITE]);
                            DrawShape (view, *shape, true);
                        }
                    }
                    else {
                        std::vector<blas::Matrix3> frames;
                        ext::BezierPolygon3 (
                            ioLofter ().path.polygon).ToPolygon (ioLofter ().path.steps).GetFrames (
                                frames, ioLofter ().contour);
                        for (std::size_t i = 0, count = extLofter ().shapes.size (); i < count; ++i) {
                            Color color (extLofter ().shapes[i]->shape.level == ioLofter ().path.level ?
                                DefaultPalette[WHITE] : DefaultPalette[YELLOW]);
                            util::ui16 level = extLofter ().shapes[i]->shape.level;
                            blas::Matrix4 frame (frames[level]);
                            ModelView modelView (frame, false);
                            DrawShape (view, *extLofter ().shapes[i], false);
                        }
                    }
                }

                void Lofter::DrawShape (
                        const View &,
                        const ext::Lofter::Shape &shape,
                        bool first) const {
                    for (std::size_t i = 0, polygonCount = shape.polygons.size ();
                            i < polygonCount; ++i) {
                        assert (shape.polygons[i]->outer.get () != 0);
                        DrawPolygon (*shape.polygons[i]->outer);
                        if (first) {
                            Color color (DefaultPalette[BLACK]);
                            DrawIcon (GetTickIcon (7), shape.polygons[i]->outer->points[0]);
                        }
                        for (std::size_t j = 0, innerCount = shape.polygons[i]->inner.size ();
                                j < innerCount; ++j) {
                            DrawPolygon (*shape.polygons[i]->inner[j]);
                            if (first) {
                                Color color (DefaultPalette[BLACK]);
                                DrawIcon (GetTickIcon (7), shape.polygons[i]->inner[j]->points[0]);
                            }
                        }
                    }
                }

                void Lofter::Swap (Lofter &lofter) {
                    // FIXME: implement
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
