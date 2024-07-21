// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_opengl.
//
// libthekogans_mex_3ds_opengl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_opengl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_opengl. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_opengl_lofter_Lofter_h)
#define __thekogans_mex_3ds_opengl_lofter_Lofter_h

#include <memory>
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/lofter/Lofter.h"
#include "thekogans/mex/3ds/ext/lofter/Lofter.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/Module.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/shaper/BezierPolygon.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct Project;

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Lofter : public Module {
                    typedef std::unique_ptr<Lofter> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_STD_ALLOCATOR_FUNCTIONS

                    struct Path {
                        const io::Lofter::Path &path;

                        explicit Path (const io::Lofter::Path &path_) : path (path_) {}

                        void Draw (const View &view) const;
                    };

                    explicit Lofter (Project &project) : Module (project) {}
                    explicit Lofter (const ext::Lofter &lofter);

                    virtual View::UniquePtr GetView (const io::View &view) const;
                    virtual View::UniquePtr GetCurrView () const;
                    virtual void Draw (const View &view) const;

                    void Swap (Lofter &lofter);

                private:
                    void DrawShape (const View &view, const ext::Lofter::Shape &shape, bool first) const;

                    Lofter (const Lofter &);
                    Lofter &operator = (const Lofter &);
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_lofter_Lofter_h)
