// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io.
//
// libthekogans_mex_3ds_io is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_Module_h)
#define __thekogans_mex_3ds_io_Module_h

#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct Project;

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Module {
                    Project &project;
                    std::string openFileDirectory;
                    std::string openFileName;
                    ViewLayout viewLayout;

                    explicit Module (Project &project_) :
                        project (project_) {}
                    virtual ~Module () {}

                    void Swap (Module &module);

                  protected:
                    Module (const Module &);
                    Module &operator = (const Module &);

                    void ReadVIEWPORT_LAYOUT (ChunkReader &mainChunk);

                    void WriteVIEWPORT_LAYOUT (util::File &stream) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_Module_h)
