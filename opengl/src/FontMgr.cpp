// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/opengl/FontMgr.h"
#include "system10.h"
#include "oldenglish18.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            util::OwnerMap<std::string, FontMgr::Font> FontMgr::fonts;

            util::i32 FontMgr::Font::Glyph::GetKernAmount (util::ui16 ch) const {
                util::ui32 count = GetKernInfoCount ();
                if (count) {
                    const std::pair<util::ui16, util::i32> *kernInfo = GetKernInfo ();
                    for (util::ui32 i = 0; i < count; ++i) {
                        if (kernInfo[i].first == ch) {
                            return kernInfo[i].second;
                        }
                    }
                }
                return 0;
            }

            FontMgr::FontMgr () :
                systemFont (new oldenglish18) {}

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
