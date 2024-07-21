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

#include <numeric>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/blas/Matrix3.h"
#include "thekogans/mex/blas/Bound.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (Mesh)

                blas::Bound2 Mesh::GetBound2 (
                        const blas::Matrix3 &xform,
                        bool ignoreHiddenVertexFlags) const {
                    blas::Bound2 bound (blas::Bound2::Empty);
                    if (!ignoreHiddenVertexFlags && !mesh.vertexFlags.empty ()) {
                        class CalcBoundJob : public util::Vectorizer::Job {
                        private:
                            blas::Bound2 &bound;
                            const std::vector<blas::Point3> &vertices;
                            const std::vector<util::ui16> &vertexFlags;
                            const blas::Matrix3 &xform;
                            std::vector<blas::Bound2> bounds;

                        public:
                            CalcBoundJob (
                                blas::Bound2 &bound_,
                                const std::vector<blas::Point3> &vertices_,
                                const std::vector<util::ui16> &vertexFlags_,
                                const blas::Matrix3 &xform_) :
                                bound (bound_),
                                vertices (vertices_),
                                vertexFlags (vertexFlags_),
                                xform (xform_) {}

                            virtual void Prolog (std::size_t chunks) throw () {
                                bounds.insert (bounds.begin (), chunks, blas::Bound2::Empty);
                            }
                            virtual void Execute (
                                    std::size_t sidx,
                                    std::size_t eidx,
                                    std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    if (!util::Flags16 (vertexFlags[sidx]).Test (io::Mesh::Hidden)) {
                                        bounds[rank] += blas::Point2 (vertices[sidx] * xform);
                                    }
                                }
                            }
                            virtual void Epilog () throw () {
                                bound = std::accumulate (bounds.begin (), bounds.end (), blas::Bound2::Empty);
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (bound, mesh.vertices, mesh.vertexFlags, xform);
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                    else {
                        class CalcBoundJob : public util::Vectorizer::Job {
                        private:
                            blas::Bound2 &bound;
                            const std::vector<blas::Point3> &vertices;
                            const blas::Matrix3 &xform;
                            std::vector<blas::Bound2> bounds;

                        public:
                            CalcBoundJob (
                                blas::Bound2 &bound_,
                                const std::vector<blas::Point3> &vertices_,
                                const blas::Matrix3 &xform_) :
                                bound (bound_),
                                vertices (vertices_),
                                xform (xform_) {}

                            virtual void Prolog (std::size_t chunks) throw () {
                                bounds.insert (bounds.begin (), chunks, blas::Bound2::Empty);
                            }
                            virtual void Execute (
                                    std::size_t sidx,
                                    std::size_t eidx,
                                    std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    bounds[rank] += blas::Point2 (vertices[sidx] * xform);
                                }
                            }
                            virtual void Epilog () throw () {
                                bound = std::accumulate (bounds.begin (), bounds.end (), blas::Bound2::Empty);
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (bound, mesh.vertices, xform);
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                    return bound;
                }

                blas::Bound3 Mesh::GetBound3 (bool ignoreHiddenVertexFlags) const {
                    blas::Bound3 bound (blas::Bound3::Empty);
                    if (!ignoreHiddenVertexFlags && !mesh.vertexFlags.empty ()) {
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
                                bound = std::accumulate (
                                    bounds.begin (),
                                    bounds.end (),
                                    blas::Bound3::Empty);
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (bound, mesh.vertices, mesh.vertexFlags, mesh.xform.Invert ());
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
                                bound = std::accumulate (
                                    bounds.begin (),
                                    bounds.end (),
                                    blas::Bound3::Empty);
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (bound, mesh.vertices, mesh.xform.Invert ());
                        util::GlobalVectorizer::Instance ()->Execute (job);
                    }
                    return bound;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
