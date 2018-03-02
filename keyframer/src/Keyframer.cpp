// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_keyframer.
//
// libthekogans_mex_keyframer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_keyframer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_keyframer. If not, see <http://www.gnu.org/licenses/>.

#include "thekogans/mex/core/command/DrawViewLayoutFinalOperation.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/keyframer/Config.h"
#include "thekogans/mex/keyframer/Keyframer.h"

namespace thekogans {
    namespace mex {
        namespace keyframer {

            THEKOGANS_MEX_CORE_IMPLEMENT_MODULE (Keyframer, "Keyframer")

            void Keyframer::GoToFrame (util::ui32 frame) {
                _3ds::io::Keyframer &ioKeyframer = static_cast<_3ds::io::Keyframer &> (GetIOModule ());
                _3ds::io::Keyframer::Segment &segment = ioKeyframer.segment;
                if (segment.currFrame != frame) {
                    segment.currFrame = frame;
                    _3ds::ext::Keyframer &extKeyframer = static_cast<_3ds::ext::Keyframer &> (GetExtModule ());
                    extKeyframer.SetCurrentFrame ();
                    _3ds::opengl::Keyframer &openglKeyframer = static_cast<_3ds::opengl::Keyframer &> (GetOpenGLModule ());
                    openglKeyframer.SetCurrentFrame ();
                    core::command::DrawViewLayoutFinalOperation (*this).Execute ();
                    core::command::FlipFramebufferFinalOperation ().Execute ();
                }
            }

        } // namespace keyframer
    } // namespace mex
} // namespace thekogans

using namespace thekogans::util;
using namespace thekogans::mex;

extern "C" _LIB_THEKOGANS_MEX_KEYFRAMER_DECL const core::Module::ModuleInterface &
    _LIB_THEKOGANS_MEX_KEYFRAMER_API GetModuleInterface (const char *name) {
    struct KeyframerInterface : public core::Module::ModuleInterface {
        KeyframerInterface () {}
        virtual ui32 GetMajorVersion () const {return 1;}
        virtual ui32 GetMinorVersion () const {return 0;}
        virtual core::Module *GetModule () const {
            static keyframer::Keyframer keyframer;
            return &keyframer;
        }
    } static const keyframerInterface;
    return keyframerInterface;
}
