// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_editor.
//
// libthekogans_mex_editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_editor. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_editor_CameraDefinitionDialog_h)
#define __thekogans_mex_editor_CameraDefinitionDialog_h

#include <string>
#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtWidgets/QDialog>
#include "thekogans/util/Types.h"
#include "thekogans/mex/core/Module.h"
#include "ui/CameraDefinitionDialog-uic.h"

namespace thekogans {
    namespace mex {
        namespace editor {

            struct CameraDefinitionDialog : public QDialog, public Ui::CameraDefinitionDialog {
                Q_OBJECT

            private:
                core::Module &module;
                std::string &name;
                util::f32 &roll;
                util::f32 &fov;
                bool &showCone;

            public:
                CameraDefinitionDialog (
                    core::Module &module_,
                    std::string &name_,
                    util::f32 &roll_,
                    util::f32 &fov_,
                    bool &showCone_,
                    QWidget *parent = 0);

            private:
                void DoLens (util::f32 lens);

            public slots:
                void OnCalculate (bool);
                void OnLens15MM (bool);
                void OnLens20MM (bool);
                void OnLens24MM (bool);
                void OnLens28MM (bool);
                void OnLens35MM (bool);
                void OnLens50MM (bool);
                void OnLens85MM (bool);
                void OnLens135MM (bool);
                void OnLens200MM (bool);
                virtual void accept ();
            };

        } // namespace editor
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_editor_CameraDefinitionDialog_h)
