#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/command/ViewLayoutCommands.h"
#include "thekogans/mex/3ds/mex/core/Module.h"
#include "thekogans/mex/3ds/mex/core/Tool.h"
#include "thekogans/mex/3ds/mex/core/CursorMgr.h"
#include "Util.h"
#include "ViewLayoutWindow.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                void ViewLayoutWindow::Draw () {
                    if (viewLayout != 0) {
                        openGLRC->SetCurrent ();
                        opengl::ViewLayout (viewLayout->module.GetIOModule ().viewLayout).Draw (
                            viewLayout->module.GetOpenGLModule ());
                        core::HideCursor hideCursor;
                        openGLRC->SwapBuffers ();
                    }
                }

                void ViewLayoutWindow::OnIdle () {
                    if (::GetFocus () == m_hWnd) {
                        CRect rect;
                        GetClientRect (&rect);
                        ClientToScreen (&rect);
                        POINT pt;
                        GetCursorPos (&pt);
                        if (!rect.PtInRect (pt)) {
                            core::CursorMgr::Instance ()->OnMouseMove (blas::Point (-1, -1));
                        }
                    }
                }

                void ViewLayoutWindow::SetViewLayout (core::ViewLayout &viewLayout_) {
                    if (viewLayout != 0) {
                        viewLayout->KillFocus ();
                    }
                    viewLayout = &viewLayout_;
                    if (viewLayout != 0) {
                        command::ViewLayoutSetSizeCommand (
                            viewLayout->module.GetIOModule ().viewLayout, GetSize ()).Execute ();
                        viewLayout->SetFocus ();
                    }
                    Invalidate ();
                }

                void ViewLayoutWindow::DrawView (const io::View &view) {
                    if (viewLayout != 0) {
                        openGLRC->SetCurrent ();
                        viewLayout->module.GetOpenGLModule ().GetView (view)->Draw (
                            viewLayout->module.GetOpenGLModule ());
                        core::HideCursor hideCursor;
                        openGLRC->SwapBuffers (view.size);
                    }
                }

                BOOL ViewLayoutWindow::PreCreateWindow (CREATESTRUCT &createStruct) {
                    if (!CWnd::PreCreateWindow (createStruct)) {
                        return FALSE;
                    }
                    createStruct.lpszClass = AfxRegisterWndClass (
                        CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                        ::LoadCursor (NULL, IDC_ARROW), HBRUSH (COLOR_WINDOW + 1), NULL);
                    createStruct.dwExStyle |= WS_EX_CLIENTEDGE;
                    createStruct.style &= ~WS_BORDER;
                    return TRUE;
                }

                int ViewLayoutWindow::OnCreate (LPCREATESTRUCT lpCreateStruct) {
                    if (CWnd::OnCreate (lpCreateStruct) == -1) {
                        return -1;
                    }
                    openGLRC.reset (new win::util::OpenGLRC (m_hWnd));
                    assert (core::UI::Instance ()->viewLayoutWindow == 0);
                    core::UI::Instance ()->viewLayoutWindow = this;
                    return 0;
                }

                void ViewLayoutWindow::OnDestroy () {
                    openGLRC.reset (0);
                    CWnd::OnDestroy ();
                }

                void ViewLayoutWindow::OnKillFocus (CWnd *pNewWnd) {
                    core::CursorMgr::Instance ()->OnMouseMove (blas::Point (-1, -1));
                }

                void ViewLayoutWindow::OnActivate (UINT nState, CWnd *pWndOther, BOOL bMinimized) {
                    CWnd::OnActivate (nState, pWndOther, bMinimized);
                    bMouseActivate = nState == WA_INACTIVE;
                }

                int ViewLayoutWindow::OnMouseActivate (CWnd *pDesktopWnd, UINT nHitTest, UINT message) {
                    if (bMouseActivate && nHitTest == HTCLIENT) {
                        bMouseActivate = false;
                        return MA_ACTIVATEANDEAT;
                    }
                    return CWnd::OnMouseActivate (pDesktopWnd, nHitTest, message);
                }

                void ViewLayoutWindow::OnCancelMode () {
                    if (viewLayout != 0) {
                        blas::Point pt = GetMousePosition ();
                        viewLayout->RButtonDown (0, pt);
                        viewLayout->RButtonUp (0, pt);
                    }
                    CWnd::OnCancelMode ();
                }

                void ViewLayoutWindow::OnSize (UINT nType, int cx, int cy) {
                    CWnd::OnSize (nType, cx, cy);
                    if (viewLayout != 0 && cx > 0 && cy > 0) {
                        command::ViewLayoutSetSizeCommand (
                            viewLayout->module.GetIOModule ().viewLayout,
                            blas::Size (0, 0, cx, cy)).Execute ();
                    }
                }

                BOOL ViewLayoutWindow::OnSetCursor (CWnd *pWnd, UINT nHitTest, UINT message) {
                    //if (::GetFocus () == m_hWnd) {
                        core::CursorMgr::Instance ()->OnSetCursor ();
                    //}
                    //else {
                        //SetCursor (arrowCursor);
                    //}
                    return TRUE;
                }

                BOOL ViewLayoutWindow::OnEraseBkgnd (CDC *) {
                    return TRUE;
                }

                void ViewLayoutWindow::OnPaint () {
                    CPaintDC dc (this);
                    Draw ();
                }

                void ViewLayoutWindow::OnLButtonDown (UINT flags, CPoint dp) {
                    if (viewLayout != 0) {
                        viewLayout->LButtonDown (GetControlKeyState (), openGLRC->Win2GL (dp));
                    }
                }

                void ViewLayoutWindow::OnLButtonDblClk (UINT flags, CPoint dp) {
                    if (viewLayout != 0) {
                        viewLayout->LButtonDblClk (GetControlKeyState (), openGLRC->Win2GL (dp));
                    }
                }

                void ViewLayoutWindow::OnLButtonUp (UINT flags, CPoint dp) {
                    if (viewLayout != 0) {
                        viewLayout->LButtonUp (GetControlKeyState (), openGLRC->Win2GL (dp));
                    }
                }

                void ViewLayoutWindow::OnRButtonDown (UINT flags, CPoint dp) {
                    if (viewLayout != 0) {
                        viewLayout->RButtonDown (GetControlKeyState (), openGLRC->Win2GL (dp));
                    }
                }

                void ViewLayoutWindow::OnRButtonUp (UINT flags, CPoint dp) {
                    if (viewLayout != 0) {
                        viewLayout->RButtonUp (GetControlKeyState (), openGLRC->Win2GL (dp));
                    }
                }

                void ViewLayoutWindow::OnMouseMove (UINT flags, CPoint dp) {
                    //if (::GetFocus () == m_hWnd) {
                        openGLRC->SetCurrent ();
                        blas::Point pt = openGLRC->Win2GL (dp);
                        core::CursorMgr::Instance ()->OnMouseMove (pt);
                        if (viewLayout != 0) {
                            viewLayout->MouseMove (GetControlKeyState (), pt);
                        }
                    //}
                }

                void ViewLayoutWindow::OnKeyDown (UINT key, UINT repeatCount, UINT flags) {
                    if (viewLayout != 0) {
                        viewLayout->KeyDown (key, repeatCount, GetControlKeyState ());
                    }
                }

                LRESULT ViewLayoutWindow::OnDisplayChange (WPARAM wp, LPARAM lp) {
                    openGLRC.reset (new win::util::OpenGLRC (m_hWnd));
                    return LRESULT (TRUE);
                }

                BEGIN_MESSAGE_MAP (ViewLayoutWindow, CWnd)
                    ON_WM_CREATE ()
                    ON_WM_DESTROY ()
                    ON_WM_KILLFOCUS ()
                    ON_WM_ACTIVATE ()
                    ON_WM_MOUSEACTIVATE ()
                    ON_WM_CANCELMODE ()
                    ON_WM_SIZE ()
                    ON_WM_SETCURSOR ()
                    ON_WM_ERASEBKGND ()
                    ON_WM_PAINT ()
                    ON_WM_LBUTTONDOWN ()
                    ON_WM_LBUTTONDBLCLK ()
                    ON_WM_LBUTTONUP ()
                    ON_WM_RBUTTONDOWN ()
                    ON_WM_RBUTTONUP ()
                    ON_WM_MOUSEMOVE ()
                    ON_WM_KEYDOWN ()
                    ON_MESSAGE (WM_DISPLAYCHANGE, OnDisplayChange)
                END_MESSAGE_MAP ()

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans
