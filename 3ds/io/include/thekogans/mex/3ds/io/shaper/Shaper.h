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

#if !defined (__thekogans_mex_3ds_io_shaper_Shaper_h)
#define __thekogans_mex_3ds_io_shaper_Shaper_h

#include <vector>
#include <string>
#if defined (TOOLCHAIN_OS_Windows)
    #if !defined (_WINDOWS_)
        #if !defined (WIN32_LEAN_AND_MEAN)
            #define WIN32_LEAN_AND_MEAN
        #endif // !defined (WIN32_LEAN_AND_MEAN)
        #if !defined (NOMINMAX)
            #define NOMINMAX
        #endif // !defined (NOMINMAX)
        #include <windows.h>
    #endif // !defined (_WINDOWS_)
#endif // defined (TOOLCHAIN_OS_Windows)
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/Tape.h"
#include "thekogans/mex/3ds/io/Axis.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Shaper : public Module {
                    Tape2 tape;
                    Axis2 axis;
                    util::ui16 steps;
                    bool showFirst;
                    struct Create {
                        util::ui16 freehandSmoothness;
                        util::ui16 ngonVertices;
                        util::ui16 star2Vertices;
                        std::string fontsPath;
                        std::string fontFile;
                        std::string text;

                        Create () :
                            freehandSmoothness (49),
                            ngonVertices (4),
                            star2Vertices (8) {}

                        inline void Swap (Create &create) {
                            std::swap (freehandSmoothness, create.freehandSmoothness);
                            std::swap (ngonVertices, create.ngonVertices);
                            std::swap (star2Vertices, create.star2Vertices);
                            std::swap (fontsPath, create.fontsPath);
                            std::swap (fontFile, create.fontFile);
                            std::swap (text, create.text);
                        }
                    } create;
                    bool show3D;
                    std::vector<std::string> meshes;
                    util::ui16 selectMask;
                    util::ui16 unselectMask;
                    util::f32 masterScale;
                    struct Hook {
                        bool visible;
                        blas::Point2 pt;

                        Hook () :
                            visible (false),
                            pt (blas::Point2::Zero) {}

                        inline void Swap (Hook &hook) {
                            std::swap (visible, hook.visible);
                            std::swap (pt, hook.pt);
                        }
                    } hook;
                    bool shapeOk;
                    // I have thought long, and hard about this hack,
                    // and in the end decided it was infinitelly better
                    // then trying to keep the io data pristine.
                    // Here is what's happening: Shaper is, inherently, a
                    // 2D module. Yet, in their infinite wisdom, the original
                    // developers decided to store polygons as 3D entities.
                    // The z coord is not used anywhere, and some exporters even
                    // write it out incorrectly. All this leads to some very
                    // complicated problems with down stream algorithms. I have
                    // decided to maintain two sets of polygons. The original
                    // (read from the file) will be stored in polygons3, and a
                    // 2D copy (minus z coord) will be stored in polygons2.
                    // io::command::ShaperValidateAfterOpenCommand and
                    // io::command::ShaperValidateBeforeSaveCommand maintain
                    // polygons3, and polygons2 automatically, reducing the
                    // need for manual intervention.
                    util::OwnerVector<BezierPolygon3> polygons3;
                    util::OwnerVector<BezierPolygon2> polygons2;

                    explicit Shaper (Project &project) :
                        Module (project),
                        steps (5),
                        showFirst (false),
                        show3D (false),
                        selectMask (0x0010),
                        unselectMask (0xffef),
                        masterScale (1.0f),
                        shapeOk (true) {}

                    void Swap (Shaper &shaper);

                    void ReadSMAGIC (ChunkReader &mainChunk);
                    void WriteSMAGIC (util::File &stream) const;

                private:
                    Shaper (const Shaper &);
                    Shaper &operator = (const Shaper &);

                    void WriteMASTER_SCALE (util::f32 masterScale, util::File &stream) const;
                    void WriteSHAPE_HOOK (const blas::Point2 &hook, util::File &stream) const;
                    void WriteSHAPE_OK (util::File &stream) const;
                    void WriteSHAPE_NOT_OK (util::File &stream) const;
                    void WritePOLY_2D (const BezierPolygon3 &polygon, util::File &stream) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_shaper_Shaper_h)
