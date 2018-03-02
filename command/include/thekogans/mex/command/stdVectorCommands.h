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

#if !defined (__thekogans_mex_command_stdVectorCommands_h)
#define __thekogans_mex_command_stdVectorCommands_h

#include <vector>
#include <algorithm>
#include "thekogans/util/Heap.h"
#include "thekogans/mex/command/Command.h"

namespace thekogans {
    namespace mex {
        namespace command {

            template<typename T>
            struct stdVectorpush_backCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (stdVectorpush_backCommand<T>)

            private:
                std::vector<T> &vector;
                T value;

            public:
                stdVectorpush_backCommand (
                    std::vector<T> &vector_,
                    const T &value_) :
                    vector (vector_),
                    value (value_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    vector.erase (vector.end () - 1);
                    return true;
                }
                virtual bool Redo () {
                    vector.push_back (value);
                    return true;
                }
            };

            template<typename T>
            struct stdVectorinsertCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (stdVectorinsertCommand<T>)

            private:
                std::vector<T> &vector;
                std::size_t index;
                T value;

            public:
                stdVectorinsertCommand (
                    std::vector<T> &vector_,
                    std::size_t index_,
                    const T &value_) :
                    vector (vector_),
                    index (index_),
                    value (value_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    vector.erase (vector.begin () + index);
                    return true;
                }
                virtual bool Redo () {
                    vector.insert (vector.begin () + index, value);
                    return true;
                }
            };

            template<typename T>
            struct stdVectorassignCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (stdVectorassignCommand<T>)

            private:
                std::vector<T> &vector;
                std::size_t index;
                T value;

            public:
                stdVectorassignCommand (
                    std::vector<T> &vector_,
                    std::size_t index_,
                    const T &value_) :
                    vector (vector_),
                    index (index_),
                    value (value_) {}

                virtual bool Execute () {
                    return Redo ();
                }
                virtual bool Undo () {
                    return Redo ();
                }
                virtual bool Redo () {
                    std::swap (vector[index], value);
                    return true;
                }
            };

            template<typename T>
            struct stdVectoreraseCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (stdVectoreraseCommand<T>)

            private:
                std::vector<T> &vector;
                std::size_t index;
                T value;

            public:
                stdVectoreraseCommand (
                    std::vector<T> &vector_,
                    std::size_t index_) :
                    vector (vector_),
                    index (index_) {}

                virtual bool Execute () {
                    value = vector[index];
                    return Redo ();
                }
                virtual bool Undo () {
                    vector.insert (vector.begin () + index, value);
                    return true;
                }
                virtual bool Redo () {
                    vector.erase (vector.begin () + index);
                    return true;
                }
            };

            template<typename T>
            struct stdVectorclearCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (stdVectorclearCommand<T>)

            private:
                std::vector<T> &vector;
                std::vector<T> savedVector;

            public:
                stdVectorclearCommand (std::vector<T> &vector_) :
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
            struct stdVectorsortCommand : public Command {
                THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (stdVectorsortCommand)

            private:
                std::vector<T> &vector;
                Compare compare;
                std::vector<T> newOrder;

            public:
                stdVectorsortCommand (
                    std::vector<T> &vector_,
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

#endif // !defined (__thekogans_mex_command_stdVectorCommands_h)
