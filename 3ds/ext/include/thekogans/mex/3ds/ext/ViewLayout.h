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

#if !defined (__thekogans_mex_3ds_ext_ViewLayout_h)
#define __thekogans_mex_3ds_ext_ViewLayout_h

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Fraction.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL ViewLayout {
                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Dimension {
                    public:
                        typedef std::vector<util::Fraction> Coverage;

                    private:
                        static const util::ui16 BORDER_WIDTH;
                        static const util::ui16 VIEW_FRAME_WIDTH;
                        static const util::ui16 EXTERNAL_BORDER_WIDTH;
                        static const util::ui16 INTERNAL_BORDER_WIDTH;

                        const util::ui16 start;
                        const util::ui16 size;
                        const util::ui16 totalViews;
                        Coverage coverage;
                        // Cached values used in GetViewSize. These are
                        // view agnostic. We amortize their cost by
                        // calculating them in the constructor.
                        // adjustedSize == entire dimention - all view borders.
                        util::ui16 adjustedSize;
                        // remainder == dimention part not taken up by
                        // any views. We distribute it evenly to first
                        // raminder views.
                        util::ui16 remainder;

                    public:
                        Dimension (
                            util::ui16 start_,
                            util::ui16 size_,
                            util::ui16 totalViews_,
                            const Coverage &coverage_ = Coverage ());

                        inline util::ui16 GetViewStart (util::ui16 view) const {
                            assert (view < totalViews);
                            util::ui16 viewStart = start + EXTERNAL_BORDER_WIDTH;
                            for (util::ui16 i = 0; i < view; ++i) {
                                viewStart += GetViewSize (i) + INTERNAL_BORDER_WIDTH;
                            }
                            return viewStart;
                        }
                        inline util::ui16 GetViewFrameStart (util::ui16 view) const {
                            return GetViewStart (view) - VIEW_FRAME_WIDTH;
                        }
                        inline util::ui16 GetViewHighlightStart (util::ui16 view) const {
                            return GetViewFrameStart (view) - (BORDER_WIDTH / 2) * 2;
                        }
                        inline util::ui16 GetViewBorderStart (util::ui16 view) const {
                            return GetViewHighlightStart (view);
                        }

                        inline util::ui16 GetViewSize (util::ui16 view) const {
                            assert (view < totalViews);
                            util::ui16 viewSize = adjustedSize * coverage[view];
                            // Spread the remainder by adding 1 to first remainder views.
                            // Much better (more symmetrical) then glumming the whole
                            // thing on to the last view.
                            if (view < remainder) {
                                ++viewSize;
                            }
                            return viewSize;
                        }
                        inline util::ui16 GetViewFrameSize (util::ui16 view) const {
                            return GetViewSize (view) + 2 * VIEW_FRAME_WIDTH;
                        }
                        inline util::ui16 GetViewHighlightSize (util::ui16 view) const {
                            return GetViewFrameSize (view) + 3 * BORDER_WIDTH / 2;
                        }
                        inline util::ui16 GetViewBorderSize (util::ui16 view) const {
                            return GetViewHighlightSize (view);
                        }

                    private:
                        Dimension (const Dimension &);
                        Dimension &operator = (const Dimension &);
                    };

                    const io::ViewLayout &viewLayout;

                    explicit ViewLayout (const io::ViewLayout &viewLayout_) :
                        viewLayout (viewLayout_) {}

                    util::ui16 ViewFromPoint (const blas::Point &pt) const;
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_ViewLayout_h)
