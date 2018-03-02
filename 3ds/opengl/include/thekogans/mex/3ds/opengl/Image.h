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

#if !defined (__thekogans_mex_3ds_opengl_Image_h)
#define __thekogans_mex_3ds_opengl_Image_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/Buffer.h"
#include "thekogans/mex/3ds/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Image {
                    typedef std::unique_ptr<Image> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Image)

                    util::ui32 width;
                    util::ui32 height;
                    struct RGBA {
                        util::ui8 r;
                        util::ui8 g;
                        util::ui8 b;
                        util::ui8 a;

                        RGBA () {}
                        RGBA (
                            util::ui8 r_,
                            util::ui8 g_,
                            util::ui8 b_,
                            util::ui8 a_) :
                            r (r_),
                            g (g_),
                            b (b_),
                            a (a_) {}
                    };
                    std::vector<RGBA> bits;

                    struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Driver {
                        virtual ~Driver () {}

                        virtual bool Load (
                            util::Buffer &buffer,
                            util::ui32 &width,
                            util::ui32 &height,
                            std::vector<RGBA> &bits) = 0;

                    protected:
                        util::ui32 Align (util::ui32 value);
                    };

                    typedef Driver *(*Factory) ();
                    struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL VectorInitializer {
                        explicit VectorInitializer (Factory factory);
                    };

                    Image () :
                        width (0),
                        height (0) {}
                    Image (util::ui32 width_,
                            util::ui32 height_) :
                            width (width_),
                            height (height_) {
                        bits.resize (width * height);
                    }

                    bool Load (const std::string &name, const std::vector<std::string> &paths);
                    bool Load (util::Buffer &buffer);

                    // Doesn't do any scaling, but takes account of
                    // width and height differences, and, if
                    // necessary, pads with black.
                    void Copy (Image &image) const;
                };

                #define DECLARE_IMAGE_DRIVER(type)\
                public:\
                    static thekogans::mex::_3ds::opengl::Image::VectorInitializer vectorInitializer;\
                    static thekogans::mex::_3ds::opengl::Image::Driver *Create () {\
                        static type driver;\
                        return &driver;\
                    }

                #define IMPLEMENT_IMAGE_DRIVER(type)\
                    thekogans::mex::_3ds::opengl::Image::VectorInitializer type::vectorInitializer (type::Create);

                inline util::Serializer &operator >> (util::Serializer &serializer, Image::RGBA &rgba) {
                    serializer >> rgba.r >> rgba.g >> rgba.b >> rgba.a;
                    return serializer;
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_Image_h)
