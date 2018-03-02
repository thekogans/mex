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

#if !defined (__thekogans_mex_3ds_opengl_editor_Element_h)
#define __thekogans_mex_3ds_opengl_editor_Element_h

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/View.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Element : public ext::Mesh::Element {
                    typedef std::unique_ptr<Element> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Element)

                    const Material &material;
                    std::vector<blas::Point3> vertices;
                    std::vector<blas::Point3> normals;
                    std::vector<blas::Point2> textureVertices;

                    Element (
                        const io::Mesh &mesh,
                        const std::vector<util::ui16> &faces,
                        const Material &material_);

                    void UpdateVerticesAndNormals (
                        const std::vector<blas::Point3> &vertices,
                        const ext::Mesh::Normals &normals,
                        bool flat = false);
                    void Draw () const;

                private:
                    Element (const Element &);
                    Element &operator = (const Element &);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_editor_Element_h)
