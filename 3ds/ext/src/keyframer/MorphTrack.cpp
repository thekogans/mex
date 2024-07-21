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

#include <algorithm>
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/3ds/ext/keyframer/MorphTrack.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                namespace {
                    struct MeshWithName {
                        const std::string &name;
                        explicit MeshWithName (const std::string &name_) :
                            name (name_) {}
                        inline bool operator () (const io::Mesh *mesh) const {
                            return mesh->name == name;
                        }
                    };
                }

                MorphTrack::MorphTrack (
                        const io::stringTrack &track_,
                        const util::OwnerVector<io::Mesh> &meshes,
                        const io::Keyframer::Segment &segment) :
                        track (track_) {
                    // Initialize morphTracks.
                    if (!track.keys.empty ()) {
                        std::vector<const io::Mesh *> morphTargets;
                        {
                            class GetMorphTargetsJob : public util::Vectorizer::Job {
                            private:
                                std::vector<const io::Mesh *> &morphTargets;
                                const io::stringTrack &track;
                                const util::OwnerVector<io::Mesh> &meshes;

                            public:
                                GetMorphTargetsJob (
                                        std::vector<const io::Mesh *> &morphTargets_,
                                        const io::stringTrack &track_,
                                        const util::OwnerVector<io::Mesh> &meshes_) :
                                        morphTargets (morphTargets_),
                                        track (track_),
                                        meshes (meshes_) {
                                    morphTargets.insert (morphTargets.begin (), track.keys.size (), 0);
                                }

                                virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
                                    for (; sidx < eidx; ++sidx) {
                                        util::OwnerVector<io::Mesh>::const_iterator it =
                                            std::find_if (
                                                meshes.begin (),
                                                meshes.end (),
                                                MeshWithName (track.keys[sidx].value));
                                        if (it != meshes.end ()) {
                                            morphTargets[sidx] = *it;
                                        }
                                        assert (morphTargets[sidx] != 0);
                                    }
                                }
                                virtual std::size_t Size () const throw () {
                                    return morphTargets.size ();
                                }
                            } job (morphTargets, track, meshes);
                            util::GlobalVectorizer::Instance ()->Execute (job);
                        }
                        {
                            class CreateMorphTracksJob : public util::Vectorizer::Job {
                            private:
                                util::OwnerVector<io::Point3Track> &ioMorphTracks;
                                util::OwnerVector<Point3Track> &morphTracks;
                                const std::vector<const io::Mesh *> &morphTargets;
                                const io::stringTrack &track;
                                const io::Keyframer::Segment &segment;
                                std::vector<blas::Matrix3> ixforms;

                            public:
                                CreateMorphTracksJob (
                                        util::OwnerVector<io::Point3Track> &ioMorphTracks_,
                                        util::OwnerVector<Point3Track> &morphTracks_,
                                        const std::vector<const io::Mesh *> &morphTargets_,
                                        const io::stringTrack &track_,
                                        const io::Keyframer::Segment &segment_) :
                                        ioMorphTracks (ioMorphTracks_),
                                        morphTracks (morphTracks_),
                                        morphTargets (morphTargets_),
                                        track (track_),
                                        segment (segment_) {
                                    assert (!morphTargets.empty ());
                                    ioMorphTracks.insert (ioMorphTracks.begin (), morphTargets[0]->vertices.size (), 0);
                                    morphTracks.insert (morphTracks.begin (), morphTargets[0]->vertices.size (), 0);
                                    ixforms.resize (morphTargets.size ());
                                    for (std::size_t i = 0, count = morphTargets.size (); i < count; ++i) {
                                        ixforms[i] = morphTargets[i]->xform.Invert ();
                                    }
                                }

                                virtual void Execute (std::size_t sidx, std::size_t eidx, std::size_t rank) throw () {
                                    for (; sidx < eidx; ++sidx) {
                                        ioMorphTracks[sidx] = new io::Point3Track;
                                        ioMorphTracks[sidx]->flags = track.flags;
                                        for (std::size_t i = 0, count = morphTargets.size (); i < count; ++i) {
                                        #if defined (TOOLCHAIN_OS_Windows)
                                            // FIXME: VisualStudio linker is complaining
                                            // about the missing io::Point3Track::Key ctor.
                                            io::Point3Track::Key key;
                                            key.frame = track.keys[i].frame;
                                            key.flags = track.keys[i].flags;
                                            key.tension = track.keys[i].tension;
                                            key.continuity = track.keys[i].continuity;
                                            key.bias = track.keys[i].bias;
                                            key.easeFrom = track.keys[i].easeFrom;
                                            key.easeTo = track.keys[i].easeTo;
                                            key.value = morphTargets[i]->vertices[sidx] * ixforms[i];
                                            ioMorphTracks[sidx]->keys.push_back (key);
                                        #else // defined (TOOLCHAIN_OS_Windows)
                                            ioMorphTracks[sidx]->keys.push_back (
                                                io::Point3Track::Key (track.keys[i].frame,
                                                    track.keys[i].flags,
                                                    track.keys[i].tension,
                                                    track.keys[i].continuity,
                                                    track.keys[i].bias,
                                                    track.keys[i].easeFrom,
                                                    track.keys[i].easeTo,
                                                    morphTargets[i]->vertices[sidx] * ixforms[i]));
                                        #endif // defined (TOOLCHAIN_OS_Windows)
                                        }
                                        morphTracks[sidx] = new Point3Track (*ioMorphTracks[sidx], segment);
                                    }
                                }
                                virtual std::size_t Size () const throw () {
                                    return ioMorphTracks.size ();
                                }
                            } job (ioMorphTracks, morphTracks, morphTargets, track, segment);
                            util::GlobalVectorizer::Instance ()->Execute (job);
                        }
                    }
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
