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

#if !defined (__thekogans_mex_core_CursorMgr_h)
#define __thekogans_mex_core_CursorMgr_h

#include "thekogans/util/Environment.h"
#if defined (TOOLCHAIN_OS_Windows)
    #include "thekogans/util/os/windows/WindowsHeader.h"
#endif // defined (TOOLCHAIN_OS_Windows)
#include <vector>
#include <QtGui/QCursor>
#include "thekogans/util/Types.h"
#include "thekogans/util/Singleton.h"
#include "thekogans/util/Mutex.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/core/Config.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct _LIB_THEKOGANS_MEX_CORE_DECL CursorMgr :
                    public util::Singleton<CursorMgr, util::Mutex> {
            private:
                blas::Point ptLast;
                bool visible;
                std::vector<const QCursor *> cursors;
                const QCursor *cursor;

            public:
                // Common cursors used by various tools.
                enum {
                    PICK3_CURSOR,
                    PICK5_CURSOR,
                    PICK7_CURSOR,
                    PICK9_CURSOR,
                    PICK11_CURSOR,
                    FOURWAY_CURSOR,
                    HORIZONTAL_CURSOR,
                    VERTICAL_CURSOR,
                    DOWN_CURSOR,
                    LEFT_CURSOR,
                    RIGHT_CURSOR,
                    UP_CURSOR,
                    CW_CURSOR,
                    CCW_CURSOR,
                    NESW_CURSOR,
                    NWSE_CURSOR,
                    ARROW_CURSOR,
                    WAIT_CURSOR,
                    CROSS_CURSOR,   // Special full view cross cursor.
                    LAST_CURSOR_INDEX
                };

                CursorMgr ();

                inline bool IsCursorVisible () const {
                    return visible;
                }
                inline const QCursor *GetCurrCursor () const {
                    return cursor;
                }
                inline const QCursor *GetCursor (util::ui32 index) const {
                    return cursors[index];
                }
                inline void SetCursor (util::ui32 index) {
                    assert (index < LAST_CURSOR_INDEX);
                    SetCursor (cursors[index]);
                }
                void SetCursor (const QCursor *cursor_);
                void SetPickCursor (util::ui32 pickSize);

                void ShowCursor (bool show_);

                // ViewLayoutWindow::mouseMoveEvent calls this.
                void OnMouseMove (const blas::Point &point);

            private:
                void Draw ();
            };

            struct HideCursor {
                bool visible;
                HideCursor () : visible (CursorMgr::Instance ()->IsCursorVisible ()) {
                    if (visible) {
                        CursorMgr::Instance ()->ShowCursor (false);
                    }
                }
                ~HideCursor () {
                    if (visible) {
                        CursorMgr::Instance ()->ShowCursor (true);
                    }
                }

            private:
                HideCursor (const HideCursor &);
                HideCursor &operator = (const HideCursor &);
            };

            struct SetCursor {
                const QCursor *oldCursor;
                SetCursor (const QCursor *newCursor) :
                        oldCursor (CursorMgr::Instance ()->GetCurrCursor ()) {
                    CursorMgr::Instance ()->SetCursor (newCursor);
                }
                SetCursor (util::ui32 index) :
                oldCursor (CursorMgr::Instance ()->GetCurrCursor ()) {
                    CursorMgr::Instance ()->SetCursor (index);
                }
                virtual ~SetCursor () {
                    CursorMgr::Instance ()->SetCursor (oldCursor);
                }

            private:
                SetCursor (const SetCursor &);
                SetCursor &operator = (const SetCursor &);
            };

            struct WaitCursor : public SetCursor {
                WaitCursor () : SetCursor (CursorMgr::WAIT_CURSOR) {}

            private:
                WaitCursor (const WaitCursor &);
                WaitCursor &operator = (const WaitCursor &);
            };

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_CursorMgr_h)
