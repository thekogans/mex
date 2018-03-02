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

#if !defined (__thekogans_mex_3ds_ext_Gamma_h)
#define __thekogans_mex_3ds_ext_Gamma_h

#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                inline util::f32 GammaCorrect (util::f32 v, util::f32 gamma) {
                    return powf (v, 1.0f / gamma);
                }
                inline util::f32 DeGammaCorrect (util::f32 v, util::f32 gamma) {
                    return powf (v, gamma);
                }
                inline util::ui8 GammaCorrect (util::ui8 v, util::f32 gamma) {
                    return (util::ui8)(powf ((util::f32)v / 255.0f, 1.0f / gamma) * 255.0f);
                }
                inline util::ui8 DeGammaCorrect (util::ui8 v, util::f32 gamma) {
                    return (util::ui8)(powf ((util::f32)v / 255.0f, gamma) * 255.0f);
                }

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Gamma {
                    const io::Project::GammaControl &gammaControl;

                    util::ui8 displayGammaTable[256];
                    util::ui8 frameBufferGammaTable[256];
                    util::ui8 fileInDeGammaTable[256];
                    util::ui8 fileOutGammaTable[256];

                    Gamma (const io::Project::GammaControl &gammaControl_) :
                            gammaControl (gammaControl_) {
                        BuildGammaTable (displayGammaTable, gammaControl.display);
                        BuildGammaTable (frameBufferGammaTable, gammaControl.frameBuffer);
                        BuildDeGammaTable (fileInDeGammaTable, gammaControl.inFile);
                        BuildGammaTable (fileOutGammaTable, gammaControl.outFile);
                    }

                    inline opengl::ui8Color DisplayGammaCorrect (const opengl::ui8Color &color) {
                        return opengl::ui8Color (
                            displayGammaTable[color.r],
                            displayGammaTable[color.g],
                            displayGammaTable[color.b]);
                    }
                    inline opengl::f32Color DisplayGammaCorrect (const opengl::f32Color &color) {
                        return opengl::f32Color (
                            GammaCorrect (color.r, gammaControl.display),
                            GammaCorrect (color.g, gammaControl.display),
                            GammaCorrect (color.b, gammaControl.display));
                    }

                private:
                    void BuildGammaTable (util::ui8 *gammaTable, util::f32 gamma);
                    void BuildDeGammaTable (util::ui8 *deGammaTable, util::f32 gamma);
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_Gamma_h)
