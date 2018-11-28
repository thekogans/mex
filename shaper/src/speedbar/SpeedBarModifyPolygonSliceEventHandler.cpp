// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_shaper.
//
// libthekogans_mex_shaper is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_shaper is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_shaper. If not, see <http://www.gnu.org/licenses/>.

#if 0
// This file is a part of the B&F Software Solutions
// MFC 3D Modeling EXtensions (MEX) C++ Class library.
// Copyright (C) 1996 B&F Software Solutions Corporation
// All rights reserved.

#include "stdafx.h"
#include "PickInfo2D.h"
#include "Module2D.h"

#if defined (THEKOGANS_MEX_SHAPER_CONFIG_Debug)
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif // defined (THEKOGANS_MEX_SHAPER_CONFIG_Debug)

namespace MEX {

    class ModifyPolygonSliceEventHandler : public Item::EventHandler {
        DECLARE_EVENT_HANDLER (ModifyPolygonSliceEventHandler, Module2D)

    private:
        Editor *editor;
        // Slice a polygon or a selection set of polygons with a knife.
        class Tool : public Tool2D {
        private:
            Point3 p1;
            Point3 p2;
            Bound3 bound;
            PtrArray<BezierPolygon> bezierPolygons;

        public:
            Tool (Editor *editor) : Tool2D (editor) {}

            virtual void SetFocus () {
                GetCursorMgr ()->SetPickCursor (GetGlobalInfo ()->GetPickSize ());
                GetConsole ()->Print ("Select polygon to slice\n");
            }

            virtual void LButtonDown (View *view, ui32 flags, const Point3 &pt) {
                if (GetState () == 0) {
                    BaseProlog (view);
                    bool select = flags & FLAG_CTRL;
                    if (select || !GetEditor ()->IsSelected ()) {
                        bound = GetPickBound (view, pt);
                        PickInfo2D pickInfo (GetEditor (), bound, PickInfo2D::POLYGON);
                        if (pickInfo.FindFirst ()) {
                            BezierPolygon *bezierPolygon = GetEditor ()->GetPoly (pickInfo.pidx);
                            assert (bezierPolygon != 0);
                            if (select) {
                                bezierPolygon->ToggleSelect (GetEditor ()->GetSelset ());
                                GetEditor ()->DrawObject (bezierPolygon, true);
                            }
                            else {
                                bound = bezierPolygon->GetBound ();
                                view->DrawBoundGDIS (bound);
                                bezierPolygons.Add (bezierPolygon);
                            }
                        }
                    }
                    else {
                        GetScene ()->GetSelectedPolys (GetEditor ()->GetSelset (), bezierPolygons);
                        if (bezierPolygons.GetCount ()) {
                            bound = GetBoundPolygonList (bezierPolygons);
                            view->DrawBoundGDIS (bound);
                        }
                    }
                }
                else if (GetState () == 2) {
                    ScrollProlog (view);
                    p1 = pt;
                    p2 = pt;
                    view->DrawArrow2DGDIS (p1, p2);
                }
                else if (GetState () == 4) {
                    GetCursorMgr ()->ShowCursor (false);
                    SetScrollLock (view->SetScrollLock (true));
                    IncState ();
                    p2 = pt;
                    view->DrawArrow2DGDIS (p1, p2);
                }
            }

            virtual void LButtonUp (View *view, ui32 flags, const Point3 &pt) {
                if (GetState () == 1) {
                    BaseEpilog (view);
                    if (bezierPolygons.GetCount ()) {
                        view->DrawBoundGDIS (bound);
                        for (ui32 i = 0; i < bezierPolygons.GetCount (); ++i) {
                            if (!bezierPolygons[i]->IsClosed ()) {
                                GetConsole ()->Print ("Polygon must be closed.");
                                bezierPolygons.Flush (false);
                                return;
                            }
                        }
                        SetState (2);
                        GetCursorMgr ()->SetCursor (CROSS_CURSOR);
                        GetConsole ()->Print ("Select point for cut plane\n");
                    }
                }
                else if (GetState () == 3) {
                    GetCursorMgr ()->ShowCursor (true);
                    view->SetScrollLock (IsScrollLock ());
                    IncState ();
                    GetConsole ()->Print ("Select angle for cut plane\n");
                }
                else if (GetState () == 5) {
                    BaseEpilog (view);
                    GetCursorMgr ()->SetPickCursor (GetGlobalInfo ()->GetPickSize ());
                    PtrArray<BezierPolygon> tempBezierPolygons;
                    PtrArray<BezierPolygon> newBezierPolygons;
                    Point3 t = p2 - p1;
                    t.Normalize ();
                    Matrix2 xform = Translate (-p1) * RotateZ (atan2 (t[0], t[1]));
                    Matrix2 ixform = xform.Invert ();
                    GetEditor ()->DrawPolyList (bezierPolygons, false);
                    for (ui32 i = 0; i < bezierPolygons.GetCount (); ++i) {
                        bezierPolygons[i]->Xform (xform);
                        bezierPolygons[i]->Slice (tempBezierPolygons);
                        for (ui32 j=0 ; j<tempBezierPolygons.GetCount () ; ++j) {
                            tempBezierPolygons[j]->Xform (ixform);
                            newBezierPolygons.Add (tempBezierPolygons[j]);
                        }
                        tempBezierPolygons.Flush (false);
                    }
                    GetEditor ()->DeletePolyList (bezierPolygons, false);
                    bezierPolygons.Flush (false);
                    GetEditor ()->AddPolyList (newBezierPolygons, true);
                    newBezierPolygons.Flush (false);
                    GetConsole ()->Print ("Select polygon to slice\n");
                }
            }

            virtual void RButtonDown (View *view, ui32 flags, const Point3 &pt) {
                if (GetState () == 0) {
                    BaseProlog (view);
                }
                else if (GetState () == 2) {
                    ScrollProlog (view);
                    GetCursorMgr ()->SetPickCursor (GetGlobalInfo ()->GetPickSize ());
                    bezierPolygons.Flush (false);
                }
                else if (GetState () == 4) {
                    SetScrollLock (view->SetScrollLock (true));
                    IncState ();
                    GetCursorMgr ()->SetPickCursor (GetGlobalInfo ()->GetPickSize ());
                    GetCursorMgr ()->ShowCursor (false);
                    p2 = pt;
                    view->DrawArrow2DGDIS (p1, p2);
                    bezierPolygons.Flush (false);
                }
            }

            virtual void RButtonUp (View *view, ui32 flags, const Point3 &pt) {
                if (IsOddState ()) {
                    ScrollEpilog (view);
                    GetConsole ()->Print ("Select object to slice\n");
                }
            }

            virtual void MouseMoved (View *view, ui32 flags, const Point3 &pt) {
                if (GetState () == 0 || GetState () == 2) {
                    Tool2D::MouseMoved (view, flags, pt);
                }
                else if (GetState () == 4) {
                    view->DrawArrow2DGDIS (p1, p2);
                    p2 = pt;
                    view->DrawArrow2DGDIS (p1, p2);
                    Tool2D::MouseMoved (view, flags, pt);
                }
            }

            virtual void UpdateUI (View *view) {
                if (GetState () == 4) {
                    view->DrawArrow2DGDIS (p1, p2);
                    SetScrolled (true);
                }
            }
        } tool;

    public:
        ModifyPolygonSliceEventHandler (Editor *editor_) :
            editor (editor_), tool (editor) {}

        virtual void SetFocus () {editor->SetTool (&tool);}
    };

    IMPLEMENT_EVENT_HANDLER (ModifyPolygonSliceEventHandler, Module2D)

} // namespace MEX
#endif
