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

#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif // !defined (WIN32_LEAN_AND_MEAN)
    #if !defined (NOMINMAX)
        #define NOMINMAX
    #endif // !defined (NOMINMAX)
#endif // defined (TOOLCHAIN_OS_Windows)
#include <QtGui/QCursor>
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QScrollBar>
#include "thekogans/util/Types.h"
#include "thekogans/mex/opengl/Font.h"
#include "thekogans/mex/opengl/Viewport2D.h"
#include "thekogans/mex/opengl/DrawBuffer.h"
#include "thekogans/mex/opengl/ClearColor.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ui8Color.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/SpeedBarWindow.h"

namespace thekogans {
    namespace mex {

        const util::ui32 SpeedBarWindow::defaultPalette[SB_COLOR_COUNT] = {
            _3ds::opengl::BLUE,
            _3ds::opengl::CYAN,
            _3ds::opengl::WHITE,
            _3ds::opengl::YELLOW,
            _3ds::opengl::LIGHT_CYAN,
        };

        namespace {
            const util::ui32 ID_FONT = 101;
            const util::ui32 ID_COLOR = 102;
        }

        SpeedBarWindow::SpeedBarWindow (QWidget *parent) :
                QGLWidget (parent),
                speedBar (0),
                font (opengl::FontMgr::Instance ().GetSystemFont ()) {
            assert (font != 0);
            setObjectName ("SpeedBarWindow");
            setMouseTracking (true);
            setFocusPolicy (Qt::NoFocus);
            for (util::ui32 i = 0; i < SB_COLOR_COUNT; ++i) {
                colors[i] = _3ds::opengl::DefaultPalette[defaultPalette[i]];
            }
            assert (core::UI::Instance ().speedBarWindow == 0);
            core::UI::Instance ().speedBarWindow = this;
        }

        SpeedBarWindow::~SpeedBarWindow () {
            makeCurrent ();
        }

        void SpeedBarWindow::Save (const char *keyName) {
            /*
              CWinApp *winApp = AfxGetApp ();
              ASSERT_VALID (winApp);
              winApp->WriteProfileInt (keyName, "SB_COLOR_BKGRND", util::ui8Color::Toui32 (colors[SB_COLOR_BKGRND]));
              winApp->WriteProfileInt (keyName, "SB_COLOR_TEXT", util::ui8Color::Toui32 (colors[SB_COLOR_TEXT]));
              winApp->WriteProfileInt (keyName, "SB_COLOR_POPUP", util::ui8Color::Toui32 (colors[SB_COLOR_POPUP]));
              winApp->WriteProfileInt (keyName, "SB_COLOR_ITEM", util::ui8Color::Toui32 (colors[SB_COLOR_ITEM]));
              winApp->WriteProfileInt (keyName, "SB_COLOR_FLYBY", util::ui8Color::Toui32 (colors[SB_COLOR_FLYBY]));
            */
        }

        void SpeedBarWindow::Restore (const char *keyName) {
            /*
              CWinApp *winApp = AfxGetApp ();
              ASSERT_VALID (winApp);
              colors[SB_COLOR_BKGRND] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_BKGRND", util::ui8Color::Toui32 (colors[SB_COLOR_BKGRND])));
              colors[SB_COLOR_TEXT] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_TEXT", util::ui8Color::Toui32 (colors[SB_COLOR_TEXT])));
              colors[SB_COLOR_POPUP] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_POPUP", util::ui8Color::Toui32 (colors[SB_COLOR_POPUP])));
              colors[SB_COLOR_ITEM] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_ITEM", util::ui8Color::Toui32 (colors[SB_COLOR_ITEM])));
              colors[SB_COLOR_FLYBY] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_FLYBY", util::ui8Color::Toui32 (colors[SB_COLOR_FLYBY])));
            */
        }

        void SpeedBarWindow::Draw () {
            makeCurrent ();
            opengl::Viewport2D viewport2D (
                blas::Size (0, 0, WidthInPixels (), HeightInPixels ()));
            opengl::ClearColor clearColor (colors[SB_COLOR_BKGRND]);
            glClear (GL_COLOR_BUFFER_BIT);
            if (speedBar != 0) {
                util::i32 startLine = 0;
                util::i32 level = 0;
                for (const core::SpeedBar::Item *item = &speedBar->root;
                        item != 0; item = item->GetCurrChild ()) {
                    const std::vector<core::SpeedBar::Item *> &children = item->children;
                    for (std::size_t i = 0, count = children.size (); i < count; ++i) {
                        DrawItem (children[i], startLine + i, level, colors[SB_COLOR_TEXT]);
                    }
                    startLine += children.size ();
                    ++level;
                }
            }
            swapBuffers ();
        }

