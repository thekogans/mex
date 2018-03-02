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

#if !defined (__thekogans_mex_3ds_io_VideoPost_h)
#define __thekogans_mex_3ds_io_VideoPost_h

#include <memory>
#include "thekogans/util/Types.h"
#include "thekogans/util/OwnerVector.h"
#include "thekogans/mex/3ds/io/FixedString.h"
#include "thekogans/mex/3ds/io/Color.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                struct VideoPost {
                    util::ui32 length;
                    struct QueueEntry {
                        typedef std::unique_ptr<QueueEntry> UniquePtr;

                        FixedString bitmapName;
                        FixedString processName;
                        FixedString alphaName;
                        FixedString transitionName;
                        util::ui32 fromFrame;
                        util::ui32 toFrame;
                        util::ui32 unknown1;
                        util::ui32 unknown2;
                        thekogans::mex::opengl::f32Color solidColor;
                        struct VGradient {
                            util::f32 percent;
                            Color bottom;
                            Color middle;
                            Color top;

                            VGradient () :
                                percent (0.5f),
                                bottom (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)),
                                middle (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)),
                                top (thekogans::mex::opengl::f32Color (0.0f, 0.0f, 0.0f)) {}
                        } vGradient;
                        enum Type {
                            QE_TYPE_NONE,
                            QE_TYPE_IMAGE,
                            QE_TYPE_PROC,
                            QE_TYPE_SOLID,
                            QE_TYPE_GRADIENT,
                            QE_TYPE_KF
                        } type;
                        util::f32 imageGamma;
                        bool useImageGamma;
                        bool motionBlur;
                        util::ui32 blurFrames;
                        struct Bitmap {
                            enum Type {
                                QE_BITMAP_RESIZE,
                                QE_BITMAP_OFFSET,
                                QE_BITMAP_ALIGN
                            } type;
                            util::ui32 offsetX;
                            util::ui32 offsetY;
                            util::ui32 alignType;
                        } bitmap;
                        util::ui32 width;
                        util::ui32 height;
                        bool customSize;
                        struct Alpha {
                            enum Type {
                                QE_ALPHA_NONE,
                                QE_ALPHA_RGB_MASK,
                                QE_ALPHA_ALPHA_MASK,
                                QE_ALPHA_QUEUE_ALPHA_CHANNEL,
                                QE_ALPHA_QUEUE_RGB
                            } type;
                            bool blur;
                            Color keyColor;
                            util::ui16 useKeyColor;
                            util::ui32 cutOff;
                            bool negative;
                        } alpha;
                        struct Transition {
                            enum Type {
                                QE_TRANSITION_NONE,
                                QE_TRANSITION_IMAGE,
                                QE_TRANSITION_FADEIN,
                                QE_TRANSITION_FADEOUT,
                            } type;
                            util::ui32 frames;
                            bool negative;
                        } transition;
                        std::vector<util::ui8> xdata;

                        QueueEntry () :
                            bitmapName (13),
                            processName (13),
                            alphaName (13),
                            transitionName (13) {}
                    };
                    util::OwnerVector<QueueEntry> queueEntries;

                    // FIXME: initialize
                    VideoPost () {}

                    void Swap (VideoPost &videoPost);
                    void Validate ();

                    void ReadVPDATA (ChunkReader &mainChunk);
                    void WriteVPDATA (util::File &file) const;

                private:
                    void ReadP_QUEUE_ENTRY (QueueEntry &queueEntry, ChunkReader &mainChunk);
                };

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_VideoPost_h)
