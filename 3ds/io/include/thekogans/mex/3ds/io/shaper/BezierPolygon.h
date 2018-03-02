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

#if !defined (__thekogans_mex_3ds_io_shaper_BezierPolygon_h)
#define __thekogans_mex_3ds_io_shaper_BezierPolygon_h

#include <memory>
#include <vector>
#include "thekogans/util/Types.h"
#include "thekogans/util/Flags.h"
#include "thekogans/mex/blas/Point2.h"
#include "thekogans/mex/blas/Point3.h"
#include "thekogans/mex/3ds/io/ChunkWriter.h"
#include "thekogans/mex/3ds/io/ChunkReader.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                template<typename T>
                struct BezierPolygon {
                    typedef std::unique_ptr<BezierPolygon<T> > UniquePtr;

                    struct Vertex {
                        T pt;
                        T red;
                        T yellow;
                        enum {
                            Last = 0x0001,          // Last vertex in polygon.
                            Closed = 0x0002,        // If last vertex has this bit set, polygon is closed.
                            SelectionSetA = 0x0010, // Part of selection set A
                            SelectionSetB = 0x0020, // Part of selection set B
                            SelectionSetC = 0x0040, // Part of selection set C
                            Shape = 0x0100,         // Part of the current shape.
                            First = 0x0200,         // First vertex in polygon.
                            Hidden = 0x0800,        // If set, vertex is hidden.
                            Frozen = 0x1000         // If set, vertex is frozen.
                        };
                        util::ui16 flags;

                        Vertex () :
                            flags (0) {}
                        explicit Vertex (
                            const T &pt_,
                            const T &red_ = T::Zero,
                            const T &yellow_ = T::Zero,
                            util::ui16 flags_ = 0) :
                            pt (pt_),
                            red (red_),
                            yellow (yellow_),
                            flags (flags_) {}

                        static util::ui16 GetSelectMask (util::ui16 selectionSet) {
                            return selectionSet == 0 ? SelectionSetA :
                                selectionSet == 1 ? SelectionSetB : SelectionSetC;
                        }
                        static util::ui16 GetSelectionSet (util::ui16 selecMask) {
                            return selecMask == SelectionSetA ? 0 : selecMask == SelectionSetB ? 1 : 2;
                        }

                        inline void Swap (Vertex &vertex) {
                            std::swap (pt, vertex.pt);
                            std::swap (red, vertex.red);
                            std::swap (yellow, vertex.yellow);
                            std::swap (flags, vertex.flags);
                        }

                        inline bool IsLast () const {
                            return util::Flags16 (flags).Test (Last);
                        }
                        inline bool IsClosed () const {
                            return util::Flags16 (flags).Test (Closed);
                        }
                        inline bool IsSelected (util::ui16 selectMask) const {
                            return util::Flags16 (flags).Test (selectMask);
                        }
                        inline bool IsSelectionSetA () const {
                            return util::Flags16 (flags).Test (SelectionSetA);
                        }
                        inline bool IsSelectionSetB () const {
                            return util::Flags16 (flags).Test (SelectionSetB);
                        }
                        inline bool IsSelectionSetC () const {
                            return util::Flags16 (flags).Test (SelectionSetC);
                        }
                        inline bool IsShape () const {
                            return util::Flags16 (flags).Test (Shape);
                        }
                        inline bool IsFirst () const {
                            return util::Flags16 (flags).Test (First);
                        }
                        inline bool IsHidden () const {
                            return util::Flags16 (flags).Test (Hidden);
                        }
                        inline bool IsFrozen () const {
                            return util::Flags16 (flags).Test (Frozen);
                        }
                    };
                    std::vector<Vertex> vertices;

                    inline void Swap (BezierPolygon<T> &bezierPolygon) {
                        vertices.swap (bezierPolygon.vertices);
                    }

                    inline bool IsClosed () const {
                        return !vertices.empty () && vertices.back ().IsClosed ();
                    }
                };

                typedef BezierPolygon<blas::Point2> BezierPolygon2;
                typedef BezierPolygon<blas::Point3> BezierPolygon3;

                _LIB_THEKOGANS_MEX_3DS_IO_DECL BezierPolygon3::UniquePtr _LIB_THEKOGANS_MEX_3DS_IO_API
                    BezierPolygon2To3 (const BezierPolygon2 &bezierPolygon2);
                _LIB_THEKOGANS_MEX_3DS_IO_DECL BezierPolygon2::UniquePtr _LIB_THEKOGANS_MEX_3DS_IO_API
                    BezierPolygon3To2 (const BezierPolygon3 &bezierPolygon3);

                template<typename T>
                inline ChunkWriter &operator << (ChunkWriter &chunk, const BezierPolygon<T> &bezierPolygon) {
                    util::ui16 vertexCount = (util::ui16)bezierPolygon.vertices.size ();
                    chunk << vertexCount;
                    for (util::ui16 i = 0; i < vertexCount; ++i) {
                        chunk <<
                            bezierPolygon.vertices[i].pt <<
                            bezierPolygon.vertices[i].red <<
                            bezierPolygon.vertices[i].yellow <<
                            bezierPolygon.vertices[i].flags;
                    }
                    return chunk;
                }

                template<typename T>
                inline ChunkReader &operator >> (ChunkReader &chunk, BezierPolygon<T> &bezierPolygon) {
                    util::ui16 vertexCount;
                    chunk >> vertexCount;
                    bezierPolygon.vertices.resize (vertexCount);
                    for (util::ui16 i = 0; i < vertexCount; ++i) {
                        chunk >>
                            bezierPolygon.vertices[i].pt >>
                            bezierPolygon.vertices[i].red >>
                            bezierPolygon.vertices[i].yellow >>
                            bezierPolygon.vertices[i].flags;
                    }
                    return chunk;
                }

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_shaper_BezierPolygon_h)
