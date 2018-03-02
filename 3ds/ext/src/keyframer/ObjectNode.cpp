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
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/3ds/ext/keyframer/ObjectNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (ObjectNode)

                ObjectNode::ObjectNode (const io::Mesh &mesh_, const io::ObjectNode &objectNode_,
                    const util::OwnerVector<io::Mesh> &meshes, const io::Keyframer::Segment &segment) :
                    Node (objectNode_), mesh (mesh_), objectNode (objectNode_),
                    positionTrack (objectNode.positionTrack, segment),
                    rotationTrack (objectNode.rotationTrack, segment),
                    scaleTrack (objectNode.scaleTrack, segment),
                    hideTrack (objectNode.hideTrack, segment),
                    morphTrack (objectNode.morphTrack, meshes, segment),
                    ixform (mesh.xform.Invert ()),
                    isXformLeftHanded (mesh.xform.IsLeftHanded ()) {
                    vertices.resize (mesh.vertices.size ());
                }

                io::Mesh::UniquePtr ObjectNode::Snapshot () const {
                    io::Mesh::UniquePtr newMesh;
                    if (objectNode.name != io::ObjectNode::DUMMY_NAME) {
                        newMesh.reset (new io::Mesh (mesh));
                        assert (newMesh.get () != 0);
                        newMesh->vertices = vertices;
                        // Reset the new mesh xform. This is necessary because
                        // the new mesh is going to get default keyframe data.
                        newMesh->xform = blas::Matrix3::Identity;
                        newMesh->hidden = hidden;
                    }
                    return newMesh;
                }

                void ObjectNode::SetCurrentFrame (util::ui32 frame, const blas::Matrix3 &parentXform) {
                    blas::Matrix3 frameXform = GetFrameXform (frame, parentXform);
                    xform = blas::Matrix3::Translate (-objectNode.pivot) * frameXform;
                    if (isXformLeftHanded) {
                        xform.x *= -1.0f;
                    }
                    if (!morphTrack.morphTracks.empty ()) {
                        assert (morphTrack.morphTracks.size () == mesh.vertices.size ());
                        class MorphVerticesJob : public util::Vectorizer::Job {
                        private:
                            std::vector<blas::Point3> &vertices;
                            const util::OwnerVector<Point3Track> &morphTracks;
                            const util::ui32 frame;
                            const blas::Matrix3 &xform;

                        public:
                            MorphVerticesJob (std::vector<blas::Point3> &vertices_,
                                const util::OwnerVector<Point3Track> &morphTracks_,
                                const util::ui32 frame_, const blas::Matrix3 &xform_) :
                                vertices (vertices_), morphTracks (morphTracks_), frame (frame_), xform (xform_) {}

                            virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    vertices[sidx] = morphTracks[sidx]->GetTween (frame) * xform;
                                }
                            }
                            virtual std::size_t Size () const throw () {
                                return morphTracks.size ();
                            }
                        } job (vertices, morphTrack.morphTracks, frame, xform);
                        util::GlobalVectorizer::Instance ().Execute (job);
                    }
                    else {
                        class XformVerticesJob : public util::Vectorizer::Job {
                        private:
                            std::vector<blas::Point3> &result;
                            const std::vector<blas::Point3> &vertices;
                            blas::Matrix3 xform;

                        public:
                            XformVerticesJob (std::vector<blas::Point3> &result_,
                                const std::vector<blas::Point3> &vertices_, const blas::Matrix3 &xform_) :
                                result (result_), vertices (vertices_), xform (xform_) {}

                            virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
                                for (; sidx < eidx; ++sidx) {
                                    result[sidx] = vertices[sidx] * xform;
                                }
                            }
                            virtual std::size_t Size () const throw () {
                                return vertices.size ();
                            }
                        } job (vertices, mesh.vertices, ixform * xform);
                        util::GlobalVectorizer::Instance ().Execute (job);
                    }
                    hidden = objectNode.IsHide () || !hideTrack.GetTween (frame);
                    for (std::vector<Node *>::iterator it = children.begin (), end = children.end (); it != end; ++it) {
                        (*it)->SetCurrentFrame (frame, frameXform);
                    }
                }

                blas::Bound2 ObjectNode::GetBound2 (const blas::Matrix3 &xform) const {
                    blas::Bound2 bound (blas::Bound2::Empty);
                    if (objectNode.name == io::ObjectNode::DUMMY_NAME) {
                        blas::Matrix3 globalXform = this->xform * xform;
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.min.x, objectNode.bound.min.y, objectNode.bound.min.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.max.x, objectNode.bound.min.y, objectNode.bound.min.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.max.x, objectNode.bound.max.y, objectNode.bound.min.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.min.x, objectNode.bound.max.y, objectNode.bound.min.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.min.x, objectNode.bound.min.y, objectNode.bound.max.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.max.x, objectNode.bound.min.y, objectNode.bound.max.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.max.x, objectNode.bound.max.y, objectNode.bound.max.z) * globalXform);
                        bound += blas::Point2 (blas::Point3 (objectNode.bound.min.x, objectNode.bound.max.y, objectNode.bound.max.z) * globalXform);
                    }
                    else {
                        class CalcBoundJob : public util::Vectorizer::Job {
                        private:
                            blas::Bound2 &bound;
                            const std::vector<blas::Point3> &vertices;
                            const blas::Matrix3 &xform;
                            std::vector<blas::Bound2> bounds;

                        public:
                            CalcBoundJob (blas::Bound2 &bound_, const std::vector<blas::Point3> &vertices_,
                                const blas::Matrix3 &xform_) : bound (bound_), vertices (vertices_), xform (xform_) {}

                            virtual void Prolog (std::size_t chunks) throw () {
                                bounds.insert (bounds.begin (), chunks, blas::Bound2::Empty);
                            }
                            virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
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
                        } job (bound, vertices, xform);
                        util::GlobalVectorizer::Instance ().Execute (job);
                    }
                    return bound;
                }

                blas::Bound3 ObjectNode::GetBound3 () const {
                    if (objectNode.name == io::ObjectNode::DUMMY_NAME) {
                        return objectNode.bound;
                    }
                    blas::Bound3 bound (blas::Bound3::Empty);
                    class CalcBoundJob : public util::Vectorizer::Job {
                    private:
                        blas::Bound3 &bound;
                        const std::vector<blas::Point3> &vertices;
                        blas::Matrix3 xform;
                        std::vector<blas::Bound3> bounds;

                    public:
                        CalcBoundJob (blas::Bound3 &bound_, const std::vector<blas::Point3> &vertices_,
                            const blas::Matrix3 &xform_) : bound (bound_), vertices (vertices_), xform (xform_) {}

                        virtual void Prolog (std::size_t chunks) throw () {
                            bounds.insert (bounds.begin (), chunks, blas::Bound3::Empty);
                        }
                        virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
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
                    util::GlobalVectorizer::Instance ().Execute (job);
                    return bound;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
