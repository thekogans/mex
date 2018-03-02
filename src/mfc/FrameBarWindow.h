#if !defined (__thekogans_3ds_mex_ui_FrameBarWindow_h)
#define __thekogans_3ds_mex_ui_FrameBarWindow_h

#if !defined (WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif // !defined (WIN32_LEAN_AND_MEAN)
#if !defined (NOMINMAX)
    #define NOMINMAX
#endif // !defined (NOMINMAX)
#include <afxwin.h>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/mex/core/Module.h"
#include "thekogans/mex/3ds/mex/core/UI.h"

namespace thekogans {
    namespace _3ds {
        namespace mex {
            namespace ui {

                struct FrameBarWindow : public CWnd, public core::UI::FrameBarWindow {
                private:
                    bool dragging;
                    bool boxMode;
                    HFONT hfont;
                    EventSink *eventSink;

                public:
                    FrameBarWindow () : dragging (false),
                        boxMode (false), hfont (0), eventSink (0) {}

                    inline void SetFont (HFONT hfont_) {hfont = hfont_;}

                    virtual void Show (bool show) {
                        ShowWindow (show ? SW_SHOW : SW_HIDE);
                    }

                    virtual void Draw ();
                    virtual void SetEventSink (EventSink *eventSink_) {
                        eventSink = eventSink_;
                    }

                    bool IsVisible () const {
                        return eventSink != 0 && eventSink->IsVisible ();
                    }

                private:
                    inline util::ui32 GetStartFrame () const {
                        return eventSink != 0 ? eventSink->GetStartFrame () : 0;
                    }
                    inline util::ui32 GetEndFrame () const {
                        return eventSink != 0 ? eventSink->GetEndFrame () : 0;
                    }
                    inline util::ui32 GetCurrFrame () const {
                        return eventSink != 0 ? eventSink->GetCurrFrame () : 0;
                    }
                    inline bool IsBoxMode () const {
                        return eventSink != 0 && eventSink->IsBoxMode ();
                    }
                    inline void SetBoxMode (bool boxMode) {
                        if (eventSink != 0) {
                            eventSink->SetBoxMode (boxMode);
                        }
                    }

                    bool GoToFrame (CPoint point);
                    util::i32 GetInsideWidth (CRect &rect, CDC *dc = 0);
                    util::i32 GetCurrFrameWidth (CDC *dc);

                protected:
                    afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
                    afx_msg BOOL OnEraseBkgnd (CDC *pDC);
                    afx_msg void OnPaint ();
                    afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
                    afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
                    afx_msg void OnMouseMove (UINT nFlags, CPoint point);

                    DECLARE_MESSAGE_MAP ()
                };

            } // namespace ui
        } // namespace mex
    } // namespace _3ds
} // namespace thekogans

#endif // !defined (__thekogans_3ds_mex_ui_FrameBarWindow_h)
