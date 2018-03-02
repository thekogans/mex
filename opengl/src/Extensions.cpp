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

#include <cassert>
#include <cstring>
#include <iostream>
#include "thekogans/mex/opengl/Extensions.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            Extensions::Extensions () {
                for (const char *start = (const char *)glGetString (GL_EXTENSIONS); start != 0;) {
                    const char *end = strchr (start, ' ');
                    if (end != 0) {
                        extensions.insert (std::string (start, end));
                        start = ++end;
                    }
                    else {
                        extensions.insert (start);
                        start = end;
                    }
                }
            }

            bool Extensions::IsExtensionSupported (const std::string &extension) const {
                std::set<std::string>::const_iterator it = extensions.find (extension);
                return it != extensions.end ();
            }

            void Extensions::Dump () const {
                for (std::set<std::string>::const_iterator it = extensions.begin (),
                        end = extensions.end (); it != end; ++it) {
                    std::cout << it->c_str () << std::endl;
                }
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
