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

#if !defined (__thekogans_mex_3ds_io_Progress_h)
#define __thekogans_mex_3ds_io_Progress_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct Progress {
                    typedef std::unique_ptr<Progress> UniquePtr;

                    virtual ~Progress () {}

                    virtual void SetTotal (util::ui32 total) = 0;
                    virtual void SetPosition (util::ui32 position) = 0;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Progress_h)
