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

#if !defined (__thekogans_mex_core_Tool_h)
#define __thekogans_mex_core_Tool_h

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/command/Command.h"
#include "thekogans/mex/command/FinalOperation.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ShadeModel.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/Scissor.h"
#include "thekogans/mex/opengl/ModelView.h"
#include "thekogans/mex/opengl/Projection.h"
#include "thekogans/mex/opengl/Viewport.h"
#include "thekogans/mex/opengl/LogicOp.h"
#include "thekogans/mex/opengl/DrawBuffer.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/core/Config.h"
#include "thekogans/mex/core/StatusBar.h"
#include "thekogans/mex/core/Util.h"

namespace thekogans {
    namespace mex {
        namespace core {

            struct ViewLayout;
            struct Module;

            // Base tool class. Every MEX tool has one of these as
            // it's ancestor. Defines base functionality every tool
            // can have. Most geometry manipulation tools have certain
            // basic features in common:
            //
            //      1. If you hold down the CONTROL key before
            //          clicking on geometry, you select it in
            //          to current selection set.
            //      2. If Module::IsSelected () return true,
            //          the tool acts on currently selected
            //          geometry.
            //      3. If you hold down the SHIFT key before
            //          clicking on geometry, you clone it at
            //          the end of the operation.
            //      4. Right click to cancel the operation.
            //      5. Directional tools (move, scale 2D, skew...)
            //          can have their direction changed in the
            //          middle of the operation by hitting the TAB key.
            //          The new direction is indicated by a directional
            //          cursor (LEFT_RIGHT, UP_DOWN or BOTH).
            //
            // To get a feel for how the various tool variables and
            // functions are used, see the tool examples provided with MEX.

            struct _LIB_THEKOGANS_MEX_CORE_DECL Tool {
                struct Viewport2D {
                private:
                    opengl::ShadeModel shadeModel;
                    opengl::Enable lighting;
                    opengl::Enable depthTest;
                    opengl::Enable cullFace;
                    opengl::Enable texture2D;
                    opengl::Enable scissorTest;
                    opengl::Scissor scissor;
                    opengl::ModelView modeView;
                    opengl::Projection projection;
                    opengl::Viewport viewport;
                    opengl::LogicOp logicOp;
                    opengl::DrawBuffer drawBuffer;
                    opengl::Color color;

                public:
                    Viewport2D (
                        const _3ds::opengl::View &view,
                        const opengl::ui8Color &color_ = _3ds::opengl::SystemXORPalette[_3ds::opengl::WHITE]) :
                        shadeModel (GL_FLAT),
                        lighting (GL_LIGHTING, false),
                        depthTest (GL_DEPTH_TEST, false),
                        cullFace (GL_CULL_FACE, false),
                        texture2D (GL_TEXTURE_2D, false),
                        scissorTest (GL_SCISSOR_TEST, true),
                        scissor (view.view.size),
                        modeView (blas::Matrix4::Identity),
                        projection (blas::Point2::Zero, view.projectionXform),
                        viewport (view.view.size),
                        logicOp (GL_XOR),
                        drawBuffer (GL_FRONT),
                        color (color_) {}

                private:
                    Viewport2D (const Viewport2D &);
                    Viewport2D &operator = (const Viewport2D &);
                };

            protected:
                Module &module;     // The module to which this tool belongs.
                                    // This reference is the tool's window on
                                    // the rest of the hierarchy (views,
                                    // objects...).

                util::ui32 cursor;  // CursorMgr maintains an array of
                                    // frequently used cursors. This is
                                    // an index in to that array. If your
                                    // tool has custom cursors, you will
                                    // have to load them yourself. CursorMgr
                                    // has a SetCursor (const QCursor *) which
                                    // you can use to set current cursor.

                util::ui32 state;   // Tools present state. As users click
                                    // the mouse buttons to interact with
                                    // geometry, state keeps track of their
                                    // progress.

                util::ui32 flags;   // Passed in from various event handlers.
                                    // Used to indicate the state of various
                                    // modifier keys (Ctrl/Alt/Shift).

                bool scrollLock;    // Holds the view's current scroll lock
                                    // state. Most MEX tools implement click
                                    // type operation (mouse button down and up).
                                    // While in mid click (mouse down), the tool
                                    // state is indeterminate. During this
                                    // state the tool should lock the view
                                    // from scrolling by setting scrollLock to true.

            public:
                explicit Tool (Module &module_) :
                    module (module_),
                    cursor (0),
                    state (0),
                    flags (0),
                    scrollLock (false) {}
                virtual ~Tool () {}

                inline bool IsScrollLock () const {
                    return scrollLock;
                }

