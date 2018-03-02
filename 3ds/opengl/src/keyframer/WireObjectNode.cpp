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

#include <numeric>
#include "thekogans/util/Vectorizer.h"
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ShadeModel.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/DepthFunc.h"
#include "thekogans/mex/opengl/CullFace.h"
#include "thekogans/mex/opengl/LineStipple.h"
#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/3ds/io/editor/Mesh.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Material.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/keyframer/WireObjectNode.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP (WireObjectNode)

                WireObjectNode::WireObjectNode (
                        const ext::ObjectNode &objectNode,
                        const io::Project &project) :
                    ObjectNode (objectNode, project), vertexCount (0) {
                    if (objectNode.objectNode.IsHasPath ()) {
                        objectNode.GetPath (project.keyframer.segment, path, keys);
                    }
                    if (objectNode.objectNode.name != io::ObjectNode::DUMMY_NAME) {
                        ext::Mesh (objectNode.mesh).BuildEdges (edges);
                        vertices.resize (edges.size () * 2);
                    }
                }

                void WireObjectNode::SetCurrentFrame () {
                    if (objectNode.objectNode.name != io::ObjectNode::DUMMY_NAME) {
                        vertexCount = 0;
                        for (std::size_t i = 0, count = edges.size (); i < count; ++i) {
                            const ext::Mesh::Edge &edge = edges[i];
                            if (edge.IsVisible ()) {
                                vertices[vertexCount++] = objectNode.vertices[edge.v1];
                                vertices[vertexCount++] = objectNode.vertices[edge.v2];
                            }
                        }
                    }
                }

                void WireObjectNode::Draw (const View &view) const {
                    if (!objectNode.hidden) {
                        ShadeModel shadeModel (GL_FLAT);
                        Enable lighting (GL_LIGHTING, false);
                        Enable depthTesting (GL_DEPTH_TEST, false);
                        Enable cullFace (GL_CULL_FACE, false);
                        Enable texture2D (GL_TEXTURE_2D, false);
                        if (objectNode.objectNode.IsHasPath ()) {
                            DrawPath (path, keys, project.keyframer.segment);
                        }
                        Color color (DefaultPalette[
                            objectNode.objectNode.IsKeyFrame (project.keyframer.segment.currFrame) ?
                                OBJECT_PALETTE_START_INDEX + objectNode.mesh.paletteIndex :
                                BLACK]);
                        if (objectNode.objectNode.name == io::ObjectNode::DUMMY_NAME) {
                            LineStipple lineStipple (1, 0x3333);
                            DrawBox (view, objectNode.GetBound3 (), objectNode.xform);
                        }
                        else if (!project.display.detail) {
                            DrawBox (view, objectNode.GetBound3 (), objectNode.xform);
                        }
                        else if (vertexCount != 0) {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &vertices[0]);
                            glDrawArrays (GL_LINES, 0, vertexCount);
                        }
                    }
                }

                void WireObjectNode::DrawWithColor (
                        const View &view,
                        const thekogans::mex::opengl::ui8Color &color) const {
                    if (!objectNode.hidden) {
                        ShadeModel shadeModel (GL_FLAT);
                        Enable lighting (GL_LIGHTING, false);
                        Enable depthTesting (GL_DEPTH_TEST, false);
                        Enable cullFace (GL_CULL_FACE, false);
                        Enable texture2D (GL_TEXTURE_2D, false);
                        if (objectNode.objectNode.IsHasPath ()) {
                            DrawPathWithColor (path, keys, project.keyframer.segment, color);
                        }
                        Color _color (color);
                        if (objectNode.objectNode.name == io::ObjectNode::DUMMY_NAME) {
                            LineStipple lineStipple (1, 0x3333);
                            DrawBox (view, objectNode.GetBound3 (), objectNode.xform);
                        }
                        else if (!project.display.detail) {
                            DrawBox (view, objectNode.GetBound3 (), objectNode.xform);
                        }
                        else if (vertexCount != 0) {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &vertices[0]);
                            glDrawArrays (GL_LINES, 0, vertexCount);
                        }
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
