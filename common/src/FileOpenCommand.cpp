// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_common.
//
// libthekogans_mex_common is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_common is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_common. If not, see <http://www.gnu.org/licenses/>.

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QFileDialog>
#include "thekogans/util/Path.h"
#include "thekogans/mex/command/Command.h"
#include "thekogans/mex/3ds/io/Progress.h"
#include "thekogans/mex/3ds/io/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/io/command/shaper/ShaperCommands.h"
#include "thekogans/mex/3ds/io/command/lofter/LofterCommands.h"
#include "thekogans/mex/3ds/io/command/editor/EditorCommands.h"
#include "thekogans/mex/3ds/io/command/keyframer/KeyframerCommands.h"
#include "thekogans/mex/3ds/ext/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/ext/command/shaper/ShaperCommands.h"
#include "thekogans/mex/3ds/ext/command/lofter/LofterCommands.h"
#include "thekogans/mex/3ds/ext/command/editor/EditorCommands.h"
#include "thekogans/mex/3ds/ext/command/keyframer/KeyframerCommands.h"
#include "thekogans/mex/3ds/opengl/command/CommandSpecializations.h"
#include "thekogans/mex/3ds/opengl/command/shaper/ShaperCommands.h"
#include "thekogans/mex/3ds/opengl/command/lofter/LofterCommands.h"
#include "thekogans/mex/3ds/opengl/command/editor/EditorCommands.h"
#include "thekogans/mex/3ds/opengl/command/keyframer/KeyframerCommands.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/command/SetViewLayoutFinalOperation.h"
#include "thekogans/mex/common/FileOpenCommand.h"

namespace thekogans {
    namespace mex {
        namespace common {

            namespace {
                util::i32 stricmp (
                        const char *str1,
                        const char *str2) {
                    for (std::size_t i = 0;; ++i) {
                        char ch1 = tolower (str1[i]);
                        char ch2 = tolower (str2[i]);
                        if (ch1 == 0) {
                            if (ch2 == 0) {
                                return 0;
                            }
                            return -1;
                        }
                        if (ch2 == 0) {
                            if (ch1 == 0) {
                                return 0;
                            }
                            return 1;
                        }
                        if (ch1 < ch2) {
                            return -1;
                        }
                        if (ch1 > ch2) {
                            return 1;
                        }
                    }
                    return 0;
                }

                util::i32 strnicmp (
                        const char *str1,
                        const char *str2,
                        std::size_t count) {
                    for (std::size_t i = 0; i < count; ++i) {
                        char ch1 = tolower (str1[i]);
                        char ch2 = tolower (str2[i]);
                        if (ch1 == 0) {
                            if (ch2 == 0) {
                                return 0;
                            }
                            return -1;
                        }
                        if (ch2 == 0) {
                            if (ch1 == 0) {
                                return 0;
                            }
                            return 1;
                        }
                        if (ch1 < ch2) {
                            return -1;
                        }
                        if (ch1 > ch2) {
                            return 1;
                        }
                    }
                    return 0;
                }
            }

