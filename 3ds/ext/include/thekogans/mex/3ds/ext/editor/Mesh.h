// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_ext.
//
// libthekogans_mex_3ds_ext is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_ext is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_ext. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_ext_editor_Mesh_h)
#define __thekogans_mex_3ds_ext_editor_Mesh_h

#include <cassert>
#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Mesh {
                    typedef std::unique_ptr<Mesh> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    const io::Mesh &mesh;

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Edge {
                        util::ui16 v1;
                        util::ui16 v2;
                        util::ui16 f1;
                        util::ui16 f2;
                        enum Flags {
                            Visible = 0x00000001
                        };
                        util::ui32 flags;

                        Edge () {}
                        Edge (util::ui16 v1_, util::ui16 v2_, util::ui16 f1_, util::ui16 f2_, util::ui32 flags_) :
                            v1 (v1_), v2 (v2_), f1 (f1_), f2 (f2_), flags (flags_) {}

                        inline bool IsVisible () const {
                            return util::Flags32 (flags).Test (Visible);
                        }
                        inline void SetVisible (bool visible) const {
                            util::Flags32 (flags).Set (Visible, visible);
                        }
                    };

                    void BuildEdges (std::vector<Edge> &edges) const;

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Face {
                        const io::Mesh::Face &face;

                        explicit Face (const io::Mesh::Face &face_) : face (face_) {}

                        enum {
                            AB = 1,
                            BA = -1,
                            BC = 2,
                            CB = -2,
                            CA = 3,
                            AC = -3
                        };
                        inline util::ui32 HasEdge (const Edge &edge) const {
                            return
                                face.a == edge.v1 && face.b == edge.v2 ? AB :
                                face.a == edge.v2 && face.b == edge.v1 ? BA :
                                face.b == edge.v1 && face.c == edge.v2 ? BC :
                                face.b == edge.v2 && face.c == edge.v1 ? CB :
                                face.c == edge.v1 && face.a == edge.v2 ? CA :
                                face.c == edge.v2 && face.a == edge.v1 ? AC : 0;
                        }

                        inline bool IsFacingViewerOrthographic (
                                const std::vector<blas::Point3> &vertices) const {
                            return
                                (vertices[face.b].x - vertices[face.a].x) *
                                (vertices[face.c].y - vertices[face.a].y) -
                                (vertices[face.b].y - vertices[face.a].y) *
                                (vertices[face.c].x - vertices[face.a].x) >= 0.0f;
                        }
                        inline bool IsFacingViewerPerspective (
                                const std::vector<blas::Point3> &vertices) const {
                            return blas::Dot (blas::Cross (vertices[face.b] - vertices[face.a],
                                vertices[face.c] - vertices[face.a]), vertices[face.a]) >= 0.0f;
                        }
                        inline bool IsFacingViewer (
                                const io::View &view,
                                const std::vector<blas::Point3> &vertices) const {
                            return view.IsOrthographic () ?
                                IsFacingViewerOrthographic (vertices) :
                                IsFacingViewerPerspective (vertices);
                        }
                        typedef bool (Face::*IsFacingViewerFunc)(
                            const std::vector<blas::Point3> &vertices) const;
                        static IsFacingViewerFunc GetFacingViewerFunc (const io::View &view) {
                            return view.IsOrthographic () ?
                                &Face::IsFacingViewerOrthographic :
                                &Face::IsFacingViewerPerspective;
                        }
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL SelectionInfo {
                        const io::Mesh &mesh;
                        util::ui32 vertexCount[3];
                        util::ui32 faceCount[3];

                        explicit SelectionInfo (const io::Mesh &mesh_) :
                            mesh (mesh_) {}

                        void Build ();

                        inline bool IsSelected (util::ui32 selectionSet) const {
                            assert (selectionSet < 3);
                            return vertexCount[selectionSet] == mesh.vertices.size ();
                        }

                    private:
                        SelectionInfo (const SelectionInfo &);
                        SelectionInfo &operator = (const SelectionInfo &);
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Normals {
                        const io::Mesh &mesh;
                        const std::vector<blas::Point3> &vertices;

                        struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL VertexNormal {
                            THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                            blas::Point3 normal;
                            util::ui32 smooth;
                            VertexNormal *next;

                            VertexNormal (
                                const blas::Point3 &normal_,
                                util::ui32 smooth_) :
                                normal (normal_),
                                smooth (smooth_),
                                next (0) {}
                            ~VertexNormal () {delete next;}

                            void AddNormal (
                                const blas::Point3 &normal_,
                                util::ui32 smooth_);
                            const blas::Point3 &GetNormal (util::ui32 smooth_) const;
                            void Normalize ();

                        private:
                            VertexNormal (const VertexNormal &);
                            VertexNormal &operator = (const VertexNormal &);
                        };
                        util::OwnerVector<VertexNormal> vertexNormals;
                        // In the event the mesh has one normal per vertex,
                        // we build vertexNormals2 vector to speed up the
                        // interface between various APIs (mainly OpenGL).
                        // In that case vertexNormals will be empty.
                        std::vector<blas::Point3> vertexNormals2;
                        std::vector<blas::Point3> faceNormals;

                        explicit Normals (const io::Mesh &mesh_) :
                            mesh (mesh_),
                            vertices (mesh.vertices) {}
                        Normals (
                            const io::Mesh &mesh_,
                            const std::vector<blas::Point3> &vertices_) :
                            mesh (mesh_),
                            vertices (vertices_) {}

                        void Build ();

                        inline const blas::Point3 &GetFaceNormal (std::size_t faceIndex) const {
                            assert (faceIndex < faceNormals.size ());
                            return faceNormals[faceIndex];
                        }

                        void GetFaceVertexNormals (
                            std::size_t faceIndex,
                            blas::Point3 &a,
                            blas::Point3 &b,
                            blas::Point3 &c) const;

                    private:
                        Normals (const Normals &);
                        Normals &operator = (const Normals &);
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_EXT_DECL Element {
                        const io::Mesh &mesh;
                        std::vector<util::ui16> faces;

                        explicit Element (const io::Mesh &mesh_) :
                            mesh (mesh_) {}
                        Element (
                            const io::Mesh &mesh_,
                            const std::vector<util::ui16> &faces_) :
                            mesh (mesh_),
                            faces (faces_) {}

                        void Build (util::ui16 face, bool hidden = false);

                    private:
                        Element (const Element &);
                        Element &operator = (const Element &);
                    };

                    explicit Mesh (const io::Mesh &mesh_) :
                        mesh (mesh_) {}

                    static io::Mesh::UniquePtr CreateBox (
                        const std::string &name,
                        util::f32 length,
                        util::f32 width,
                        util::f32 height,
                        const blas::Matrix3 &xform);
                    static io::Mesh::UniquePtr CreateLSphere (
                        const std::string &name,
                        util::f32 radius,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateGSphere (
                        const std::string &name,
                        util::f32 radius,
                        util::ui32 nsub,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateHemisphere (
                        const std::string &name,
                        util::f32 radius,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateCylinder (
                        const std::string &name,
                        util::f32 radius,
                        util::f32 height,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateTube (
                        const std::string &name,
                        util::f32 inRadius,
                        util::f32 outRadius,
                        util::f32 height,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateTorus (
                        const std::string &name,
                        util::f32 outRadius,
                        util::f32 inRadius,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateCone (
                        const std::string &name,
                        util::f32 loRadius,
                        util::f32 highRadius,
                        util::f32 height,
                        util::ui32 bands,
                        util::ui32 segs,
                        const blas::Matrix3 &xform,
                        bool smooth);
                    static io::Mesh::UniquePtr CreateGear (
                        const std::string &name,
                        util::f32 radius,
                        util::f32 thickness,
                        util::f32 height,
                        util::ui32 teeth,
                        util::f32 taper,
                        const blas::Matrix3 &xform);
                    static io::Mesh::UniquePtr CreateTeapot (
                        const std::string &name,
                        util::ui32 usegs,
                        util::ui32 vsegs,
                        const blas::Matrix3 &xform,
                        bool weld, bool smooth);

                    blas::Bound2 GetBound2 (
                        const blas::Matrix3 &xform,
                        bool ignoreHiddenVertexFlags = false) const;
                    blas::Bound3 GetBound3 (bool ignoreHiddenVertexFlags = false) const;

                private:
                    Mesh (const Mesh &);
                    Mesh &operator = (const Mesh &);
                };

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_ext_editor_Mesh_h)
