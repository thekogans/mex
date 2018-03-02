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

#if !defined (__thekogans_mex_3ds_io_lofter_Lofter_h)
#define __thekogans_mex_3ds_io_lofter_Lofter_h

#include <memory>
#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/File.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/Tape.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/shaper/BezierPolygon.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Lofter : public Module {
                    Tape3 tape;
                    bool show3D;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL SurfaceOfRevolution {
                        util::f32 diameter;
                        util::f32 degrees;
                        util::ui16 vertices;
                        enum {
                            CW,
                            CCW
                        };
                        util::ui16 direction;

                        SurfaceOfRevolution () :
                            diameter (100.0f),
                            degrees (360.0f),
                            vertices (10),
                            direction (CCW) {}

                        inline void Swap (SurfaceOfRevolution &surfaceOfRevolution) {
                            std::swap (diameter, surfaceOfRevolution.diameter);
                            std::swap (degrees, surfaceOfRevolution.degrees);
                            std::swap (vertices, surfaceOfRevolution.vertices);
                            std::swap (direction, surfaceOfRevolution.direction);
                        }
                    } surfaceOfRevolution;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Helix {
                        util::f32 startDiameter;
                        util::f32 endDiameter;
                        util::f32 height;
                        util::f32 turns;
                        util::f32 degrees;
                        util::ui16 vertices;
                        enum {
                            CW,
                            CCW
                        };
                        util::ui16 direction;

                        Helix () :
                            startDiameter (100.0f),
                            endDiameter (100.0f),
                            height (200.0f),
                            turns (1.0f),
                            degrees (0.0f),
                            vertices (20),
                            direction (CCW) {}

                        inline void Swap (Helix &helix) {
                            std::swap (startDiameter, helix.startDiameter);
                            std::swap (endDiameter, helix.endDiameter);
                            std::swap (height, helix.height);
                            std::swap (turns, helix.turns);
                            std::swap (degrees, helix.degrees);
                            std::swap (vertices, helix.vertices);
                            std::swap (direction, helix.direction);
                        }
                    } helix;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL ModelParams {
                        bool capStart;
                        bool capEnd;
                        bool smoothLength;
                        bool smoothWidth;
                        struct Mapping {
                            bool use;
                            util::f32 length;
                            bool normalizeLength;
                            util::f32 perimeter;

                            Mapping () :
                                use (false),
                                length (1.0f),
                                normalizeLength (true),
                                perimeter (1.0f) {}

                            inline void Swap (Mapping &mapping) {
                                std::swap (use, mapping.use);
                                std::swap (length, mapping.length);
                                std::swap (normalizeLength, mapping.normalizeLength);
                                std::swap (perimeter, mapping.perimeter);
                            }
                        } mapping;
                        bool optimize;
                        bool weldVertices;
                        enum {
                            Low,
                            Medium,
                            High
                        };
                        util::ui16 pathDetail;
                        util::ui16 shapeDetail;

                        ModelParams () :
                            capStart (true),
                            capEnd (true),
                            smoothLength (true),
                            smoothWidth (true),
                            optimize (true),
                            weldVertices (false),
                            pathDetail (High),
                            shapeDetail (High) {}

                        inline void Swap (ModelParams &modelParams) {
                            std::swap (capStart, modelParams.capStart);
                            std::swap (capEnd, modelParams.capEnd);
                            std::swap (smoothLength, modelParams.smoothLength);
                            std::swap (smoothWidth, modelParams.smoothWidth);
                            mapping.Swap (modelParams.mapping);
                            std::swap (optimize, modelParams.optimize);
                            std::swap (weldVertices, modelParams.weldVertices);
                            std::swap (pathDetail, modelParams.pathDetail);
                            std::swap (shapeDetail, modelParams.shapeDetail);
                        }
                    } modelParams;
                    util::f32 masterScale;
                    util::ui16 masterShapeSteps;
                    struct Path {
                        util::ui16 steps;
                        util::ui16 level;
                        BezierPolygon3 polygon;
                        struct Transform {
                            blas::Point3 x;
                            blas::Point3 y;
                            blas::Point3 z;

                            Transform () :
                                x (blas::Point3::X),
                                y (blas::Point3::Y),
                                z (blas::Point3::Z) {}

                            inline void Swap (Transform &transform) {
                                std::swap (x, transform.x);
                                std::swap (y, transform.y);
                                std::swap (z, transform.z);
                            }
                        } transform;

                        Path () :
                            steps (5),
                            level (0) {}

                        inline void Swap (Path &path) {
                            std::swap (steps, path.steps);
                            std::swap (level, path.level);
                            polygon.Swap (path.polygon);
                            transform.Swap (path.transform);
                        }
                    } path;
                    struct Shape {
                        typedef std::unique_ptr<Shape> UniquePtr;

                        util::ui16 level;
                        BezierPolygon3 polygons;

                        Shape () :
                            level (0) {}

                        inline void Swap (Shape &shape) {
                            std::swap (level, shape.level);
                            polygons.Swap (shape.polygons);
                        }
                    };
                    util::OwnerVector<Shape> shapes;
                    struct Deformation {
                        struct Curve {
                            BezierPolygon2 bezierPolygon;
                            util::ui16 steps;

                            Curve () :
                                steps (5) {}

                            inline void Swap (Curve &curve) {
                                bezierPolygon.Swap (curve.bezierPolygon);
                                std::swap (steps, curve.steps);
                            }
                        };
                        Curve xzCurve;
                        Curve yzCurve;

                        inline void Swap (Deformation &deformation) {
                            xzCurve.Swap (deformation.xzCurve);
                            yzCurve.Swap (deformation.yzCurve);
                        }
                    };
                    struct ScaleDeformation : public Deformation {
                        util::ui16 symmetry;
                        util::f32 limit;

                        ScaleDeformation () :
                            symmetry (1),
                            limit (400.0f) {}

                        inline void Swap (ScaleDeformation &scaleDeformation) {
                            Deformation::Swap (scaleDeformation);
                            std::swap (symmetry, scaleDeformation.symmetry);
                            std::swap (limit, scaleDeformation.limit);
                        }
                    } scaleDeformation;
                    struct TwistDeformation : public Deformation {
                        util::f32 limit;

                        TwistDeformation () :
                            limit (180.0f) {}

                        inline void Swap (TwistDeformation &twistDeformation) {
                            Deformation::Swap (twistDeformation);
                            std::swap (limit, twistDeformation.limit);
                        }
                    } twistDeformation;
                    struct TeeterDeformation : public Deformation {
                        util::ui16 symmetry;
                        util::f32 limit;

                        TeeterDeformation () :
                             symmetry (1),
                             limit (90.0f) {}

                        inline void Swap (TeeterDeformation &teeterDeformation) {
                            Deformation::Swap (teeterDeformation);
                            std::swap (symmetry, teeterDeformation.symmetry);
                            std::swap (limit, teeterDeformation.limit);
                        }
                    } teeterDeformation;
                    struct BevelDeformation : public Deformation {
                        util::f32 limit;

                        BevelDeformation () :
                            limit (100.0f) {}

                        inline void Swap (BevelDeformation &bevelDeformation) {
                            Deformation::Swap (bevelDeformation);
                            std::swap (limit, bevelDeformation.limit);
                        }
                    } bevelDeformation;
                    struct FitDeformation : public Deformation {
                        util::ui16 symmetry;

                        FitDeformation () :
                            symmetry (1) {}

                        inline void Swap (FitDeformation &fitDeformation) {
                            Deformation::Swap (fitDeformation);
                            std::swap (symmetry, fitDeformation.symmetry);
                        }
                    } fitDeformation;
                    bool tween;
                    bool contour;
                    bool scale;
                    bool twist;
                    bool teeter;
                    bool bevel;
                    bool fit;

                    explicit Lofter (Project &project) :
                        Module (project),
                        show3D (false),
                        masterScale (1.0f),
                        masterShapeSteps (5),
                        tween (false),
                        contour (true),
                        scale (false),
                        twist (false),
                        teeter (false),
                        bevel (false),
                        fit (false) {}

                    void Swap (Lofter &lofter);

                    void ReadLMAGIC (ChunkReader &mainChunk);
                    void WriteLMAGIC (util::File &stream) const;

                private:
                    Lofter (const Lofter &);
                    Lofter &operator = (const Lofter &);

                    void ReadPATH_3D (ChunkReader &mainChunk);
                    void ReadSHAPE_2D (Shape &shape, ChunkReader &mainChunk);
                    void ReadM_SCALE (ChunkReader &mainChunk);
                    void ReadM_TWIST (ChunkReader &mainChunk);
                    void ReadM_TEETER (ChunkReader &mainChunk);
                    void ReadM_BEVEL (ChunkReader &mainChunk);
                    void ReadM_FIT (ChunkReader &mainChunk);
                    void ReadDeformationCurve (Deformation::Curve &curve, ChunkReader &mainChunk);

                    void WriteMASTER_SCALE (util::f32 masterScale, util::File &stream) const;
                    void WriteINTERPCT (util::ui16 masterShapeSteps, util::File &stream) const;
                    void WritePATH_3D (const Path &path, util::File &stream) const;
                    void WritePATH_MATRIX (const Path::Transform &transform, util::File &stream) const;
                    void WriteSHAPE_2D (const Shape &shape, util::File &stream) const;
                    void WriteM_SCALE (const ScaleDeformation &scaleDeformation, util::File &stream) const;
                    void WriteM_TWIST (const TwistDeformation &twistDeformation, util::File &stream) const;
                    void WriteM_TEETER (const TeeterDeformation &teeterDeformation, util::File &stream) const;
                    void WriteM_BEVEL (const BevelDeformation &bevelDeformation, util::File &stream) const;
                    void WriteM_FIT (const FitDeformation &fitDeformation, util::File &stream) const;
                    void WriteDEFORM_LIMIT (util::f32 limit, util::File &stream) const;
                    void WriteDeformationCurve (util::ui16 id, const Deformation::Curve &curve, util::File &stream) const;
                    void WriteUSE_TWEEN (util::File &stream) const;
                    void WriteUSE_CONTOUR (util::File &stream) const;
                    void WriteUSE_SCALE (util::File &stream) const;
                    void WriteUSE_TWIST (util::File &stream) const;
                    void WriteUSE_TEETER (util::File &stream) const;
                    void WriteUSE_BEVEL (util::File &stream) const;
                    void WriteUSE_FIT (util::File &stream) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_lofter_Lofter_h)
