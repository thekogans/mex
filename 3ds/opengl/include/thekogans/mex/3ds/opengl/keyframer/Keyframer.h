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

#if !defined (__thekogans_mex_3ds_opengl_keyframer_Keyframer_h)
#define __thekogans_mex_3ds_opengl_keyframer_Keyframer_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Heap.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/util/OwnerMap.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/io/editor/Light.h"
#include "thekogans/mex/3ds/ext/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/opengl/Config.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/Module.h"
#include "thekogans/mex/3ds/opengl/View.h"
#include "thekogans/mex/3ds/opengl/keyframer/WireObjectNode.h"
#include "thekogans/mex/3ds/opengl/keyframer/SolidObjectNode.h"
#include "thekogans/mex/3ds/opengl/keyframer/LightNode.h"
#include "thekogans/mex/3ds/opengl/keyframer/SpotLightNode.h"
#include "thekogans/mex/3ds/opengl/keyframer/CameraNode.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                struct Project;

                struct _LIB_THEKOGANS_MEX_3DS_OPENGL_DECL Keyframer : public Module {
                    typedef std::unique_ptr<Keyframer> UniquePtr;

                    THEKOGANS_UTIL_DECLARE_HEAP (Keyframer)

                    util::OwnerMap<std::string, Material> materials;
                    util::OwnerVector<WireObjectNode> wireObjectNodes;
                    util::OwnerVector<SolidObjectNode> solidObjectNodes;
                    util::OwnerVector<LightNode> lightNodes;
                    util::OwnerVector<SpotLightNode> spotLightNodes;
                    util::OwnerVector<CameraNode> cameraNodes;

                    explicit Keyframer (Project &project) : Module (project) {}
                    explicit Keyframer (const ext::Keyframer &keyframer);

                    virtual View::UniquePtr GetView (const io::View &view) const;
                    virtual View::UniquePtr GetCurrView () const;
                    virtual void Draw (const View &view) const;

                    void Swap (Keyframer &keyframer);

                    void SetCurrentFrame ();

                private:
                    Keyframer (const Keyframer &);
                    Keyframer &operator = (const Keyframer &);

                    io::Light::UniquePtr GetLight (const std::string &name) const;
                    void GetLights (io::Color &ambientLight, util::OwnerVector<io::Light> &lights) const;
                    io::Camera::UniquePtr GetCamera (const std::string &name) const;
                };

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_opengl_keyframer_Keyframer_h)
