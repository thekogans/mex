#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include "thekogans/util/Types.h"
#include "thekogans/util/ui8Color.h"
#include "thekogans/mex/opengl/Font.h"
#include "thekogans/mex/opengl/Viewport2D.h"
#include "thekogans/mex/opengl/DrawBuffer.h"
#include "thekogans/mex/opengl/ClearColor.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "SpeedBarWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                namespace {
                    const util::ui32 ID_FONT = 101;
                    const util::ui32 ID_COLOR = 102;

                    const util::ui32 defaultColors[] = {
                        opengl::BLUE,
                        opengl::CYAN,
                        opengl::WHITE,
                        opengl::YELLOW,
                        opengl::LIGHT_CYAN,
                    };
                }

                SpeedBarWindow::SpeedBarWindow () : speedBar (0),
                    font (util::FontMgr::Instance ()->GetSystemFont ()) {
                    assert (font != 0);
                    for (util::ui32 i = 0; i < SB_COLOR_COUNT; ++i) {
                        colors[i] = opengl::DefaultPalette[defaultColors[i]];
                    }
                }

                void SpeedBarWindow::Save (const char *keyName) {
                    CWinApp *winApp = AfxGetApp ();
                    ASSERT_VALID (winApp);
                    winApp->WriteProfileInt (keyName, "SB_COLOR_BKGRND", util::ui8Color::Toui32 (colors[SB_COLOR_BKGRND]));
                    winApp->WriteProfileInt (keyName, "SB_COLOR_TEXT", util::ui8Color::Toui32 (colors[SB_COLOR_TEXT]));
                    winApp->WriteProfileInt (keyName, "SB_COLOR_POPUP", util::ui8Color::Toui32 (colors[SB_COLOR_POPUP]));
                    winApp->WriteProfileInt (keyName, "SB_COLOR_ITEM", util::ui8Color::Toui32 (colors[SB_COLOR_ITEM]));
                    winApp->WriteProfileInt (keyName, "SB_COLOR_FLYBY", util::ui8Color::Toui32 (colors[SB_COLOR_FLYBY]));
                }

                void SpeedBarWindow::Restore (const char *keyName) {
                    CWinApp *winApp = AfxGetApp ();
                    ASSERT_VALID (winApp);
                    colors[SB_COLOR_BKGRND] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_BKGRND", util::ui8Color::Toui32 (colors[SB_COLOR_BKGRND])));
                    colors[SB_COLOR_TEXT] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_TEXT", util::ui8Color::Toui32 (colors[SB_COLOR_TEXT])));
                    colors[SB_COLOR_POPUP] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_POPUP", util::ui8Color::Toui32 (colors[SB_COLOR_POPUP])));
                    colors[SB_COLOR_ITEM] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_ITEM", util::ui8Color::Toui32 (colors[SB_COLOR_ITEM])));
                    colors[SB_COLOR_FLYBY] = util::ui8Color::Fromui32 (winApp->GetProfileInt (keyName, "SB_COLOR_FLYBY", util::ui8Color::Toui32 (colors[SB_COLOR_FLYBY])));
                }

                namespace {
                    blas::Size GetWindowSize (HWND hwnd) {
                        CRect rect;
                        GetClientRect (hwnd, &rect);
                        return blas::Size (0, 0, rect.right, rect.bottom);
                    }

                    struct Viewport2D {
                        thekogans::opengl::Viewport2D viewport2D;
                        explicit Viewport2D (HWND hwnd) : viewport2D (GetWindowSize (hwnd)) {}
                    };
                }

                void SpeedBarWindow::Draw () {
                    openGLRC->SetCurrent ();
                    Viewport2D viewport2D (m_hWnd);
                    thekogans::opengl::ClearColor clearColor (colors[SB_COLOR_BKGRND]);
                    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    if (speedBar != 0) {
                        util::i32 startLine = 0;
                        util::i32 level = 0;
                        for (const core::SpeedBar::Item *item = &speedBar->root; item != 0; item = item->GetCurrChild ()) {
                            const std::vector<core::SpeedBar::Item *> &children = item->children;
                            for (util::ui32 i = 0; i < children.size (); ++i) {
                                DrawItem (children[i], startLine + i, level, colors[SB_COLOR_TEXT]);
                            }
                            startLine += children.size ();
                            ++level;
                        }
                    }
                    openGLRC->SwapBuffers ();
                }

                void SpeedBarWindow::OnIdle () {
                    CRect rect;
                    GetClientRect (&rect);
                    ClientToScreen (&rect);
                    POINT pt;
                    GetCursorPos (&pt);
                    if (!rect.PtInRect (pt)) {
                        RemoveFlyby ();
                        flyby = HitTestInfo ();
                    }
                }

                void SpeedBarWindow::SetSpeedBar (core::SpeedBar &speedBar_) {
                    speedBar = &speedBar_;
                    UpdateScrollBars ();
                    Invalidate ();
                }

                BOOL SpeedBarWindow::PreCreateWindow (CREATESTRUCT &createStruct)  {
                    if (!CWnd::PreCreateWindow (createStruct)) {
                        return FALSE;
                    }
                    createStruct.lpszClass = AfxRegisterWndClass (
                        CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                        LoadCursor (0, IDC_ARROW));
                    createStruct.style |= WS_HSCROLL | WS_VSCROLL;
                    return TRUE;
                }

                int SpeedBarWindow::OnCreate (LPCREATESTRUCT createStruct) {
                    if (CWnd::OnCreate (createStruct) == -1) {
                        return -1;
                    }
                    openGLRC.reset (new win::util::OpenGLRC (m_hWnd));
                    assert (core::UI::Instance ()->speedBarWindow == 0);
                    core::UI::Instance ()->speedBarWindow = this;
                    return 0;
                }

                void SpeedBarWindow::OnDestroy () {
                    openGLRC.reset (0);
                    CWnd::OnDestroy ();
                }

                void SpeedBarWindow::OnSize (UINT nType, int cx, int cy) {
                    CWnd::OnSize (nType, cx, cy);
                    if (cx > 0 && cy > 0) {
                        UpdateScrollBars ();
                    }
                }

                BOOL SpeedBarWindow::OnEraseBkgnd (CDC *pDC) {
                    return TRUE;
                }

                void SpeedBarWindow::OnPaint () {
                    CPaintDC dc (this);
                    Draw ();
                }

                void SpeedBarWindow::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
                    if (speedBar != 0) {
                        int iMin;
                        int iMax;
                        GetScrollRange (SB_HORZ, &iMin, &iMax);
                        RECT rect;
                        GetClientRect (&rect);
                        switch (nSBCode) {
                            case SB_LINEDOWN:
                                speedBar->origin.x += 1;
                                break;
                            case SB_LINEUP:
                                speedBar->origin.x -= 1;
                                break;
                            case SB_PAGEDOWN:
                                speedBar->origin.x += rect.right / font->GetAveCharWidth ();
                                break;
                            case SB_PAGEUP:
                                speedBar->origin.x -= rect.right / font->GetAveCharWidth ();
                                break;
                            case SB_THUMBTRACK:
                            case SB_THUMBPOSITION:
                                speedBar->origin.x = nPos;
                                break;
                            default:
                                break;
                        }
                        speedBar->origin.x = std::max (0, std::min (speedBar->origin.x, iMax));
                        if (speedBar->origin.x != GetScrollPos (SB_HORZ)) {
                            SetScrollPos (SB_HORZ, speedBar->origin.x, TRUE);
                            Invalidate ();
                        }
                    }
                }

                void SpeedBarWindow::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
                    if (speedBar != 0) {
                        int iMin;
                        int iMax;
                        GetScrollRange (SB_VERT, &iMin, &iMax);
                        RECT rect;
                        GetClientRect (&rect);
                        switch (nSBCode) {
                            case SB_LINEDOWN:
                                speedBar->origin.y += 1;
                                break;
                            case SB_LINEUP:
                                speedBar->origin.y -= 1;
                                break;
                            case SB_PAGEDOWN:
                                speedBar->origin.y += rect.bottom / font->GetHeight ();
                                break;
                            case SB_PAGEUP:
                                speedBar->origin.y -= rect.bottom / font->GetHeight ();
                                break;
                            case SB_THUMBTRACK:
                            case SB_THUMBPOSITION:
                                speedBar->origin.y = nPos;
                                break;
                            default:
                                break;
                        }
                        speedBar->origin.y = std::max (0, std::min (speedBar->origin.y, iMax));
                        if (speedBar->origin.y != GetScrollPos (SB_VERT)) {
                            SetScrollPos (SB_VERT, speedBar->origin.y, TRUE);
                            Invalidate ();
                        }
                    }
                }

                void SpeedBarWindow::OnLButtonDown (UINT nFlags, CPoint pt) {
                    ShowCursor (FALSE);
                    SetCapture ();
                }

                void SpeedBarWindow::OnLButtonUp (UINT nFlags, CPoint pt) {
                    ShowCursor (TRUE);
                    ReleaseCapture ();
                    if (speedBar != 0) {
                        HitTestInfo hitTestInfo;
                        if (HitTest (pt, hitTestInfo)) {
                            speedBar->SetCurrItem (hitTestInfo.sbidx);
                            UpdateScrollBars ();
                            Invalidate ();
                        }
                    }
                }

                void SpeedBarWindow::OnRButtonDown (UINT nFlags, CPoint point) {
                    CMenu menu;
                    if (menu.CreatePopupMenu ()) {
                        menu.AppendMenu (MF_STRING, ID_FONT, "Font...");
                        menu.AppendMenu (MF_STRING, ID_COLOR, "Color...");
                        SetForegroundWindow ();
                        ClientToScreen (&point);
                        menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
                    }
                }

                void SpeedBarWindow::OnMouseMove (UINT nFlags, CPoint point) {
                    if (speedBar != 0) {
                        HitTestInfo hitTestInfo;
                        if (HitTest (point, hitTestInfo)) {
                            if (hitTestInfo.sbidx == flyby.sbidx) {
                                return;
                            }
                            if (flyby.sbidx != util::NIDX32) {
                                openGLRC->SetCurrent ();
                                Viewport2D viewport2D (m_hWnd);
                                thekogans::opengl::DrawBuffer drawBuffer (GL_FRONT);
                                DrawItem (flyby.item, flyby.sbidx, flyby.level, colors[SB_COLOR_TEXT]);
                            }
                            {
                                flyby = hitTestInfo;
                                openGLRC->SetCurrent ();
                                Viewport2D viewport2D (m_hWnd);
                                thekogans::opengl::DrawBuffer drawBuffer (GL_FRONT);
                                DrawItem (flyby.item, flyby.sbidx, flyby.level, colors[SB_COLOR_FLYBY]);
                            }
                        }
                        else {
                            RemoveFlyby ();
                        }
                    }
                }

                namespace {
                    /*
                    struct FontDialog : public CDialog {
                        // FIXME: implement
                    };
                    */
                }

                void SpeedBarWindow::OnFont () {
                    /*
                    RemoveFlyby ();
                    CFontDialog fontDialog (NULL, CF_SCREENFONTS, NULL, this);
                    fontDialog.m_cf.lpLogFont = &logFont;
                    fontDialog.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT;
                    if (fontDialog.DoModal () == IDOK) {
                        GetSurfaceOSB ()->SetFont (CreateFontIndirect (logFont));
                        UpdateScrollBars ();
                        Invalidate ();
                    }
                    */
                    // FIXME: implement
                    assert (0);
                }

                namespace {
                    /*
                    struct ColorDialog : public CDialog {
                        // FIXME: implement
                    };
                    */
                }

                void SpeedBarWindow::OnColor () {
                    /*
                    RemoveFlyby ();
                    SpeedBarWindowColorDlg sbcd (colors, this);
                    sbcd.DoModal ();
                    */
                    // FIXME: implement
                    assert (0);
                }

                BEGIN_MESSAGE_MAP (SpeedBarWindow, CWnd)
                    ON_WM_CREATE ()
                    ON_WM_DESTROY ()
                    ON_WM_SIZE ()
                    ON_WM_ERASEBKGND ()
                    ON_WM_PAINT ()
                    ON_WM_HSCROLL ()
                    ON_WM_VSCROLL ()
                    ON_WM_LBUTTONDOWN ()
                    ON_WM_LBUTTONUP ()
                    ON_WM_RBUTTONDOWN ()
                    ON_WM_MOUSEMOVE ()
                    ON_COMMAND (ID_FONT, OnFont)
                    ON_COMMAND (ID_COLOR, OnColor)
                END_MESSAGE_MAP ()

                bool SpeedBarWindow::HitTest (POINT pt, HitTestInfo &hitTestInfo) {
                    if (speedBar != 0) {
                        util::ui32 iidx = speedBar->origin.y + pt.y / font->GetHeight ();
                        hitTestInfo.level = 0;
                        for (core::SpeedBar::Item *item = &speedBar->root; item; item = item->GetCurrChild ()) {
                            const std::vector<core::SpeedBar::Item *> &children = item->children;
                            if (iidx < children.size ()) {
                                item = children[iidx];
                                util::i32 left = (hitTestInfo.level * 2 - speedBar->origin.x) * font->GetAveCharWidth ();
                                util::ui32 starWidth = 0;
                                /*
                                if (item->IsChecked ()) {
                                    starWidth = font->GetStringWidth ("*");
                                    left -= starWidth;
                                }
                                */
                                if (pt.x < left) {
                                    return false;
                                }
                                util::ui32 textWidth = font->GetStringWidth (item->text.c_str ());
                                if (pt.x > left + textWidth) {
                                    return false;
                                }
                                hitTestInfo.item = item;
                                hitTestInfo.sbidx = speedBar->origin.y + pt.y / font->GetHeight ();
                                hitTestInfo.pidx = iidx;
                                hitTestInfo.rect.left = left;
                                hitTestInfo.rect.top = (pt.y / font->GetHeight ()) * font->GetHeight ();
                                hitTestInfo.rect.right = left + textWidth + starWidth;
                                hitTestInfo.rect.bottom = hitTestInfo.rect.top + font->GetHeight ();
                                return true;
                            }
                            ++hitTestInfo.level;
                            iidx -= children.size ();
                        }
                    }
                    return false;
                }

                void SpeedBarWindow::UpdateScrollBars () {
                    if (speedBar == 0) {
                        ShowScrollBar (SB_HORZ, FALSE);
                        ShowScrollBar (SB_VERT, FALSE);
                    }
                    else {
                        util::ui32 width = GetWidth ();
                        util::ui32 height = GetHeight ();
                        RECT rect;
                        GetClientRect (&rect);
                        if ((util::ui32)rect.right < width) {
                            util::i32 range = (width - rect.right) / font->GetAveCharWidth ();
                            range += (width - rect.right) % font->GetAveCharWidth () > 0;
                            SetScrollRange (SB_HORZ, 0, range, FALSE);
                            SetScrollPos (SB_HORZ, speedBar->origin.x, TRUE);
                        }
                        else {
                            ShowScrollBar (SB_HORZ, FALSE);
                            if (speedBar->origin.x) {
                                speedBar->origin.x = 0;
                                Invalidate ();
                            }
                        }
                        if ((util::ui32)rect.bottom < height) {
                            util::i32 range = (height - rect.bottom) / font->GetHeight ();
                            range += (height - rect.bottom) % font->GetHeight () > 0;
                            SetScrollRange (SB_VERT, 0, range, FALSE);
                            SetScrollPos (SB_VERT, speedBar->origin.y, TRUE);
                        }
                        else {
                            ShowScrollBar (SB_VERT, FALSE);
                            if (speedBar->origin.y) {
                                speedBar->origin.y = 0;
                                Invalidate ();
                            }
                        }
                    }
                }

                void SpeedBarWindow::RemoveFlyby () {
                    if (flyby.sbidx != util::NIDX32) {
                        openGLRC->SetCurrent ();
                        Viewport2D viewport2D (m_hWnd);
                        thekogans::opengl::DrawBuffer drawBuffer (GL_FRONT);
                        DrawItem (flyby.item, flyby.sbidx, flyby.level, colors[SB_COLOR_TEXT]);
                        flyby.sbidx = util::NIDX32;
                    }
                }

                util::ui32 SpeedBarWindow::GetWidth () const {
                    util::ui32 width = 0;
                    if (speedBar) {
                        util::ui32 level = 0;
                        for (const core::SpeedBar::Item *item = &speedBar->root; item != 0; item = item->GetCurrChild ()) {
                            const std::vector<core::SpeedBar::Item *> &children = item->children;
                            for (util::ui32 i = 0; i < children.size (); ++i) {
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

                void SpeedBarWindow::DrawItem (core::SpeedBar::Item *item, util::i32 startLine,
                    util::i32 level, const util::ui8Color &color) {
                    class UI : public core::SpeedBar::Item::EventHandler::UI {
                    public:
                        bool sticky;
                        bool enabled;
                        bool checked;

                        UI (core::SpeedBar::Item &item) : core::SpeedBar::Item::EventHandler::UI (item),
                            sticky (false), enabled (false), checked (false) {}

                        virtual void SetSticky (bool sticky_) {sticky = sticky_;}
                        virtual void SetEnabled (bool enabled_) {enabled = enabled_;}
                        virtual void SetChecked (bool checked_) {checked = checked_;}
                    } ui (*item);
                    item->eventHandler->OnUpdateUI (ui);
                    thekogans::opengl::Color color_ (ui.enabled ?
                        !item->children.empty () ? colors[SB_COLOR_POPUP] : colors[SB_COLOR_ITEM] : color);
                    util::ui32 x = (level * 2 - speedBar->origin.x) * font->GetAveCharWidth ();
                    util::ui32 y = GetWindowSize (m_hWnd).height - (startLine - speedBar->origin.y + 1) * font->GetHeight ();
                    if (ui.checked) {
                        thekogans::opengl::Font (font).DrawText (x - font->GetStringWidth ("*"), y, "*");
                    }
                    thekogans::opengl::Font (font).DrawText (x, y, item->text.c_str ());
                }

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
