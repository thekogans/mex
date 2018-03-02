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

#include "thekogans/mex/3ds/opengl/Project.h"
#include "thekogans/mex/3ds/opengl/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                Module::Module (const ext::Module &module) :
                    dummy (new Project (module.project)), project (*dummy) {
                }

                Module::~Module () {
                    delete dummy;
                }

                const io::Project &Module::ioProject () const {
                    return project.project.project;
                }

                const io::Shaper &Module::ioShaper () const {
                    return project.project.project.shaper;
                }

                const io::Lofter &Module::ioLofter () const {
                    return project.project.project.lofter;
                }

                const io::Editor &Module::ioEditor () const {
                    return project.project.project.editor;
                }

                const io::Keyframer &Module::ioKeyframer () const {
                    return project.project.project.keyframer;
                }

                const ext::Project &Module::extProject () const {
                    return project.project;
                }

                const ext::Shaper &Module::extShaper () const {
                    return project.project.shaper;
                }

                const ext::Lofter &Module::extLofter () const {
                    return project.project.lofter;
                }

                const ext::Editor &Module::extEditor () const {
                    return project.project.editor;
                }

                const ext::Keyframer &Module::extKeyframer () const {
                    return project.project.keyframer;
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
