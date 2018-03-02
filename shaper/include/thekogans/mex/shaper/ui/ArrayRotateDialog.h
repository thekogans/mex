// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_shaper_ArrayRotateDialog_h)
#define __thekogans_mex_shaper_ArrayRotateDialog_h

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
#include "ui/ArrayRotateDialog-uic.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct ArrayRotateDialog :
                    public QDialog,
                    public Ui::ArrayRotateDialog {
                Q_OBJECT

            private:
                util::ui32 &objectsInArray;
                bool &rotate;

            public:
                ArrayRotateDialog (
                    util::ui32 &objectsInArray_,
                    bool &rotate_,
                    QWidget *parent = 0);

            public slots:
                virtual void accept ();
            };

        } // namespace shaper
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_shaper_ArrayRotateDialog_h)
