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

#if !defined (__thekogans_mex_command_OwnerVectorCommands_h)
#define __thekogans_mex_command_OwnerVectorCommands_h

#include <algorithm>
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/command/Command.h"

namespace thekogans {
    namespace mex {
        namespace command {

            template<typename T>
            struct OwnerVectorpush_backCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (OwnerVectorpush_backCommand<T>)

            private:
                util::OwnerVector<T> &vector;
                std::unique_ptr<T> value;

            public:
                OwnerVectorpush_backCommand (
                    util::OwnerVector<T> &vector_,
                    std::unique_ptr<T> value_) :
                    vector (vector_),
                    value (std::move (value_)) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    assert (value.get () == 0);
                    value.reset (vector.back ());
                    vector.erase (vector.end () - 1);
                    return true;
                }
                virtual bool Redo () {
                    assert (value.get () != 0);
                    vector.push_back (value.get ());
                    value.release ();
                    return true;
                }
            };

            template<typename T>
            struct OwnerVectorinsertCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (OwnerVectorinsertCommand<T>)

            private:
                util::OwnerVector<T> &vector;
                std::size_t index;
                std::unique_ptr<T> value;

            public:
                OwnerVectorinsertCommand (
                    util::OwnerVector<T> &vector_,
                    std::size_t index_,
                    std::unique_ptr<T> value_) :
                    vector (vector_),
                    index (index_),
                    value (value_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    assert (value.get () == 0);
                    value.reset (vector[index]);
                    vector.erase (vector.begin () + index);
                    return true;
                }
                virtual bool Redo () {
                    assert (value.get () != 0);
                    vector.insert (vector.begin () + index, value.get ());
                    value.release ();
                    return true;
                }
            };

            template<typename T>
            struct OwnerVectorassignCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (OwnerVectorassignCommand<T>)

            private:
                util::OwnerVector<T> &vector;
                std::size_t index;
                std::unique_ptr<T> value;

            public:
                OwnerVectorassignCommand (
                    util::OwnerVector<T> &vector_,
                    std::size_t index_,
                    std::unique_ptr<T> value_) :
                    vector (vector_),
                    index (index_),
                    value (std::move (value_)) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    return Redo ();
                }
                virtual bool Redo () {
                    assert (value.get () != 0);
                    T *temp = vector[index];
                    vector[index] = value.release ();
                    value.reset (temp);
                    return true;
                }
            };

            template<typename T>
            struct OwnerVectoreraseCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (OwnerVectoreraseCommand<T>)

            private:
                util::OwnerVector<T> &vector;
                std::size_t index;
                std::unique_ptr<T> value;

            public:
                OwnerVectoreraseCommand (
                    util::OwnerVector<T> &vector_,
                    std::size_t index_) :
                    vector (vector_),
                    index (index_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    assert (value.get () != 0);
                    vector.insert (vector.begin () + index, value.get ());
                    value.release ();
                    return true;
                }
                virtual bool Redo () {
                    assert (value.get () == 0);
                    value.reset (vector[index]);
                    vector.erase (vector.begin () + index);
                    return true;
                }
            };

            template<typename T>
            struct OwnerVectorclearCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (OwnerVectorclearCommand<T>)

            private:
                util::OwnerVector<T> &vector;
                util::OwnerVector<T> savedVector;

            public:
                OwnerVectorclearCommand (util::OwnerVector<T> &vector_) :
                    vector (vector_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    std::swap (vector, savedVector);
                    return true;
                }
                virtual bool Redo () {
                    std::swap (vector, savedVector);
                    return true;
                }
            };

            template<typename T, typename Compare>
            struct OwnerVectorsortCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (OwnerVectorsortCommand)

            private:
                util::OwnerVector<T> &vector;
                Compare compare;
                std::vector<T *> newOrder;

            public:
                OwnerVectorsortCommand (
                        util::OwnerVector<T> &vector_,
                        Compare compare_) :
                        vector (vector_),
                        compare (compare_),
                        newOrder (vector.begin (), vector.end ()) {
                    std::sort (newOrder.begin (), newOrder.end (), compare);
                }

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    std::swap (vector, newOrder);
                    return true;
                }
                virtual bool Redo () {
                    std::swap (vector, newOrder);
                    return true;
                }
            };

        } // namespace command
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_command_OwnerVectorCommands_h)
