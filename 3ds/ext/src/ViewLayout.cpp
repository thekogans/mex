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

#include <numeric>
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/3ds/ext/ViewLayout.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                const util::ui16 ViewLayout::Dimension::BORDER_WIDTH = 3;
                const util::ui16 ViewLayout::Dimension::VIEW_FRAME_WIDTH = 1;
                const util::ui16 ViewLayout::Dimension::EXTERNAL_BORDER_WIDTH =
                    ViewLayout::Dimension::BORDER_WIDTH +
                    ViewLayout::Dimension::VIEW_FRAME_WIDTH;
                const util::ui16 ViewLayout::Dimension::INTERNAL_BORDER_WIDTH =
                    ViewLayout::Dimension::VIEW_FRAME_WIDTH +
                    ViewLayout::Dimension::BORDER_WIDTH +
                    ViewLayout::Dimension::VIEW_FRAME_WIDTH;

                ViewLayout::Dimension::Dimension (util::ui16 start_, util::ui16 size_,
                    util::ui16 totalViews_, const Coverage &coverage_) :
                    start (start_), size (size_),
                    totalViews (totalViews_),
                    coverage (coverage_),
                    adjustedSize (size - 2 * EXTERNAL_BORDER_WIDTH -
                        (totalViews - 1) * INTERNAL_BORDER_WIDTH) {
                    assert (totalViews > 0);
                    // If coverage is not provided, divide the
                    // space in this dimension equally amongst
                    // all views.
                    if (coverage.empty ()) {
                        coverage.insert (coverage.begin (), totalViews,
                            util::Fraction (1, totalViews));
                    }
                    // After everything is said and done, we
                    // have to have coverage info for all
                    // views. Either it was provided, or we
                    // generated it ourselves.
                    assert (coverage.size () == totalViews);
                    // Make sure view's fractional coverages
                    // add up to 1.
                    assert (std::accumulate (coverage.begin (), coverage.end (),
                        util::Fraction::Zero) == util::Fraction::One);
                    // Integer division being what it is, we
                    // calculate remainder by subtracting out
                    // all views from the adjustedSize. This
                    // is the exact remainder when coverage is
                    // taken in to account, and does not
                    // suffer from off by one errors that are
                    // sometimes present when doing (naively)
                    // remainder = adjustedSize % totalViews;
                    remainder = adjustedSize;
                    for (util::ui16 i = 0; i < totalViews; ++i) {
                        remainder -= adjustedSize * coverage[i];
                    }
                    assert (remainder < totalViews);
                }

                util::ui16 ViewLayout::ViewFromPoint (const blas::Point &pt) const {
                    for (util::ui16 i = 0,
                            count = viewLayout.GetViewCount (); i < count; ++i) {
                        if (pt.InSize (viewLayout[i].size)) {
                            return i;
                        }
                    }
                    return util::NIDX16;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
