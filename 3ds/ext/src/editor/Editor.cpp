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

#include <string>
#include <cctype>
#include "thekogans/util/Flags.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/3ds/io/Project.h"
#include "thekogans/mex/3ds/ext/View.h"
#include "thekogans/mex/3ds/ext/editor/Mesh.h"
#include "thekogans/mex/3ds/ext/editor/Light.h"
#include "thekogans/mex/3ds/ext/editor/Camera.h"
#include "thekogans/mex/3ds/ext/editor/Editor.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                View::UniquePtr Editor::GetView (const io::View &view) const {
                    if (view.IsNone () || view.IsOrthographic ()) {
                        return View::UniquePtr (new View (view));
                    }
                    else if (view.IsSpotLight ()) {
                        const io::Light *light = ioEditor ().GetLight (view.name.value);
                        assert (light != 0);
                        if (light != 0) {
                            assert (light->spot.get () != 0);
                            return View::UniquePtr (new View (view, *light));
                        }
                    }
                    else if (view.IsCamera ()) {
                        const io::Camera *camera = ioEditor ().GetCamera (view.name.value);
                        assert (camera != 0);
                        if (camera != 0) {
                            return View::UniquePtr (new View (view, *camera));
                        }
                    }
                    assert (0);
                    return View::UniquePtr ();
                }

                View::UniquePtr Editor::GetCurrView () const {
                    return GetView (ioEditor ().viewLayout.GetCurrView ());
                }

                blas::Bound2 Editor::GetViewBound (const View &view) const {
                    blas::Bound2 bound = blas::Bound2::Empty;
                    {
                        typedef util::OwnerVector<io::Mesh>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().meshes.begin (), end = ioEditor ().meshes.end (); it != end; ++it) {
                            bound += Mesh (**it).GetBound2 (view.xform);
                        }
                    }
                    if (ioEditor ().project.display.showLights) {
                        typedef util::OwnerVector<io::Light>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().lights.begin (), end = ioEditor ().lights.end (); it != end; ++it) {
                            bound += Light (**it).GetBound2 (view.xform);
                        }
                    }
                    if (ioEditor ().project.display.showCameras) {
                        typedef util::OwnerVector<io::Camera>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().cameras.begin (), end = ioEditor ().cameras.end (); it != end; ++it) {
                            bound += Camera (**it).GetBound2 (view.xform);
                        }
                    }
                    return bound;
                }

                bool Editor::CheckName (const std::string &name) const {
                    {
                        typedef util::OwnerVector<io::Mesh>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().meshes.begin (), end = ioEditor ().meshes.end (); it != end; ++it) {
                            if ((*it)->name == name) {
                                return true;
                            }
                        }
                    }
                    {
                        typedef util::OwnerVector<io::Light>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().lights.begin (), end = ioEditor ().lights.end (); it != end; ++it) {
                            if ((*it)->name == name) {
                                return true;
                            }
                        }
                    }
                    {
                        typedef util::OwnerVector<io::Camera>::const_iterator const_iterator;
                        for (const_iterator it = ioEditor ().cameras.begin (), end = ioEditor ().cameras.end (); it != end; ++it) {
                            if ((*it)->name == name) {
                                return true;
                            }
                        }
                    }
                    return false;
                }

                namespace {
                    std::string IncName (const std::string &name) {
                        assert (!name.empty ());
                        std::size_t length = name.size ();
                        while (length > 0 && isdigit (name[--length]));
                        if (!isdigit (name[length])) {
                            ++length;
                        }
                        util::ui32 inc = 1;
                        if (length < name.size ()) {
                            inc = util::stringToui32 (&name[length]) + 1;
                        }
                        return name.substr (0, length) + util::ui32Tostring (inc, "%02d");
                    }
                }

                std::string Editor::GetUniqueName (const std::string &name) const {
                    std::string uniqueName = IncName (name);
                    while (CheckName (uniqueName)) {
                        uniqueName = IncName (uniqueName);
                    }
                    return uniqueName;
                }

                void Editor::Swap (Editor &editor) {
                    // FIXME: imlement
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
