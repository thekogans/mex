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

#if !defined (__thekogans_mex_3ds_io_editor_Mesh_h)
#define __thekogans_mex_3ds_io_editor_Mesh_h

#include <memory>
#include <string>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/Util.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Mesh {
                    typedef std::unique_ptr<Mesh> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Mesh)

                    std::string name;
                    std::vector<blas::Point3> vertices;
                    enum {
                        Hidden = 0x1000,        // Vertex is hidden
                        SelectionSetC = 0x2000, // Vertex belongs to selection set C
                        SelectionSetB = 0x4000, // Vertex belongs to selection set B
                        SelectionSetA = 0x8000  // Vertex belongs to selection set A
                    };
                    static util::ui16 GetSelectMask (util::ui16 selectionSet) {
                        return
                            selectionSet == 0 ? SelectionSetA :
                            selectionSet == 1 ? SelectionSetB : SelectionSetC;
                    }
                    static util::ui16 GetSelectionSet (util::ui16 selecMask) {
                        return
                            selecMask == SelectionSetA ? 0 :
                            selecMask == SelectionSetB ? 1 : 2;
                    }
                    std::vector<util::ui16> vertexFlags;
                    std::vector<blas::Point2> textureVertices;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Face {
                        util::ui16 a;
                        util::ui16 b;
                        util::ui16 c;
                        enum {
                            // FIXME: check these flags
                            EdgeCAVisible = 0x0001,
                            EdgeBCVisible = 0x0002,
                            EdgeABVisible = 0x0004,
                            UWrap = 0x0008,
                            VWrap = 0x0010,
                            Visible = 0x0020,
                            Hidden = 0x1000,
                            SelectionSetC = 0x2000,
                            SelectionSetB = 0x4000,
                            SelectionSetA = 0x8000
                        };
                        util::ui16 flags;

                        Face () {}
                        Face (
                            util::ui16 a_,
                            util::ui16 b_,
                            util::ui16 c_,
                            util::ui16 flags_) :
                            a (a_),
                            b (b_),
                            c (c_),
                            flags (flags_) {}

                        inline bool IsEdgeABVisible () const {
                            return util::Flags16 (flags).Test (EdgeABVisible);
                        }
                        inline bool SetEdgeABVisible (bool edgeABVisible) {
                            return util::Flags16 (flags).Set (EdgeABVisible, edgeABVisible);
                        }
                        inline bool FlipEdgeABVisible () {
                            return util::Flags16 (flags).Flip (EdgeABVisible);
                        }
                        inline bool IsEdgeBCVisible () const {
                            return util::Flags16 (flags).Test (EdgeBCVisible);
                        }
                        inline bool SetEdgeBCVisible (bool edgeBCVisible) {
                            return util::Flags16 (flags).Set (EdgeBCVisible, edgeBCVisible);
                        }
                        inline bool FlipEdgeBCVisible () {
                            return util::Flags16 (flags).Flip (EdgeBCVisible);
                        }
                        inline bool IsEdgeCAVisible () const {
                            return util::Flags16 (flags).Test (EdgeCAVisible);
                        }
                        inline bool SetEdgeCAVisible (bool edgeCAVisible) {
                            return util::Flags16 (flags).Set (EdgeCAVisible, edgeCAVisible);
                        }
                        inline bool FlipEdgeCAVisible () {
                            return util::Flags16 (flags).Flip (EdgeCAVisible);
                        }
                        inline bool IsUWrap () const {
                            return util::Flags16 (flags).Test (UWrap);
                        }
                        inline bool IsVWrap () const {
                            return util::Flags16 (flags).Test (VWrap);
                        }
                        inline bool IsVisible () const {
                            return util::Flags16 (flags).Test (Visible);
                        }
                        inline bool IsHidden () const {
                            return util::Flags16 (flags).Test (Hidden);
                        }
                        inline bool IsSelectionSetC () const {
                            return util::Flags16 (flags).Test (SelectionSetC);
                        }
                        inline bool IsSelectionSetB () const {
                            return util::Flags16 (flags).Test (SelectionSetB);
                        }
                        inline bool IsSelectionSetA () const {
                            return util::Flags16 (flags).Test (SelectionSetA);
                        }

                        static util::ui16 GetSelectMask (util::ui16 selectionSet) {
                            return
                                selectionSet == 0 ? SelectionSetA :
                                selectionSet == 1 ? SelectionSetB : SelectionSetC;
                        }
                        static util::ui16 GetSelectionSet (util::ui16 selecMask) {
                            return
                                selecMask == SelectionSetA ? 0 :
                                selecMask == SelectionSetB ? 1 : 2;
                        }
                    };
                    std::vector<Face> faces;
                    std::vector<util::ui32> faceSmoothGroups;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Material {
                        typedef std::unique_ptr<Material> UniquePtr;

                        std::string name;
                        std::vector<util::ui16> faces;

                        Material (const std::string &name_) :
                            name (name_) {}
                        Material (const Material &material) :
                            name (material.name),
                            faces (material.faces) {}

                        Material &operator = (const Material &material) {
                            if (&material != this) {
                                name = material.name;
                                faces = material.faces;
                            }
                            return *this;
                        }
                    };
                    util::OwnerVectorWithCopyCtor<Material> materials;
                    blas::Matrix3 xform;
                    util::ui8 paletteIndex;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Map {
                        enum Type {
                            Planar,
                            Cylindrical,
                            Spherical
                        };
                        util::ui16 type;
                        util::f32 tilex;
                        util::f32 tiley;
                        blas::Point3 center;
                        util::f32 scale;
                        blas::Matrix3 xform;
                        util::f32 planarWidth;
                        util::f32 planarHeight;
                        util::f32 cylindricalHeight;

                        Map () :
                            type (Planar),
                            tilex (0.0f),
                            tiley (0.0f),
                            center (blas::Point3::Zero),
                            scale (1.0f),
                            xform (blas::Matrix3::Identity),
                            planarWidth (1.0f),
                            planarHeight (1.0f),
                            cylindricalHeight (1.0f) {}
                    } map;
                    std::vector<std::string> boxMap;
                    std::string procName;
                    std::vector<util::ui8> procData;
                    bool hidden;
                    bool visibleInLofter;
                    bool matte;
                    bool castShadow;
                    bool receiveShadows;
                    bool fastDraw;
                    bool frozen;
                    bool useMap;
                    bool useBoxMap;
                    bool useProcData;

                    Mesh () {}
                    explicit Mesh (const std::string &name_) :
                        name (name_),
                        xform (blas::Matrix3::Identity),
                        paletteIndex (0),
                        hidden (false),
                        visibleInLofter (false),
                        matte (false),
                        castShadow (true),
                        receiveShadows (true),
                        fastDraw (false),
                        frozen (false),
                        useMap (false),
                        useBoxMap (false),
                        useProcData (false) {}
                    Mesh (const Mesh &mesh) :
                        name (mesh.name),
                        vertices (mesh.vertices),
                        vertexFlags (mesh.vertexFlags),
                        textureVertices (mesh.textureVertices),
                        faces (mesh.faces),
                        faceSmoothGroups (mesh.faceSmoothGroups),
                        materials (mesh.materials),
                        xform (mesh.xform),
                        paletteIndex (mesh.paletteIndex),
                        map (mesh.map),
                        boxMap (mesh.boxMap),
                        procName (mesh.procName),
                        procData (mesh.procData),
                        hidden (mesh.hidden),
                        visibleInLofter (mesh.visibleInLofter),
                        matte (mesh.matte),
                        castShadow (mesh.castShadow),
                        receiveShadows (mesh.receiveShadows),
                        fastDraw (mesh.fastDraw),
                        frozen (mesh.frozen),
                        useMap (mesh.useMap),
                        useBoxMap (mesh.useBoxMap) {}

                    inline Mesh &operator = (const Mesh &mesh) {
                        // Maintain transactional semantics.
                        if (&mesh != this) {
                            Mesh tmp (mesh);
                            Swap (tmp);
                        }
                        return *this;
                    }

                    void Swap (Mesh &mesh);

                    inline bool IsHidden () const {
                        return hidden;
                    }
                    inline bool IsVisibleInLofter () const {
                        return visibleInLofter;
                    }
                    inline bool IsMatte () const {
                        return matte;
                    }
                    inline bool IsCastShadow () const {
                        return castShadow;
                    }
                    inline bool IsReceiveShadow () const {
                        return receiveShadows;
                    }
                    inline bool IsFastDraw () const {
                        return fastDraw;
                    }
                    inline bool IsFrozen () const {
                        return frozen;
                    }
                    inline bool IsUseMap () const {
                        return useMap;
                    }
                    inline bool IsUseBoxMap () const {
                        return useBoxMap;
                    }
                    inline bool IsUseProcData () const {
                        return useProcData;
                    }

                    // FIXME: This is here because ObjectNode (Mesh &) uses it to
                    // initialize itself. Think about removing that ctor (command?),
                    // and removing this api.
                    blas::Bound3 GetBound3 (bool ignoreHiddenVertexFlags = true) const;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_editor_Mesh_h)
