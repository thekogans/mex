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

#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/ext/shaper/Shaper.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                View::UniquePtr Shaper::GetView (const io::View &view) const {
                    assert (view.IsNone () || view.IsShape ());
                    return View::UniquePtr (new View (view));
                }

                View::UniquePtr Shaper::GetCurrView () const {
                    return GetView (ioShaper ().viewLayout.GetCurrView ());
                }

                blas::Bound2 Shaper::GetViewBound (const View &view) const {
                    blas::Bound2 bound (blas::Bound2::Empty);
                    typedef util::OwnerVector<io::BezierPolygon2>::const_iterator const_iterator;
                    for (const_iterator it = ioShaper ().polygons2.begin (), end = ioShaper ().polygons2.end (); it != end; ++it) {
                        bound += BezierPolygon2 (**it).GetBound2 (ioShaper ().steps, view.xform);
                    }
                    return bound;
                }

                void Shaper::Swap (Shaper &shaper) {
                    // FIXME: implement
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
