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

#if !defined (__thekogans_mex_SpeedBarWindow_h)
#define __thekogans_mex_SpeedBarWindow_h

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtOpenGL/QGLWidget>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/mex/opengl/ui8Color.h"
#include "thekogans/mex/opengl/Font.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/SpeedBar.h"

namespace thekogans {
    namespace mex {

        struct SpeedBarWindow : public QGLWidget, public core::UI::SpeedBarWindow {
            Q_OBJECT

        public:
            enum {
                SB_COLOR_BKGRND,
                SB_COLOR_TEXT,
                SB_COLOR_POPUP,
                SB_COLOR_ITEM,
                SB_COLOR_FLYBY,
                SB_COLOR_COUNT
            };
            static const util::ui32 defaultPalette[SB_COLOR_COUNT];

        protected:
            core::SpeedBar *speedBar;
            std::unique_ptr<opengl::Font> font;
            opengl::ui8Color colors[SB_COLOR_COUNT];
            struct HitTestInfo {
                core::SpeedBar::Item *item;
                util::ui32 itemIndex;
                util::i32 level;

                HitTestInfo () : item (0), itemIndex (util::NIDX32), level (0) {}
            } flyby;

        public:
            explicit SpeedBarWindow (QWidget *parent);
            ~SpeedBarWindow ();

            virtual QWidget *GetQWidget () {return this;}
            virtual void Save (const char *keyName);
            virtual void Restore (const char *keyName);
            virtual void Draw ();
            virtual void OnIdle ();
            virtual void SetSpeedBar (core::SpeedBar &speedBar);

            virtual QSize sizeHint () const;

        protected:
            virtual void paintEvent (QPaintEvent *event);
            virtual void mousePressEvent (QMouseEvent *event);
            virtual void mouseReleaseEvent (QMouseEvent *event);
            virtual void mouseMoveEvent (QMouseEvent *event);

        private:
            void UpdateScrollBars ();
            bool HitTest (const QPoint &pt, HitTestInfo &hitTestInfo);
            void RemoveFlyby ();
            util::ui32 GetWidth () const;
            util::ui32 GetHeight () const;
            void DrawItem (core::SpeedBar::Item *item, util::i32 startLine,
                util::i32 level, const opengl::ui8Color &color);
            inline int WidthInPixels () const {
                return width () * devicePixelRatio ();
            }
            inline int HeightInPixels () const {
                return height () * devicePixelRatio ();
            }
        };

    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_SpeedBarWindow_h)
