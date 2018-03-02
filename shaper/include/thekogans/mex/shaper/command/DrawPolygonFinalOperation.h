// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_shaper_command_DrawPolygonFinalOperation_h)
#define __thekogans_mex_shaper_command_DrawPolygonFinalOperation_h

#include <vector>
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/shaper/Config.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct _LIB_THEKOGANS_MEX_SHAPER_DECL DrawPolygonFinalOperation :
                    public thekogans::mex::command::FinalOperation {
                THEKOGANS_MEX_COMMAND_DECLARE_FINAL_OPERATION (DrawPolygonFinalOperation)

                const _3ds::io::View &view;
                const _3ds::io::BezierPolygon2 &bezierPolygon;

                explicit DrawPolygonFinalOperation (
                    const _3ds::io::View &view_,
                    const _3ds::io::BezierPolygon2 &bezierPolygon_) :
                    view (view_),
                    bezierPolygon (bezierPolygon_) {}

                virtual bool Execute ();
            };

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_shaper_command_DrawPolygonFinalOperation_h)
