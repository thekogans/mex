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

#include <list>
#include "thekogans/mex/3ds/ext/keyframer/Node.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace ext {

                void Node::AddChild (Node *child) {
                    assert (child != 0);
                    if (child != 0 && child->parent != this) {
                        if (child->parent != 0) {
                            // FIXME: do some error handling.
                            child->parent->RemoveChild (child);
                        }
                        children.push_back (child);
                        child->parent = this;
                    }
                }

                bool Node::RemoveChild (Node *child) {
                    assert (child->parent == this);
                    if (child->parent == this) {
                        // Remove child from parent's children list.
                        for (std::vector<Node *>::iterator it = children.begin (), end = children.end (); it != end; ++it) {
                            if (*it == child) {
                                children.erase (it);
                                return true;
                            }
                        }
                    }
                    return false;
                }

                blas::Matrix3 Node::GetHierarchyXform (util::ui32 frame) const {
                    std::list<const Node *> ancestors;
                    for (const Node *ancestor = parent; ancestor != 0; ancestor = ancestor->parent) {
                        ancestors.push_front (ancestor);
                    }
                    blas::Matrix3 parentXform = blas::Matrix3::Identity;
                    for (std::list<const Node *>::const_iterator it = ancestors.begin (), end = ancestors.end (); it != end; ++it) {
                        parentXform = (*it)->GetFrameXform (frame, parentXform);
                    }
                    return parentXform;
                }

                void Node::GetPath (const io::Keyframer::Segment &segment,
                    std::vector<blas::Point3> &path, std::vector<util::ui32> &keys) const {
                    std::vector<blas::Point3> path_;
                    path_.reserve (segment.endFrame + 1);
                    const Point3Track &positionTrack = GetPositionTrack ();
                    {
                        for (util::ui32 i = segment.startFrame; i <= segment.endFrame; ++i) {
                            path_.push_back (positionTrack.GetTween (i) * GetHierarchyXform (i));
                        }
                    }
                    std::vector<util::ui32> keys_;
                    {
                        for (std::size_t i = 0, count = positionTrack.keys.size (); i < count; ++i) {
                            keys_.push_back (positionTrack.keys[i].frame);
                        }
                    }
                    keys_.swap (keys);
                    path_.swap (path);
                }

            } // namespace ext
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
