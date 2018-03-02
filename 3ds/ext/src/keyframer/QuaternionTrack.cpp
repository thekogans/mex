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

#include "thekogans/mex/blas/MathUtil.h"
#include "thekogans/mex/3ds/ext/keyframer/Util.h"
#include "thekogans/mex/3ds/ext/keyframer/QuaternionTrack.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                QuaternionTrack::QuaternionTrack (const io::AngleAxisTrack &track_,
                    const io::Keyframer::Segment &segment) : track (track_) {
                    {
                        // vectorize
                        for (std::size_t i = 0, count = track.keys.size (); i < count; ++i) {
                            keys.push_back (QuaternionKey (track.keys[i]));
                        }
                    }
                    {
                        // vectorize
                        // Setup the keys first...
                        for (std::size_t i = 0, count = keys.size (); i < count; ++i) {
                            // Derive the absolute quat from angle/axis
                            MakeAngPositive (i);
                            keys[i].value.axis.Normalize ();
                            keys[i].quat = blas::Quaternion::FromAngleAxis (keys[i].value);
                            // Make absolute
                            if (i) {
                                keys[i].quat = keys[i - 1].quat * keys[i].quat;
                            }
                        }
                    }
                    {
                        // vectorize
                        // Then calculate the derivatives
                        for (std::size_t i = 0, count = keys.size (); i < count; ++i) {
                            CompAB (i, segment);
                        }
                    }
                    {
                        // Setup tween cache.
                        tweenCache.resize (segment.length + 1);
                        for (util::ui32 frame = 0, count = segment.length + 1; frame < count; ++frame) {
                            tweenCache[frame] = GetTweenHelper (frame);
                        }
                    }
                }

                blas::Quaternion QuaternionTrack::GetTweenHelper (util::ui32 frame) const {
                    if (keys.empty ()) {
                        return blas::Quaternion::Identity;
                    }

                    if (track.IsRepeats ()) {
                        frame = keys[0].frame + frame % (keys.back ().frame - keys[0].frame);
                    }

                    if (!track.IsLoops ()) {
                        if (frame < keys[0].frame) {
                            return keys[0].quat;
                        }
                        else if (frame > keys.back ().frame) {
                            return keys.back ().quat;
                        }
                    }

                    std::size_t kidx = 0;
                    while (kidx < keys.size () && keys[kidx].frame < frame) {
                        ++kidx;
                    }
                    if (kidx == keys.size () || (kidx && frame != keys[kidx].frame)) {
                        --kidx;
                    }

                    if (frame == keys[kidx].frame) {
                        return keys[kidx].quat;
                    }

                    if (!kidx && frame < keys[0].frame) {
                        kidx = keys.size () - 1;
                    }

                    std::size_t nkidx = kidx + 1;
                    if (nkidx == keys.size ()) {
                        nkidx = 0;
                    }

                    util::f32 t = Ease (static_cast<util::f32> (frame - keys[kidx].frame) / (keys[nkidx].frame - keys[kidx].frame),
                        keys[kidx].easeFrom, keys[nkidx].easeTo);

                    return blas::SquadRev (t,
                        keys[nkidx].value.angle, keys[nkidx].value.axis,
                        keys[kidx].quat, keys[kidx].ds,
                        keys[nkidx].dd, keys[nkidx].quat);
                }

                void QuaternionTrack::MakeAngPositive (std::size_t kidx) {
                    if (keys[kidx].value.angle < 0.0f) {
                        keys[kidx].value.angle = -keys[kidx].value.angle;
                        keys[kidx].value.axis = -keys[kidx].value.axis;
                    }
                }

                void QuaternionTrack::CompAB (std::size_t kidx, const io::Keyframer::Segment &segment) {
                    std::size_t count = keys.size ();
                    blas::Quaternion qm (blas::Quaternion::Identity);
                    if (kidx > 0 || track.IsLoops ()) {
                        if (keys[kidx].value.angle > util::TWOPI - util::EPSILON) {
                            qm = blas::Quaternion (keys[kidx].value.axis.x,
                                keys[kidx].value.axis.y, keys[kidx].value.axis.z, 0.0f).Log ();
                        }
                        else {
                            blas::Quaternion qprev = kidx == 0 ? keys.back ().quat : keys[kidx - 1].quat;
                            if (blas::Dot (qprev, keys[kidx].quat) < 0.0f) {
                                qprev = -qprev;
                            }
                            qm = (keys[kidx].quat / qprev).Log ();
                        }
                    }

                    blas::Quaternion qp (blas::Quaternion::Identity);
                    if (kidx < count - 1 || track.IsLoops ()) {
                        std::size_t i1 = kidx == count - 1 ? 0 : kidx + 1;
                        if (keys[i1].value.angle > util::TWOPI - util::EPSILON) {
                            qp = blas::Quaternion (keys[i1].value.axis.x,
                                keys[i1].value.axis.y, keys[i1].value.axis.z, 0.0f).Log ();
                        }
                        else {
                            blas::Quaternion qnext = keys[i1].quat;
                            if (blas::Dot (qnext, keys[kidx].quat) < 0.0f) {
                                qnext = -qnext;
                            }
                            qp = (qnext / keys[kidx].quat).Log ();
                        }
                    }

                    if (kidx == 0 && !track.IsLoops ()) {
                        qm = qp;
                    }
                    if (kidx == count - 1 && !track.IsLoops ()) {
                        qp = qm;
                    }

                    util::f32 fp = 1.0f;
                    util::f32 fn = 1.0f;
                    util::f32 cm = 1.0f - keys[kidx].continuity;
                    if ((kidx > 0 && kidx < count - 1) || track.IsLoops ()) {
                        if (kidx == 0) {
                            util::f32 dtb = (util::f32)segment.endFrame - keys[count - 1].frame + keys[0].frame - segment.startFrame;
                            util::f32 dt = 0.5f * (util::f32)(dtb + keys[1].frame - keys[0].frame);
                            fp = dtb / dt;
                            fn = (util::f32)(keys[1].frame - keys[0].frame) / dt;
                        }
                        else if (kidx == count - 1) {
                            util::f32 dta = (util::f32)segment.endFrame - keys[count - 1].frame + keys[0].frame - segment.startFrame;
                            util::f32 dt = 0.5f * (util::f32)(dta + keys[count - 1].frame - keys[count - 2].frame);
                            fp = dta / dt;
                            fn = ((util::f32)(keys[count - 1].frame - keys[count - 2].frame)) / dt;
                        }
                        else {
                            util::f32 dt = 0.5f * util::f32 (keys[kidx + 1].frame - keys[kidx - 1].frame);
                            fp = (util::f32)(keys[kidx].frame - keys[kidx - 1].frame) / dt;
                            fn = (util::f32)(keys[kidx + 1].frame - keys[kidx].frame) / dt;
                        }
                        util::f32 c = fabsf (keys[kidx].continuity);
                        fp = fp + c - c * fp;
                        fn = fn + c - c * fn;
                    }

                    util::f32 tm = 0.5f * (1.0f - keys[kidx].tension);
                    util::f32 cp = 2.0f - cm;
                    util::f32 bm = 1.0f - keys[kidx].bias;
                    util::f32 bp = 2.0f - bm;
                    util::f32 tmcm = tm * cm;
                    util::f32 tmcp = tm * cp;
                    util::f32 ksm  = 1.0f - tmcm * bp * fp;
                    util::f32 ksp  = -tmcp * bm * fp;
                    util::f32 kdm  = tmcp * bp * fn;
                    util::f32 kdp  = tmcm * bm * fn - 1.0f;

                    blas::Quaternion  qa (
                        0.5f * (kdm * qm.x + kdp * qp.x),
                        0.5f * (kdm * qm.y + kdp * qp.y),
                        0.5f * (kdm * qm.z + kdp * qp.z),
                        0.5f * (kdm * qm.w + kdp * qp.w));
                    blas::Quaternion  qb (
                        0.5f * (ksm * qm.x + ksp * qp.x),
                        0.5f * (ksm * qm.y + ksp * qp.y),
                        0.5f * (ksm * qm.z + ksp * qp.z),
                        0.5f * (ksm * qm.w + ksp * qp.w));

                    keys[kidx].ds = keys[kidx].quat * qa.Exp ();
                    keys[kidx].dd = keys[kidx].quat * qb.Exp ();
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
