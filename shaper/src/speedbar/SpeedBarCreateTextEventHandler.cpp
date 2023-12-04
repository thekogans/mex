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

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Path.h"
#include "thekogans/mex/blas/Point.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Matrix2.h"
#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/command/OwnerVectorCommands.h"
#include "thekogans/mex/3ds/ext/Units.h"
#include "thekogans/mex/3ds/ext/shaper/BezierPolygon.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/Util.h"
#include "thekogans/mex/shaper/ui/TextDialog.h"

namespace thekogans {
    namespace mex {
        namespace command {

            THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND_EX_T (
                OwnerVectorassignCommand<_3ds::io::BezierPolygon2>, 256)

        } // namespace command
        namespace shaper {

            class SpeedBarCreateTextFontEventHandler : public core::SpeedBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateTextFontEventHandler)

            public:
                explicit SpeedBarCreateTextFontEventHandler (core::Module &) {}

                virtual void OnSetFocus () {
                    ChooseFontDialog (core::UI::Instance ().mainFrameWindow->GetQWidget ());
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateTextFontEventHandler, Shaper)

            class SpeedBarCreateTextEnterEventHandler : public core::SpeedBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateTextEnterEventHandler)

            public:
                explicit SpeedBarCreateTextEnterEventHandler (core::Module &) {}

                virtual void OnSetFocus () {
                    core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                    TextDialog textDialog;
                    textDialog.exec ();
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateTextEnterEventHandler, Shaper)

            namespace {
                const char *IDS_CREATETEXT_0 = "Place one corner of bounding rect:\n";
                const char *IDS_CREATETEXT_1 = "Place opposite corner of bounding rect:\n";
                const char *IDS_CREATETEXT_2 = "[%s %s]";
            }

