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

#if !defined (__thekogans_mex_3ds_io_View_h)
#define __thekogans_mex_3ds_io_View_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/FixedString.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL View {
                    typedef std::unique_ptr<View> UniquePtr;

                    enum Flags {
                        Enabled = 0x0001,       // View is enabled.
                        UseSnap = 0x0002,       // Cursor is snapped to current snap values.
                        UseGrid = 0x0004,       // Displays a grid of dots.
                        ShapeFlag = 0x0008,     // Shape view (combined with type == Top)
                        Fast = 0x0010,          // View is in fast mode (draw partial objects).
                        Solid = 0x0020,         // View is in solid mode (draw solid objects).
                        SafeFrame = 0x0040,     // Display a safe area frame.
                        Background = 0x0080,    // Display a bitmap background.
                        ScrollLock = 0x0100     // View does not scroll.
                    };
                    util::ui16 flags;
                    util::ui16 axisLockout;
                    blas::Size size;
                    enum Type {
                        None,
                        Top,
                        Bottom,
                        Left,
                        Right,
                        Front,
                        Back,
                        User,
                        Shape,
                        SpotLight = 0x0012,
                        Camera = 0xffff
                    };
                    util::ui16 type;
                    util::f32 zoom;
                    blas::Point3 worldOrigin;
                    util::f32 horizontalAngle;
                    util::f32 verticalAngle;
                    FixedString name;

                    View () :
                        flags (0),
                        axisLockout (0),
                        size (blas::Size::Empty),
                        type (None),
                        zoom (0.7f),
                        worldOrigin (blas::Point3::Zero),
                        horizontalAngle (30.0f),
                        verticalAngle (20.0f),
                        name (11) {}
                    View (util::ui16 flags_,
                        util::ui16 axisLockout_,
                        const blas::Size &size_,
                        util::ui16 type_,
                        util::f32 zoom_,
                        blas::Point3 worldOrigin_,
                        util::f32 horizontalAngle_,
                        util::f32 verticalAngle_,
                        const char *name_) :
                        flags (flags_),
                        axisLockout (axisLockout_),
                        size (size_),
                        type (type_),
                        zoom (zoom_),
                        worldOrigin (worldOrigin_),
                        horizontalAngle (horizontalAngle_),
                        verticalAngle (verticalAngle_),
                        name (name_, 11) {}

                    inline void Swap (View &view) {
                        std::swap (flags, view.flags);
                        std::swap (axisLockout, view.axisLockout);
                        size.Swap (view.size);
                        std::swap (type, view.type);
                        std::swap (zoom, view.zoom);
                        std::swap (worldOrigin, view.worldOrigin);
                        std::swap (horizontalAngle, view.horizontalAngle);
                        std::swap (verticalAngle, view.verticalAngle);
                        name.value.swap (view.name.value);
                    }

                    inline bool IsEnabled () const {
                        return util::Flags16 (flags).Test (Enabled);
                    }
                    inline void SetEnabled (bool enabled) {
                        util::Flags16 (flags).Set (Enabled, enabled);
                    }
                    inline bool IsUseSnap () const {
                        return util::Flags16 (flags).Test (UseSnap);
                    }
                    inline void SetUseSnap (bool useSnap) {
                        util::Flags16 (flags).Set (UseSnap, useSnap);
                    }
                    inline bool IsUseGrid () const {
                        return util::Flags16 (flags).Test (UseGrid);
                    }
                    inline void SetUseGrid (bool useGrid) {
                        util::Flags16 (flags).Set (UseGrid, useGrid);
                    }
                    inline bool IsShapeFlag () const {
                        return util::Flags16 (flags).Test (ShapeFlag);
                    }
                    inline void SetShapeFlag (bool shapeFlag) {
                        util::Flags16 (flags).Set (ShapeFlag, shapeFlag);
                    }
                    inline bool IsFast () const {
                        return util::Flags16 (flags).Test (Fast);
                    }
                    inline void SetFast (bool fast) {
                        util::Flags16 (flags).Set (Fast, fast);
                    }
                    inline bool IsSolid () const {
                        return util::Flags16 (flags).Test (Solid);
                    }
                    inline void SetSolid (bool fast) {
                        util::Flags16 (flags).Set (Solid, fast);
                    }
                    inline bool IsSafeFrame () const {
                        return util::Flags16 (flags).Test (SafeFrame);
                    }
                    inline void SetSafeFrame (bool safeFrame) {
                        util::Flags16 (flags).Set (SafeFrame, safeFrame);
                    }
                    inline bool IsBackground () const {
                        return util::Flags16 (flags).Test (Background);
                    }
                    inline void SetBackground (bool background) {
                        util::Flags16 (flags).Set (Background, background);
                    }
                    inline bool IsScrollLock () const {
                        return util::Flags16 (flags).Test (ScrollLock);
                    }
                    inline void SetScrollLock (bool scrollLock) {
                        util::Flags16 (flags).Set (ScrollLock, scrollLock);
                    }

                    inline bool IsNone () const {
                        return type == None;
                    }

                    inline bool IsShape () const {
                        return type == Shape ||
                            (IsShapeFlag () && type == Top);
                    }
                    inline bool IsOrthographic () const {
                        return type == Top || type == Bottom ||
                            type == Left || type == Right ||
                            type == Front || type == Back ||
                            type == User;
                    }
                    inline bool IsUser () const {
                        return type == User;
                    }

                    inline bool IsPerspective () const {
                        return type == SpotLight || type == Camera;
                    }
                    inline bool IsSpotLight () const {
                        return type == SpotLight;
                    }
                    inline bool IsCamera () const {
                        return type == Camera;
                    }

                    inline bool IsValid () const {
                        // type
                        if (!IsNone () && !IsShape () &&
                                !IsOrthographic () && !IsPerspective ()) {
                            return false;
                        }
                        // zoom
                        if ((IsShape () || IsOrthographic ()) &&
                                (zoom < 0.0000001f || zoom > 1000000.0f)) {
                            return false;
                        }
                        // horizontalAngle/verticalAngle
                        if (IsUser () &&
                                (horizontalAngle < -180.0f || horizontalAngle > 180.0f) &&
                                (verticalAngle < -180.0f || verticalAngle > 180.0f)) {
                            return false;
                        }
                        // name
                        if (IsPerspective () && name.value.empty ()) {
                            return false;
                        }
                        return true;
                    }
                };

                // Helper classes for creating specific view types.
                struct NoneView : public View {
                    explicit NoneView (const blas::Size &size) :
                        View (0, 0, size, None, 0.7f,
                            blas::Point3::Zero, 0.0f, 0.0f, "") {}
                };

                struct TopView : public View {
                    explicit TopView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled, 0, size, Top, zoom,
                            worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct BottomView : public View {
                    explicit BottomView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled, 0, size, Bottom, zoom,
                            worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct LeftView : public View {
                    explicit LeftView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled, 0, size, Left, zoom,
                            worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct RightView : public View {
                    explicit RightView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled, 0, size, Right, zoom,
                            worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct FrontView : public View {
                    explicit FrontView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled, 0, size, Front, zoom,
                            worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct BackView : public View {
                    explicit BackView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled, 0, size, Back, zoom, worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct UserView : public View {
                    explicit UserView (
                        const blas::Size &size,
                        util::f32 zoom = 2.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero,
                        util::f32 horizontalAngle = 20.0f,
                        util::f32 verticalAngle = 30.0f) :
                        View (Enabled, 0, size, User, zoom,
                            worldOrigin, horizontalAngle, verticalAngle, "") {}
                };

                struct ShapeView : public View {
                    explicit ShapeView (
                        const blas::Size &size,
                        util::f32 zoom = 1.0f,
                        const blas::Point3 &worldOrigin = blas::Point3::Zero) :
                        View (Enabled | ShapeFlag, 0, size, Top,
                            zoom, worldOrigin, 0.0f, 0.0f, "") {}
                };

                struct SpotLightView : public View {
                    SpotLightView (
                        const blas::Size &size,
                        const char *name) :
                        View (Enabled, 0, size, SpotLight, 1.0f,
                            blas::Point3::Zero, 0.0f, 0.0f, name) {}
                };

                struct CameraView : public View {
                    CameraView (
                        const blas::Size &size,
                        const char *name) :
                        View (Enabled, 0, size, Camera, 1.0f,
                            blas::Point3::Zero, 0.0f, 0.0f, name) {}
                };

                inline ChunkReader &operator >> (ChunkReader &chunk, View &view) {
                    chunk >> view.flags >> view.axisLockout >>
                        view.size >> view.type >> view.zoom >>
                        view.worldOrigin >> view.horizontalAngle >>
                        view.verticalAngle >> view.name;
                    return chunk;
                }

                inline ChunkWriter &operator << (ChunkWriter &chunk, const View &view) {
                    chunk << view.flags << view.axisLockout <<
                        view.size << view.type << view.zoom <<
                        view.worldOrigin << view.horizontalAngle <<
                        view.verticalAngle << view.name;
                    return chunk;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_View_h)
