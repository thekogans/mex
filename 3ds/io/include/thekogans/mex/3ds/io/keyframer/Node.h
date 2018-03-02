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

#if !defined (__thekogans_mex_3ds_io_keyframer_Node_h)
#define __thekogans_mex_3ds_io_keyframer_Node_h

#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/util/Constants.h"
#include "thekogans/util/Flags.h"
#include "thekogans/util/Heap.h"
#include "thekogans/mex/3ds/io/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct _LIB_THEKOGANS_MEX_3DS_IO_DECL Node {
                    util::ui16 id;
                    std::string name;
                    enum Flags1 {
                        Off = 0x0008,
                        Hide = 0x0800,
                        Fast = 0x1000
                    };
                    util::ui16 flags1;
                    enum Flags2 {
                        HasPath = 0x0001,
                        AutoSmooth = 0x0002,
                        Frozen = 0x0004,
                        MotionBlur = 0x0010,
                        BlurBranch = 0x0020,
                        MorphMtl = 0x0040,
                        MorphOb = 0x0080
                    };
                    util::ui16 flags2;
                    util::ui16 parent;

                    Node (
                        util::ui16 id_ = util::NIDX16,
                        const std::string &name_ = std::string (),
                        util::ui16 flags1_ = 0,
                        util::ui16 flags2_ = 0,
                        util::ui16 parent_ = util::NIDX16) :
                        id (id_),
                        name (name_),
                        flags1 (flags1_),
                        flags2 (flags2_),
                        parent (parent_) {}
                    virtual ~Node () {}

                    inline void Swap (Node &node) {
                        std::swap (id, node.id);
                        name.swap (node.name);
                        std::swap (flags1, node.flags1);
                        std::swap (flags2, node.flags2);
                        std::swap (parent, node.parent);
                    }

                    inline bool IsOff () const {
                        return util::Flags16 (flags1).Test (Off);
                    }
                    inline bool IsHide () const {
                        return util::Flags16 (flags1).Test (Hide);
                    }
                    inline bool IsFast () const {
                        return util::Flags16 (flags1).Test (Fast);
                    }

                    inline bool IsHasPath () const {
                        return util::Flags16 (flags2).Test (HasPath);
                    }
                    inline bool IsAutoSmooth () const {
                        return util::Flags16 (flags2).Test (AutoSmooth);
                    }
                    inline bool IsFrozen () const {
                        return util::Flags16 (flags2).Test (Frozen);
                    }
                    inline bool IsMotionBlur () const {
                        return util::Flags16 (flags2).Test (MotionBlur);
                    }
                    inline bool IsBlurBranch () const {
                        return util::Flags16 (flags2).Test (BlurBranch);
                    }
                    inline bool IsMorphMtl () const {
                        return util::Flags16 (flags2).Test (MorphMtl);
                    }
                    inline bool IsMorphOb () const {
                        return util::Flags16 (flags2).Test (MorphOb);
                    }

                    virtual std::string GetName () const {
                        return name;
                    }

                    // FIXME: move to ext?
                    virtual bool IsKeyFrame (util::ui32 frame) const = 0;
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_keyframer_Node_h)
