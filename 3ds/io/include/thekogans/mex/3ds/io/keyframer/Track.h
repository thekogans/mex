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

#if !defined (__thekogans_mex_3ds_io_keyframer_Track_h)
#define __thekogans_mex_3ds_io_keyframer_Track_h

#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/Serializer.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/blas/AngleAxis.h"
#include "thekogans/mex/3ds/io/Config.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                template<typename T>
                struct Track {
                    enum {
                        Single = 0x0000,
                        Repeats = 0x0002,
                        Loops = 0x0003,
                        LockX = 0x0008,
                        LockY = 0x0010,
                        LockZ = 0x0020,
                        Lock = LockX | LockY | LockZ,
                        NoLinkX = 0x0080,
                        NoLinkY = 0x0100,
                        NoLinkZ = 0x0200,
                        NoLink = NoLinkX | NoLinkY | NoLinkZ
                    };
                    util::ui16 flags;
                    struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Key {
                        util::ui32 frame;
                        enum {
                            UsesNone = 0x0000,
                            UsesTension = 0x0001,
                            UsesContinuity = 0x0002,
                            UsesBias = 0x0004,
                            UsesEaseTo = 0x0008,
                            UsesEaseFrom = 0x0010
                        };
                        util::ui16 flags;
                        util::f32 tension;
                        util::f32 continuity;
                        util::f32 bias;
                        util::f32 easeFrom;
                        util::f32 easeTo;
                        T value;

                        // NOTE: Value is not initialized.
                        Key () :
                            frame (0),
                            flags (UsesNone),
                            tension (0.0f),
                            continuity (0.0f),
                            bias (0.0f),
                            easeFrom (0.0f),
                            easeTo (0.0f) {}
                        // Standard key creating ctor.
                        Key (util::ui32 frame_,
                            const T &value_) :
                            frame (frame_),
                            flags (UsesNone),
                            tension (0.0f),
                            continuity (0.0f),
                            bias (0.0f),
                            easeFrom (0.0f),
                            easeTo (0.0f),
                            value (value_) {}
                        Key (
                            util::ui32 frame_,
                            util::ui16 flags_,
                            util::f32 tension_,
                            util::f32 continuity_,
                            util::f32 bias_,
                            util::f32 easeFrom_,
                            util::f32 easeTo_,
                            const T &value_) :
                            frame (frame_),
                            flags (flags_),
                            tension (tension_),
                            continuity (continuity_),
                            bias (bias_),
                            easeFrom (easeFrom_),
                            easeTo (easeTo_),
                            value (value_) {}
                        // NOTE: This ctor is not declared explicit on
                        // purpose. It needs to implicitly convert key.value
                        // from different types.
                        Key (const Key &key) :
                            frame (key.frame),
                            flags (key.flags),
                            tension (key.tension),
                            continuity (key.continuity),
                            bias (key.bias),
                            easeFrom (key.easeFrom),
                            easeTo (key.easeTo),
                            value (key.value) {}

                        Key &operator = (const Key &key) {
                            if (&key != this) {
                                frame = key.frame;
                                flags = key.flags;
                                tension = key.tension;
                                continuity = key.continuity;
                                bias = key.bias;
                                easeFrom = key.easeFrom;
                                easeTo = key.easeTo;
                                value = key.value;
                            }
                            return *this;
                        }

                        inline void Swap (Key &key) {
                            std::swap (frame, key.frame);
                            std::swap (flags, key.flags);
                            std::swap (tension, key.tension);
                            std::swap (continuity, key.continuity);
                            std::swap (bias, key.bias);
                            std::swap (easeFrom, key.easeFrom);
                            std::swap (easeTo, key.easeTo);
                            std::swap (value, key.value);
                        }

                        inline bool IsUsesTension () const {
                            return util::Flags16 (flags).Test (UsesTension);
                        }
                        inline bool IsUsesContinuity () const {
                            return util::Flags16 (flags).Test (UsesContinuity);
                        }
                        inline bool IsUsesBias () const {
                            return util::Flags16 (flags).Test (UsesBias);
                        }
                        inline bool IsUsesEaseTo () const {
                            return util::Flags16 (flags).Test (UsesEaseTo);
                        }
                        inline bool IsUsesEaseFrom () const {
                            return util::Flags16 (flags).Test (UsesEaseFrom);
                        }
                    };
                    std::vector<Key> keys;

                    Track (util::ui16 flags_ = Single) :
                        flags (flags_) {}

                    inline void Swap (Track &track) {
                        std::swap (flags, track.flags);
                        keys.swap (track.keys);
                    }

                    inline bool IsSingle () const {
                        return !IsRepeats ();
                    }
                    inline bool IsRepeats () const {
                        return util::Flags16 (flags).Test (Repeats);
                    }
                    inline bool IsLoops () const {
                        return util::Flags16 (flags).Test (Loops);
                    }
                    inline bool IsLockX () const {
                        return util::Flags16 (flags).Test (LockX);
                    }
                    inline bool IsLockY () const {
                        return util::Flags16 (flags).Test (LockY);
                    }
                    inline bool IsLockZ () const {
                        return util::Flags16 (flags).Test (LockZ);
                    }
                    inline bool IsLock () const {
                        return (flags & Lock) != 0;
                    }
                    inline bool IsNoLinkX () const {
                        return util::Flags16 (flags).Test (NoLinkX);
                    }
                    inline bool IsNoLinkY () const {
                        return util::Flags16 (flags).Test (NoLinkY);
                    }
                    inline bool IsNoLinkZ () const {
                        return util::Flags16 (flags).Test (NoLinkZ);
                    }
                    inline bool IsNoLink () const {
                        return (flags & NoLink) != 0;
                    }

                    // FIXME: move to ext?
                    // Return the index of the first key with either
                    // key.frame == frame or key -> last key in the
                    // list with key.frame < frame.
                    inline std::size_t GetKeyIdx (util::ui32 frame) const {
                        std::size_t kidx = 0;
                        while (kidx < keys.size () && keys[kidx].frame < frame) {
                            ++kidx;
                        }
                        if (kidx == keys.size () || (kidx && frame != keys[kidx].frame)) {
                            --kidx;
                        }
                        return kidx;
                    }

                    inline bool IsKeyFrame (util::ui32 frame) const {
                        for (std::size_t i = 0, count = keys.size (); i < count; ++i) {
                            if (keys[i].frame == frame) {
                                return true;
                            }
                        }
                        return false;
                    }
                };

                // Keep down on template gobbledygook.
                typedef Track<bool> boolTrack;
                typedef Track<util::f32> f32Track;
                typedef Track<blas::Point3> Point3Track;
                typedef Track<blas::AngleAxis> AngleAxisTrack;
                typedef Track<std::string> stringTrack;

                template<typename T>
                inline util::Serializer &operator << (util::Serializer &serializer, const Track<T> &track) {
                    // FIXME: get right values
                    util::ui32 unused1 = 0;
                    util::ui32 unused2 = 0;
                    util::ui32 keyCount = (util::ui32)track.keys.size ();
                    serializer << track.flags << unused1 << unused2  << keyCount;
                    for (util::ui32 i = 0; i < keyCount; ++i) {
                        serializer << track.keys[i].frame << track.keys[i].flags;
                        if (track.keys[i].IsUsesTension ()) {
                            serializer << track.keys[i].tension;
                        }
                        if (track.keys[i].IsUsesContinuity ()) {
                            serializer << track.keys[i].continuity;
                        }
                        if (track.keys[i].IsUsesBias ()) {
                            serializer << track.keys[i].bias;
                        }
                        if (track.keys[i].IsUsesEaseFrom ()) {
                            serializer << track.keys[i].easeFrom;
                        }
                        if (track.keys[i].IsUsesEaseTo ()) {
                            serializer << track.keys[i].easeTo;
                        }
                        serializer << track.keys[i].value;
                    }
                    return serializer;
                }

                template<>
                inline util::Serializer &operator << (util::Serializer &serializer, const boolTrack &track) {
                    // FIXME: get right values
                    util::ui32 unused1 = 0;
                    util::ui32 unused2 = 0;
                    util::ui32 keyCount = (util::ui32)track.keys.size ();
                    serializer << track.flags << unused1 << unused2  << keyCount;
                    for (util::ui32 i = 0; i < keyCount; ++i) {
                        serializer << track.keys[i].frame << track.keys[i].flags;
                        if (track.keys[i].IsUsesTension ()) {
                            serializer << track.keys[i].tension;
                        }
                        if (track.keys[i].IsUsesContinuity ()) {
                            serializer << track.keys[i].continuity;
                        }
                        if (track.keys[i].IsUsesBias ()) {
                            serializer << track.keys[i].bias;
                        }
                        if (track.keys[i].IsUsesEaseFrom ()) {
                            serializer << track.keys[i].easeFrom;
                        }
                        if (track.keys[i].IsUsesEaseTo ()) {
                            serializer << track.keys[i].easeTo;
                        }
                        // bool keys have no value.
                    }
                    return serializer;
                }

                template<typename T>
                inline ChunkReader &operator >> (ChunkReader &chunk, Track<T> &track) {
                    util::ui32 unused1;
                    util::ui32 unused2;
                    util::ui32 keyCount;
                    chunk >> track.flags >> unused1 >> unused2  >> keyCount;
                    track.keys.resize (keyCount);
                    for (util::ui32 i = 0; i < keyCount; ++i) {
                        chunk >> track.keys[i].frame >> track.keys[i].flags;
                        if (track.keys[i].IsUsesTension ()) {
                            chunk >> track.keys[i].tension;
                        }
                        if (track.keys[i].IsUsesContinuity ()) {
                            chunk >> track.keys[i].continuity;
                        }
                        if (track.keys[i].IsUsesBias ()) {
                            chunk >> track.keys[i].bias;
                        }
                        if (track.keys[i].IsUsesEaseFrom ()) {
                            chunk >> track.keys[i].easeFrom;
                        }
                        if (track.keys[i].IsUsesEaseTo ()) {
                            chunk >> track.keys[i].easeTo;
                        }
                        chunk >> track.keys[i].value;
                    }
                    return chunk;
                }

                template<>
                inline ChunkReader &operator >> (ChunkReader &chunk, boolTrack &track) {
                    util::ui32 unused1;
                    util::ui32 unused2;
                    util::ui32 keyCount;
                    chunk >> track.flags >> unused1 >> unused2  >> keyCount;
                    track.keys.resize (keyCount);
                    for (util::ui32 i = 0; i < keyCount; ++i) {
                        chunk >> track.keys[i].frame >> track.keys[i].flags;
                        if (track.keys[i].IsUsesTension ()) {
                            chunk >> track.keys[i].tension;
                        }
                        if (track.keys[i].IsUsesContinuity ()) {
                            chunk >> track.keys[i].continuity;
                        }
                        if (track.keys[i].IsUsesBias ()) {
                            chunk >> track.keys[i].bias;
                        }
                        if (track.keys[i].IsUsesEaseFrom ()) {
                            chunk >> track.keys[i].easeFrom;
                        }
                        if (track.keys[i].IsUsesEaseTo ()) {
                            chunk >> track.keys[i].easeTo;
                        }
                        // bool keys have no value.
                    }
                    return chunk;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_keyframer_Track_h)
