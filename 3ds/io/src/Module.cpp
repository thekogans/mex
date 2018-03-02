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

#include "thekogans/mex/blas/Size.h"
#include "thekogans/mex/3ds/io/ChunkTypes.h"
#include "thekogans/mex/3ds/io/View.h"
#include "thekogans/mex/3ds/io/ViewLayout.h"
#include "thekogans/mex/3ds/io/Module.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                void Module::Swap (Module &module) {
                    openFileDirectory.swap (module.openFileDirectory);
                    openFileName.swap (module.openFileName);
                    viewLayout.Swap (module.viewLayout);
                }

                namespace {
                    inline ChunkReader &operator >> (ChunkReader &chunk, ViewLayout::Layout &layout) {
                        chunk >> layout.currStyle >> layout.currViewIndex >> layout.flags1 >>
                            layout.maximized >> layout.flags2 >> layout.lastStyle >> layout.lastViewIndex;
                        return chunk;
                    }
                }

                void Module::ReadVIEWPORT_LAYOUT (ChunkReader &mainChunk) {
                    assert (viewLayout.views.empty ());
                    assert (viewLayout.views3.empty ());
                    mainChunk >> viewLayout.layout;
                    while (!mainChunk.IsEnd ()) {
                        ChunkReader chunk (mainChunk);
                        switch (chunk.GetID ()) {
                            case VIEWPORT_SIZE:
                                chunk >> viewLayout.size;
                                break;
                            case VIEWPORT_DATA_3: {
                                View view;
                                chunk >> view;
                                viewLayout.views3.push_back (view);
                                break;
                            }
                            case VIEWPORT_DATA: {
                                View view;
                                chunk >> view;
                                viewLayout.views.push_back (view);
                                break;
                            }
                            default:
                                chunk.HandleUknownChunk ("Module", "ReadVIEWPORT_LAYOUT");
                                break;
                        }
                    }
                }

                namespace {
                    inline ChunkWriter &operator << (ChunkWriter &chunk, const ViewLayout::Layout &layout) {
                        chunk << layout.currStyle << layout.currViewIndex << layout.flags1 <<
                            layout.maximized << layout.flags2 << layout.lastStyle << layout.lastViewIndex;
                        return chunk;
                    }

                    inline void WriteVIEWPORT_SIZE (const blas::Size &size, util::File &file) {
                        ChunkWriter chunk (file, VIEWPORT_SIZE);
                        chunk << size;
                    }

                    inline void WriteVIEWPORT_DATA_3 (const View &view3, util::File &file) {
                        ChunkWriter chunk (file, VIEWPORT_DATA_3);
                        chunk << view3;
                    }

                    inline void WriteVIEWPORT_DATA (const View &view, util::File &file) {
                        ChunkWriter chunk (file, VIEWPORT_DATA);
                        chunk << view;
                    }
                }

                void Module::WriteVIEWPORT_LAYOUT (util::File &file) const {
                    ChunkWriter chunk (file, VIEWPORT_LAYOUT);
                    chunk << viewLayout.layout;
                    WriteVIEWPORT_SIZE (viewLayout.size, file);
                    {
                        for (std::size_t i = 0, count = viewLayout.views3.size (); i < count; ++i) {
                            WriteVIEWPORT_DATA_3 (viewLayout.views3[i], file);
                        }
                    }
                    {
                        for (std::size_t i = 0, count = viewLayout.views.size (); i < count; ++i) {
                            WriteVIEWPORT_DATA (viewLayout.views[i], file);
                        }
                    }
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
