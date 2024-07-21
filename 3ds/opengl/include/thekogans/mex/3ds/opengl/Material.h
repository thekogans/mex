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

#if !defined (__thekogans_mex_3ds_opengl_Material_h)
#define __thekogans_mex_3ds_opengl_Material_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/Material.h"
#include "thekogans/mex/3ds/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Material {
                    typedef std::unique_ptr<Material> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                private:
                    const io::Material &material;

                    // Translated colors
                    util::f32 ambient[4];
                    util::f32 diffuse[4];
                    util::f32 specular[4];
                    util::f32 shininess;
                    GLuint texture;

                    // FIXME: add other material attributes

                public:
                    Material (
                        const io::Material &material_,
                        const std::vector<std::string> &mapPaths =
                            std::vector<std::string> ());
                    ~Material ();

                    inline const util::f32 *GetAmbient () const {
                        return ambient;
                    }
                    inline const util::f32 *GetDiffuse () const {
                        return diffuse;
                    }
                    inline const util::f32 *GetSpecular () const {
                        return specular;
                    }
                    inline util::f32 GetShininess () const {
                        return shininess;
                    }
                    inline GLuint GetTexture () const {
                        return texture;
                    }

                    void Setup () const;
                };

                const Material defaultMaterial = Material (io::defaultMaterial);

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Material_h)