        void SpeedBarWindow::OnIdle () {
            if (!QRect (0, 0, WidthInPixels (), HeightInPixels ()).contains (
                    mapFromGlobal (QCursor::pos ()) * devicePixelRatio ())) {
                RemoveFlyby ();
                flyby = HitTestInfo ();
            }
        }

        void SpeedBarWindow::SetSpeedBar (core::SpeedBar &speedBar_) {
            speedBar = &speedBar_;
            UpdateScrollBars ();
        }

        QSize SpeedBarWindow::sizeHint () const {
            return QSize (GetWidth (), GetHeight ());
        }

        void SpeedBarWindow::paintEvent (QPaintEvent *event) {
            event->accept ();
            if (speedBar != 0) {
                //speedBar->origin.x = scrollArea->horizontalScrollBar ()->value () / font->GetAveCharWidth ();
                //speedBar->origin.y = scrollArea->verticalScrollBar ()->value () / font->GetHeight ();
            }
            Draw ();
        }

        void SpeedBarWindow::mousePressEvent (QMouseEvent *event) {
            event->accept ();
            if (event->button () == Qt::LeftButton) {
                setCursor (QCursor (Qt::BlankCursor));
                grabMouse ();
            }
            else if (event->button () == Qt::RightButton) {
                /*
                  CMenu menu;
                  if (menu.CreatePopupMenu ()) {
                  menu.AppendMenu (MF_STRING, ID_FONT, "Font...");
                  menu.AppendMenu (MF_STRING, ID_COLOR, "Color...");
                  SetForegroundWindow ();
                  ClientToScreen (&point);
                  menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
                  }
                */
            }
        }

        void SpeedBarWindow::mouseReleaseEvent (QMouseEvent *event) {
            event->accept ();
            if (event->button () == Qt::LeftButton) {
                setCursor (QCursor (Qt::ArrowCursor));
                releaseMouse ();
                if (speedBar != 0) {
                    HitTestInfo hitTestInfo;
                    if (HitTest (event->pos () * devicePixelRatio (), hitTestInfo)) {
                        core::ViewLayout *viewLayout =
                            core::UI::Instance ().viewLayoutWindow->GetViewLayout ();
                        if (viewLayout != 0 && viewLayout->tool != 0) {
                            viewLayout->tool->KillFocus ();
                        }
                        speedBar->SetCurrItem (hitTestInfo.itemIndex);
                        UpdateScrollBars ();
                    }
                }
            }
        }

        void SpeedBarWindow::mouseMoveEvent (QMouseEvent *event) {
            event->accept ();
            if (speedBar != 0) {
                HitTestInfo hitTestInfo;
                if (HitTest (event->pos () * devicePixelRatio (), hitTestInfo)) {
                    if (hitTestInfo.itemIndex == flyby.itemIndex) {
                        return;
                    }
                    if (flyby.itemIndex != util::NIDX32) {
                        makeCurrent ();
                        opengl::Viewport2D viewport2D (
                            blas::Size (0, 0, WidthInPixels (), HeightInPixels ()));
                        opengl::DrawBuffer drawBuffer (GL_FRONT);
                        DrawItem (flyby.item, flyby.itemIndex, flyby.level, colors[SB_COLOR_TEXT]);
                    }
                    {
                        flyby = hitTestInfo;
                        makeCurrent ();
                        opengl::Viewport2D viewport2D (
                            blas::Size (0, 0, WidthInPixels (), HeightInPixels ()));
                        opengl::DrawBuffer drawBuffer (GL_FRONT);
                        DrawItem (flyby.item, flyby.itemIndex, flyby.level, colors[SB_COLOR_FLYBY]);
                    }
                }
                else {
                    RemoveFlyby ();
                }
            }
        }

        void SpeedBarWindow::UpdateScrollBars () {
            /*
              util::i32 width = GetWidth ();
              util::i32 height = GetHeight ();
              setMinimumSize (width, height);
              setMaximumSize (width, height);
              scrollArea->horizontalScrollBar ()->setMaximum (width - scrollArea->width ());
              scrollArea->horizontalScrollBar ()->setValue (speedBar->origin.x * font->GetAveCharWidth ());
              scrollArea->verticalScrollBar ()->setMaximum (height - scrollArea->height ());
              scrollArea->verticalScrollBar ()->setValue (speedBar->origin.y * font->GetHeight ());
            */
            update ();
        }

