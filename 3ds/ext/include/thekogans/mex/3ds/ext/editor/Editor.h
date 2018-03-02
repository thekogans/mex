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

#if !defined (__thekogans_mex_3ds_ext_editor_Editor_h)
#define __thekogans_mex_3ds_ext_editor_Editor_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/ext/Config.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Editor : public Module {
                    // FIXME: cache ext::Mesh/Light/Camera and have
                    // Editor (const io::Editor &editor) build the cache.
                    explicit Editor (const io::Editor &editor) :
                        Module (editor) {}
                    explicit Editor (Project &project) :
                        Module (project) {}

                    virtual View::UniquePtr GetView (const io::View &view) const;
                    virtual View::UniquePtr GetCurrView () const;
                    virtual blas::Bound2 GetViewBound (const View &view) const;

                    bool CheckName (const std::string &name) const;
                    std::string GetUniqueName (const std::string &name) const;

                    void Swap (Editor &editor);

                private:
                    Editor (const Editor &);
                    Editor &operator = (const Editor &);
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_editor_Editor_h)
