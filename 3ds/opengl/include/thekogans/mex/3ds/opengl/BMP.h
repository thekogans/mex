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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl.  If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_BMP_h)
#define __thekogans_mex_3ds_opengl_BMP_h

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Buffer.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/Image.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL BMP : public Image::Driver {
                    DECLARE_IMAGE_DRIVER (BMP)

                    struct FileHeader {
                        util::i16 bfType;
                        util::i32 bfSize;
                        util::i16 bfReserved1;
                        util::i16 bfReserved2;
                        util::i32 bfOffBits;
                    } fileHeader;

                    struct InfoHeader {
                        util::i32 biSize;
                        util::i32 biWidth;
                        util::i32 biHeight;
                        util::i16 biPlanes;
                        util::i16 biBitCount;
                        util::i32 biCompression;
                        util::i32 biSizeImage;
                        util::i32 biXPelsPerMeter;
                        util::i32 biYPelsPerMeter;
                        util::i32 biClrUsed;
                        util::i32 biClrImportant;
                    } infoHeader;

                    virtual bool Load (
                        util::Buffer &buffer,
                        util::ui32 &width,
                        util::ui32 &height,
                        std::vector<Image::RGBA> &bits);
                };

                inline util::Serializer &operator >> (
                        util::Serializer &serializer,
                        BMP::FileHeader &fileHeader) {
                    serializer >>
                        fileHeader.bfType >>
                        fileHeader.bfSize >>
                        fileHeader.bfReserved1 >>
                        fileHeader.bfReserved2 >>
                        fileHeader.bfOffBits;
                    return serializer;
                }

                inline util::Serializer &operator >> (
                        util::Serializer &serializer,
                        BMP::InfoHeader &infoHeader) {
                    serializer >>
                        infoHeader.biSize >>
                        infoHeader.biWidth >>
                        infoHeader.biHeight >>
                        infoHeader.biPlanes >>
                        infoHeader.biBitCount >>
                        infoHeader.biCompression >>
                        infoHeader.biSizeImage >>
                        infoHeader.biXPelsPerMeter >>
                        infoHeader.biYPelsPerMeter >>
                        infoHeader.biClrUsed >>
                        infoHeader.biClrImportant;
                    return serializer;
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_BMP_h)
