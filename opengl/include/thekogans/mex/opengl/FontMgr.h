// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_opengl.
//
// libthekogans_mex_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_opengl_FontMgr_h)
#define __thekogans_mex_opengl_FontMgr_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerMap.h"
#include "thekogans/util/Singleton.h"
#include "thekogans/util/SpinLock.h"
#include "thekogans/mex/opengl/Config.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            struct _LIB_THEKOGANS_MEX_OPENGL_DECL FontMgr : public util::Singleton<FontMgr, util::SpinLock> {
                struct _LIB_THEKOGANS_MEX_OPENGL_DECL Font {
                    typedef std::unique_ptr<Font> UniquePtr;

                    struct _LIB_THEKOGANS_MEX_OPENGL_DECL Glyph {
                        virtual ~Glyph () {}
                        virtual util::ui32 GetWidth () const = 0;
                        virtual const util::ui8 *GetBitmap () const = 0;
                        virtual util::ui32 GetKernInfoCount () const {
                            return 0;
                        }
                        virtual const std::pair<util::ui16, util::i32> *GetKernInfo () const {
                            return 0;
                        }
                        util::i32 GetKernAmount (util::ui16 ch) const;
                    };

                public:
                    virtual ~Font () {}
                    virtual Font::UniquePtr Clone () const = 0;
                    virtual const char *GetName () const = 0;
                    virtual util::ui32 GetAveCharWidth () const = 0;
                    virtual util::ui32 GetHeight () const = 0;
                    virtual const Glyph *GetGlyph (util::ui16 ch) const = 0;

                    inline util::ui32 GetStringWidth (const char *str) const {
                        util::ui32 width = 0;
                        for (; *str; ++str) {
                            width += GetGlyph (*str)->GetWidth ();
                        }
                        return width;
                    }
                };

            protected:
                Font::UniquePtr systemFont;
                static util::OwnerMap<std::string, Font> fonts;

            public:
                FontMgr ();

                inline const Font *GetSystemFont () {
                    return systemFont.get ();
                }
                inline const util::OwnerMap<std::string, Font> &GetFonts () const {
                    return fonts;
                }
            };

            #define DECLARE_FONTMGR_FONT(name)\
            public:\
                static const char * const NAME;\
                static thekogans::opengl::FontMgr::Font::UniquePtr Create () {\
                    return new type ();\
                }\
                static thekogans::opengl::FontMgr::MapInitializer mapInitializer;

            #define IMPLEMENT_FONTMGR_FONT(type)\
                const char * const type::NAME = #type;\
                thekogans::opengl::FontMgr::MapInitializer type::mapInitializer (\
                    type::NAME, type::Create);

        } // namespace opengl
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_opengl_FontMgr_h)
