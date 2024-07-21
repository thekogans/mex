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

#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/opengl/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/opengl/editor/WireMesh.h"
#include "thekogans/mex/3ds/opengl/Axis.h"
#include "thekogans/mex/3ds/opengl/Tape.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/shaper/Hook.h"
#include "thekogans/mex/3ds/opengl/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Shaper)

                Shaper::Shaper (const ext::Shaper &shaper) : Module (shaper) {
                    // FIXME: implement
                }

                View::UniquePtr Shaper::GetView (const io::View &view) const {
                    assert (view.IsNone () || view.IsShape ());
                    return View::UniquePtr (new View (view));
                }

                View::UniquePtr Shaper::GetCurrView () const {
                    return GetView (ioShaper ().viewLayout.GetCurrView ());
                }

                void Shaper::Draw (const View &view) const {
                    // Draw background meshes.
                    if (ioShaper ().show3D) {
                        typedef std::vector<std::string>::const_iterator const_iterator;
                        for (const_iterator it = ioShaper ().meshes.begin (),
                                end = ioShaper ().meshes.end (); it != end; ++it) {
                            const io::Mesh *mesh = ioEditor ().GetMesh (*it);
                            if (mesh != 0) {
                                WireMesh (*mesh,
                                    ioProject ()).DrawWithColor (view, DefaultPalette[LIGHT_GRAY]);
                            }
                        }
                    }
                    // Draw polygons.
                    {
                        util::ui32 flags = BezierPolygon2::ShowVertices;
                        if (ioShaper ().showFirst) {
                            flags |= BezierPolygon2::ShowFirst;
                        }
                        typedef util::OwnerVector<io::BezierPolygon2>::const_iterator const_iterator;
                        for (const_iterator it = ioShaper ().polygons2.begin (),
                                end = ioShaper ().polygons2.end (); it != end; ++it) {
                            if (!ext::BezierPolygon2 (**it).IsHidden ()) {
                                BezierPolygon2 (
                                    **it,
                                    flags,
                                    ioShaper ().steps,
                                    io::Lofter::ModelParams::High,
                                    io::BezierPolygon2::Vertex::GetSelectionSet (
                                        ioShaper ().selectMask)).Draw (view);
                            }
                        }
                    }
                    // Draw decorations.
                    if (ioShaper ().axis.visible) {
                        opengl::Axis2 (ioShaper ().axis).Draw (view);
                    }
                    if (ioShaper ().tape.visible) {
                        opengl::Tape2 (ioShaper ().tape).Draw (view);
                    }
                    if (ioShaper ().hook.visible) {
                        opengl::Hook (ioShaper ().hook).Draw (view);
                    }
                }

                void Shaper::Swap (Shaper &shaper) {
                    // FIXME: implement
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
