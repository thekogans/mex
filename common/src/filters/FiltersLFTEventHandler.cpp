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

#include "thekogans/util/Path.h"
#include "thekogans/mex/3ds/io/StdErrorHandler.h"
#include "thekogans/mex/3ds/io/command/lofter/LofterCommands.h"
#include "thekogans/mex/3ds/ext/command/lofter/LofterCommands.h"
#include "thekogans/mex/3ds/opengl/command/lofter/LofterCommands.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/common/filters/FiltersLFTEventHandler.h"

namespace thekogans {
    namespace mex {
        namespace common {

            _3ds::io::Project::FileType FiltersLFTEventHandler::OnRead (
                    const std::string &path,
                     _3ds::io::Project &project,
                    _3ds::io::Progress *progress) {
                _3ds::io::StdErrorHandler errorHandler;
                project.Open (path, progress, &errorHandler);
                return _3ds::io::Project::LFT;
            }

            _3ds::io::Project::FileType FiltersLFTEventHandler::OnWrite (
                    const std::string &path,
                     _3ds::io::Project &project,
                    _3ds::io::Progress *) {
                // FIXME: validate lofter before saving
                assert (0);
                project.SaveLFT (path);
                return _3ds::io::Project::LFT;
            }

        } // namespace common
    } // namespace mex
} // namespace thekogans
