// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_command.
//
// libthekogans_mex_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_command_SimpleTypeCommands_h)
#define __thekogans_mex_command_SimpleTypeCommands_h

#include "thekogans/util/Types.h"
#include "thekogans/util/GUID.h"
#include "thekogans/util/Fraction.h"
#include "thekogans/util/Variant.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/command/Command.h"

namespace thekogans {
    namespace mex {
        namespace command {

            template<typename T>
            struct SimpleTypeSetCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SimpleTypeSetCommand<T>)

            private:
                T &lVal;
                T rVal;

            public:
                SimpleTypeSetCommand (
                    T &lVal_,
                    const T &rVal_) :
                    lVal (lVal_),
                    rVal (rVal_) {}

                virtual bool Execute () {
                    return Do ();
                }
                virtual bool Undo () {
                    return Do ();
                }
                virtual bool Redo () {
                    return Do ();
                }

            private:
                inline bool Do () {
                    std::swap (lVal, rVal);
                    return true;
                }
            };

            typedef SimpleTypeSetCommand<bool> boolSetCommand;
            typedef SimpleTypeSetCommand<util::i8> i8SetCommand;
            typedef SimpleTypeSetCommand<util::ui8> ui8SetCommand;
            typedef SimpleTypeSetCommand<util::i16> i16SetCommand;
            typedef SimpleTypeSetCommand<util::ui16> ui16SetCommand;
            typedef SimpleTypeSetCommand<util::i32> i32SetCommand;
            typedef SimpleTypeSetCommand<util::ui32> ui32SetCommand;
            typedef SimpleTypeSetCommand<util::i64> i64SetCommand;
            typedef SimpleTypeSetCommand<util::ui64> ui64SetCommand;
            typedef SimpleTypeSetCommand<util::f32> f32SetCommand;
            typedef SimpleTypeSetCommand<util::f64> f64SetCommand;
            typedef SimpleTypeSetCommand<util::GUID> GUIDSetCommand;
            typedef SimpleTypeSetCommand<util::Fraction> FractionSetCommand;
            typedef SimpleTypeSetCommand<util::Variant> VariantSetCommand;
            typedef SimpleTypeSetCommand<util::Flags16> Flags16SetCommand;
            typedef SimpleTypeSetCommand<util::Flags32> Flags32SetCommand;
            typedef SimpleTypeSetCommand<util::Flags64> Flags64SetCommand;

