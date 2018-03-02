#ifndef __thekogans_3ds_mex_ui_SpeedBarWindow_h
#define __thekogans_3ds_mex_ui_SpeedBarWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/ui8Color.h"
#include "thekogans/util/FontMgr.h"
#include "thekogans/mex/blas/Rect.h"
#include "thekogans/win/util/OpenGLRC.h"
#include "thekogans/mex/3ds/mex/core/UI.h"
#include "thekogans/mex/3ds/mex/core/SpeedBar.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct SpeedBarWindow : public CWnd, public core::UI::SpeedBarWindow {
                protected:
                    core::SpeedBar *speedBar;
                    const util::FontMgr::Font *font;
                    enum {
                        SB_COLOR_BKGRND,
                        SB_COLOR_TEXT,
                        SB_COLOR_POPUP,
                        SB_COLOR_ITEM,
                        SB_COLOR_FLYBY,
                        SB_COLOR_COUNT
                    };
                    util::ui8Color colors[SB_COLOR_COUNT];
                    struct HitTestInfo {
                        core::SpeedBar::Item *item;
                        util::ui32 sbidx;
                        util::ui32 pidx;
                        util::i32 level;
                        blas::Rect rect;

                        HitTestInfo () :
                            item (0), sbidx (util::NIDX32), pidx (util::NIDX32),
                            level (0), rect (blas::Rect::Zero) {}
                    } flyby;
                    std::auto_ptr<win::util::OpenGLRC> openGLRC;

                public:
                    SpeedBarWindow ();

                    virtual void Save (const char *keyName);
                    virtual void Restore (const char *keyName);
                    virtual void Draw ();
                    virtual void OnIdle ();
                    virtual void SetSpeedBar (core::SpeedBar &speedBar_);

                protected:
                    virtual BOOL PreCreateWindow (CREATESTRUCT &cs);

                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg void OnDestroy ();
                    afx_msg void OnSize (UINT nType, int cx, int cy);
                    afx_msg BOOL OnEraseBkgnd (CDC *pDC);
                    afx_msg void OnPaint ();
                    afx_msg void OnHScroll (UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
                    afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
                    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
                    afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
                    afx_msg void OnRButtonDown (UINT nFlags, CPoint point);
                    afx_msg void OnMouseMove (UINT nFlags, CPoint point);
                    afx_msg void OnFont ();
                    afx_msg void OnColor ();

                    DECLARE_MESSAGE_MAP ()

                private:
                    bool HitTest (POINT pt, HitTestInfo &hti);

                    void UpdateScrollBars ();
                    void RemoveFlyby ();

                    util::ui32 GetWidth () const;
                    inline util::ui32 GetHeight () const {
                        return speedBar ? speedBar->GetItemCount () * font->GetHeight () : 0;
                    }

                    void DrawItem (core::SpeedBar::Item *item, util::i32 startLine,
                        util::i32 level, const util::ui8Color &color);
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_SpeedBarWindow_h)
