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

#include <memory>
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/Util.h"
#include "thekogans/mex/3ds/io/VideoPost.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void VideoPost::Swap (VideoPost &videoPost) {
                    std::swap (length, videoPost.length);
                    queueEntries.swap (videoPost.queueEntries);
                }

                void VideoPost::Validate () {
                    // FIXME: implement
                    assert (0);
                }

                void VideoPost::ReadVPDATA (ChunkReader &mainChunk) {
                    mainChunk >> length;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case P_QUEUE_ENTRY: {
                                QueueEntry::UniquePtr queueEntry (new QueueEntry);
                                ReadP_QUEUE_ENTRY (*queueEntry, chunk);
                                queueEntries.push_back (queueEntry.get ());
                                queueEntry.release ();
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("VideoPost", "ReadVPDATA");
                                break;
                        }
                    }
                }

                void VideoPost::WriteVPDATA (util::File &file) const {
                    // FIXME: implement
                    assert (0);
                }

                void VideoPost::ReadP_QUEUE_ENTRY (QueueEntry &queueEntry, ChunkReader &mainChunk) {
                    mainChunk >>
                        queueEntry.bitmapName >>
                        queueEntry.processName >>
                        queueEntry.alphaName >>
                        queueEntry.transitionName >>
                        queueEntry.fromFrame >>
                        queueEntry.toFrame >>
                        queueEntry.unknown1 >>
                        queueEntry.unknown2;

                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case COLOR_F:
                                chunk >> queueEntry.solidColor;
                                break;
                            case V_GRADIENT:
                                chunk >> queueEntry.vGradient.percent;
                                ReadColor (queueEntry.vGradient.bottom, chunk);
                                ReadColor (queueEntry.vGradient.middle, chunk);
                                ReadColor (queueEntry.vGradient.top, chunk);
                                break;
                            case P_QUEUE_IMAGE:
                                queueEntry.type = QueueEntry::QE_TYPE_IMAGE;
                                break;
                            case P_QUEUE_USEIGAMMA:
                                chunk >> queueEntry.imageGamma;
                                queueEntry.useImageGamma = true;
                                break;
                            case P_QUEUE_PROC:
                                queueEntry.type = QueueEntry::QE_TYPE_PROC;
                                break;
                            case P_QUEUE_SOLID:
                                queueEntry.type = QueueEntry::QE_TYPE_SOLID;
                                break;
                            case P_QUEUE_GRADIENT:
                                queueEntry.type = QueueEntry::QE_TYPE_GRADIENT;
                                break;
                            case P_QUEUE_KF:
                                queueEntry.type = QueueEntry::QE_TYPE_KF;
                                break;
                            case P_QUEUE_MOTBLUR:
                                queueEntry.motionBlur = true;
                                break;
                            case P_QUEUE_MB_REPEAT:
                                chunk >> queueEntry.blurFrames;
                                break;
                            case P_QUEUE_NONE:
                                queueEntry.type = QueueEntry::QE_TYPE_NONE;
                                break;
                            case P_QUEUE_RESIZE:
                                queueEntry.bitmap.type = QueueEntry::Bitmap::QE_BITMAP_RESIZE;
                                break;
                            case P_QUEUE_OFFSET:
                                queueEntry.bitmap.type = QueueEntry::Bitmap::QE_BITMAP_OFFSET;
                                chunk >> queueEntry.bitmap.offsetX;
                                chunk >> queueEntry.bitmap.offsetY;
                                break;
                            case P_QUEUE_ALIGN:
                                queueEntry.bitmap.type = QueueEntry::Bitmap::QE_BITMAP_ALIGN;
                                chunk >> queueEntry.bitmap.alignType;
                                break;
                            case P_CUSTOM_SIZE:
                                chunk >> queueEntry.width;
                                chunk >> queueEntry.height;
                                queueEntry.customSize = true;
                                break;
                            case P_ALPH_NONE:
                                queueEntry.alpha.type = QueueEntry::Alpha::QE_ALPHA_NONE;
                                break;
                            case P_ALPH_PSEUDO:
                                // FIXME: implement
                                assert (0);
                                break;
                            case P_ALPH_OP_PSEUDO:
                                // FIXME: implement
                                assert (0);
                                break;
                            case P_ALPH_BLUR:
                                queueEntry.alpha.blur = true;
                                break;
                            case P_ALPH_PCOL:
                                // FIXME: implement
                                assert (0);
                                break;
                            case P_ALPH_C0:
                                // FIXME: implement
                                assert (0);
                                break;
                            case P_ALPH_OP_KEY:
                                queueEntry.alpha.useKeyColor = 1;
                                break;
                            case P_ALPH_KCOL:
                                ReadColor (queueEntry.alpha.keyColor, chunk);
                                break;
                            case P_ALPH_OP_NOCONV:
                                // FIXME: implement
                                assert (0);
                                break;
                            case P_ALPH_IMAGE:
                                queueEntry.alpha.type = QueueEntry::Alpha::QE_ALPHA_RGB_MASK;
                                break;
                            case P_ALPH_ALPHA:
                                queueEntry.alpha.type = QueueEntry::Alpha::QE_ALPHA_ALPHA_MASK;
                                break;
                            case P_ALPH_QUES:
                                queueEntry.alpha.type = QueueEntry::Alpha::QE_ALPHA_QUEUE_ALPHA_CHANNEL;
                                break;
                            case P_ALPH_QUEIMG:
                                queueEntry.alpha.type = QueueEntry::Alpha::QE_ALPHA_QUEUE_RGB;
                                break;
                            case P_ALPH_CUTOFF:
                                chunk >> queueEntry.alpha.cutOff;
                                break;
                            case P_ALPHANEG:
                                queueEntry.alpha.negative = true;
                                break;
                            case P_TRAN_NONE:
                                queueEntry.transition.type = QueueEntry::Transition::QE_TRANSITION_NONE;
                                break;
                            case P_TRAN_IMAGE:
                                queueEntry.transition.type = QueueEntry::Transition::QE_TRANSITION_IMAGE;
                                break;
                            case P_TRAN_FRAMES:
                                chunk >> queueEntry.transition.frames;
                                break;
                            case P_TRAN_FADEIN:
                                queueEntry.transition.type = QueueEntry::Transition::QE_TRANSITION_FADEIN;
                                break;
                            case P_TRAN_FADEOUT:
                                queueEntry.transition.type = QueueEntry::Transition::QE_TRANSITION_FADEOUT;
                                break;
                            case P_TRANNEG:
                                queueEntry.transition.negative = true;
                                break;
                            case P_RANGES:
                                // FIXME: implement
                                assert (0);
                                break;
                            case P_PROC_DATA: {
                                util::ui32 dataLength = chunk.GetLength () - 6;
                                if (dataLength) {
                                    queueEntry.xdata.resize (dataLength);
                                    chunk.Read (&queueEntry.xdata[0], dataLength);
                                }
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("VideoPost", "ReadP_QUEUE_ENTRY");
                                break;
                        }
                    }
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
