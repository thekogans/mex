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

#include "thekogans/util/Flags.h"
#include "thekogans/mex/core/CursorMgr.h"
#include "thekogans/mex/core/Tool.h"
#include "thekogans/mex/core/ViewLayout.h"
#include "thekogans/mex/core/UI.h"
#include "thekogans/mex/core/Module.h"
#include "thekogans/mex/core/Model.h"
#include "thekogans/mex/core/Util.h"
#include "thekogans/mex/core/command/TransactionFactory.h"

namespace thekogans {
    namespace mex {
        namespace core {

            void Tool::KillFocus () {
                UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                AbortTransaction ();
            }

            void Tool::LButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {
                if (IsEvenState ()) {
                    UpdateState (++state, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                }
            }

            void Tool::LButtonUp (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {
                if (IsOddState ()) {
                    UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                }
            }

            void Tool::RButtonDown (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {
                if (IsEvenState ()) {
                    UpdateState (++state, flags | ScrollLockOn | CursorHidden | ViewCaptured);
                }
            }

            void Tool::RButtonUp (
                    const _3ds::opengl::View &view,
                    util::ui32 flags,
                    const blas::Point2 &pt) {
                if (IsOddState ()) {
                    UpdateState (0, flags | ScrollLockOff | CursorVisible | ViewReleased);
                }
            }

            void Tool::UpdateState (util::ui32 state_, util::ui32 stateFlags) {
                state = state_;
                flags = stateFlags & (FLAG_CTRL | FLAG_ALT | FLAG_SHIFT);
                if (util::Flags32 (stateFlags).Test (CursorVisible)) {
                    CursorMgr::Instance ().ShowCursor (true);
                }
                else if (util::Flags32 (stateFlags).Test (CursorHidden)) {
                    CursorMgr::Instance ().ShowCursor (false);
                }
                if (util::Flags32 (stateFlags).Test (ScrollLockOn)) {
                    scrollLock = true;
                }
                else if (util::Flags32 (stateFlags).Test (ScrollLockOff)) {
                    scrollLock = true;
                }
                if (util::Flags32 (stateFlags).Test (ViewCaptured)) {
                    module.viewLayout->CaptureView ();
                }
                else if (util::Flags32 (stateFlags).Test (ViewReleased)) {
                    module.viewLayout->ReleaseView ();
                }
            }

            blas::Bound2 Tool::GetPickBound (
                    const _3ds::opengl::View &view,
                    const blas::Point2 &pt) {
                util::i32 delta = GetIOProject ().programConfiguration.pickBoxSize / 2;
                blas::Point d = view.P2D (pt);
                return blas::Bound2 (
                    view.D2P (blas::Point (d.x - delta, d.y - delta)),
                    view.D2P (blas::Point (d.x + delta, d.y + delta)));
            }

            bool Tool::IsTransactionPending () {
                return GetCommandDispatcher ().IsTransactionPending (GetName ());
            }

            bool Tool::IsPendingTransactionEmpty () const {
                return GetCommandDispatcher ().IsPendingTransactionEmpty ();
            }

            void Tool::BeginTransaction () {
                if (!IsTransactionPending ()) {
                    GetCommandDispatcher ().BeginTransaction (
                        thekogans::mex::command::TransactionFactory::UniquePtr (
                            new command::TransactionFactory (GetName ())));
                }
            }

            void Tool::CheckpointTransaction () {
                if (IsTransactionPending ()) {
                    GetCommandDispatcher ().CheckpointTransaction ();
                }
            }

            void Tool::RollbackTransaction () {
                if (IsTransactionPending ()) {
                    GetCommandDispatcher ().RollbackTransaction ();
                }
            }

            void Tool::CommitTransaction () {
                if (IsTransactionPending ()) {
                    GetCommandDispatcher ().CommitTransaction ();
                }
            }

            void Tool::AbortTransaction () {
                if (IsTransactionPending ()) {
                    GetCommandDispatcher ().AbortTransaction ();
                }
            }

            bool Tool::ExecuteAndAddCommand (
                    thekogans::mex::command::Command::UniquePtr command) {
                return IsTransactionPending () &&
                    GetCommandDispatcher ().ExecuteAndAddCommand (std::move (command));
            }

            bool Tool::ExecuteAndAddFinalOperation (
                    thekogans::mex::command::FinalOperation::UniquePtr finalOperation) {
                return IsTransactionPending () &&
                    GetCommandDispatcher ().ExecuteAndAddFinalOperation (std::move (finalOperation));
            }

        } // namespace core
    } // namespace mex
} // namespace thekogans
