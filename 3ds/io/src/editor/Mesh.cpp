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

#include <numeric>
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS_EX (
                    Mesh,
                    util::SpinLock,
                    1024,
                    util::DefaultAllocator::Instance ().Get ())

                void Mesh::Swap (Mesh &mesh) {
                    assert (&mesh != this);
                    name.swap (mesh.name);
                    vertices.swap (mesh.vertices);
                    vertexFlags.swap (mesh.vertexFlags);
                    textureVertices.swap (mesh.textureVertices);
                    faces.swap (mesh.faces);
                    faceSmoothGroups.swap (mesh.faceSmoothGroups);
                    materials.swap (mesh.materials);
                    std::swap (xform, mesh.xform);
                    std::swap (paletteIndex, mesh.paletteIndex);
                    std::swap (map, mesh.map);
                    boxMap.swap (mesh.boxMap);
                    procName.swap (mesh.procName);
                    procData.swap (mesh.procData);
                    std::swap (hidden, mesh.hidden);
                    std::swap (visibleInLofter, mesh.visibleInLofter);
                    std::swap (matte, mesh.matte);
                    std::swap (castShadow, mesh.castShadow);
                    std::swap (receiveShadows, mesh.receiveShadows);
                    std::swap (fastDraw, mesh.fastDraw);
                    std::swap (frozen, mesh.frozen);
                    std::swap (useMap, mesh.useMap);
                    std::swap (useBoxMap, mesh.useBoxMap);
                    std::swap (useProcData, mesh.useProcData);
                }

                blas::Bound3 Mesh::GetBound3 (bool ignoreHiddenVertexFlags) const {
                    blas::Bound3 bound (blas::Bound3::Empty);
                    if (!ignoreHiddenVertexFlags && !vertexFlags.empty ()) {
                        class CalcBoundJob : public util::Vectorizer::Job {
                        private:
                            blas::Bound3 &bound;
                            const std::vector<blas::Point3> &vertices;
                            const std::vector<util::ui16> &vertexFlags;
                            blas::Matrix3 xform;
                            std::vector<blas::Bound3> bounds;

                        public:
                            CalcBoundJob (
                                blas::Bound3 &bound_,
                                const std::vector<blas::Point3> &vertices_,
                                const std::vector<util::ui16> &vertexFlags_,
                                const blas::Matrix3 &xform_) :
                                bound (bound_),
                                vertices (vertices_),
                                vertexFlags (vertexFlags_),
                                xform (xform_) {}

                            virtual void Prolog (std::size_t chunks) throw () {
                                bounds.insert (bounds.begin (), chunks, blas::Bound3::Empty);
                            }
                            virtual void Execute (
                                    std::size_t sidx,
                                    std::size_t eidx,
                                    std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    if (!util::Flags16 (vertexFlags[sidx]).Test (io::Mesh::Hidden)) {
                                        bounds[rank] += vertices[sidx] * xform;
                                    }
                                }
                            }
                            virtual void Epilog () throw () {
                                bound = std::accumulate (bounds.begin (), bounds.end (), blas::Bound3::Empty);
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (bound, vertices, vertexFlags, xform.Invert ());
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                    else {
                        class CalcBoundJob : public util::Vectorizer::Job {
                        private:
                            blas::Bound3 &bound;
                            const std::vector<blas::Point3> &vertices;
                            blas::Matrix3 xform;
                            std::vector<blas::Bound3> bounds;

                        public:
                            CalcBoundJob (
                                blas::Bound3 &bound_,
                                const std::vector<blas::Point3> &vertices_,
                                const blas::Matrix3 &xform_) :
                                bound (bound_),
                                vertices (vertices_),
                                xform (xform_) {}

                            virtual void Prolog (std::size_t chunks) throw () {
                                bounds.insert (bounds.begin (), chunks, blas::Bound3::Empty);
                            }
                            virtual void Execute (
                                    std::size_t sidx,
                                    std::size_t eidx,
                                    std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    bounds[rank] += vertices[sidx] * xform;
                                }
                            }
                            virtual void Epilog () throw () {
                                bound = std::accumulate (bounds.begin (), bounds.end (), blas::Bound3::Empty);
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (bound, vertices, xform.Invert ());
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                    return bound;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
