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

#if !defined (__thekogans_mex_3ds_io_MaterialsEditor_h)
#define __thekogans_mex_3ds_io_MaterialsEditor_h

#include <string>
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/Material.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL MaterialsEditor {
                    std::string materialsPath;
                    std::string materialFile;
                    util::OwnerVector<Material> workMaterials;
                    util::OwnerVector<Material> materials;

                    inline void Swap (MaterialsEditor &materialsEditor) {
                        std::swap (materialsPath, materialsEditor.materialsPath);
                        std::swap (materialFile, materialsEditor.materialFile);
                        workMaterials.swap (materialsEditor.workMaterials);
                        materials.swap (materialsEditor.materials);
                    }

                    void Validate ();

                    void ReadMLIBMAGIC (ChunkReader &mainChunk);
                    static void ReadMAT_ENTRY (Material &material, ChunkReader &mainChunk);

                    void WriteMLIBMAGIC (util::File &stream) const;
                    static void WriteMAT_ENTRY (const Material &material, util::File &stream);
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_MaterialsEditor_h)
