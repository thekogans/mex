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

#include <cassert>
#include "thekogans/mex/opengl/Color.h"
#include "thekogans/mex/opengl/ShadeModel.h"
#include "thekogans/mex/opengl/Enable.h"
#include "thekogans/mex/opengl/DepthFunc.h"
#include "thekogans/mex/opengl/CullFace.h"
#include "thekogans/mex/opengl/ClientState.h"
#include "thekogans/mex/opengl/LineStipple.h"
#include "thekogans/mex/3ds/opengl/DefaultPalette.h"
#include "thekogans/mex/3ds/opengl/Icons.h"
#include "thekogans/mex/3ds/opengl/Util.h"
#include "thekogans/mex/3ds/opengl/editor/WireMesh.h"

using namespace thekogans::mex::opengl;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace opengl {

                THEKOGANS_UTIL_IMPLEMENT_HEAP_FUNCTIONS (WireMesh)

                WireMesh::WireMesh (
                        const io::Mesh &mesh,
                        const io::Project &project) :
                        Mesh (mesh, project),
                        selectionInfo (mesh) {
                    selectionInfo.Build ();
                    if (project.display.detail) {
                        ext::Mesh (mesh).BuildEdges (edges);
                    }
                    else {
                        bound = ext::Mesh (mesh).GetBound3 ();
                    }
                }

                void WireMesh::Draw (const View &view) const {
                    if (!mesh.IsHidden ()) {
                        ShadeModel shadeModel (GL_FLAT);
                        Enable lighting (GL_LIGHTING, false);
                        Enable depthTesting (GL_DEPTH_TEST, false);
                        Enable cullFace (GL_CULL_FACE, false);
                        Enable texture2D (GL_TEXTURE_2D, false);
                        if (project.display.detail) {
                            ui8Color edgeOrFaceColor;
                            ui8Color selectedEdgeOrFaceColor;
                            ui8Color vertexColor;
                            ui8Color selectedVertexColor;
                            if (mesh.frozen) {
                                edgeOrFaceColor = selectedEdgeOrFaceColor =
                                    vertexColor = selectedVertexColor = DefaultPalette[GRAY];
                            }
                            else {
                                {
                                    util::ui16 selectionSet = io::Mesh::GetSelectionSet (project.editor.selectMask);
                                    if (selectionInfo.vertexCount[selectionSet] == 0) {
                                        vertexColor = selectedVertexColor = DefaultPalette[OBJECT_PALETTE_START_INDEX + mesh.paletteIndex];
                                    }
                                    else if (selectionInfo.vertexCount[selectionSet] == mesh.vertices.size ()) {
                                        vertexColor = selectedVertexColor = DefaultPalette[RED];
                                    }
                                    else {
                                        vertexColor = DefaultPalette[OBJECT_PALETTE_START_INDEX + mesh.paletteIndex];
                                        selectedVertexColor = DefaultPalette[RED];
                                    }
                                }
                                {
                                    util::ui16 selectionSet = io::Mesh::Face::GetSelectionSet (project.editor.selectMask);
                                    if (selectionInfo.faceCount[selectionSet] == 0) {
                                        edgeOrFaceColor = selectedEdgeOrFaceColor = DefaultPalette[OBJECT_PALETTE_START_INDEX + mesh.paletteIndex];
                                    }
                                    else if (selectionInfo.faceCount[selectionSet] == mesh.faces.size ()) {
                                        edgeOrFaceColor = selectedEdgeOrFaceColor = DefaultPalette[RED];
                                    }
                                    else {
                                        edgeOrFaceColor = DefaultPalette[OBJECT_PALETTE_START_INDEX + mesh.paletteIndex];
                                        selectedEdgeOrFaceColor = DefaultPalette[RED];
                                    }
                                }
                            }
                            if (!mesh.fastDraw && !view.view.IsFast () && !project.display.fastDraw) {
                                DrawEdges (view, edgeOrFaceColor, selectedEdgeOrFaceColor);
                            }
                            else {
                                DrawFaces (view, edgeOrFaceColor, selectedEdgeOrFaceColor);
                            }
                            DrawVertices (view, vertexColor, selectedVertexColor);
                        }
                        else {
                            Color color (
                                mesh.frozen ?
                                    DefaultPalette[GRAY] :
                                    selectionInfo.IsSelected (io::Mesh::GetSelectionSet (project.editor.selectMask)) ?
                                        DefaultPalette[RED] :
                                        DefaultPalette[OBJECT_PALETTE_START_INDEX + mesh.paletteIndex]);
                            DrawBox (view, bound, mesh.xform);
                        }
                    }
                }

                void WireMesh::DrawWithColor (const View &view, const ui8Color &color) const {
                    if (project.display.detail) {
                        if (!mesh.fastDraw && !view.view.IsFast () && !project.display.fastDraw) {
                            DrawEdges (view, color, color);
                        }
                        else {
                            DrawFaces (view, color, color);
                        }
                        DrawVertices (view, color, color);
                    }
                    else {
                        Color _color (color);
                        DrawBox (view, bound, mesh.xform);
                    }
                }

                void WireMesh::DrawVertices (const View &view, const ui8Color &vertexColor,
                        const ui8Color &selectedVertexColor) const {
                    if (mesh.vertexFlags.empty ()) {
                        Color color (vertexColor);
                        if (project.display.vertexTicks) {
                            util::ui32 icon = GetTickIcon (7);
                            for (std::size_t i = 0, count = mesh.vertices.size (); i < count; ++i) {
                                DrawIcon (icon, mesh.vertices[i]);
                            }
                        }
                        else {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &mesh.vertices[0]);
                            glDrawArrays (GL_POINTS, 0, mesh.vertices.size ());
                        }
                    }
                    else {
                        assert (mesh.vertexFlags.size () == mesh.vertices.size ());
                        std::vector<blas::Point3> vertices;
                        vertices.reserve (mesh.vertices.size ());
                        std::vector<blas::Point3> selectedVertices;
                        selectedVertices.reserve (mesh.vertices.size ());
                        if (vertexColor == selectedVertexColor) {
                            for (std::size_t i = 0, count = mesh.vertices.size (); i < count; ++i) {
                                if (!util::Flags16 (mesh.vertexFlags[i]).Test (io::Mesh::Hidden)) {
                                    vertices.push_back (mesh.vertices[i]);
                                }
                            }
                        }
                        else {
                            for (std::size_t i = 0, count = mesh.vertices.size (); i < count; ++i) {
                                if (!util::Flags16 (mesh.vertexFlags[i]).Test (io::Mesh::Hidden)) {
                                    if (!util::Flags16 (mesh.vertexFlags[i]).Test (project.editor.selectMask)) {
                                        vertices.push_back (mesh.vertices[i]);
                                    }
                                    else {
                                        selectedVertices.push_back (mesh.vertices[i]);
                                    }
                                }
                            }
                        }
                        if (project.display.vertexTicks) {
                            if (!vertices.empty ()) {
                                Color color (vertexColor);
                                util::ui32 icon = GetTickIcon (7);
                                for (std::size_t i = 0, count = vertices.size (); i < count; ++i) {
                                    DrawIcon (icon, vertices[i]);
                                }
                            }
                            if (!selectedVertices.empty ()) {
                                Color color (selectedVertexColor);
                                util::ui32 icon = GetTickIcon (7);
                                for (std::size_t i = 0, count = selectedVertices.size (); i < count; ++i) {
                                    DrawIcon (icon, selectedVertices[i]);
                                }
                            }
                        }
                        else {
                            if (!vertices.empty ()) {
                                Color color (vertexColor);
                                ClientState clientState (ClientState::VERTEX_ARRAY);
                                glVertexPointer (3, GL_FLOAT, 0, &vertices[0]);
                                glDrawArrays (GL_POINTS, 0, vertices.size ());
                            }
                            if (!selectedVertices.empty ()) {
                                Color color (selectedVertexColor);
                                ClientState clientState (ClientState::VERTEX_ARRAY);
                                glVertexPointer (3, GL_FLOAT, 0, &selectedVertices[0]);
                                glDrawArrays (GL_POINTS, 0, selectedVertices.size ());
                            }
                        }
                    }
                }

                void WireMesh::DrawEdges (const View &view, const ui8Color &edgeColor,
                        const ui8Color &selectedEdgeColor) const {
                    const std::vector<blas::Point3> &vertices = mesh.vertices;
                    const std::vector<io::Mesh::Face> &faces = mesh.faces;
                    // Check face visibility.
                    std::vector<bool> faceVisibility;
                    faceVisibility.resize (faces.size ());
                    if (project.display.seeThrough) {
                        for (std::size_t i = 0, count = faces.size (); i < count; ++i) {
                            faceVisibility[i] = !faces[i].IsHidden ();
                        }
                    }
                    else {
                        ext::Mesh::Face::IsFacingViewerFunc IsFacingViewer = ext::Mesh::Face::GetFacingViewerFunc (view.view);
                        for (std::size_t i = 0, count = faces.size (); i < count; ++i) {
                            const io::Mesh::Face &face = faces[i];
                            faceVisibility[i] = !face.IsHidden () && (ext::Mesh::Face (face).*IsFacingViewer)(vertices);
                        }
                    }
                    std::vector<blas::Point3> solidEdges;
                    solidEdges.reserve (edges.size () * 2);
                    std::vector<blas::Point3> selectedSolidEdges;
                    selectedSolidEdges.reserve (edges.size () * 2);
                    std::vector<blas::Point3> stippledEdges;
                    stippledEdges.reserve (edges.size () * 2);
                    std::vector<blas::Point3> selectedStippledEdges;
                    selectedStippledEdges.reserve (edges.size () * 2);
                    // Sort edges in to their respective buckets.
                    if (edgeColor == selectedEdgeColor) {
                        if (project.display.showAllLines) {
                            for (std::size_t i = 0, count = edges.size (); i < count; ++i) {
                                const ext::Mesh::Edge &edge = edges[i];
                                if (faceVisibility[edge.f1] ||
                                    (edge.f2 != util::NIDX16 && faceVisibility[edge.f2])) {
                                    if (edge.IsVisible ()) {
                                        solidEdges.push_back (vertices[edge.v1]);
                                        solidEdges.push_back (vertices[edge.v2]);
                                    }
                                    else {
                                        stippledEdges.push_back (vertices[edge.v1]);
                                        stippledEdges.push_back (vertices[edge.v2]);
                                    }
                                }
                            }
                        }
                        else {
                            for (std::size_t i = 0, count = edges.size (); i < count; ++i) {
                                const ext::Mesh::Edge &edge = edges[i];
                                if (edge.IsVisible () && (faceVisibility[edge.f1] ||
                                        (edge.f2 != util::NIDX16 && faceVisibility[edge.f2]))) {
                                    solidEdges.push_back (vertices[edge.v1]);
                                    solidEdges.push_back (vertices[edge.v2]);
                                }
                            }
                        }
                    }
                    else {
                        if (project.display.showAllLines) {
                            for (std::size_t i = 0, count = edges.size (); i < count; ++i) {
                                const ext::Mesh::Edge &edge = edges[i];
                                bool f1visible = faceVisibility[edge.f1];
                                bool f2visible = edge.f2 != util::NIDX16 && faceVisibility[edge.f2];
                                if ((f1visible && util::Flags16 (faces[edge.f1].flags).Test (project.editor.selectMask)) ||
                                    (f2visible && util::Flags16 (faces[edge.f2].flags).Test (project.editor.selectMask))) {
                                    if (edge.IsVisible ()) {
                                        selectedSolidEdges.push_back (vertices[edge.v1]);
                                        selectedSolidEdges.push_back (vertices[edge.v2]);
                                    }
                                    else {
                                        selectedStippledEdges.push_back (vertices[edge.v1]);
                                        selectedStippledEdges.push_back (vertices[edge.v2]);
                                    }
                                }
                                else if (f1visible || f2visible) {
                                    if (edge.IsVisible ()) {
                                        solidEdges.push_back (vertices[edge.v1]);
                                        solidEdges.push_back (vertices[edge.v2]);
                                    }
                                    else {
                                        stippledEdges.push_back (vertices[edge.v1]);
                                        stippledEdges.push_back (vertices[edge.v2]);
                                    }
                                }
                            }
                        }
                        else {
                            for (std::size_t i = 0, count = edges.size (); i < count; ++i) {
                                const ext::Mesh::Edge &edge = edges[i];
                                if (edge.IsVisible ()) {
                                    bool f1visible = faceVisibility[edge.f1];
                                    bool f2visible = edge.f2 != util::NIDX16 && faceVisibility[edge.f2];
                                    if ((f1visible && util::Flags16 (faces[edge.f1].flags).Test (project.editor.selectMask)) ||
                                        (f2visible && util::Flags16 (faces[edge.f2].flags).Test (project.editor.selectMask))) {
                                        selectedSolidEdges.push_back (vertices[edge.v1]);
                                        selectedSolidEdges.push_back (vertices[edge.v2]);
                                    }
                                    else if (f1visible || f2visible) {
                                        solidEdges.push_back (vertices[edge.v1]);
                                        solidEdges.push_back (vertices[edge.v2]);
                                    }
                                }
                            }
                        }
                    }
                    // Draw unselected edges.
                    {
                        Color color (edgeColor);
                        if (!solidEdges.empty ()) {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &solidEdges[0]);
                            glDrawArrays (GL_LINES, 0, solidEdges.size ());
                        }
                        if (!stippledEdges.empty ()) {
                            LineStipple lineStipple (1, 0x3333);
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &stippledEdges[0]);
                            glDrawArrays (GL_LINES, 0, stippledEdges.size ());
                        }
                    }
                    // Draw selected edges.
                    {
                        Color color (selectedEdgeColor);
                        if (!selectedSolidEdges.empty ()) {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &selectedSolidEdges[0]);
                            glDrawArrays (GL_LINES, 0, selectedSolidEdges.size ());
                        }
                        if (!selectedStippledEdges.empty ()) {
                            LineStipple lineStipple (1, 0x3333);
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &selectedStippledEdges[0]);
                            glDrawArrays (GL_LINES, 0, selectedStippledEdges.size ());
                        }
                    }
                }

                void WireMesh::DrawFaces (const View &view, const ui8Color &faceColor,
                        const ui8Color &selectedFaceColor) const {
                    const std::vector<blas::Point3> &vertices = mesh.vertices;
                    const std::vector<io::Mesh::Face> &faces = mesh.faces;
                    std::vector<blas::Point3> solidEdges;
                    solidEdges.reserve (faces.size () * 3);
                    std::vector<blas::Point3> selectedSolidEdges;
                    selectedSolidEdges.reserve (faces.size () * 3);
                    std::vector<blas::Point3> stippledEdges;
                    stippledEdges.reserve (faces.size () * 3);
                    std::vector<blas::Point3> selectedStippledEdges;
                    selectedStippledEdges.reserve (faces.size () * 3);
                    if (faceColor == selectedFaceColor) {
                        if (project.display.showAllLines) {
                            if (project.display.seeThrough) {
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    if (!ioFace.IsHidden ()) {
                                        if (ioFace.IsEdgeABVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.a]);
                                            solidEdges.push_back (vertices[ioFace.b]);
                                        }
                                        else {
                                            stippledEdges.push_back (vertices[ioFace.a]);
                                            stippledEdges.push_back (vertices[ioFace.b]);
                                        }
                                        if (ioFace.IsEdgeBCVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.b]);
                                            solidEdges.push_back (vertices[ioFace.c]);
                                        }
                                        else {
                                            stippledEdges.push_back (vertices[ioFace.b]);
                                            stippledEdges.push_back (vertices[ioFace.c]);
                                        }
                                        if (ioFace.IsEdgeCAVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.c]);
                                            solidEdges.push_back (vertices[ioFace.a]);
                                        }
                                        else {
                                            stippledEdges.push_back (vertices[ioFace.c]);
                                            stippledEdges.push_back (vertices[ioFace.a]);
                                        }
                                    }
                                }
                            }
                            else {
                                ext::Mesh::Face::IsFacingViewerFunc IsFacingViewer = ext::Mesh::Face::GetFacingViewerFunc (view.view);
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    const ext::Mesh::Face extFace (ioFace);
                                    if (!ioFace.IsHidden () && (extFace.*IsFacingViewer)(vertices)) {
                                        if (ioFace.IsEdgeABVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.a]);
                                            solidEdges.push_back (vertices[ioFace.b]);
                                        }
                                        else {
                                            stippledEdges.push_back (vertices[ioFace.a]);
                                            stippledEdges.push_back (vertices[ioFace.b]);
                                        }
                                        if (ioFace.IsEdgeBCVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.b]);
                                            solidEdges.push_back (vertices[ioFace.c]);
                                        }
                                        else {
                                            stippledEdges.push_back (vertices[ioFace.b]);
                                            stippledEdges.push_back (vertices[ioFace.c]);
                                        }
                                        if (ioFace.IsEdgeCAVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.c]);
                                            solidEdges.push_back (vertices[ioFace.a]);
                                        }
                                        else {
                                            stippledEdges.push_back (vertices[ioFace.c]);
                                            stippledEdges.push_back (vertices[ioFace.a]);
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (project.display.seeThrough) {
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    if (!ioFace.IsHidden ()) {
                                        if (ioFace.IsEdgeABVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.a]);
                                            solidEdges.push_back (vertices[ioFace.b]);
                                        }
                                        if (ioFace.IsEdgeBCVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.b]);
                                            solidEdges.push_back (vertices[ioFace.c]);
                                        }
                                        if (ioFace.IsEdgeCAVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.c]);
                                            solidEdges.push_back (vertices[ioFace.a]);
                                        }
                                    }
                                }
                            }
                            else {
                                ext::Mesh::Face::IsFacingViewerFunc IsFacingViewer = ext::Mesh::Face::GetFacingViewerFunc (view.view);
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    const ext::Mesh::Face extFace (ioFace);
                                    if (!ioFace.IsHidden () && (extFace.*IsFacingViewer)(vertices)) {
                                        if (ioFace.IsEdgeABVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.a]);
                                            solidEdges.push_back (vertices[ioFace.b]);
                                        }
                                        if (ioFace.IsEdgeBCVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.b]);
                                            solidEdges.push_back (vertices[ioFace.c]);
                                        }
                                        if (ioFace.IsEdgeCAVisible ()) {
                                            solidEdges.push_back (vertices[ioFace.c]);
                                            solidEdges.push_back (vertices[ioFace.a]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else {
                        if (project.display.showAllLines) {
                            if (project.display.seeThrough) {
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    if (!ioFace.IsHidden ()) {
                                        if (util::Flags16 (ioFace.flags).Test (project.editor.selectMask)) {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            else {
                                                selectedStippledEdges.push_back (vertices[ioFace.a]);
                                                selectedStippledEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            else {
                                                selectedStippledEdges.push_back (vertices[ioFace.b]);
                                                selectedStippledEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                            }
                                            else {
                                                selectedStippledEdges.push_back (vertices[ioFace.c]);
                                                selectedStippledEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                        else {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.a]);
                                                solidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            else {
                                                stippledEdges.push_back (vertices[ioFace.a]);
                                                stippledEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.b]);
                                                solidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            else {
                                                stippledEdges.push_back (vertices[ioFace.b]);
                                                stippledEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.c]);
                                                solidEdges.push_back (vertices[ioFace.a]);
                                            }
                                            else {
                                                stippledEdges.push_back (vertices[ioFace.c]);
                                                stippledEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                    }
                                }
                            }
                            else {
                                ext::Mesh::Face::IsFacingViewerFunc IsFacingViewer = ext::Mesh::Face::GetFacingViewerFunc (view.view);
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    const ext::Mesh::Face extFace (ioFace);
                                    if (!ioFace.IsHidden () && (extFace.*IsFacingViewer)(vertices)) {
                                        if (util::Flags16 (ioFace.flags).Test (project.editor.selectMask)) {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            else {
                                                selectedStippledEdges.push_back (vertices[ioFace.a]);
                                                selectedStippledEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            else {
                                                selectedStippledEdges.push_back (vertices[ioFace.b]);
                                                selectedStippledEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                            }
                                            else {
                                                selectedStippledEdges.push_back (vertices[ioFace.c]);
                                                selectedStippledEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                        else {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.a]);
                                                solidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            else {
                                                stippledEdges.push_back (vertices[ioFace.a]);
                                                stippledEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.b]);
                                                solidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            else {
                                                stippledEdges.push_back (vertices[ioFace.b]);
                                                stippledEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.c]);
                                                solidEdges.push_back (vertices[ioFace.a]);
                                            }
                                            else {
                                                stippledEdges.push_back (vertices[ioFace.c]);
                                                stippledEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (project.display.seeThrough) {
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    if (!ioFace.IsHidden ()) {
                                        if (util::Flags16 (ioFace.flags).Test (project.editor.selectMask)) {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                        else {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.a]);
                                                solidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.b]);
                                                solidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.c]);
                                                solidEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                    }
                                }
                            }
                            else {
                                ext::Mesh::Face::IsFacingViewerFunc IsFacingViewer = ext::Mesh::Face::GetFacingViewerFunc (view.view);
                                for (std::size_t i = 0, count = faces.size (); i < count; i += project.display.speedCount) {
                                    const io::Mesh::Face &ioFace = faces[i];
                                    const ext::Mesh::Face extFace (ioFace);
                                    if (!ioFace.IsHidden () && (extFace.*IsFacingViewer)(vertices)) {
                                        if (util::Flags16 (ioFace.flags).Test (project.editor.selectMask)) {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.b]);
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                selectedSolidEdges.push_back (vertices[ioFace.c]);
                                                selectedSolidEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                        else {
                                            if (ioFace.IsEdgeABVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.a]);
                                                solidEdges.push_back (vertices[ioFace.b]);
                                            }
                                            if (ioFace.IsEdgeBCVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.b]);
                                                solidEdges.push_back (vertices[ioFace.c]);
                                            }
                                            if (ioFace.IsEdgeCAVisible ()) {
                                                solidEdges.push_back (vertices[ioFace.c]);
                                                solidEdges.push_back (vertices[ioFace.a]);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // Draw unselected edges.
                    {
                        Color color (faceColor);
                        if (!solidEdges.empty ()) {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &solidEdges[0]);
                            glDrawArrays (GL_LINES, 0, solidEdges.size ());
                        }
                        if (!stippledEdges.empty ()) {
                            LineStipple lineStipple (1, 0x3333);
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &stippledEdges[0]);
                            glDrawArrays (GL_LINES, 0, stippledEdges.size ());
                        }
                    }
                    // Draw selected edges.
                    {
                        Color color (selectedFaceColor);
                        if (!selectedSolidEdges.empty ()) {
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &selectedSolidEdges[0]);
                            glDrawArrays (GL_LINES, 0, selectedSolidEdges.size ());
                        }
                        if (!selectedStippledEdges.empty ()) {
                            LineStipple lineStipple (1, 0x3333);
                            ClientState clientState (ClientState::VERTEX_ARRAY);
                            glVertexPointer (3, GL_FLOAT, 0, &selectedStippledEdges[0]);
                            glDrawArrays (GL_LINES, 0, selectedStippledEdges.size ());
                        }
                    }
                }

            } // namespace opengl
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
