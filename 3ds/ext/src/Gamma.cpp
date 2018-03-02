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

#include "thekogans/mex/3ds/ext/Gamma.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                void Gamma::BuildGammaTable (util::ui8 *gammaTable, util::f32 gamma) {
                    for (util::ui32 i = 0; i < 256; ++i) {
                        gammaTable[i] = GammaCorrect ((util::ui8)i, gamma);
                    }
                }

                void Gamma::BuildDeGammaTable (util::ui8 *degammaTable, util::f32 gamma) {
                    for (util::ui32 i = 0; i < 256; ++i) {
                        degammaTable[i] = DeGammaCorrect ((util::ui8)i, gamma);
                    }
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
