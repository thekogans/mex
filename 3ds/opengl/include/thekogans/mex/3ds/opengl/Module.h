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

#if !defined (__thekogans_mex_3ds_opengl_Module_h)
#define __thekogans_mex_3ds_opengl_Module_h

#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/Project.h"
#include "thekogans/mex/3ds/ext/shaper/Shaper.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct Project;

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Module {
                private:
                    Project *dummy;

                public:
                    Project &project;

                    explicit Module (const ext::Module &module);
                    explicit Module (Project &project_) :
                        dummy (0),
                        project (project_) {}
                    virtual ~Module ();

                    virtual View::UniquePtr GetView (const io::View &view) const = 0;
                    virtual View::UniquePtr GetCurrView () const = 0;
                    virtual void Draw (const View &view) const = 0;

                    // Usually, I am not a fan of these helpers,
                    // but these make for a lot less typing.
                    const io::Project &ioProject () const;
                    const io::Shaper &ioShaper () const;
                    const io::Lofter &ioLofter () const;
                    const io::Editor &ioEditor () const;
                    const io::Keyframer &ioKeyframer () const;

                    const ext::Project &extProject () const;
                    const ext::Shaper &extShaper () const;
                    const ext::Lofter &extLofter () const;
                    const ext::Editor &extEditor () const;
                    const ext::Keyframer &extKeyframer () const;
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Module_h)
