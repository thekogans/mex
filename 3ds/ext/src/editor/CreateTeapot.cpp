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

#include "thekogans/mex/blas/Point4.h"
#include "thekogans/mex/blas/Matrix4.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                namespace {
                    const blas::Point3 verts[] = {
                        blas::Point3 ( 1.40000f,  0.00000f,  2.40000f),   // 1
                        blas::Point3 ( 1.40000f, -0.78400f,  2.40000f),
                        blas::Point3 ( 0.78400f, -1.40000f,  2.40000f),
                        blas::Point3 ( 0.00000f, -1.40000f,  2.40000f),
                        blas::Point3 ( 1.33750f,  0.00000f,  2.53125f),
                        blas::Point3 ( 1.33750f, -0.74900f,  2.53125f),
                        blas::Point3 ( 0.74900f, -1.33750f,  2.53125f),
                        blas::Point3 ( 0.00000f, -1.33750f,  2.53125f),
                        blas::Point3 ( 1.43750f,  0.00000f,  2.53125f),
                        blas::Point3 ( 1.43750f, -0.80500f,  2.53125f),   // 10
                        blas::Point3 ( 0.80500f, -1.43750f,  2.53125f),
                        blas::Point3 ( 0.00000f, -1.43750f,  2.53125f),
                        blas::Point3 ( 1.50000f,  0.00000f,  2.40000f),
                        blas::Point3 ( 1.50000f, -0.84000f,  2.40000f),
                        blas::Point3 ( 0.84000f, -1.50000f,  2.40000f),
                        blas::Point3 ( 0.00000f, -1.50000f,  2.40000f),
                        blas::Point3 (-0.78400f, -1.40000f,  2.40000f),
                        blas::Point3 (-1.40000f, -0.78400f,  2.40000f),
                        blas::Point3 (-1.40000f,  0.00000f,  2.40000f),
                        blas::Point3 (-0.74900f, -1.33750f,  2.53125f),   // 20
                        blas::Point3 (-1.33750f, -0.74900f,  2.53125f),
                        blas::Point3 (-1.33750f,  0.00000f,  2.53125f),
                        blas::Point3 (-0.80500f, -1.43750f,  2.53125f),
                        blas::Point3 (-1.43750f, -0.80500f,  2.53125f),
                        blas::Point3 (-1.43750f,  0.00000f,  2.53125f),
                        blas::Point3 (-0.84000f, -1.50000f,  2.40000f),
                        blas::Point3 (-1.50000f, -0.84000f,  2.40000f),
                        blas::Point3 (-1.50000f,  0.00000f,  2.40000f),
                        blas::Point3 (-1.40000f,  0.78400f,  2.40000f),
                        blas::Point3 (-0.78400f,  1.40000f,  2.40000f),   // 30
                        blas::Point3 ( 0.00000f,  1.40000f,  2.40000f),
                        blas::Point3 (-1.33750f,  0.74900f,  2.53125f),
                        blas::Point3 (-0.74900f,  1.33750f,  2.53125f),
                        blas::Point3 ( 0.00000f,  1.33750f,  2.53125f),
                        blas::Point3 (-1.43750f,  0.80500f,  2.53125f),
                        blas::Point3 (-0.80500f,  1.43750f,  2.53125f),
                        blas::Point3 ( 0.00000f,  1.43750f,  2.53125f),
                        blas::Point3 (-1.50000f,  0.84000f,  2.40000f),
                        blas::Point3 (-0.84000f,  1.50000f,  2.40000f),
                        blas::Point3 ( 0.00000f,  1.50000f,  2.40000f),   // 40
                        blas::Point3 ( 0.78400f,  1.40000f,  2.40000f),
                        blas::Point3 ( 1.40000f,  0.78400f,  2.40000f),
                        blas::Point3 ( 0.74900f,  1.33750f,  2.53125f),
                        blas::Point3 ( 1.33750f,  0.74900f,  2.53125f),
                        blas::Point3 ( 0.80500f,  1.43750f,  2.53125f),
                        blas::Point3 ( 1.43750f,  0.80500f,  2.53125f),
                        blas::Point3 ( 0.84000f,  1.50000f,  2.40000f),
                        blas::Point3 ( 1.50000f,  0.84000f,  2.40000f),
                        blas::Point3 ( 1.75000f,  0.00000f,  1.87500f),
                        blas::Point3 ( 1.75000f, -0.98000f,  1.87500f),   // 50
                        blas::Point3 ( 0.98000f, -1.75000f,  1.87500f),
                        blas::Point3 ( 0.00000f, -1.75000f,  1.87500f),
                        blas::Point3 ( 2.00000f,  0.00000f,  1.35000f),
                        blas::Point3 ( 2.00000f, -1.12000f,  1.35000f),
                        blas::Point3 ( 1.12000f, -2.00000f,  1.35000f),
                        blas::Point3 ( 0.00000f, -2.00000f,  1.35000f),
                        blas::Point3 ( 2.00000f,  0.00000f,  0.90000f),
                        blas::Point3 ( 2.00000f, -1.12000f,  0.90000f),
                        blas::Point3 ( 1.12000f, -2.00000f,  0.90000f),
                        blas::Point3 ( 0.00000f, -2.00000f,  0.90000f),   // 60
                        blas::Point3 (-0.98000f, -1.75000f,  1.87500f),
                        blas::Point3 (-1.75000f, -0.98000f,  1.87500f),
                        blas::Point3 (-1.75000f,  0.00000f,  1.87500f),
                        blas::Point3 (-1.12000f, -2.00000f,  1.35000f),
                        blas::Point3 (-2.00000f, -1.12000f,  1.35000f),
                        blas::Point3 (-2.00000f,  0.00000f,  1.35000f),
                        blas::Point3 (-1.12000f, -2.00000f,  0.90000f),
                        blas::Point3 (-2.00000f, -1.12000f,  0.90000f),
                        blas::Point3 (-2.00000f,  0.00000f,  0.90000f),
                        blas::Point3 (-1.75000f,  0.98000f,  1.87500f),   // 70
                        blas::Point3 (-0.98000f,  1.75000f,  1.87500f),
                        blas::Point3 ( 0.00000f,  1.75000f,  1.87500f),
                        blas::Point3 (-2.00000f,  1.12000f,  1.35000f),
                        blas::Point3 (-1.12000f,  2.00000f,  1.35000f),
                        blas::Point3 ( 0.00000f,  2.00000f,  1.35000f),
                        blas::Point3 (-2.00000f,  1.12000f,  0.90000f),
                        blas::Point3 (-1.12000f,  2.00000f,  0.90000f),
                        blas::Point3 ( 0.00000f,  2.00000f,  0.90000f),
                        blas::Point3 ( 0.98000f,  1.75000f,  1.87500f),
                        blas::Point3 ( 1.75000f,  0.98000f,  1.87500f),   // 80
                        blas::Point3 ( 1.12000f,  2.00000f,  1.35000f),
                        blas::Point3 ( 2.00000f,  1.12000f,  1.35000f),
                        blas::Point3 ( 1.12000f,  2.00000f,  0.90000f),
                        blas::Point3 ( 2.00000f,  1.12000f,  0.90000f),
                        blas::Point3 ( 2.00000f,  0.00000f,  0.45000f),
                        blas::Point3 ( 2.00000f, -1.12000f,  0.45000f),
                        blas::Point3 ( 1.12000f, -2.00000f,  0.45000f),
                        blas::Point3 ( 0.00000f, -2.00000f,  0.45000f),
                        blas::Point3 ( 1.50000f,  0.00000f,  0.22500f),
                        blas::Point3 ( 1.50000f, -0.84000f,  0.22500f),   // 90
                        blas::Point3 ( 0.84000f, -1.50000f,  0.22500f),
                        blas::Point3 ( 0.00000f, -1.50000f,  0.22500f),
                        blas::Point3 ( 1.50000f,  0.00000f,  0.15000f),
                        blas::Point3 ( 1.50000f, -0.84000f,  0.15000f),
                        blas::Point3 ( 0.84000f, -1.50000f,  0.15000f),
                        blas::Point3 ( 0.00000f, -1.50000f,  0.15000f),
                        blas::Point3 (-1.12000f, -2.00000f,  0.45000f),
                        blas::Point3 (-2.00000f, -1.12000f,  0.45000f),
                        blas::Point3 (-2.00000f,  0.00000f,  0.45000f),
                        blas::Point3 (-0.84000f, -1.50000f,  0.22500f),   // 100
                        blas::Point3 (-1.50000f, -0.84000f,  0.22500f),
                        blas::Point3 (-1.50000f,  0.00000f,  0.22500f),
                        blas::Point3 (-0.84000f, -1.50000f,  0.15000f),
                        blas::Point3 (-1.50000f, -0.84000f,  0.15000f),
                        blas::Point3 (-1.50000f,  0.00000f,  0.15000f),
                        blas::Point3 (-2.00000f,  1.12000f,  0.45000f),
                        blas::Point3 (-1.12000f,  2.00000f,  0.45000f),
                        blas::Point3 ( 0.00000f,  2.00000f,  0.45000f),
                        blas::Point3 (-1.50000f,  0.84000f,  0.22500f),
                        blas::Point3 (-0.84000f,  1.50000f,  0.22500f),   // 110
                        blas::Point3 ( 0.00000f,  1.50000f,  0.22500f),
                        blas::Point3 (-1.50000f,  0.84000f,  0.15000f),
                        blas::Point3 (-0.84000f,  1.50000f,  0.15000f),
                        blas::Point3 ( 0.00000f,  1.50000f,  0.15000f),
                        blas::Point3 ( 1.12000f,  2.00000f,  0.45000f),
                        blas::Point3 ( 2.00000f,  1.12000f,  0.45000f),
                        blas::Point3 ( 0.84000f,  1.50000f,  0.22500f),
                        blas::Point3 ( 1.50000f,  0.84000f,  0.22500f),
                        blas::Point3 ( 0.84000f,  1.50000f,  0.15000f),
                        blas::Point3 ( 1.50000f,  0.84000f,  0.15000f),   // 120
                        blas::Point3 (-1.60000f,  0.00000f,  2.02500f),
                        blas::Point3 (-1.60000f, -0.30000f,  2.02500f),
                        blas::Point3 (-1.50000f, -0.30000f,  2.25000f),
                        blas::Point3 (-1.50000f,  0.00000f,  2.25000f),
                        blas::Point3 (-2.30000f,  0.00000f,  2.02500f),
                        blas::Point3 (-2.30000f, -0.30000f,  2.02500f),
                        blas::Point3 (-2.50000f, -0.30000f,  2.25000f),
                        blas::Point3 (-2.50000f,  0.00000f,  2.25000f),
                        blas::Point3 (-2.70000f,  0.00000f,  2.02500f),
                        blas::Point3 (-2.70000f, -0.30000f,  2.02500f),   // 130
                        blas::Point3 (-3.00000f, -0.30000f,  2.25000f),
                        blas::Point3 (-3.00000f,  0.00000f,  2.25000f),
                        blas::Point3 (-2.70000f,  0.00000f,  1.80000f),
                        blas::Point3 (-2.70000f, -0.30000f,  1.80000f),
                        blas::Point3 (-3.00000f, -0.30000f,  1.80000f),
                        blas::Point3 (-3.00000f,  0.00000f,  1.80000f),
                        blas::Point3 (-1.50000f,  0.30000f,  2.25000f),
                        blas::Point3 (-1.60000f,  0.30000f,  2.02500f),
                        blas::Point3 (-2.50000f,  0.30000f,  2.25000f),
                        blas::Point3 (-2.30000f,  0.30000f,  2.02500f),   // 140
                        blas::Point3 (-3.00000f,  0.30000f,  2.25000f),
                        blas::Point3 (-2.70000f,  0.30000f,  2.02500f),
                        blas::Point3 (-3.00000f,  0.30000f,  1.80000f),
                        blas::Point3 (-2.70000f,  0.30000f,  1.80000f),
                        blas::Point3 (-2.70000f,  0.00000f,  1.57500f),
                        blas::Point3 (-2.70000f, -0.30000f,  1.57500f),
                        blas::Point3 (-3.00000f, -0.30000f,  1.35000f),
                        blas::Point3 (-3.00000f,  0.00000f,  1.35000f),
                        blas::Point3 (-2.50000f,  0.00000f,  1.12500f),
                        blas::Point3 (-2.50000f, -0.30000f,  1.12500f),   // 150
                        blas::Point3 (-2.65000f, -0.30000f,  0.93750f),
                        blas::Point3 (-2.65000f,  0.00000f,  0.93750f),
                        blas::Point3 (-2.00000f, -0.30000f,  0.90000f),
                        blas::Point3 (-1.90000f, -0.30000f,  0.60000f),
                        blas::Point3 (-1.90000f,  0.00000f,  0.60000f),
                        blas::Point3 (-3.00000f,  0.30000f,  1.35000f),
                        blas::Point3 (-2.70000f,  0.30000f,  1.57500f),
                        blas::Point3 (-2.65000f,  0.30000f,  0.93750f),
                        blas::Point3 (-2.50000f,  0.30000f,  1.25000f),
                        blas::Point3 (-1.90000f,  0.30000f,  0.60000f),   // 160
                        blas::Point3 (-2.00000f,  0.30000f,  0.90000f),
                        blas::Point3 ( 1.70000f,  0.00000f,  1.42500f),
                        blas::Point3 ( 1.70000f, -0.66000f,  1.42500f),
                        blas::Point3 ( 1.70000f, -0.66000f,  0.60000f),
                        blas::Point3 ( 1.70000f,  0.00000f,  0.60000f),
                        blas::Point3 ( 2.60000f,  0.00000f,  1.42500f),
                        blas::Point3 ( 2.60000f, -0.66000f,  1.42500f),
                        blas::Point3 ( 3.10000f, -0.66000f,  0.82500f),
                        blas::Point3 ( 3.10000f,  0.00000f,  0.82500f),
                        blas::Point3 ( 2.30000f,  0.00000f,  2.10000f),   // 170
                        blas::Point3 ( 2.30000f, -0.25000f,  2.10000f),
                        blas::Point3 ( 2.40000f, -0.25000f,  2.02500f),
                        blas::Point3 ( 2.40000f,  0.00000f,  2.02500f),
                        blas::Point3 ( 2.70000f,  0.00000f,  2.40000f),
                        blas::Point3 ( 2.70000f, -0.25000f,  2.40000f),
                        blas::Point3 ( 3.30000f, -0.25000f,  2.40000f),
                        blas::Point3 ( 3.30000f,  0.00000f,  2.40000f),
                        blas::Point3 ( 1.70000f,  0.66000f,  0.60000f),
                        blas::Point3 ( 1.70000f,  0.66000f,  1.42500f),
                        blas::Point3 ( 3.10000f,  0.66000f,  0.82500f),   // 180
                        blas::Point3 ( 2.60000f,  0.66000f,  1.42500f),
                        blas::Point3 ( 2.40000f,  0.25000f,  2.02500f),
                        blas::Point3 ( 2.30000f,  0.25000f,  2.10000f),
                        blas::Point3 ( 3.30000f,  0.25000f,  2.40000f),
                        blas::Point3 ( 2.70000f,  0.25000f,  2.40000f),
                        blas::Point3 ( 2.80000f,  0.00000f,  2.47500f),
                        blas::Point3 ( 2.80000f, -0.25000f,  2.47500f),
                        blas::Point3 ( 3.52500f, -0.25000f,  2.49375f),
                        blas::Point3 ( 3.52500f,  0.00000f,  2.49375f),
                        blas::Point3 ( 2.90000f,  0.00000f,  2.47500f),   // 190
                        blas::Point3 ( 2.90000f, -0.15000f,  2.47500f),
                        blas::Point3 ( 3.45000f, -0.15000f,  2.51250f),
                        blas::Point3 ( 3.45000f,  0.00000f,  2.51250f),
                        blas::Point3 ( 2.80000f,  0.00000f,  2.40000f),
                        blas::Point3 ( 2.80000f, -0.15000f,  2.40000f),
                        blas::Point3 ( 3.20000f, -0.15000f,  2.40000f),
                        blas::Point3 ( 3.20000f,  0.00000f,  2.40000f),
                        blas::Point3 ( 3.52500f,  0.25000f,  2.49375f),
                        blas::Point3 ( 2.80000f,  0.25000f,  2.47500f),
                        blas::Point3 ( 3.45000f,  0.15000f,  2.51250f),   // 200
                        blas::Point3 ( 2.90000f,  0.15000f,  2.47500f),
                        blas::Point3 ( 3.20000f,  0.15000f,  2.40000f),
                        blas::Point3 ( 2.80000f,  0.15000f,  2.40000f),
                        blas::Point3 ( 0.00000f,  0.00000f,  3.15000f),
                        blas::Point3 ( 0.00000f, -0.00200f,  3.15000f),
                        blas::Point3 ( 0.02000f,  0.00000f,  3.15000f),
                        blas::Point3 ( 0.80000f,  0.00000f,  3.15000f),
                        blas::Point3 ( 0.80000f, -0.45000f,  3.15000f),
                        blas::Point3 ( 0.45000f, -0.80000f,  3.15000f),
                        blas::Point3 ( 0.00000f, -0.80000f,  3.15000f),   // 210
                        blas::Point3 ( 0.00000f,  0.00000f,  2.85000f),
                        blas::Point3 ( 0.20000f,  0.00000f,  2.70000f),
                        blas::Point3 ( 0.20000f, -0.11200f,  2.70000f),
                        blas::Point3 ( 0.11200f, -0.20000f,  2.70000f),
                        blas::Point3 ( 0.00000f, -0.20000f,  2.70000f),
                        blas::Point3 (-0.00200f,  0.00000f,  3.15000f),
                        blas::Point3 (-0.45000f, -0.80000f,  3.15000f),
                        blas::Point3 (-0.80000f, -0.45000f,  3.15000f),
                        blas::Point3 (-0.80000f,  0.00000f,  3.15000f),
                        blas::Point3 (-0.11200f, -0.20000f,  2.70000f),   // 220
                        blas::Point3 (-0.20000f, -0.11200f,  2.70000f),
                        blas::Point3 (-0.20000f,  0.00000f,  2.70000f),
                        blas::Point3 ( 0.00000f,  0.00200f,  3.15000f),
                        blas::Point3 (-0.80000f,  0.45000f,  3.15000f),
                        blas::Point3 (-0.45000f,  0.80000f,  3.15000f),
                        blas::Point3 ( 0.00000f,  0.80000f,  3.15000f),
                        blas::Point3 (-0.20000f,  0.11200f,  2.70000f),
                        blas::Point3 (-0.11200f,  0.20000f,  2.70000f),
                        blas::Point3 ( 0.00000f,  0.20000f,  2.70000f),
                        blas::Point3 ( 0.45000f,  0.80000f,  3.15000f),   // 230
                        blas::Point3 ( 0.80000f,  0.45000f,  3.15000f),
                        blas::Point3 ( 0.11200f,  0.20000f,  2.70000f),
                        blas::Point3 ( 0.20000f,  0.11200f,  2.70000f),
                        blas::Point3 ( 0.40000f,  0.00000f,  2.55000f),
                        blas::Point3 ( 0.40000f, -0.22400f,  2.55000f),
                        blas::Point3 ( 0.22400f, -0.40000f,  2.55000f),
                        blas::Point3 ( 0.00000f, -0.40000f,  2.55000f),
                        blas::Point3 ( 1.30000f,  0.00000f,  2.55000f),
                        blas::Point3 ( 1.30000f, -0.72800f,  2.55000f),
                        blas::Point3 ( 0.72800f, -1.30000f,  2.55000f),   // 240
                        blas::Point3 ( 0.00000f, -1.30000f,  2.55000f),
                        blas::Point3 ( 1.30000f,  0.00000f,  2.40000f),
                        blas::Point3 ( 1.30000f, -0.72800f,  2.40000f),
                        blas::Point3 ( 0.72800f, -1.30000f,  2.40000f),
                        blas::Point3 ( 0.00000f, -1.30000f,  2.40000f),
                        blas::Point3 (-0.22400f, -0.40000f,  2.55000f),
                        blas::Point3 (-0.40000f, -0.22400f,  2.55000f),
                        blas::Point3 (-0.40000f,  0.00000f,  2.55000f),
                        blas::Point3 (-0.72800f, -1.30000f,  2.55000f),
                        blas::Point3 (-1.30000f, -0.72800f,  2.55000f),   // 250
                        blas::Point3 (-1.30000f,  0.00000f,  2.55000f),
                        blas::Point3 (-0.72800f, -1.30000f,  2.40000f),
                        blas::Point3 (-1.30000f, -0.72800f,  2.40000f),
                        blas::Point3 (-1.30000f,  0.00000f,  2.40000f),
                        blas::Point3 (-0.40000f,  0.22400f,  2.55000f),
                        blas::Point3 (-0.22400f,  0.40000f,  2.55000f),
                        blas::Point3 ( 0.00000f,  0.40000f,  2.55000f),
                        blas::Point3 (-1.30000f,  0.72800f,  2.55000f),
                        blas::Point3 (-0.72800f,  1.30000f,  2.55000f),
                        blas::Point3 ( 0.00000f,  1.30000f,  2.55000f),   // 260
                        blas::Point3 (-1.30000f,  0.72800f,  2.40000f),
                        blas::Point3 (-0.72800f,  1.30000f,  2.40000f),
                        blas::Point3 ( 0.00000f,  1.30000f,  2.40000f),
                        blas::Point3 ( 0.22400f,  0.40000f,  2.55000f),
                        blas::Point3 ( 0.40000f,  0.22400f,  2.55000f),
                        blas::Point3 ( 0.72800f,  1.30000f,  2.55000f),
                        blas::Point3 ( 1.30000f,  0.72800f,  2.55000f),
                        blas::Point3 ( 0.72800f,  1.30000f,  2.40000f),
                        blas::Point3 ( 1.30000f,  0.72800f,  2.40000f),
                        blas::Point3 ( 0.00000f,  0.00000f,  0.00000f),   // 270
                        blas::Point3 ( 1.50000f,  0.00000f,  0.15000f),
                        blas::Point3 ( 1.50000f,  0.84000f,  0.15000f),
                        blas::Point3 ( 0.84000f,  1.50000f,  0.15000f),
                        blas::Point3 ( 0.00000f,  1.50000f,  0.15000f),
                        blas::Point3 ( 1.50000f,  0.00000f,  0.07500f),
                        blas::Point3 ( 1.50000f,  0.84000f,  0.07500f),
                        blas::Point3 ( 0.84000f,  1.50000f,  0.07500f),
                        blas::Point3 ( 0.00000f,  1.50000f,  0.07500f),
                        blas::Point3 ( 1.42500f,  0.00000f,  0.00000f),
                        blas::Point3 ( 1.42500f,  0.79800f,  0.00000f),   // 280
                        blas::Point3 ( 0.79800f,  1.42500f,  0.00000f),
                        blas::Point3 ( 0.00000f,  1.42500f,  0.00000f),
                        blas::Point3 (-0.84000f,  1.50000f,  0.15000f),
                        blas::Point3 (-1.50000f,  0.84000f,  0.15000f),
                        blas::Point3 (-1.50000f,  0.00000f,  0.15000f),
                        blas::Point3 (-0.84000f,  1.50000f,  0.07500f),
                        blas::Point3 (-1.50000f,  0.84000f,  0.07500f),
                        blas::Point3 (-1.50000f,  0.00000f,  0.07500f),
                        blas::Point3 (-0.79800f,  1.42500f,  0.00000f),
                        blas::Point3 (-1.42500f,  0.79800f,  0.00000f),   // 290
                        blas::Point3 (-1.42500f,  0.00000f,  0.00000f),
                        blas::Point3 (-1.50000f, -0.84000f,  0.15000f),
                        blas::Point3 (-0.84000f, -1.50000f,  0.15000f),
                        blas::Point3 ( 0.00000f, -1.50000f,  0.15000f),
                        blas::Point3 (-1.50000f, -0.84000f,  0.07500f),
                        blas::Point3 (-0.84000f, -1.50000f,  0.07500f),
                        blas::Point3 ( 0.00000f, -1.50000f,  0.07500f),
                        blas::Point3 (-1.42500f, -0.79800f,  0.00000f),
                        blas::Point3 (-0.79800f, -1.42500f,  0.00000f),
                        blas::Point3 ( 0.00000f, -1.42500f,  0.00000f),   // 300
                        blas::Point3 ( 0.84000f, -1.50000f,  0.15000f),
                        blas::Point3 ( 1.50000f, -0.84000f,  0.15000f),
                        blas::Point3 ( 0.84000f, -1.50000f,  0.07500f),
                        blas::Point3 ( 1.50000f, -0.84000f,  0.07500f),
                        blas::Point3 ( 0.79800f, -1.42500f,  0.00000f),
                        blas::Point3 ( 1.42500f, -0.79800f,  0.00000f),
                    };

                    const util::ui32 body[12][16] = {
                        {  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15},
                        {  3,  16,  17,  18,   7,  19,  20,  21,  11,  22,  23,  24,  15,  25,  26,  27},
                        { 18,  28,  29,  30,  21,  31,  32,  33,  24,  34,  35,  36,  27,  37,  38,  39},
                        { 30,  40,  41,   0,  33,  42,  43,   4,  36,  44,  45,   8,  39,  46,  47,  12},
                        { 12,  13,  14,  15,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59},
                        { 15,  25,  26,  27,  51,  60,  61,  62,  55,  63,  64,  65,  59,  66,  67,  68},
                        { 27,  37,  38,  39,  62,  69,  70,  71,  65,  72,  73,  74,  68,  75,  76,  77},
                        { 39,  46,  47,  12,  71,  78,  79,  48,  74,  80,  81,  52,  77,  82,  83,  56},
                        { 56,  57,  58,  59,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95},
                        { 59,  66,  67,  68,  87,  96,  97,  98,  91,  99, 100, 101,  95, 102, 103, 104},
                        { 68,  75,  76,  77,  98, 105, 106, 107, 101, 108, 109, 110, 104, 111, 112, 113},
                        { 77,  82,  83,  56, 107, 114, 115,  84, 110, 116, 117,  88, 113, 118, 119,  92},
                    };

                    const util::ui32 handle[4][16] = {
                        {120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135},
                        {123, 136, 137, 120, 127, 138, 139, 124, 131, 140, 141, 128, 135, 142, 143, 132},
                        {132, 133, 134, 135, 144, 145, 146, 147, 148, 149, 150, 151,  68, 152, 153, 154},
                        {135, 142, 143, 132, 147, 155, 156, 144, 151, 157, 158, 148, 154, 159, 160,  68},
                    };

                    const util::ui32 spout[4][16] = {
                        {161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176},
                        {164, 177, 178, 161, 168, 179, 180, 165, 172, 181, 182, 169, 176, 183, 184, 173},
                        {173, 174, 175, 176, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196},
                        {176, 183, 184, 173, 188, 197, 198, 185, 192, 199, 200, 189, 196, 201, 202, 193},
                    };

                    const util::ui32 lid[8][16] = {
                        {203, 203, 203, 203, 206, 207, 208, 209, 210, 210, 210, 210, 211, 212, 213, 214},
                        {203, 203, 203, 203, 209, 216, 217, 218, 210, 210, 210, 210, 214, 219, 220, 221},
                        {203, 203, 203, 203, 218, 223, 224, 225, 210, 210, 210, 210, 221, 226, 227, 228},
                        {203, 203, 203, 203, 225, 229, 230, 206, 210, 210, 210, 210, 228, 231, 232, 211},
                        {211, 212, 213, 214, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244},
                        {214, 219, 220, 221, 236, 245, 246, 247, 240, 248, 249, 250, 244, 251, 252, 253},
                        {221, 226, 227, 228, 247, 254, 255, 256, 250, 257, 258, 259, 253, 260, 261, 262},
                        {228, 231, 232, 211, 256, 263, 264, 233, 259, 265, 266, 237, 262, 267, 268, 241},
                    };

                    const util::ui32 bottom[4][16] = {
                        {269, 269, 269, 269, 278, 279, 280, 281, 274, 275, 276, 277,  92, 119, 118, 113},
                        {269, 269, 269, 269, 281, 288, 289, 290, 277, 285, 286, 287, 113, 112, 111, 104},
                        {269, 269, 269, 269, 290, 297, 298, 299, 287, 294, 295, 296, 104, 103, 102,  95},
                        {269, 269, 269, 269, 299, 304, 305, 278, 296, 302, 303, 274,  95,  94,  93,  92},
                    };

                    const blas::Matrix4 Basis (
                        blas::Point4 (-1.0f,  3.0f, -3.0f,  1.0f),
                        blas::Point4 ( 3.0f, -6.0f,  3.0f,  0.0f),
                        blas::Point4 (-3.0f,  3.0f,  0.0f,  0.0f),
                        blas::Point4 ( 1.0f,  0.0f,  0.0f,  0.0f)
                    );

                    void SplitPatch (
                            std::vector<blas::Point3> &vertices,
                            std::vector<io::Mesh::Face> &faces,
                            const util::ui32 *patch,
                            util::ui32 usegs,
                            util::ui32 vsegs) {
                        blas::Matrix4 xm;
                        blas::Matrix4 ym;
                        blas::Matrix4 zm;
                        {
                            for (util::ui32 i = 0; i < 4; ++i) {
                                for (util::ui32 j = 0; j < 4; ++j) {
                                    xm[i][j] = verts[patch[i * 4 + j]][0];
                                    ym[i][j] = verts[patch[i * 4 + j]][1];
                                    zm[i][j] = verts[patch[i * 4 + j]][2];
                                }
                            }
                            xm = Basis * xm * Basis;
                            ym = Basis * ym * Basis;
                            zm = Basis * zm * Basis;
                        }
                        {
                            util::ui32 vertexCount = vertices.size ();
                            util::f32 udelta = 1.0f / usegs;
                            util::f32 vdelta = 1.0f / vsegs;
                            util::f32 u = 0.0f;
                            for (util::ui32 i = 0; i <= usegs; ++i, u += udelta) {
                                blas::Point4 upt (u * u * u, u * u, u, 1.0f);
                                blas::Point4 xpt (upt * xm);
                                blas::Point4 ypt (upt * ym);
                                blas::Point4 zpt (upt * zm);
                                util::f32 v = 0.0f;
                                for (util::ui32 j = 0; j <= vsegs; ++j, v += vdelta) {
                                    blas::Point4 vpt (v * v * v, v * v, v, 1);
                                    vertices.push_back (blas::Point3 (blas::Dot (xpt, vpt),
                                        blas::Dot (ypt, vpt), blas::Dot (zpt, vpt)));
                                    if (i < usegs && j < vsegs) {
                                        faces.push_back (io::Mesh::Face (
                                            vertexCount + i * (vsegs + 1) + j,
                                            vertexCount + i * (vsegs + 1) + j + 1,
                                            vertexCount + (i + 1) * (vsegs + 1) + j,
                                            io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeCAVisible));
                                        faces.push_back (io::Mesh::Face (
                                            vertexCount + i * (vsegs + 1) + j + 1,
                                            vertexCount + (i + 1) * (vsegs + 1) + j + 1,
                                            vertexCount + (i + 1) * (vsegs + 1) + j,
                                            io::Mesh::Face::EdgeABVisible | io::Mesh::Face::EdgeBCVisible));
                                    }
                                }
                            }
                        }
                    }
                }

                io::Mesh::UniquePtr Mesh::CreateTeapot (
                        const std::string &name,
                        util::ui32 usegs,
                        util::ui32 vsegs,
                        const blas::Matrix3 &xform,
                        bool weld,
                        bool smooth) {
                    io::Mesh::UniquePtr mesh (new io::Mesh (name));
                    assert (mesh.get () != 0);
                    if (mesh.get () != 0) {
                        std::vector<blas::Point3> &vertices = mesh->vertices;
                        vertices.reserve ((usegs + 1) * (vsegs + 1) * (12 + 4 + 4 + 8 + 4));
                        std::vector<io::Mesh::Face> &faces = mesh->faces;
                        faces.reserve (usegs * vsegs * (12 + 4 + 4 + 8 + 4) * 2);
                        {
                            for (util::ui32 i = 0; i < 12; ++i) {
                                SplitPatch (vertices, faces, body[i], usegs, vsegs);
                            }
                        }
                        {
                            for (util::ui32 i = 0; i < 4; ++i) {
                                SplitPatch (vertices, faces, handle[i], usegs, vsegs);
                            }
                        }
                        {
                            for (util::ui32 i = 0; i < 4; ++i) {
                                SplitPatch (vertices, faces, spout[i], usegs, vsegs);
                            }
                        }
                        {
                            for (util::ui32 i = 0; i < 8; ++i) {
                                SplitPatch (vertices, faces, lid[i], usegs, vsegs);
                            }
                        }
                        {
                            for (util::ui32 i = 0; i < 4; ++i) {
                                SplitPatch (vertices, faces, bottom[i], usegs, vsegs);
                            }
                        }
                        if (smooth) {
                            std::vector<util::ui32> &faceSmoothGroups = mesh->faceSmoothGroups;
                            faceSmoothGroups.resize (faces.size ());
                            for (std::size_t i = 0, count = faceSmoothGroups.size (); i < count; ++i) {
                                faceSmoothGroups[i] = 1;
                            }
                        }
                        if (weld) {
                            // FIXME: implement (Lofter.cpp)
                            assert (0);
                            //mesh->WeldVerts ();
                        }
                        mesh->xform = xform;
                    }
                    return mesh;
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
