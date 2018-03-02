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

#if !defined (__thekogans_mex_3ds_opengl_Tape_h)
#define __thekogans_mex_3ds_opengl_Tape_h

#include <cassert>
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/3ds/io/Tape.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                template<typename T>
                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Tape {
                    const io::Tape<T> &tape;

                    explicit Tape (const io::Tape<T> &tape_) :
                        tape (tape_) {}

                    inline void Draw (const View &view) const {
                        DrawWithColor (view, DefaultPalette[GREEN]);
                    }
                    inline void DrawWithColor (const View &view,
                            const thekogans::mex::opengl::ui8Color &color_ =
                                DefaultPalette[MEDIUM_GRAY]) const {
                        if (view.view.IsOrthographic ()) {
                            thekogans::mex::opengl::Color color (color_);
                            DrawLine3 (tape.start, tape.end);
                            DrawIcon (BOX_5x5, tape.start);
                            DrawIcon (BOX_3x3, tape.end);
                        }
                    }

                    inline bool IsVisible () const {
                        return tape.visible;
                    }
                    inline bool IsVSnap () const {
                        return tape.vsnap;
                    }
                };

                typedef Tape<blas::Point2> Tape2;
                typedef Tape<blas::Point3> Tape3;

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Tape_h)