            template<typename T>
            struct SimpleTypeSwapCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SimpleTypeSwapCommand<T>)

            private:
                T &t1;
                T &t2;

            public:
                SimpleTypeSwapCommand (
                    T &t1_,
                    T &t2_) :
                    t1 (t1_),
                    t2 (t2_) {}

                virtual bool Execute () {
                    return Do ();
                }
                virtual bool Undo () {
                    return Do ();
                }
                virtual bool Redo () {
                    return Do ();
                }

            private:
                inline bool Do () {
                    std::swap (t1, t2);
                    return true;
                }
            };

            typedef SimpleTypeSwapCommand<bool> boolSwapCommand;
            typedef SimpleTypeSwapCommand<util::i8> i8SwapCommand;
            typedef SimpleTypeSwapCommand<util::ui8> ui8SwapCommand;
            typedef SimpleTypeSwapCommand<util::i16> i16SwapCommand;
            typedef SimpleTypeSwapCommand<util::ui16> ui16SwapCommand;
            typedef SimpleTypeSwapCommand<util::i32> i32SwapCommand;
            typedef SimpleTypeSwapCommand<util::ui32> ui32SwapCommand;
            typedef SimpleTypeSwapCommand<util::i64> i64SwapCommand;
            typedef SimpleTypeSwapCommand<util::ui64> ui64SwapCommand;
            typedef SimpleTypeSwapCommand<util::f32> f32SwapCommand;
            typedef SimpleTypeSwapCommand<util::f64> f64SwapCommand;
            typedef SimpleTypeSwapCommand<util::GUID> GUIDSwapCommand;
            typedef SimpleTypeSwapCommand<util::Fraction> FractionSwapCommand;
            typedef SimpleTypeSwapCommand<util::Variant> VariantSwapCommand;
            typedef SimpleTypeSwapCommand<util::Flags16> Flags16SwapCommand;
            typedef SimpleTypeSwapCommand<util::Flags32> Flags32SwapCommand;
            typedef SimpleTypeSwapCommand<util::Flags64> Flags64SwapCommand;

            template<typename T>
            struct SimpleTypeIncCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SimpleTypeIncCommand<T>)

            private:
                T &lVal;

            public:
                SimpleTypeIncCommand (T &lVal_) :
                    lVal (lVal_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    --lVal;
                    return true;
                }
                virtual bool Redo () {
                    ++lVal;
                    return true;
                }
            };

            typedef SimpleTypeIncCommand<util::i8> i8IncCommand;
            typedef SimpleTypeIncCommand<util::ui8> ui8IncCommand;
            typedef SimpleTypeIncCommand<util::i16> i16IncCommand;
            typedef SimpleTypeIncCommand<util::ui16> ui16IncCommand;
            typedef SimpleTypeIncCommand<util::i32> i32IncCommand;
            typedef SimpleTypeIncCommand<util::ui32> ui32IncCommand;
            typedef SimpleTypeIncCommand<util::i64> i64IncCommand;
            typedef SimpleTypeIncCommand<util::ui64> ui64IncCommand;
            typedef SimpleTypeIncCommand<util::f32> f32IncCommand;
            typedef SimpleTypeIncCommand<util::f64> f64IncCommand;

            template<typename T>
            struct SimpleTypeAddCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SimpleTypeAddCommand<T>)

            private:
                T &lVal;
                T rVal;

            public:
                SimpleTypeAddCommand (
                    T &lVal_,
                    const T &rVal_) :
                    lVal (lVal_),
                    rVal (rVal_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    lVal -= rVal;
                    return true;
                }
                virtual bool Redo () {
                    lVal += rVal;
                    return true;
                }
            };

            typedef SimpleTypeAddCommand<util::i8> i8AddCommand;
            typedef SimpleTypeAddCommand<util::ui8> ui8AddCommand;
            typedef SimpleTypeAddCommand<util::i16> i16AddCommand;
            typedef SimpleTypeAddCommand<util::ui16> ui16AddCommand;
            typedef SimpleTypeAddCommand<util::i32> i32AddCommand;
            typedef SimpleTypeAddCommand<util::ui32> ui32AddCommand;
            typedef SimpleTypeAddCommand<util::i64> i64AddCommand;
            typedef SimpleTypeAddCommand<util::ui64> ui64AddCommand;
            typedef SimpleTypeAddCommand<util::f32> f32AddCommand;
            typedef SimpleTypeAddCommand<util::f64> f64AddCommand;

            template<typename T>
            struct SimpleTypeDecCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SimpleTypeDecCommand<T>)

            private:
                T &lVal;

            public:
                SimpleTypeDecCommand (T &lVal_) :
                    lVal (lVal_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    ++lVal;
                    return true;
                }
                virtual bool Redo () {
                    --lVal;
                    return true;
                }
            };

            typedef SimpleTypeDecCommand<util::i8> i8DecCommand;
            typedef SimpleTypeDecCommand<util::ui8> ui8DecCommand;
            typedef SimpleTypeDecCommand<util::i16> i16DecCommand;
            typedef SimpleTypeDecCommand<util::ui16> ui16DecCommand;
            typedef SimpleTypeDecCommand<util::i32> i32DecCommand;
            typedef SimpleTypeDecCommand<util::ui32> ui32DecCommand;
            typedef SimpleTypeDecCommand<util::i64> i64DecCommand;
            typedef SimpleTypeDecCommand<util::ui64> ui64DecCommand;
            typedef SimpleTypeDecCommand<util::f32> f32DecCommand;
            typedef SimpleTypeDecCommand<util::f64> f64DecCommand;

            template<typename T>
            struct SimpleTypeSubCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (SimpleTypeSubCommand<T>)

            private:
                T &lVal;
                T rVal;

            public:
                SimpleTypeSubCommand (
                    T &lVal_,
                    const T &rVal_) :
                    lVal (lVal_),
                    rVal (rVal_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    lVal += rVal;
                    return true;
                }
                virtual bool Redo () {
                    lVal -= rVal;
                    return true;
                }
            };

            typedef SimpleTypeSubCommand<util::i8> i8SubCommand;
            typedef SimpleTypeSubCommand<util::ui8> ui8SubCommand;
            typedef SimpleTypeSubCommand<util::i16> i16SubCommand;
            typedef SimpleTypeSubCommand<util::ui16> ui16SubCommand;
            typedef SimpleTypeSubCommand<util::i32> i32SubCommand;
            typedef SimpleTypeSubCommand<util::ui32> ui32SubCommand;
            typedef SimpleTypeSubCommand<util::i64> i64SubCommand;
            typedef SimpleTypeSubCommand<util::ui64> ui64SubCommand;
            typedef SimpleTypeSubCommand<util::f32> f32SubCommand;
            typedef SimpleTypeSubCommand<util::f64> f64SubCommand;

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_command_SimpleTypeCommands_h)
