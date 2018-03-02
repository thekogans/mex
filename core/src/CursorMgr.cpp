// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_core.
//
// libthekogans_mex_core is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_core is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_core. If not, see <http://www.gnu.org/licenses/>.

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtCore/qglobal.h>
#include <QtGui/QBitmap>
#include "thekogans/util/Flags.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/LogicOp.h"
#include "thekogans/mex/opengl/DrawBuffer.h"
#include "thekogans/mex/opengl/Viewport2D.h"
#include "thekogans/mex/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/CursorMgr.h"

using namespace thekogans::mex::opengl;

void InitResources () {
    Q_INIT_RESOURCE (mex_core);
}

namespace thekogans {
    namespace mex {
        namespace core {

            CursorMgr::CursorMgr () :
                    ptLast (-1, -1),
                    visible (true) {
                InitResources ();
                cursors.push_back (new QCursor (QBitmap (":/pick3.bmp"), QBitmap (":/pick3.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/pick5.bmp"), QBitmap (":/pick5.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/pick7.bmp"), QBitmap (":/pick7.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/pick9.bmp"), QBitmap (":/pick9.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/pick11.bmp"), QBitmap (":/pick11.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/fourway.bmp"), QBitmap (":/fourway.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/horizontal.bmp"), QBitmap (":/horizontal.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/vertical.bmp"), QBitmap (":/vertical.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/down.bmp"), QBitmap (":/down.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/left.bmp"), QBitmap (":/left.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/right.bmp"), QBitmap (":/right.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/up.bmp"), QBitmap (":/up.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/cw.bmp"), QBitmap (":/cw.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/ccw.bmp"), QBitmap (":/ccw.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/nesw.bmp"), QBitmap (":/nesw.bmp"), 15, 15));
                cursors.push_back (new QCursor (QBitmap (":/nwse.bmp"), QBitmap (":/nwse.bmp"), 15, 15));
                cursors.push_back (new QCursor (Qt::ArrowCursor));
                cursors.push_back (new QCursor (Qt::WaitCursor));
                cursors.push_back (new QCursor (Qt::BlankCursor));
                cursor = cursors[ARROW_CURSOR];
            }

            void CursorMgr::SetCursor (const QCursor *cursor_) {
                Draw ();
                cursor = cursor_;
                ptLast = blas::Point (-1, -1);
                Draw ();
            }

            void CursorMgr::SetPickCursor (util::ui32 pickSize) {
                Draw ();
                switch (pickSize) {
                    case 3:
                        cursor = cursors[PICK3_CURSOR];
                        break;
                    case 5:
                        cursor = cursors[PICK5_CURSOR];
                        break;
                    case 7:
                        cursor = cursors[PICK7_CURSOR];
                        break;
                    case 9:
                        cursor = cursors[PICK9_CURSOR];
                        break;
                    case 11:
                        cursor = cursors[PICK11_CURSOR];
                        break;
                    default:
                        // This is not a runtime problem, and
                        // should be fixed by the app.
                        assert (0);
                        break;
                }
            }

            void CursorMgr::ShowCursor (bool show) {
                if (show != visible) {
                    if (show) {
                        visible = true;
                        ptLast = UI::Instance ().viewLayoutWindow->GetMousePosition ();
                        Draw ();
                    }
                    else {
                        Draw ();
                        ptLast = blas::Point (-1, -1);
                        visible = false;
                    }
                }
            }

            void CursorMgr::OnMouseMove (const blas::Point &point) {
                if (visible) {
                    Draw ();
                    ptLast = point;
                    Draw ();
                }
            }

            void CursorMgr::Draw () {
                if (cursor == cursors[CROSS_CURSOR] && visible) {
                    Module *module = Module::GetCurrModule ();
                    if (module != 0) {
                        const blas::Size &size = module->GetIOModule ().viewLayout.GetCurrView ().size;
                        if (ptLast.InSize (size)) {
                            UI::Instance ().viewLayoutWindow->MakeCurrent ();
                            Viewport2D viewport2D (size);
                            LogicOp logicOp (GL_XOR);
                            DrawBuffer drawBuffer (GL_FRONT);
                            Color color (_3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]);
                            HLine (size.x, size.Right () + 1, ptLast.y);
                            VLine (ptLast.x, size.y, size.Top () + 1);
                        }
                    }
                }
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