            class SpeedBarCreateTextPlaceEventHandler : public core::SpeedBar::Item::ToolEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateTextPlaceEventHandler)

            private:
                class Tool : public core::Tool {
                    THEKOGANS_MEX_CORE_DECLARE_TOOL (SpeedBarCreateTextPlaceEventHandlerTool)

                private:
                    blas::Bound2 rect;
                    bool aspect;
                    util::f32 ratio;
                    util::f32 oldx;

                    struct GlyphOutline {
                        util::f32 horizontalAdvance;
                        util::OwnerVector<_3ds::io::BezierPolygon2> bezierPolygons;

                        struct FreeTypeError {
                            std::string message;
                            explicit FreeTypeError (FT_Error error) {
                                // FIXME: implement
                                assert (0);
                            }
                            explicit FreeTypeError (const std::string &message_) :
                                message (message_) {}
                        };

                        struct FreeTypeLibrary {
                            FT_Library library;
                            FreeTypeLibrary () {
                                FT_Error error = FT_Init_FreeType (&library);
                                if (error != FT_Err_Ok) {
                                    throw FreeTypeError (error);
                                }
                            }
                            ~FreeTypeLibrary () {
                                FT_Error error = FT_Done_FreeType (library);
                                // No throwing in dtors.
                                assert (error == FT_Err_Ok);
                            }
                        };

                        struct FreeTypeFace {
                            FT_Face face;
                            FreeTypeFace (FT_Library library, const std::string &fontPath) : face (0) {
                                FT_Error error = FT_New_Face (library, fontPath.c_str (), 0, &face);
                                if (error != FT_Err_Ok) {
                                    throw FreeTypeError (error);
                                }
                                if ((face->face_flags & FT_FACE_FLAG_SCALABLE) == 0) {
                                    throw FreeTypeError ("Not a scalable font.");
                                }
                            }
                            ~FreeTypeFace () {
                                FT_Error error = FT_Done_Face (face);
                                // No throwing in dtors.
                                assert (error == FT_Err_Ok);
                            }
                        };

                        struct FreeTypeGlyph {
                            FT_Glyph glyph;
                            FreeTypeGlyph (FT_Face face, char letter) : glyph (0) {
                                FT_UInt charIndex = FT_Get_Char_Index (face, letter);
                                if (charIndex == 0) {
                                    throw FreeTypeError ("Invalid letter (no index).");
                                }
                                // All we want is the raw outline data.
                                // Keep the cooking to a minimum.
                                FT_Error error = FT_Load_Glyph (face, charIndex,
                                    FT_LOAD_NO_SCALE | FT_LOAD_NO_HINTING |
                                    FT_LOAD_NO_BITMAP | FT_LOAD_IGNORE_TRANSFORM |
                                    FT_LOAD_NO_AUTOHINT);
                                if (error != FT_Err_Ok) {
                                    throw FreeTypeError (error);
                                }
                                error = FT_Get_Glyph (face->glyph, &glyph);
                                if (error != FT_Err_Ok) {
                                    throw FreeTypeError (error);
                                }
                                if (glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
                                    throw FreeTypeError ("Not a scalable font.");
                                }
                            }
                            ~FreeTypeGlyph () {
                                // Unlike other FT_Done_* functions,
                                // this one returns void. Go figure.
                                FT_Done_Glyph (glyph);
                            }
                        };

                        GlyphOutline (const FreeTypeFace &freeTypeFace, char letter) :
                                horizontalAdvance (0.0f) {
                            FreeTypeGlyph freeTypeGlyph (freeTypeFace.face, letter);
                            horizontalAdvance = f32FromFixed (
                                freeTypeFace.face->glyph->metrics.horiAdvance);
                            FT_Outline_Funcs outlineFuncs;
                            outlineFuncs.move_to = move_to;
                            outlineFuncs.line_to = line_to;
                            outlineFuncs.conic_to = conic_to;
                            outlineFuncs.cubic_to = cubic_to;
                            outlineFuncs.shift = 0;
                            outlineFuncs.delta = 0;
                            FT_Error error = FT_Outline_Decompose (
                                &((FT_OutlineGlyph)freeTypeGlyph.glyph)->outline, &outlineFuncs, this);
                            if (error != FT_Err_Ok) {
                                throw FreeTypeError (error);
                            }
                            for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                if (!bezierPolygons[i]->vertices.empty ()) {
                                    bezierPolygons[i]->vertices[0].flags =
                                        _3ds::io::BezierPolygon2::Vertex::First;
                                    bezierPolygons[i]->vertices.back ().flags =
                                        _3ds::io::BezierPolygon2::Vertex::Last |
                                        _3ds::io::BezierPolygon2::Vertex::Closed;
                                    command::OwnerVectorassignCommand<_3ds::io::BezierPolygon2> (bezierPolygons, i,
                                        _3ds::ext::BezierPolygon2 (*bezierPolygons[i]).Weld ()).Execute ();
                                }
                            }
                        }

                        blas::Bound2 GetBound () const {
                            blas::Bound2 bound (blas::Bound2::Empty);
                            for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                bound += _3ds::ext::BezierPolygon2 (*bezierPolygons[i]).GetBound (
                                    core::GetIOProject ().shaper.steps);
                            }
                            return bound;
                        }

                        GlyphOutline &operator *= (const blas::Matrix2 &xform) {
                            for (std::size_t i = 0, count = bezierPolygons.size (); i < count; ++i) {
                                command::OwnerVectorassignCommand<_3ds::io::BezierPolygon2> (
                                    bezierPolygons, i, *bezierPolygons[i] * xform).Execute ();
                            }
                            return *this;
                        }

                    private:
                        static util::f32 f32FromFixed (FT_Pos fixed) {
                            return ((util::f32)(fixed >> 6)) + ((util::f32)(fixed & 63)) / 64.0f;
                        }

                        static int move_to (const FT_Vector *to, void *user) {
                            assert (to != 0);
                            assert (user != 0);
                            _3ds::io::BezierPolygon2::UniquePtr bezierPolygon (
                                new _3ds::io::BezierPolygon2);
                            assert (bezierPolygon.get () != 0);
                            ((GlyphOutline *)user)->bezierPolygons.push_back (bezierPolygon.get ());
                            bezierPolygon.release ();
                            return line_to (to, user);
                        }

                        static int line_to (const FT_Vector *to, void *user) {
                            assert (to != 0);
                            assert (user != 0);
                            GlyphOutline *glyphOutline = (GlyphOutline *)user;
                            assert (!glyphOutline->bezierPolygons.empty ());
                            if (!glyphOutline->bezierPolygons.empty ()) {
                                _3ds::io::BezierPolygon2 *bezierPolygon = glyphOutline->bezierPolygons.back ();
                                bezierPolygon->vertices.push_back (
                                    _3ds::io::BezierPolygon2::Vertex (
                                        blas::Point2 (f32FromFixed (to->x), f32FromFixed (to->y))));
                            }
                            return FT_Err_Ok;
                        }

                        static int conic_to (
                                const FT_Vector *control,
                                const FT_Vector *to,
                                void *user) {
                            assert (control != 0);
                            assert (to != 0);
                            assert (user != 0);
                            GlyphOutline *glyphOutline = (GlyphOutline *)user;
                            assert (!glyphOutline->bezierPolygons.empty ());
                            if (!glyphOutline->bezierPolygons.empty ()) {
                                _3ds::io::BezierPolygon2 *bezierPolygon = glyphOutline->bezierPolygons.back ();
                                _3ds::io::BezierPolygon2::Vertex &vertex = bezierPolygon->vertices.back ();
                                blas::Point2 b (f32FromFixed (control->x), f32FromFixed (control->y));
                                blas::Point2 c (f32FromFixed (to->x), f32FromFixed (to->y));
                                vertex.yellow = 2.0f / 3.0f * (b - vertex.pt);
                                bezierPolygon->vertices.push_back (
                                    _3ds::io::BezierPolygon2::Vertex (
                                        c, 2.0f / 3.0f * (b - c), blas::Point2::Zero));
                            }
                            return FT_Err_Ok;
                        }

                        static int cubic_to (
                                const FT_Vector *control1,
                                const FT_Vector *control2,
                                const FT_Vector *to,
                                void *user) {
                            assert (control1 != 0);
                            assert (control2 != 0);
                            assert (to != 0);
                            assert (user != 0);
                            GlyphOutline *glyphOutline = (GlyphOutline *)user;
                            assert (!glyphOutline->bezierPolygons.empty ());
                            if (!glyphOutline->bezierPolygons.empty ()) {
                                _3ds::io::BezierPolygon2 *bezierPolygon = glyphOutline->bezierPolygons.back ();
                                _3ds::io::BezierPolygon2::Vertex &vertex = bezierPolygon->vertices.back ();
                                blas::Point2 b (f32FromFixed (control1->x), f32FromFixed (control1->y));
                                blas::Point2 c (f32FromFixed (control2->x), f32FromFixed (control2->y));
                                blas::Point2 d (f32FromFixed (to->x), f32FromFixed (to->y));
                                vertex.yellow = b - vertex.pt;
                                bezierPolygon->vertices.push_back (
                                    _3ds::io::BezierPolygon2::Vertex (
                                        d, c - d, blas::Point2::Zero));
                            }
                            return FT_Err_Ok;
                        }
                    };

                    void CreateTextOutlines () {
                        if (core::GetIOProject ().shaper.create.text.empty () ||
                                core::GetIOProject ().shaper.create.fontFile.empty ()) {
                            core::SetCursor setCursor (core::CursorMgr::ARROW_CURSOR);
                            if (TextDialog ().exec () != QDialog::Accepted ||
                                core::GetIOProject ().shaper.create.text.empty () ||
                                core::GetIOProject ().shaper.create.fontFile.empty ()) {
                                AbortTransaction ();
                                return;
                            }
                        }
                        core::WaitCursor waitCursor;
                        util::OwnerVector<GlyphOutline> outlines;
                        blas::Bound2 bound (blas::Bound2::Empty);
                        {
                            std::unique_ptr<GlyphOutline::FreeTypeLibrary> freeTypeLibrary;
                            std::unique_ptr<GlyphOutline::FreeTypeFace> freeTypeFace;
                            try {
                                freeTypeLibrary.reset (new GlyphOutline::FreeTypeLibrary);
                                freeTypeFace.reset (
                                    new GlyphOutline::FreeTypeFace (freeTypeLibrary->library,
                                        util::MakePath (core::GetIOProject ().shaper.create.fontsPath,
                                            core::GetIOProject ().shaper.create.fontFile)));
                            }
                            catch (const GlyphOutline::FreeTypeError &error) {
                                core::UI::Instance ().consoleWindow->Print ("%s\n", error.message.c_str ());
                                return;
                            }
                            util::f32 horizontalAdvance = 0;
                            for (std::size_t i = 0,
                                    count = core::GetIOProject ().shaper.create.text.size ();
                                    i < count; ++i) {
                                try {
                                    std::unique_ptr<GlyphOutline> outline (
                                        new GlyphOutline (*freeTypeFace,
                                            core::GetIOProject ().shaper.create.text[i]));
                                    assert (outline.get () != 0);
                                    // This is here in case we are asked to render a space.
                                    util::f32 glyphHorizontalAdvance = outline->horizontalAdvance;
                                    if (!outline->bezierPolygons.empty ()) {
                                        *outline *= blas::Matrix2::Translate (
                                            blas::Point2 (horizontalAdvance, 0.0f));
                                        bound += outline->GetBound ();
                                        outlines.push_back (outline.get ());
                                        outline.release ();
                                    }
                                    horizontalAdvance += glyphHorizontalAdvance;
                                }
                                catch (const GlyphOutline::FreeTypeError &error) {
                                    core::UI::Instance ().consoleWindow->Print (
                                        "%s, skipping\n", error.message.c_str ());
                                }
                            }
                        }
                        {
                            blas::Matrix2 xform =
                                blas::Matrix2::Translate (-bound.Center ()) *
                                blas::Matrix2::Scale (blas::Point2 (
                                        rect.Extents ().x / bound.Extents ().x,
                                        rect.Extents ().y / bound.Extents ().y)) *
                                blas::Matrix2::Translate (rect.Center ());
                            for (std::size_t i = 0, count = outlines.size (); i < count; ++i) {
                                *outlines[i] *= xform;
                                AddPolygons (outlines[i]->bezierPolygons, false);
                                outlines[i]->bezierPolygons.clear ();
                            }
                        }
                        if (!IsPendingTransactionEmpty ()) {
                            ExecuteAndAddFinalOperation (
                                command::FinalOperation::SharedPtr (
                                    new core::command::FlipFramebufferFinalOperation));
                            CommitTransaction ();
                        }
                        else {
                            AbortTransaction ();
                        }
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATETEXT_0);
                    }

                public:
                    Tool (core::Module &module) : core::Tool (module) {}

                    virtual void SetFocus () {
                        core::CursorMgr::Instance ().SetCursor (core::CursorMgr::CROSS_CURSOR);
                        core::UI::Instance ().consoleWindow->Print (IDS_CREATETEXT_0);
                    }

                    virtual void LButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                            rect = pt;
                            aspect = IsCtrl ();
                            // FIXME: Respect the request to keep aspect.
                            //if (aspect) {
                            //    TEXTMETRIC textMetric;
                            //    GetSurfaceOSB ()->GetTextMetrics (textMetric);
                            //    ratio = ((util::f32)textMetric.tmDigitizedAspectY) / ((util::f32)textMetric.tmDigitizedAspectX);
                            //    oldx = pt.x;
                            //}
                            BeginTransaction ();
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            if (aspect) {
                                rect.max.x += pt.x - oldx;
                                rect.max.y += (oldx - pt.x) * ratio;
                            }
                            else {
                                rect.max = pt;
                            }
                            core::DrawBound2 (view, rect);
                        }
                    }

                    virtual void LButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 1) {
                            UpdateState (2, flags | ScrollLockOff);
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATETEXT_1);
                        }
                        else if (state == 3) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            rect = rect.Normalize ();
                            CreateTextOutlines ();
                        }
                    }

                    virtual void RButtonDown (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 0) {
                            UpdateState (1, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                        }
                        else if (state == 2) {
                            UpdateState (3, flags | ScrollLockOn);
                            core::DrawBound2 (view, rect);
                        }
                    }

                    virtual void RButtonUp (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (IsOddState ()) {
                            UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                            AbortTransaction ();
                            core::UI::Instance ().consoleWindow->Print (IDS_CREATETEXT_0);
                        }
                    }

                    virtual void MouseMove (
                            const _3ds::opengl::View &view,
                            util::ui32 flags,
                            const blas::Point2 &pt) {
                        if (state == 2) {
                            core::DrawBound2 (view, rect);
                            if (aspect) {
                                rect.max.x += pt.x - oldx;
                                rect.max.y += (oldx - pt.x) * ratio;
                                oldx = pt.x;
                            }
                            else {
                                rect.max = pt;
                            }
                            core::DrawBound2 (view, rect);
                        }
                    }

                    virtual void UpdateUI (const _3ds::opengl::View &view) {
                        if (state == 2) {
                            core::DrawBound2 (view, rect);
                        }
                    }

                    virtual void UpdateStatus (core::StatusBar::Item::EventHandler::UI &ui) {
                        if (state == 2) {
                            ui.SetText (IDS_CREATETEXT_2,
                                _3ds::ext::Units (core::GetIOProject ().units).Format (
                                    sqrtf ((rect.max.x - rect.min.x) * (rect.max.x - rect.min.x))).c_str (),
                                _3ds::ext::Units (core::GetIOProject ().units).Format (
                                    sqrtf ((rect.max.y - rect.min.y) * (rect.max.y - rect.min.y))).c_str ());
                        }
                        else {
                            ui.SetText ("");
                        }
                    }
                } tool;

            public:
                explicit SpeedBarCreateTextPlaceEventHandler (core::Module &module) :
                    ToolEventHandler (module, tool), tool (module) {}
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarCreateTextPlaceEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
