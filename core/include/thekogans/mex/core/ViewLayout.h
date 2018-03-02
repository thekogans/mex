// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_core_ViewLayout_h)
#define __thekogans_mex_core_ViewLayout_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/opengl/ViewLayout.h"
#include "thekogans/mex/core/Config.h"
#include "thekogans/mex/core/CursorMgr.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct Tool;
            struct Module;

            struct _LIB_THEKOGANS_MEX_CORE_DECL ViewLayout {
                typedef std::unique_ptr<ViewLayout> UniquePtr;

                Module &module;
                Tool *tool;
                util::ui16 capturedViewIndex;
                blas::Point lastPt;

                explicit ViewLayout (Module &module_) :
                    module (module_),
                    tool (0),
                    capturedViewIndex (util::NIDX16),
                    lastPt (-1, -1) {}

                void SetSize (const blas::Size &size);

                void SetFocus ();
                void KillFocus ();
                void LButtonDown (util::ui32 flags, const blas::Point &dp);
                void LButtonDblClk (util::ui32 flags, const blas::Point &dp);
                void LButtonUp (util::ui32 flags, const blas::Point &dp);
                void RButtonDown (util::ui32 flags, const blas::Point &dp);
                void RButtonUp (util::ui32 flags, const blas::Point &dp);
                void MouseMove (util::ui32 flags, const blas::Point &dp);
                void KeyDown (util::ui32 key, util::ui32 repeatCount, util::ui32 flags);
                void KeyUp (util::ui32 key, util::ui32 repeatCount, util::ui32 flags);

                void SetTool (Tool *tool_);

                void CaptureView ();
                void ReleaseView ();

            private:
                void ScrollView (blas::Point &dp);

                ViewLayout (const ViewLayout &);
                ViewLayout &operator = (const ViewLayout &);
            };

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_ViewLayout_h)