                virtual const char *GetName () const = 0;

                // Description:
                //      Called by the framework when the tool receives focus.
                virtual void SetFocus () {}

                // Description:
                //      Called by the framework when the tool looses focus.
                virtual void KillFocus ();

                // Description:
                //      Called by the framework in response to WM_LBUTTONDOWN message.
                // Parameters:
                //      view - view in which the message occured.
                //      flags - various key flags (FLAG_CTRL...).
                //      pt - point on view plane. To transform the point to world
                //          space, call view.V2W (pt).
                virtual void LButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_LBUTTONDBLCLK message.
                // Parameters:
                //      See LButtonDown.
                virtual void LButtonDblClk (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {}

                // Description:
                //      Called by the framework in response to WM_LBUTTONUP message.
                // Parameters:
                //      See LButtonDown.
                virtual void LButtonUp (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_RBUTTONDOWN message.
                // Parameters:
                //      See LButtonDown.
                virtual void RButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_RBUTTONUP message.
                // Parameters:
                //      See LButtonDown.
                virtual void RButtonUp (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt);

                // Description:
                //      Called by the framework in response to WM_MOUSEMOVE message.
                // Parameters:
                //      See LButtonDown.
                virtual void MouseMove (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {}

                // Description:
                //      Called by the framework in response to WM_KEYDOWN message.
                // Parameters:
                //      view - view in which the message occured.
                //      key - key scan code.
                //      repeatCount - number of repeating keystrokes in this event.
                //      flags - various key flags (FLAG_CTRL...).
                virtual void KeyDown (
                    const _3ds::opengl::View &view,
                    util::ui32 key,
                    util::ui32 repeatCount,
                    util::ui32 flags) {}

                // Description:
                //      Called by the framework in response to WM_KEYUP message.
                // Parameters:
                //      See KeyDown.
                void KeyUp (
                    const _3ds::opengl::View &view,
                    util::ui32 key,
                    util::ui32 repeatCount,
                    util::ui32 flags) {}

                // Description:
                //      Called by ViewLayout after it has scrolled the view, and redrawn the scene.
                virtual void UpdateUI (const _3ds::opengl::View &view) {}

                // Description:
                //      Called by SpeedBarToolStatusEventHandler.
                virtual void UpdateStatus (StatusBar::Item::EventHandler::UI &ui) {
                    ui.SetText ("");
                }

                virtual bool IsPenEnabled () const {
                    return false;
                }
                virtual void HandlePenInput (std::vector<blas::Point2> &stroke) {}

            protected:
                inline bool IsCtrl () const {
                    return util::Flags32 (flags).Test (FLAG_CTRL);
                }
                inline bool IsAlt () const {
                    return util::Flags32 (flags).Test (FLAG_ALT);
                }
                inline bool IsShift () const {
                    return util::Flags32 (flags).Test (FLAG_SHIFT);
                }

                inline bool IsSelect () const {
                    return IsCtrl ();
                }
                inline bool IsSelected () const {
                    return IsAlt ();
                }
                inline bool IsClone () const {
                    return IsShift ();
                }

                enum {
                    CursorVisible = 0x00000008,
                    CursorHidden = 0x00000010,
                    ScrollLockOn = 0x00000020,
                    ScrollLockOff = 0x00000040,
                    ViewCaptured = 0x00000080,
                    ViewReleased = 0x00000100
                };
                void UpdateState (util::ui32 state_, util::ui32 stateFlags);

                inline bool IsOddState () const {
                    return (state & 1) == 1;
                }
                inline bool IsEvenState () const {
                    return (state & 1) == 0;
                }

                // Description:
                //      Returns a pick bound centered around a point.
                // Return:
                //      Pick bound.
                // Parameters:
                //      view - view to calculate rectangle extents.
                //      pt - point in view space to get a rectangle around.
                blas::Bound2 GetPickBound (
                    const _3ds::opengl::View &view,
                    const blas::Point2 &pt);

                bool IsTransactionPending ();
                bool IsPendingTransactionEmpty () const;
                void BeginTransaction ();
                void CheckpointTransaction ();
                void RollbackTransaction ();
                void CommitTransaction ();
                void AbortTransaction ();

                bool ExecuteAndAddCommand (
                    thekogans::mex::command::Command::SharedPtr command);
                bool ExecuteAndAddFinalOperation (
                    thekogans::mex::command::FinalOperation::SharedPtr finalOperation);
            };

            #define THEKOGANS_MEX_CORE_DECLARE_TOOL(name)\
            public:\
                virtual const char *GetName () const {return #name;}

        } // namespace core
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_core_Tool_h)
