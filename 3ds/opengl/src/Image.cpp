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

#include <string>
#include <vector>
#include <algorithm>
#include "thekogans/util/Path.h"
#include "thekogans/util/Exception.h"
#include "thekogans/util/LoggerMgr.h"
#include "thekogans/mex/3ds/opengl/Image.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS_EX (
                    Image,
                    util::SpinLock,
                    32,
                    util::DefaultAllocator::Instance ())

                namespace {
                    // Believe it or not, but just declaring drivers static
                    // does not guarantee proper ctor call order!? Wrapping
                    // it in an accessor function does.
                    std::vector<Image::Driver *> &GetDrivers () {
                        static std::vector<Image::Driver *> drivers;
                        return drivers;
                    }
                }

                Image::VectorInitializer::VectorInitializer (Factory factory) {
                    GetDrivers ().push_back (factory ());
                }

                namespace {
                    util::ui32 BitCount (util::ui32 value) {
                        util::ui32 count = 0;
                        while (value) {
                            count += value & 1;
                            value >>= 1;
                        }
                        return count;
                    }
                }

                util::ui32 Image::Driver::Align (util::ui32 value) {
                    if (BitCount (value) > 1) {
                        util::ui32 shift = 0;
                        while (value) {
                            ++shift;
                            value >>= 1;
                        }
                        return 1 << shift;
                    }
                    return value;
                }

                bool Image::Load (const std::string &name, const std::vector<std::string> &paths) {
                    if (util::Path (name).Exists ()) {
                        return true;
                    }
                    typedef std::vector<std::string>::const_iterator const_iterator;
                    for (const_iterator path = paths.begin (), end = paths.end (); path != end; ++path) {
                        if (util::Path (util::MakePath (*path, name)).Exists ()) {
                            return true;
                        }
                    }
                    return false;
                }

                bool Image::Load (util::Buffer &buffer) {
                    typedef std::vector<Driver *>::iterator iterator;
                    for (iterator driver = GetDrivers ().begin (),
                            end = GetDrivers ().end (); driver != end; ++driver) {
                        if ((*driver)->Load (buffer, width, height, bits)) {
                            return true;
                        }
                        buffer.readOffset = 0;
                    }
                    return false;
                }

                void Image::Copy (Image &image) const {
                    for (util::ui32 i = 0, maxHeight = std::min (image.height, height); i < maxHeight; ++i) {
                        memcpy (&image.bits[i * image.width], &bits[i * width],
                            std::min (image.width, width) * sizeof (RGBA));
                        if (image.width > width) {
                            memset (&image.bits[i * image.width + width],
                                0, (image.width - width) * sizeof (RGBA));
                        }
                    }
                    if (image.height > height) {
                        memset (&image.bits[height * image.width], 0,
                            (image.height - height) * image.width * sizeof (RGBA));
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
