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

#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/Image.h"
#include "thekogans/mex/3ds/opengl/Util.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Material)

                namespace {
                    const util::f32 defaultglColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
                }

                Material::Material (const io::Material &material_, const std::vector<std::string> &mapPaths) :
                    material (material_), texture (0) {
                    Color2glColor (material.ambient, ambient, defaultglColor);
                    Color2glColor (material.diffuse, diffuse, defaultglColor);
                    Color2glColor (material.specular, specular, defaultglColor);
                    shininess = powf (2.0f, 10.0f * material.shininess / 100.f);
                    if (shininess > 128.0f) {
                        shininess = 128.0f;
                    }
                    if (!material.textureMap.map.name.empty ()) {
                        Image image;
                        if (image.Load (material.textureMap.map.name, mapPaths)) {
                            assert (image.width && image.height);
                            assert (!image.bits.empty ());
                            glGenTextures (1, &texture);
                            glBindTexture (GL_TEXTURE_2D, texture);
                            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexImage2D (GL_TEXTURE_2D, 0, 4, image.width, image.height, 0,
                                GL_RGBA, GL_UNSIGNED_BYTE, &image.bits[0]);
                            glBindTexture (GL_TEXTURE_2D, 0);
                        }
                    }
                }

                Material::~Material () {
                    if (texture != 0) {
                        glDeleteTextures (1, &texture);
                    }
                }

                void Material::Setup () const {
                    glMaterialfv (GL_FRONT, GL_AMBIENT, GetAmbient ());
                    glMaterialfv (GL_FRONT, GL_DIFFUSE, GetDiffuse ());
                    glMaterialfv (GL_FRONT, GL_SPECULAR, GetSpecular ());
                    glMaterialf (GL_FRONT, GL_SHININESS, GetShininess ());
                    glBindTexture (GL_TEXTURE_2D, GetTexture ());
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
