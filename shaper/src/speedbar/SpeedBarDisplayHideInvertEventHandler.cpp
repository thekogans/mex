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

#include <cassert>
#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/command/shaper/BezierPolygonCommands.h"
#include "thekogans/mex/core/SpeedBar.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/command/TransactionFactory.h"
#include "thekogans/mex/core/command/FlipFramebufferFinalOperation.h"
#include "thekogans/mex/shaper/command/DrawPolygonCommand.h"
#include "thekogans/mex/shaper/command/ErasePolygonCommand.h"

namespace thekogans {
    namespace mex {
        namespace shaper {

            struct SpeedBarDisplayHideInvertEventHandler : public core::SpeedBar::Item::CommandEventHandler {
                THEKOGANS_MEX_CORE_DECLARE_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplayHideInvertEventHandler)

            private:
                core::Module &module;

            public:
                explicit SpeedBarDisplayHideInvertEventHandler (core::Module &module_) :
                    module (module_) {}

                virtual void OnSetFocus () {
                    core::GetCommandDispatcher ().BeginTransaction (
                        command::TransactionFactory::UniquePtr (
                            new core::command::TransactionFactory ("SpeedBarDisplayHideInvertEventHandler")));
                    for (std::size_t i = 0, count = core::GetIOProject ().shaper.polygons2.size (); i < count; ++i) {
                        _3ds::io::BezierPolygon2 *bezierPolygon = core::GetIOProject ().shaper.polygons2[i];
                        assert (bezierPolygon != 0);
                        if (!_3ds::ext::BezierPolygon2 (*bezierPolygon).IsHidden ()) {
                            if (!_3ds::ext::BezierPolygon2 (*bezierPolygon).IsFrozen ()) {
                                // FIXME: We need to check the return code.
                                for (util::ui32 j = 0, viewCount = core::GetIOProject ().shaper.viewLayout.GetViewCount (); j < viewCount; ++j) {
                                    core::GetCommandDispatcher ().ExecuteAndAddCommand (
                                        command::Command::UniquePtr (
                                            new ErasePolygonCommand (
                                                core::GetIOProject ().shaper.viewLayout[j], *bezierPolygon)));
                                }
                                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                                    command::Command::UniquePtr (
                                        new _3ds::io::command::BezierPolygon2SetFlagsCommand (
                                            *bezierPolygon, _3ds::io::BezierPolygon2::Vertex::Hidden)));
                            }
                        }
                        else {
                            assert (!_3ds::ext::BezierPolygon2 (*bezierPolygon).IsFrozen ());
                            // FIXME: We need to check the return code.
                            core::GetCommandDispatcher ().ExecuteAndAddCommand (
                                command::Command::UniquePtr (
                                    new _3ds::io::command::BezierPolygon2ClearFlagsCommand (
                                        *bezierPolygon, _3ds::io::BezierPolygon2::Vertex::Hidden)));
                            for (util::ui32 j = 0, viewCount = core::GetIOProject ().shaper.viewLayout.GetViewCount (); j < viewCount; ++j) {
                                core::GetCommandDispatcher ().ExecuteAndAddCommand (
                                    command::Command::UniquePtr (
                                        new DrawPolygonCommand (
                                            core::GetIOProject ().shaper.viewLayout[j], *bezierPolygon)));
                            }
                        }
                    }
                    if (!core::GetCommandDispatcher ().IsPendingTransactionEmpty ()) {
                        core::GetCommandDispatcher ().ExecuteAndAddFinalOperation (
                            command::FinalOperation::UniquePtr (
                                new core::command::FlipFramebufferFinalOperation));
                        core::GetCommandDispatcher ().CommitTransaction ();
                    }
                    else {
                        core::GetCommandDispatcher ().AbortTransaction ();
                    }
                }
            };

            THEKOGANS_MEX_CORE_IMPLEMENT_SPEEDBAR_ITEM_EVENT_HANDLER (SpeedBarDisplayHideInvertEventHandler, Shaper)

        } // namespace shaper
    } // namespace mex
} // namespace thekogans
