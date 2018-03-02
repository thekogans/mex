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

#include <cassert>
#include "thekogans/mex/3ds/opengl/BMP.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                IMPLEMENT_IMAGE_DRIVER (BMP)

                bool BMP::Load (
                        util::Buffer &buffer,
                        util::ui32 &width,
                        util::ui32 &height,
                        std::vector<Image::RGBA> &bits) {
                    buffer >> fileHeader >> infoHeader;
                    // We read a fairly limited (but common) subset of formats.
                    // - one plane
                    // - 24 or 32 bpp
                    // - no compression
                    if (fileHeader.bfType != 0x4d42 ||
                            infoHeader.biSize != sizeof (InfoHeader) ||
                            infoHeader.biWidth < 1 ||
                            infoHeader.biHeight < 1 ||
                            infoHeader.biPlanes != 1 ||
                            (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) ||
                            infoHeader.biCompression != 0) {
                        return false;
                    }
                    width = infoHeader.biWidth;
                    height = infoHeader.biHeight;
                    bits.reserve (width * height);
                    buffer.readOffset = fileHeader.bfOffBits;
                    for (util::i32 y = 0; y < infoHeader.biHeight; ++y) {
                        for (util::i32 x = 0; x < infoHeader.biWidth; ++x) {
                            Image::RGBA rgba;
                            if (infoHeader.biBitCount == 24) {
                                buffer >> rgba.r >> rgba.g >> rgba.b;
                                rgba.a = 255;
                            }
                            else {
                                buffer >> rgba;
                            }
                            bits.push_back (rgba);
                        }
                    }
                    assert (bits.size () == width * height);
                    return true;
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
