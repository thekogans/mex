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

#if !defined (__thekogans_mex_keyframer_Module_h)
#define __thekogans_mex_keyframer_Module_h

#include <cstring>
#include "thekogans/mex/3ds/io/Module.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/opengl/keyframer/Keyframer.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/keyframer/Config.h"

namespace thekogans {
    namespace mex {
        namespace keyframer {

            struct _LIB_THEKOGANS_MEX_KEYFRAMER_DECL Keyframer : public core::Module, public core::UI::FrameBarWindow::EventSink {
                THEKOGANS_MEX_CORE_DECLARE_MODULE

                virtual void SetFocus () {
                    Module::SetFocus ();
                    assert (core::UI::Instance ().frameBarWindow != 0);
                    core::UI::Instance ().frameBarWindow->SetEventSink (this);
                }

                virtual void KillFocus () {
                    Module::KillFocus ();
                    assert (core::UI::Instance ().frameBarWindow != 0);
                    core::UI::Instance ().frameBarWindow->SetEventSink (0);
                }

                virtual _3ds::io::Module &GetIOModule () const {return core::GetIOProject ().keyframer;}
                virtual _3ds::ext::Module &GetExtModule () const {return core::GetExtProject ().keyframer;}
                virtual _3ds::opengl::Module &GetOpenGLModule () const {return core::GetOpenGLProject ().keyframer;}

                virtual bool IsVisible () const {
                    core::Module *module = GetCurrModule ();
                    if (module != 0) {
                        return strcmp (module->GetName (), NAME) == 0;
                    }
                    return false;
                }
                virtual util::ui32 GetStartFrame () const {
                    return core::GetIOProject ().keyframer.segment.startFrame;
                }
                virtual util::ui32 GetEndFrame () const {
                    return core::GetIOProject ().keyframer.segment.endFrame;
                }
                virtual util::ui32 GetCurrFrame () const {
                    return core::GetIOProject ().keyframer.segment.currFrame;
                }
                virtual void GoToFrame (util::ui32 frame);
                virtual bool IsBoxMode () const {
                    return core::GetIOProject ().display.detail == false;
                }
                virtual void SetBoxMode (bool boxMode) {
                    core::GetIOProject ().display.detail = boxMode == false;
                }
            };

        } // namespace keyframer
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_keyframer_Module_h)
