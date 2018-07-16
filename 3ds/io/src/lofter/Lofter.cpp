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

#include <memory>
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void Lofter::Swap (Lofter &lofter) {
                    Module::Swap (lofter);
                    tape.Swap (lofter.tape);
                    std::swap (show3D, lofter.show3D);
                    surfaceOfRevolution.Swap (lofter.surfaceOfRevolution);
                    helix.Swap (lofter.helix);
                    modelParams.Swap (lofter.modelParams);
                    std::swap (masterScale, lofter.masterScale);
                    std::swap (masterShapeSteps, lofter.masterShapeSteps);
                    path.Swap (lofter.path);
                    shapes.swap (lofter.shapes);
                    scaleDeformation.Swap (lofter.scaleDeformation);
                    twistDeformation.Swap (lofter.twistDeformation);
                    teeterDeformation.Swap (lofter.teeterDeformation);
                    bevelDeformation.Swap (lofter.bevelDeformation);
                    fitDeformation.Swap (lofter.fitDeformation);
                    std::swap (tween, lofter.tween);
                    std::swap (contour, lofter.contour);
                    std::swap (scale, lofter.scale);
                    std::swap (twist, lofter.twist);
                    std::swap (teeter, lofter.teeter);
                    std::swap (bevel, lofter.bevel);
                    std::swap (fit, lofter.fit);
                }

                void Lofter::ReadLMAGIC (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case MASTER_SCALE:
                                chunk >> masterScale;
                                break;
                            case INTERPCT:
                                chunk >> masterShapeSteps;
                                break;
                            case VIEWPORT_LAYOUT:
                                ReadVIEWPORT_LAYOUT (chunk);
                                break;
                            case PATH_3D:
                                ReadPATH_3D (chunk);
                                break;
                            case SHAPE_2D: {
                                Shape::UniquePtr shape (new Shape);
                                ReadSHAPE_2D (*shape, chunk);
                                shapes.push_back (shape.get ());
                                shape.release ();
                                break;
                            }
                            case M_SCALE:
                                ReadM_SCALE (chunk);
                                break;
                            case M_TWIST:
                                ReadM_TWIST (chunk);
                                break;
                            case M_TEETER:
                                ReadM_TEETER (chunk);
                                break;
                            case M_BEVEL:
                                ReadM_BEVEL (chunk);
                                break;
                            case M_FIT:
                                ReadM_FIT (chunk);
                                break;
                            case USE_TWEEN:
                                tween = true;
                                break;
                            case USE_CONTOUR:
                                contour = true;
                                break;
                            case USE_SCALE:
                                scale = true;
                                break;
                            case USE_TWIST:
                                twist = true;
                                break;
                            case USE_TEETER:
                                teeter = true;
                                break;
                            case USE_BEVEL:
                                bevel = true;
                                break;
                            case USE_FIT:
                                fit = true;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadLMAGIC");
                                break;
                        }
                    }
                }

                void Lofter::WriteLMAGIC (util::File &file) const {
                    ChunkWriter chunk (file, LMAGIC);
                    WriteMASTER_SCALE (masterScale, file);
                    WriteINTERPCT (masterShapeSteps, file);
                    WriteVIEWPORT_LAYOUT (file);
                    WritePATH_3D (path, file);
                    for (size_t i = 0, count = shapes.size (); i < count; ++i) {
                         WriteSHAPE_2D (*shapes[i], file);
                    }
                    WriteM_SCALE (scaleDeformation, file);
                    WriteM_TWIST (twistDeformation, file);
                    WriteM_TEETER (teeterDeformation, file);
                    WriteM_BEVEL (bevelDeformation, file);
                    WriteM_FIT (fitDeformation, file);
                    if (tween) {
                        WriteUSE_TWEEN (file);
                    }
                    if (contour) {
                        WriteUSE_CONTOUR (file);
                    }
                    if (scale) {
                        WriteUSE_SCALE (file);
                    }
                    if (twist) {
                        WriteUSE_TWIST (file);
                    }
                    if (teeter) {
                        WriteUSE_TEETER (file);
                    }
                    if (bevel) {
                        WriteUSE_BEVEL (file);
                    }
                    if (fit) {
                        WriteUSE_FIT (file);
                    }
                }

                void Lofter::ReadPATH_3D (ChunkReader &mainChunk) {
                    mainChunk >> path.steps >> path.polygon;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case PATH_MATRIX:
                                chunk >> path.transform.x >> path.transform.y >> path.transform.z;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadPATH_3D");
                                break;
                        }
                    }
                }

                void Lofter::ReadSHAPE_2D (
                        Shape &shape,
                        ChunkReader &mainChunk) {
                    mainChunk >> shape.level >> shape.polygons;
                }

                void Lofter::ReadM_SCALE (ChunkReader &mainChunk) {
                    mainChunk >> scaleDeformation.symmetry;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case DEFORM_LIMIT:
                                chunk >> scaleDeformation.limit;
                                break;
                            case XZ_CURVE:
                                ReadDeformationCurve (scaleDeformation.xzCurve, chunk);
                                break;
                            case YZ_CURVE:
                                ReadDeformationCurve (scaleDeformation.yzCurve, chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadM_SCALE");
                                break;
                        }
                    }
                }

                void Lofter::ReadM_TWIST (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case DEFORM_LIMIT:
                                chunk >> twistDeformation.limit;
                                break;
                            case XZ_CURVE:
                                ReadDeformationCurve (twistDeformation.xzCurve, chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadM_TWIST");
                                break;
                        }
                    }
                }

                void Lofter::ReadM_TEETER (ChunkReader &mainChunk) {
                    mainChunk >> teeterDeformation.symmetry;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case DEFORM_LIMIT:
                                chunk >> teeterDeformation.limit;
                                break;
                            case XZ_CURVE:
                                ReadDeformationCurve (teeterDeformation.xzCurve, chunk);
                                break;
                            case YZ_CURVE:
                                ReadDeformationCurve (teeterDeformation.yzCurve, chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadM_TEETER");
                                break;
                        }
                    }
                }

                void Lofter::ReadM_BEVEL (ChunkReader &mainChunk) {
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case DEFORM_LIMIT:
                                chunk >> bevelDeformation.limit;
                                break;
                            case XZ_CURVE:
                                ReadDeformationCurve (bevelDeformation.xzCurve, chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadM_BEVEL");
                                break;
                        }
                    }
                }

                void Lofter::ReadM_FIT (ChunkReader &mainChunk) {
                    mainChunk >> fitDeformation.symmetry;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case XZ_CURVE:
                                ReadDeformationCurve (fitDeformation.xzCurve, chunk);
                                break;
                            case YZ_CURVE:
                                ReadDeformationCurve (fitDeformation.yzCurve, chunk);
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadM_FIT");
                                break;
                        }
                    }
                }

                void Lofter::ReadDeformationCurve (
                        Deformation::Curve &curve,
                        ChunkReader &mainChunk) {
                    util::ui16 vertexCount;
                    mainChunk >> vertexCount;
                    curve.bezierPolygon.vertices.resize (vertexCount);
                    for (util::ui16 i = 0; i < vertexCount; ++i) {
                        mainChunk >>
                            curve.bezierPolygon.vertices[i].pt >>
                            curve.bezierPolygon.vertices[i].red >>
                            curve.bezierPolygon.vertices[i].yellow;
                    }

                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case INTERPCT:
                                chunk >> curve.steps;
                                break;
                            default:
                                chunk.HandleUknownChunk ("Lofter", "ReadDeformationCurve");
                                break;
                        }
                    }
                }

                void Lofter::WriteMASTER_SCALE (
                        util::f32 masterScale,
                        util::File &file) const {
                    ChunkWriter chunk (file, MASTER_SCALE);
                    chunk << masterScale;
                }

                void Lofter::WriteINTERPCT (
                        util::ui16 steps,
                        util::File &file) const {
                    ChunkWriter chunk (file, INTERPCT);
                    chunk << steps;
                }

                void Lofter::WritePATH_3D (
                        const Path &path,
                        util::File &file) const {
                    ChunkWriter chunk (file, PATH_3D);
                    chunk << path.steps << path.polygon;
                    WritePATH_MATRIX (path.transform, file);
                }

                void Lofter::WritePATH_MATRIX (
                        const Path::Transform &transform,
                        util::File &file) const {
                    ChunkWriter chunk (file, PATH_MATRIX);
                    chunk << transform.x << transform.y << transform.z;
                }

                void Lofter::WriteSHAPE_2D (
                        const Shape &shape,
                        util::File &file) const {
                    ChunkWriter chunk (file, SHAPE_2D);
                    chunk << shape.level << shape.polygons;
                }

                void Lofter::WriteM_SCALE (
                        const ScaleDeformation &scaleDeformation,
                        util::File &file) const {
                    ChunkWriter chunk (file, M_SCALE);
                    chunk << scaleDeformation.symmetry;
                    WriteDEFORM_LIMIT (scaleDeformation.limit, file);
                    WriteDeformationCurve (XZ_CURVE, scaleDeformation.xzCurve, file);
                    WriteDeformationCurve (YZ_CURVE, scaleDeformation.yzCurve, file);
                }

                void Lofter::WriteM_TWIST (
                        const TwistDeformation &twistDeformation,
                        util::File &file) const {
                    ChunkWriter chunk (file, M_TWIST);
                    WriteDEFORM_LIMIT (twistDeformation.limit, file);
                    WriteDeformationCurve (XZ_CURVE, twistDeformation.xzCurve, file);
                }

                void Lofter::WriteM_TEETER (
                        const TeeterDeformation &teeterDeformation,
                        util::File &file) const {
                    ChunkWriter chunk (file, M_TEETER);
                    chunk << teeterDeformation.symmetry;
                    WriteDEFORM_LIMIT (teeterDeformation.limit, file);
                    WriteDeformationCurve (XZ_CURVE, teeterDeformation.xzCurve, file);
                    WriteDeformationCurve (YZ_CURVE, teeterDeformation.yzCurve, file);
                }

                void Lofter::WriteM_BEVEL (
                        const BevelDeformation &bevelDeformation,
                        util::File &file) const {
                    ChunkWriter chunk (file, M_BEVEL);
                    WriteDEFORM_LIMIT (bevelDeformation.limit, file);
                    WriteDeformationCurve (XZ_CURVE, bevelDeformation.xzCurve, file);
                }

                void Lofter::WriteM_FIT (
                        const FitDeformation &fitDeformation,
                        util::File &file) const {
                    ChunkWriter chunk (file, M_FIT);
                    chunk << fitDeformation.symmetry;
                    WriteDeformationCurve (XZ_CURVE, fitDeformation.xzCurve, file);
                    WriteDeformationCurve (YZ_CURVE, fitDeformation.yzCurve, file);
                }

                void Lofter::WriteDEFORM_LIMIT (
                        util::f32 limit,
                        util::File &file) const {
                    ChunkWriter chunk (file, DEFORM_LIMIT);
                    chunk << limit;
                }

                void Lofter::WriteDeformationCurve (
                        util::ui16 id,
                        const Deformation::Curve &curve,
                        util::File &file) const {
                    ChunkWriter chunk (file, id);
                    util::ui16 vertexCount = (util::ui16)curve.bezierPolygon.vertices.size ();
                    chunk << vertexCount;
                    for (util::ui16 i = 0; i < vertexCount; ++i) {
                        chunk <<
                            curve.bezierPolygon.vertices[i].pt <<
                            curve.bezierPolygon.vertices[i].red <<
                            curve.bezierPolygon.vertices[i].yellow;
                    }
                    WriteINTERPCT (curve.steps, file);
                }

                void Lofter::WriteUSE_TWEEN (util::File &file) const {
                    ChunkWriter chunk (file, USE_TWEEN);
                }

                void Lofter::WriteUSE_CONTOUR (util::File &file) const {
                    ChunkWriter chunk (file, USE_CONTOUR);
                }

                void Lofter::WriteUSE_SCALE (util::File &file) const {
                    ChunkWriter chunk (file, USE_SCALE);
                }

                void Lofter::WriteUSE_TWIST (util::File &file) const {
                    ChunkWriter chunk (file, USE_TWIST);
                }

                void Lofter::WriteUSE_TEETER (util::File &file) const {
                    ChunkWriter chunk (file, USE_TEETER);
                }

                void Lofter::WriteUSE_BEVEL (util::File &file) const {
                    ChunkWriter chunk (file, USE_BEVEL);
                }

                void Lofter::WriteUSE_FIT (util::File &file) const {
                    ChunkWriter chunk (file, USE_FIT);
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
