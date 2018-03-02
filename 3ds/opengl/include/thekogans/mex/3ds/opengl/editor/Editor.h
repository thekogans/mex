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

#if !defined (__thekogans_mex_3ds_opengl_editor_Editor_h)
#define __thekogans_mex_3ds_opengl_editor_Editor_h

#include <memory>
#include <string>
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/OwnerMap.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/Module.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/editor/WireMesh.h"
#include "thekogans/mex/3ds/opengl/editor/SolidMesh.h"
#include "thekogans/mex/3ds/opengl/editor/Light.h"
#include "thekogans/mex/3ds/opengl/editor/Camera.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct Project;

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Editor : public Module {
                    typedef std::unique_ptr<Editor> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Editor)

                    util::OwnerMap<std::string, Material> materials;
                    util::OwnerVector<WireMesh> wireMeshes;
                    util::OwnerVector<SolidMesh> solidMeshes;
                    util::OwnerVector<Light> lights;
                    util::OwnerVector<Camera> cameras;

                    explicit Editor (Project &project) :
                        Module (project) {}
                    explicit Editor (const ext::Editor &editor);

                    virtual View::UniquePtr GetView (const io::View &view) const;
                    virtual View::UniquePtr GetCurrView () const;
                    virtual void Draw (const View &view) const;

                    void Swap (Editor &editor);

                private:
                    Editor (const Editor &);
                    Editor &operator = (const Editor &);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_editor_Editor_h)
