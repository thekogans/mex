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
#include "thekogans/mex/3ds/ext/keyframer/f32Key.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                void f32Key::CompDeriv (const f32Key &prev, const f32Key &next) {
                    util::f32 fp;
                    util::f32 fn;
                    util::f32 dt = (util::f32 (next.frame) - util::f32 (prev.frame)) * 0.5f;
                    if (dt != 0.0f) {
                        fp = ((util::f32)frame - (util::f32)prev.frame) / dt;
                        fn = ((util::f32)next.frame - (util::f32)frame) / dt;
                        util::f32 fc = fabsf (continuity);
                        fp = fp + fc - fc * fp;
                        fn = fn + fc - fc * fn;
                    }
                    else {
                        fp = fn = 1.0f;
                    }
                    util::f32 tm = 0.5f * (1.0f - tension);
                    util::f32 cm = 1.0f - continuity;
                    util::f32 cp = 2.0f - cm;
                    util::f32 bm = 1.0f - bias;
                    util::f32 bp = 2.0f - bm;
                    util::f32 tmcm = tm * cm;
                    util::f32 tmcp = tm * cp;
                    util::f32 ksm = tmcm * bp * fp;
                    util::f32 ksp = tmcp * bm * fp;
                    util::f32 kdm = tmcp * bp * fn;
                    util::f32 kdp = tmcm * bm * fn;
                    util::f32 delm = value - prev.value;
                    util::f32 delp = next.value - value;

                    ds = ksm * delm + ksp * delp;
                    dd = kdm * delm + kdp * delp;
                }

                void f32Key::CompDerivFirst (const f32Key &next) {
                    ds = 0.0f;
                    dd = ((next.value - value) * 1.5f - next.ds * 0.5f) * (1.0f - tension);
                }

                void f32Key::CompDerivLast (const f32Key &prev) {
                    ds = ((value - prev.value) * 1.5f - prev.dd * 0.5f) * (1.0f - tension);
                    dd = 0.0f;
                }

                void f32Key::CompDerivFirstLoop (const f32Key &prev, const f32Key &next, util::f32 segmentLength) {
                    util::f32 fp;
                    util::f32 fn;
                    util::f32 dt = (segmentLength + next.frame - prev.frame) * 0.5f;
                    if (dt != 0.0f) {
                        fp = (segmentLength + frame - prev.frame) / dt;
                        fn = (next.frame - frame) / dt;
                        util::f32 fc = fabsf (continuity);
                        fp = fp + fc - fc * fp;
                        fn = fn + fc - fc * fn;
                    }
                    else {
                        fp = fn = 1.0f;
                    }
                    util::f32 tm = 0.5f * (1.0f - tension);
                    util::f32 cm = 1.0f - continuity;
                    util::f32 cp = 2.0f - cm;
                    util::f32 bm = 1.0f - bias;
                    util::f32 bp = 2.0f - bm;
                    util::f32 tmcm = tm * cm;
                    util::f32 tmcp = tm * cp;
                    util::f32 ksm = tmcm * bp * fp;
                    util::f32 ksp = tmcp * bm * fp;
                    util::f32 kdm = tmcp * bp * fn;
                    util::f32 kdp = tmcm * bm * fn;
                    util::f32 delm = value - prev.value;
                    util::f32 delp = next.value - value;

                    ds = ksm * delm + ksp * delp;
                    dd = kdm * delm + kdp * delp;
                }

                void f32Key::CompDerivLastLoop (const f32Key &prev, const f32Key &next, util::f32 segmentLength) {
                    util::f32 fp;
                    util::f32 fn;
                    util::f32 dt = (segmentLength + next.frame - prev.frame) * 0.5f;
                    if (dt != 0.0f) {
                        fp = (frame - prev.frame) / dt;
                        fn = (segmentLength + next.frame - frame) / dt;
                        util::f32 fc = fabsf (continuity);
                        fp = fp + fc - fc * fp;
                        fn = fn + fc - fc * fn;
                    }
                    else {
                        fp = fn = 1.0f;
                    }
                    util::f32 tm = 0.5f * (1.0f - tension);
                    util::f32 cm = 1.0f - continuity;
                    util::f32 cp = 2.0f - cm;
                    util::f32 bm = 1.0f - bias;
                    util::f32 bp = 2.0f - bm;
                    util::f32 tmcm = tm * cm;
                    util::f32 tmcp = tm * cp;
                    util::f32 ksm = tmcm * bp * fp;
                    util::f32 ksp = tmcp * bm * fp;
                    util::f32 kdm = tmcp * bp * fn;
                    util::f32 kdp = tmcm * bm * fn;
                    util::f32 delm = value - prev.value;
                    util::f32 delp = next.value - value;

                    ds = ksm * delm + ksp * delp;
                    dd = kdm * delm + kdp * delp;
                }

                void f32Key::CompDerivTwo (f32Key &next) {
                    // 2-key spline.
                    ds = 0.0f;
                    dd = (next.value - value) * (1.0f - tension);
                    next.ds = (next.value - value) * (1.0f - next.tension);
                    next.dd = 0.0f;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
