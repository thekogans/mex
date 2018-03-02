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

#include "thekogans/mex/3ds/ext/Project.h"
#include "thekogans/mex/3ds/ext/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                Module::Module (const io::Module &module) :
                    dummy (new Project (module.project)), project (*dummy) {
                }

                Module::~Module () {
                    delete dummy;
                }

                const io::Project &Module::ioProject () const {
                    return project.project;
                }

                const io::Shaper &Module::ioShaper () const {
                    return project.project.shaper;
                }

                const io::Lofter &Module::ioLofter () const {
                    return project.project.lofter;
                }

                const io::Editor &Module::ioEditor () const {
                    return project.project.editor;
                }

                const io::Keyframer &Module::ioKeyframer () const {
                    return project.project.keyframer;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
