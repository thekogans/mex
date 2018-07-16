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

#include "thekogans/mex/blas/BezierCubic.h"
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/shaper/Shaper.h"
#include "thekogans/mex/3ds/io/Project.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void Shaper::Swap (Shaper &shaper) {
                    Module::Swap (shaper);
                    tape.Swap (shaper.tape);
                    axis.Swap (shaper.axis);
                    std::swap (steps, shaper.steps);
                    std::swap (showFirst, shaper.showFirst);
                    create.Swap (shaper.create);
                    std::swap (show3D, shaper.show3D);
                    meshes.swap (shaper.meshes);
                    std::swap (selectMask, shaper.selectMask);
                    std::swap (unselectMask, shaper.unselectMask);
                    std::swap (masterScale, shaper.masterScale);
                    hook.Swap (shaper.hook);
                    std::swap (shapeOk, shaper.shapeOk);
                    polygons3.swap (shaper.polygons3);
                    polygons2.swap (shaper.polygons2);
                }

                void Shaper::ReadSMAGIC (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MASTER_SCALE:
                                chunk >> masterScale;
                                break;
                            case VIEWPORT_LAYOUT:
                                ReadVIEWPORT_LAYOUT (chunk);
                                break;
                            case SHAPE_HOOK:
                                mainChunk >> hook.pt;
                                break;
                            case SHAPE_OK:
                                shapeOk = true;
                                break;
                            case SHAPE_NOT_OK:
                                shapeOk = false;
                                break;
                            case POLY_2D: {
                                BezierPolygon3::UniquePtr polygon (new BezierPolygon3);
                                mainChunk >> *polygon;
                                polygons3.push_back (polygon.get ());
                                polygon.release ();
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Shaper", "ReadSMAGIC");
                                break;
                        }
                    }
                }

                void Shaper::WriteSMAGIC (util::File &file) const {
                    ChunkWriter chunk (file, SMAGIC);
                    WriteMASTER_SCALE (masterScale, file);
                    WriteVIEWPORT_LAYOUT (file);
                    WriteSHAPE_HOOK (hook.pt, file);
                    if (shapeOk) {
                        WriteSHAPE_OK (file);
                    }
                    else {
                        WriteSHAPE_NOT_OK (file);
                    }
                    for (std::size_t i = 0, count = polygons3.size (); i < count; ++i) {
                        WritePOLY_2D (*polygons3[i], file);
                    }
                }

                void Shaper::WriteMASTER_SCALE (
                        util::f32 masterScale,
                        util::File &file) const {
                    ChunkWriter chunk (file, MASTER_SCALE);
                    chunk << masterScale;
                }

                void Shaper::WriteSHAPE_HOOK (
                        const blas::Point2 &pt,
                        util::File &file) const {
                    ChunkWriter chunk (file, SHAPE_HOOK);
                    chunk << pt;
                }

                void Shaper::WriteSHAPE_OK (util::File &file) const {
                    ChunkWriter chunk (file, SHAPE_OK);
                }

                void Shaper::WriteSHAPE_NOT_OK (util::File &file) const {
                    ChunkWriter chunk (file, SHAPE_NOT_OK);
                }

                void Shaper::WritePOLY_2D (
                        const BezierPolygon3 &polygon,
                        util::File &file) const {
                    ChunkWriter chunk (file, POLY_2D);
                    chunk << polygon;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
