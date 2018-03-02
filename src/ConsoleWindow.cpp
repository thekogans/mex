// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of thekogans_mex.
//
// thekogans_mex is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// thekogans_mex is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with thekogans_mex. If not, see <http://www.gnu.org/licenses/>.

#include <cstdarg>
#include "thekogans/mex/ConsoleWindow.h"

#if defined (TOOLCHAIN_OS_Windows) && (_MSC_VER <= 1200)
    #define vsnprintf _vsnprintf
#endif // defined (TOOLCHAIN_OS_Windows) && (_MSC_VER <= 1200)

namespace thekogans {
    namespace mex {

        ConsoleWindow::ConsoleWindow (QWidget *parent) : QTextEdit (parent) {
            setObjectName ("ConsoleWindow");
            setReadOnly (true);
            setFocusPolicy (Qt::NoFocus);
            assert (core::UI::Instance ().consoleWindow == 0);
            core::UI::Instance ().consoleWindow = this;
        }

        void ConsoleWindow::Print (const char *format, ...) {
            assert (format != 0);
            // FIXME: Use std::ostringstream for formatting.
            char str[512];
            {
                va_list argptr;
                va_start (argptr, format);
                vsnprintf (str, 512, format, argptr);
                va_end (argptr);
            }
            // FIXME: a hack to step on the '\n'.
            str[strlen (str) - 1] = 0;
            append (str);
            ensureCursorVisible ();
        }

    } // namespace mex
} // namespace thekogans
