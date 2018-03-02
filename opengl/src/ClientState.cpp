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

#include "thekogans/mex/opengl/ClientState.h"

namespace thekogans {
    namespace mex {
        namespace opengl {

            ClientState::ClientState (util::ui32 state_) :
                    state (state_),
                    enabled (0) {
                if (util::Flags32 (state).Test (COLOR_ARRAY) &&
                        glIsEnabled (GL_COLOR_ARRAY) == GL_FALSE) {
                    util::Flags32 (enabled).Set (COLOR_ARRAY, true);
                    glEnableClientState (GL_COLOR_ARRAY);
                }
                if (util::Flags32 (state).Test (EDGE_FLAG_ARRAY) &&
                        glIsEnabled (GL_EDGE_FLAG_ARRAY) == GL_FALSE) {
                    util::Flags32 (enabled).Set (EDGE_FLAG_ARRAY, true);
                    glEnableClientState (GL_EDGE_FLAG_ARRAY);
                }
                if (util::Flags32 (state).Test (INDEX_ARRAY) &&
                        glIsEnabled (GL_INDEX_ARRAY) == GL_FALSE) {
                    util::Flags32 (enabled).Set (INDEX_ARRAY, true);
                    glEnableClientState (GL_INDEX_ARRAY);
                }
                if (util::Flags32 (state).Test (NORMAL_ARRAY) &&
                        glIsEnabled (GL_NORMAL_ARRAY) == GL_FALSE) {
                    util::Flags32 (enabled).Set (NORMAL_ARRAY, true);
                    glEnableClientState (GL_NORMAL_ARRAY);
                }
                if (util::Flags32 (state).Test (TEXTURE_COORD_ARRAY) &&
                        glIsEnabled (GL_TEXTURE_COORD_ARRAY) == GL_FALSE) {
                    util::Flags32 (enabled).Set (TEXTURE_COORD_ARRAY, true);
                    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
                }
                if (util::Flags32 (state).Test (VERTEX_ARRAY) &&
                        glIsEnabled (GL_VERTEX_ARRAY) == GL_FALSE) {
                    util::Flags32 (enabled).Set (VERTEX_ARRAY, true);
                    glEnableClientState (GL_VERTEX_ARRAY);
                }
            }

            ClientState::~ClientState () {
                if (util::Flags32 (enabled).Test (COLOR_ARRAY)) {
                    glDisableClientState (GL_COLOR_ARRAY);
                }
                if (util::Flags32 (enabled).Test (EDGE_FLAG_ARRAY)) {
                    glDisableClientState (GL_EDGE_FLAG_ARRAY);
                }
                if (util::Flags32 (enabled).Test (INDEX_ARRAY)) {
                    glDisableClientState (GL_INDEX_ARRAY);
                }
                if (util::Flags32 (enabled).Test (NORMAL_ARRAY)) {
                    glDisableClientState (GL_NORMAL_ARRAY);
                }
                if (util::Flags32 (enabled).Test (TEXTURE_COORD_ARRAY)) {
                    glDisableClientState (GL_TEXTURE_COORD_ARRAY);
                }
                if (util::Flags32 (enabled).Test (VERTEX_ARRAY)) {
                    glDisableClientState (GL_VERTEX_ARRAY);
                }
            }

        } // namespace opengl
    } // namespace mex
} // namespace thekogans
