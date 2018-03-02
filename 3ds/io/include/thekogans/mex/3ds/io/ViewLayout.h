// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_ViewLayout_h)
#define __thekogans_mex_3ds_io_ViewLayout_h

#include <vector>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/FixedString.h"
#include "thekogans/mex/3ds/io/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL ViewLayout {
                    struct Layout {
                        enum Style {
                            Single,
                            TwoVert,
                            TwoHorz,
                            Four,
                            ThreeLeft,
                            ThreeBottom,
                            ThreeRight,
                            ThreeTop,
                            ThreeVert,
                            ThreeHorz,
                            FourLeft,
                        #define EXTENDED_VIEW_LAYOUT 1
                        #if EXTENDED_VIEW_LAYOUT
                            FourBottom,
                        #endif // EXTENDED_VIEW_LAYOUT
                            FourRight,
                        #if EXTENDED_VIEW_LAYOUT
                            FourTop,
                        #endif // EXTENDED_VIEW_LAYOUT
                        };
                        util::ui16 currStyle;
                        util::ui16 currViewIndex;
                        util::ui16 flags1;
                        bool maximized;
                        util::ui16 flags2;
                        util::ui16 lastStyle;
                        util::ui16 lastViewIndex;

                        Layout (util::ui16 style_ = Four) :
                            currStyle (style_),
                            currViewIndex (0),
                            flags1 (0),
                            maximized (0),
                            flags2 (0),
                            lastStyle (style_),
                            lastViewIndex (0) {}

                        // Call this to validate Shaper/Editor/Keyframer layout.
                        inline bool IsValid () const {
                            return maximized ?
                                currStyle == Single && currViewIndex == 0 &&
                                    lastStyle <= FourTop &&
                                    lastViewIndex < ViewLayout::GetViewCount (lastStyle) :
                                currStyle <= FourTop &&
                                currViewIndex < ViewLayout::GetViewCount (currStyle);
                        }

                        // Call this to validate Lofter layout. -> IsValid (FourLeft)
                        inline bool IsValid (util::ui16 style) const {
                            return maximized ?
                                currStyle == Single && currViewIndex == 0 &&
                                    lastStyle == style &&
                                    lastViewIndex < ViewLayout::GetViewCount (lastStyle) :
                                currStyle == style &&
                                currViewIndex < ViewLayout::GetViewCount (currStyle);
                        }

                        inline void Swap (Layout &layout) {
                            std::swap (currStyle, layout.currStyle);
                            std::swap (currViewIndex, layout.currViewIndex);
                            std::swap (flags1, layout.flags1);
                            std::swap (maximized, layout.maximized);
                            std::swap (flags2, layout.flags2);
                            std::swap (lastStyle, layout.lastStyle);
                            std::swap (lastViewIndex, layout.lastViewIndex);
                        }
                    } layout;
                    blas::Size size;
                    std::vector<View> views3;
                    std::vector<View> views;

                    inline const View &operator [] (std::size_t index) const {
                        assert (views3.size () == 5);
                        assert (index < views3.size ());
                        return views3[index];
                    }

                    inline View &operator [] (std::size_t index) {
                        assert (views3.size () == 5);
                        assert (index < views3.size ());
                        return views3[index];
                    }

                    inline const View &GetCurrView () const {
                        assert (views3.size () == 5);
                        assert (layout.currViewIndex < views3.size ());
                        return views3[layout.currViewIndex];
                    }

                    static util::ui32 GetViewCount (util::ui16 style) {
                        static const util::ui32 viewCount[] = {
                            1, 2, 2, 4, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4
                        };
                        return viewCount[style];
                    }

                    inline util::ui32 GetViewCount () const {
                        return GetViewCount (layout.currStyle);
                    }

                    inline void Swap (ViewLayout &viewLayout) {
                        layout.Swap (viewLayout.layout);
                        size.Swap (viewLayout.size);
                        views3.swap (viewLayout.views3);
                        views.swap (viewLayout.views);
                    }
                };

                _LIB_THEKOGANS_MEX_3DS_IO_DECL util::Serializer &
                _LIB_THEKOGANS_MEX_3DS_IO_API operator << (
                    util::Serializer &serializer,
                    const ViewLayout &viewLayout);
                _LIB_THEKOGANS_MEX_3DS_IO_DECL util::Serializer &
                _LIB_THEKOGANS_MEX_3DS_IO_API operator >> (
                    util::Serializer &serializer,
                    ViewLayout &viewLayout);

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_ViewLayout_h)
