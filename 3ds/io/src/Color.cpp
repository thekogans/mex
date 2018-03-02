// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/3ds/io/Color.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                namespace {

                    bool ColorPart2f32Color (const Color::Parts &part, opengl::f32Color &color) {
                        switch (part.type) {
                            case Color::Parts::NO_COLOR:
                                return false;
                            case Color::Parts::UI8_COLOR:
                                color.r = static_cast<util::f32> (part.byteColor.r) / 255.0f;
                                color.g = static_cast<util::f32> (part.byteColor.g) / 255.0f;
                                color.b = static_cast<util::f32> (part.byteColor.b) / 255.0f;
                                return true;
                            case Color::Parts::F32_COLOR:
                                // FIXME: verify 0.0 <= part.floatColor <= 1.0
                                color = part.floatColor;
                                return true;
                        }
                        // FIXME: throw something appropriate
                        assert (0);
                        return false;
                    }

                    bool ColorPart2ui8Color (const Color::Parts &part, opengl::ui8Color &color) {
                        switch (part.type) {
                            case Color::Parts::NO_COLOR:
                                return false;
                            case Color::Parts::UI8_COLOR:
                                color = part.byteColor;
                                return true;
                            case Color::Parts::F32_COLOR:
                                // FIXME: verify 0.0 <= part.floatColor <= 1.0
                                // FIXME: use ROUND?
                                color.r = static_cast<util::ui8> (part.floatColor.r * 255.0f);
                                color.g = static_cast<util::ui8> (part.floatColor.g * 255.0f);
                                color.b = static_cast<util::ui8> (part.floatColor.b * 255.0f);
                                return true;
                        }
                        // FIXME: throw something appropriate
                        assert (0);
                        return false;
                    }

                }

                opengl::f32Color Color::Getf32Color (const opengl::f32Color &defaultColor) const {
                    opengl::f32Color color;
                    // Try extracting raw first, followed by gamma corrected.
                    // Use default as last resort.
                    return ColorPart2f32Color (gammaCorrected, color) || ColorPart2f32Color (raw, color) ? color : defaultColor;
                }

                opengl::ui8Color Color::Getui8Color (const opengl::ui8Color &defaultColor) const {
                    opengl::ui8Color color;
                    // Try extracting raw first, followed by gamma corrected.
                    // Use default as last resort.
                    return ColorPart2ui8Color (gammaCorrected, color) || ColorPart2ui8Color (raw, color) ? color : defaultColor;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
