// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_common.
//
// libthekogans_mex_common is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_common is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_common. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_common_CursorMgrSetClipSizeFinalOperation_h)
#define __thekogans_mex_common_CursorMgrSetClipSizeFinalOperation_h

#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/core/CursorMgr.h"

namespace thekogans {
    namespace mex {
        namespace common {

            struct CursorMgrSetClipSizeFinalOperation : public thekogans::command::FinalOperation {
                const blas::Size &size;

                explicit CursorMgrSetClipSizeFinalOperation (const blas::Size &size_) :
                    size (size_) {}

                virtual bool Execute () {
                    core::CursorMgr::Instance ().SetClipSize (size);
                    return true;
                }
            };

        } // namespace common
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_common_CursorMgrSetClipSizeFinalOperation_h)