        bool SpeedBarWindow::HitTest (const QPoint &pt, HitTestInfo &hitTestInfo) {
            if (speedBar != 0) {
                util::ui32 itemIndex = speedBar->origin.y + pt.y () / font->GetHeight ();
                hitTestInfo.level = 0;
                for (core::SpeedBar::Item *item = &speedBar->root; item; item = item->GetCurrChild ()) {
                    const std::vector<core::SpeedBar::Item *> &children = item->children;
                    if (itemIndex < children.size ()) {
                        item = children[itemIndex];
                        util::i32 left = (hitTestInfo.level * 2 - speedBar->origin.x) * font->GetAveCharWidth ();
                        /*
                          if (item->IsChecked ()) {
                          left -= font->GetStringWidth ("*");
                          }
                        */
                        if (pt.x () < left) {
                            return false;
                        }
                        util::ui32 textWidth = font->GetStringWidth (item->text.c_str ());
                        if (pt.x () > left + textWidth) {
                            return false;
                        }
                        hitTestInfo.item = item;
                        hitTestInfo.itemIndex = speedBar->origin.y + pt.y () / font->GetHeight ();
                        return true;
                    }
                    ++hitTestInfo.level;
                    itemIndex -= children.size ();
                }
            }
            return false;
        }

        void SpeedBarWindow::RemoveFlyby () {
            if (flyby.itemIndex != util::NIDX32) {
                makeCurrent ();
                opengl::Viewport2D viewport2D (
                    blas::Size (0, 0, WidthInPixels (), HeightInPixels ()));
                opengl::DrawBuffer drawBuffer (GL_FRONT);
                DrawItem (flyby.item, flyby.itemIndex, flyby.level, colors[SB_COLOR_TEXT]);
                flyby.itemIndex = util::NIDX32;
            }
        }

        util::ui32 SpeedBarWindow::GetWidth () const {
            util::ui32 width = 0;
            if (speedBar) {
                util::ui32 level = 0;
                for (const core::SpeedBar::Item *item = &speedBar->root;
                        item != 0; item = item->GetCurrChild ()) {
                    const std::vector<core::SpeedBar::Item *> &children = item->children;
                    for (std::size_t i = 0, count = children.size (); i < count; ++i) {
                        util::ui32 textWidth = font->GetStringWidth (children[i]->text.c_str ());
                        if (width < level * font->GetAveCharWidth () * 2 + textWidth) {
                            width = level * font->GetAveCharWidth () * 2 + textWidth;
                        }
                    }
                    ++level;
                }
            }
            return width;
        }

        util::ui32 SpeedBarWindow::GetHeight () const {
            util::ui32 height = 0;
            if (speedBar) {
                for (const core::SpeedBar::Item *item = &speedBar->root;
                        item != 0; item = item->GetCurrChild ()) {
                    height += font->GetHeight () * item->children.size ();
                }
            }
            return height;
        }

        void SpeedBarWindow::DrawItem (core::SpeedBar::Item *item, util::i32 startLine,
                util::i32 level, const opengl::ui8Color &color) {
            class UI : public core::SpeedBar::Item::EventHandler::UI {
            public:
                bool sticky;
                bool enabled;
                bool checked;

                UI (core::SpeedBar::Item &item) :
                    core::SpeedBar::Item::EventHandler::UI (item),
                    sticky (false), enabled (false), checked (false) {}

                virtual void SetSticky (bool sticky_) {sticky = sticky_;}
                virtual void SetEnabled (bool enabled_) {enabled = enabled_;}
                virtual void SetChecked (bool checked_) {checked = checked_;}
            } ui (*item);
            item->eventHandler->OnUpdateUI (ui);
            opengl::Color color_ (ui.enabled ?
                !item->children.empty () ? colors[SB_COLOR_POPUP] : colors[SB_COLOR_ITEM] : color);
            util::i32 x = (level * 2 - speedBar->origin.x) * font->GetAveCharWidth ();
            util::i32 y = HeightInPixels () - (startLine - speedBar->origin.y + 1) * font->GetHeight ();
            if (ui.checked) {
                opengl::Font (font).DrawText (x - font->GetStringWidth ("*"), y, "*");
            }
            opengl::Font (font).DrawText (x, y, item->text.c_str ());
        }

    } // namespace mex
} // namespace thekogans
