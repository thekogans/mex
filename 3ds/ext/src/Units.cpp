// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#include <cstdio>
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/3ds/ext/Units.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                std::string Units::Format (util::f32 val) const {
                    char str[100];
                    val *= units.scale;
                    switch (units.type) {
                        case io::Project::Units::Decimal:
                            util::CLAMP (val, -1000000.0f, 1000000.0f);
                        #if defined (TOOLCHAIN_OS_Windows) && (_MSC_VER >= 1400)
                            sprintf_s (str, 100, "%.2f", val);
                        #else // defined (TOOLCHAIN_OS_Windows)
                            sprintf (str, "%.2f", val);
                        #endif // defined (TOOLCHAIN_OS_Windows)
                            break;
                        case io::Project::Units::Metric:
                            switch (units.metric) {
                                case io::Project::Units::Inches:
                                    val *= util::IN2M;
                                    break;
                                case io::Project::Units::Feet:
                                    val *= util::FT2M;
                                    break;
                                case io::Project::Units::Centimeters:
                                    val *= util::CM2M;
                                    break;
                            }
                            util::CLAMP (val, -1000000.0f, 1000000.0f);
                        #if defined (TOOLCHAIN_OS_Windows) && (_MSC_VER >= 1400)
                            sprintf_s (str, 100, "%.3fm", val);
                        #else // defined (TOOLCHAIN_OS_Windows)
                            sprintf (str, "%.3fm", val);
                        #endif // defined (TOOLCHAIN_OS_Windows)
                            break;
                        case io::Project::Units::Architectural:
                            switch (units.metric) {
                                case io::Project::Units::Inches:
                                    val *= util::IN2FT;
                                    break;
                                case io::Project::Units::Centimeters:
                                    val *= util::CM2FT;
                                    break;
                                case io::Project::Units::Meters:
                                    val *= util::M2FT;
                                    break;
                            }
                            util::CLAMP (val, -1000000.0f, 1000000.0f);
                            util::i32 ft = (util::i32)val;
                            util::i32 in = (util::i32)((val - ft) * 12.0f);
                            util::i32 frac = (util::i32)(((val - ft) * 12.0f - in) * units.denominator);
                        #if defined (TOOLCHAIN_OS_Windows) && (_MSC_VER >= 1400)
                            sprintf_s (str, 100, "%d\'%d\"%d", ft, abs (in), abs (frac));
                        #else // defined (TOOLCHAIN_OS_Windows)
                            sprintf (str, "%d\'%d\"%d", ft, abs (in), abs (frac));
                        #endif // defined (TOOLCHAIN_OS_Windows)
                            break;
                    }
                    return str;
                }

                util::f32 Units::UnFormat (const char *str) const {
                    switch (units.type) {
                        case io::Project::Units::Decimal:
                            return (util::f32)atof (str) / units.scale;
                        case io::Project::Units::Metric: {
                            util::f32 val = (util::f32)atof (str) / units.scale;
                            switch (units.metric) {
                                case io::Project::Units::Inches:
                                    return val / util::IN2M;
                                case io::Project::Units::Feet:
                                    return val / util::FT2M;
                                case io::Project::Units::Centimeters:
                                    return val / util::CM2M;
                            }
                            break;
                        }
                        case io::Project::Units::Architectural: {
                            util::i32 in = 0;
                            util::i32 ft = atoi (str);
                            const char *ptr = strchr (str, '\'');
                            if (ptr) {
                                in = atoi (ptr + 1);
                            }
                            util::i32 frac = 0;
                            ptr = strchr (str, '\"');
                            if (ptr) {
                                frac = atoi (ptr + 1);
                            }
                        #define Sign(x) ((x) < 0 ? -1 : 1)
                            util::f32 val = (((util::f32)frac / units.denominator + in) / 12.0f +
                                (util::f32)abs (ft)) / units.scale * Sign (ft);
                        #undef Sign
                            switch (units.metric) {
                                case io::Project::Units::Inches:
                                    return val / util::IN2FT;
                                case io::Project::Units::Centimeters:
                                    return val / util::CM2FT;
                                case io::Project::Units::Meters:
                                    return val / util::M2FT;
                            }
                            break;
                        }
                    }
                    return 0.0f;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