            bool FileOpenCommand::Execute () {
                core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                const std::vector<core::Filters::Item *> &items = module.filters->GetItems ();
                if (!items.empty ()) {
                    std::string filters = items[0]->filter;
                    for (std::size_t i = 1, count = items.size (); i < count; ++i) {
                        if (items[i]->CanRead ()) {
                            filters += ";;";
                            filters += items[i]->filter;
                        }
                    }
                    QString fileName = QFileDialog::getOpenFileName (
                        core::UI::Instance ().mainFrameWindow->GetQWidget (),
                        "Open File",
                        module.GetIOModule ().openFileDirectory.c_str (),
                        filters.c_str ());
                    if (!fileName.isEmpty ()) {
                        util::Path path (fileName.toStdString ());
                        for (std::size_t i = 0, count = items.size (); i < count; ++i) {
                            if (stricmp (items[i]->ext.c_str (), path.GetExtension ().c_str ()) == 0) {
                                _3ds::io::Progress::UniquePtr progress (
                                    core::UI::Instance ().statusBarWindow->CreateProgress ());
                                _3ds::io::Project::FileType fileType = items[i]->eventHandler->OnRead (
                                    path.path.c_str (), core::GetIOProject (), progress.get ());
                                switch (fileType) {
                                    case _3ds::io::Project::Unknown: {
                                        // FIMXE: report error
                                        assert (0);
                                        return false;
                                    }
                                    case _3ds::io::Project::PRJ: {
                                        _3ds::io::command::ProjectValidateAfterOpenCommand (
                                            core::GetIOProject ()).Execute ();
                                        _3ds::ext::command::ProjectValidateAfterOpenCommand (
                                            core::GetExtProject ()).Execute ();
                                        _3ds::opengl::command::ProjectValidateAfterOpenCommand (
                                            core::GetOpenGLProject ()).Execute ();
                                        {
                                            core::GetIOProject ().shaper.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().shaper.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        {
                                            core::GetIOProject ().lofter.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().lofter.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        {
                                            core::GetIOProject ().editor.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().editor.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        {
                                            core::GetIOProject ().keyframer.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().keyframer.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::ioProjectComplexTypeSwapCommand (
                                                    core::GetIOProject (),
                                                    core::GetIOProject ())));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::extProjectComplexTypeSwapCommand (
                                                    core::GetExtProject (),
                                                    core::GetExtProject ())));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::openglProjectComplexTypeSwapCommand (
                                                    core::GetOpenGLProject (),
                                                    core::GetOpenGLProject ())));
                                        break;
                                    }
                                    case _3ds::io::Project::_3DS: {
                                        _3ds::io::command::EditorValidateAfterOpenCommand (
                                            core::GetIOProject ().editor).Execute ();
                                        _3ds::io::command::KeyframerValidateAfterOpenCommand (
                                            core::GetIOProject ().keyframer).Execute ();
                                        _3ds::ext::command::EditorValidateAfterOpenCommand (
                                            core::GetExtProject ().editor).Execute ();
                                        _3ds::ext::command::KeyframerValidateAfterOpenCommand (
                                            core::GetExtProject ().keyframer).Execute ();
                                        _3ds::opengl::command::EditorValidateAfterOpenCommand (
                                            core::GetOpenGLProject ().editor).Execute ();
                                        _3ds::opengl::command::KeyframerValidateAfterOpenCommand (
                                            core::GetOpenGLProject ().keyframer).Execute ();
                                        {
                                            core::GetIOProject ().editor.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().editor.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        {
                                            core::GetIOProject ().keyframer.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().keyframer.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::ioEditorComplexTypeSwapCommand (
                                                    core::GetIOProject ().editor,
                                                    core::GetIOProject ().editor)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::ioKeyframerComplexTypeSwapCommand (
                                                    core::GetIOProject ().keyframer,
                                                    core::GetIOProject ().keyframer)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::extEditorComplexTypeSwapCommand (
                                                    core::GetExtProject ().editor,
                                                    core::GetExtProject ().editor)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::extKeyframerComplexTypeSwapCommand (
                                                    core::GetExtProject ().keyframer,
                                                    core::GetExtProject ().keyframer)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::openglEditorComplexTypeSwapCommand (
                                                    core::GetOpenGLProject ().editor,
                                                    core::GetOpenGLProject ().editor)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::openglKeyframerComplexTypeSwapCommand (
                                                    core::GetOpenGLProject ().keyframer,
                                                    core::GetOpenGLProject ().keyframer)));
                                        break;
                                    }
                                    case _3ds::io::Project::LFT: {
                                        _3ds::io::command::LofterValidateAfterOpenCommand (
                                            core::GetIOProject ().lofter).Execute ();
                                        _3ds::ext::command::LofterValidateAfterOpenCommand (
                                            core::GetExtProject ().lofter).Execute ();
                                        _3ds::opengl::command::LofterValidateAfterOpenCommand (
                                            core::GetOpenGLProject ().lofter).Execute ();
                                        {
                                            core::GetIOProject ().lofter.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().lofter.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::ioLofterComplexTypeSwapCommand (
                                                    core::GetIOProject ().lofter,
                                                    core::GetIOProject ().lofter)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::extLofterComplexTypeSwapCommand (
                                                    core::GetExtProject ().lofter,
                                                    core::GetExtProject ().lofter)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::openglLofterComplexTypeSwapCommand (
                                                    core::GetOpenGLProject ().lofter,
                                                    core::GetOpenGLProject ().lofter)));
                                        break;
                                    }
                                    case _3ds::io::Project::SHP: {
                                        _3ds::io::command::ShaperValidateAfterOpenCommand (
                                            core::GetIOProject ().shaper).Execute ();
                                        _3ds::ext::command::ShaperValidateAfterOpenCommand (
                                            core::GetExtProject ().shaper).Execute ();
                                        _3ds::opengl::command::ShaperValidateAfterOpenCommand (
                                            core::GetOpenGLProject ().shaper).Execute ();
                                        {
                                            core::GetIOProject ().shaper.openFileDirectory =
                                                path.GetDirectory ();
                                            core::GetIOProject ().shaper.openFileName =
                                                path.GetFullFileName ();
                                        }
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::ioShaperComplexTypeSwapCommand (
                                                    core::GetIOProject ().shaper,
                                                    core::GetIOProject ().shaper)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::extShaperComplexTypeSwapCommand (
                                                    core::GetExtProject ().shaper,
                                                    core::GetExtProject ().shaper)));
                                        ExecuteAndAddCommand (
                                            command::Command::UniquePtr (
                                                new thekogans::mex::command::openglShaperComplexTypeSwapCommand (
                                                    core::GetOpenGLProject ().shaper,
                                                    core::GetOpenGLProject ().shaper)));
                                        break;
                                    }
                                    case _3ds::io::Project::MLI: {
                                        // FIXME: implement
                                        assert (0);
                                        break;
                                    }
                                    case _3ds::io::Project::VP: {
                                        // FIXME: implement
                                        assert (0);
                                        break;
                                    }
                                }
                                ExecuteAndAddFinalOperation (
                                    command::FinalOperation::UniquePtr (
                                        new core::command::SetViewLayoutFinalOperation (module)));
                                return true;
                            }
                        }
                    }
                }
                return false;
            }

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (FileOpenCommand)

        } // namespace common
    } // namespace mex
} // namespace thekogans
