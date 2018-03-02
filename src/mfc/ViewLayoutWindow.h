#ifndef __thekogans_3ds_mex_ui_ViewLayoutWindow_h
#define __thekogans_3ds_mex_ui_ViewLayoutWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <memory>
#include "thekogans/mex/blas/Size.h"
#include "thekogans/win/util/OpenGLRC.h"
#include "thekogans/mex/3ds/mex/core/UI.h"
#include "thekogans/mex/3ds/mex/core/ViewLayout.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct ViewLayoutWindow : public CWnd, public core::UI::ViewLayoutWindow {
                private:
                    core::ViewLayout *viewLayout;
                    bool bMouseActivate;
                    HCURSOR arrowCursor;
                    std::auto_ptr<win::util::OpenGLRC> openGLRC;

                    virtual BOOL PreCreateWindow (CREATESTRUCT &createStruct);

                public:
                    ViewLayoutWindow () : viewLayout (0), bMouseActivate (false),
                        arrowCursor (LoadCursor (NULL, IDC_ARROW)) {}

                    virtual void Draw ();
                    virtual void OnIdle ();
                    virtual blas::Size GetSize () const {
                        CRect rect;
                        GetClientRect (&rect);
                        return blas::Size (0, 0, rect.right, rect.bottom);
                    }
                    virtual void SetViewLayout (core::ViewLayout &viewLayout_);
                    virtual void DrawView (const io::View &view);
                    virtual bool IsOperationCanceled () {
                        return GetAsyncKeyState (VK_ESCAPE) < 0;
                    }
                    virtual void CaptureMouse () {
                        SetCapture ();
                    }
                    virtual void ReleaseMouse () {
                        ReleaseCapture ();
                    }
                    virtual blas::Point GetMousePosition () const {
                        POINT pt;
                        GetCursorPos (&pt);
                        ScreenToClient (&pt);
                        RECT rect;
                        GetClientRect (&rect);
                        return blas::Point (pt.x, rect.bottom - pt.y);
                    }
                    virtual void SetMousePosition (const blas::Point &pt) const {
                        RECT rect;
                        GetClientRect (&rect);
                        POINT p;
                        p.x = pt.x;
                        p.y = rect.bottom - pt.y;
                        ClientToScreen (&p);
                        SetCursorPos (p.x, p.y);
                    }

                protected:
                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnDestroy ();
                    afx_msg void OnKillFocus (CWnd *pNewWnd);
                    afx_msg void OnActivate (UINT nState, CWnd *pWndOther, BOOL bMinimized);
                    afx_msg int OnMouseActivate (CWnd *pDesktopWnd, UINT nHitTest, UINT message);
                    afx_msg void OnCancelMode ();
                    afx_msg void OnSize (UINT nType, int cx, int cy);
                    afx_msg BOOL OnSetCursor (CWnd *pWnd, UINT nHitTest, UINT message);
                    afx_msg BOOL OnEraseBkgnd (CDC *);
                    afx_msg void OnPaint ();
                    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
                    afx_msg void OnLButtonDblClk (UINT nFlags, CPoint point);
                    afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
                    afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
                    afx_msg void OnRButtonUp (UINT nFlags, CPoint point);
                    afx_msg void OnMouseMove (UINT nFlags, CPoint point);
                    afx_msg void OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags);
                    afx_msg LRESULT OnDisplayChange (WPARAM wp, LPARAM lp);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_ViewLayoutWindow_h)
