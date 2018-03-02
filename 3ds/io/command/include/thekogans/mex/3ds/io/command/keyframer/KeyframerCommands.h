// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#if !defined (__thekogans_mex_3ds_io_command_keyframer_KeyframerCommands_h)
#define __thekogans_mex_3ds_io_command_keyframer_KeyframerCommands_h

#include <string>
#include "thekogans/util/Types.h"
#include "thekogans/mex/command/CompoundCommand.h"
#include "thekogans/mex/3ds/io/editor/Editor.h"
#include "thekogans/mex/3ds/io/keyframer/Keyframer.h"
#include "thekogans/mex/3ds/ext/keyframer/Node.h"
#include "thekogans/mex/3ds/io/command/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL KeyframerInitCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (KeyframerInitCommand)

                        io::Keyframer &keyframer;
                        const io::Editor &editor;
                        io::Keyframer newKeyframer;

                        KeyframerInitCommand (
                            io::Keyframer &keyframer_,
                            const io::Editor &editor_) :
                            keyframer (keyframer_),
                            editor (editor_),
                            newKeyframer (keyframer.project) {}

                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL KeyframerValidateAfterOpenCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (KeyframerValidateAfterOpenCommand)

                        io::Keyframer &keyframer;

                        explicit KeyframerValidateAfterOpenCommand (io::Keyframer &keyframer_) :
                            keyframer (keyframer_) {}

                        // Found two nodes with the same id.
                        struct DuplicateId {
                            util::ui16 id;
                            explicit DuplicateId (util::ui16 id_) :
                                id (id_) {}
                        };
                        // No node found with a given id.
                        struct NoId {
                            util::ui16 id;
                            explicit NoId (util::ui16 id_) :
                                id (id_) {}
                        };
                        // Found two nodes with the same name.
                        struct DuplicateName {
                            std::string name;
                            explicit DuplicateName (const std::string &name_) :
                                name (name_) {}
                        };
                        // No coresponding Mesh for ObjectNode.
                        struct UnknownMesh {
                            std::string name;
                            explicit UnknownMesh (const std::string &name_) :
                                name (name_) {}
                        };
                        // No coresponding Light for LightNode/SpotlightTargetNode/SpotlightNode.
                        struct UnknownLight {
                            std::string name;
                            explicit UnknownLight (const std::string &name_) :
                                name (name_) {}
                        };
                        // No coresponding Camera for CameraTargetNode/CameraNode.
                        struct UnknownCamera {
                            std::string name;
                            explicit UnknownCamera (const std::string &name_) :
                                name (name_) {}
                        };
                        // No corresponding SpotLightTargetNode for SpotLightHeadNode.
                        struct SpotLightHeadTargetMissmatch {
                            std::string name;
                            explicit SpotLightHeadTargetMissmatch (const std::string &name_) :
                                name (name_) {}
                        };
                        // No corresponding CameraTargetNode for CameraHeadNode.
                        struct CameraHeadTargetMissmatch {
                            std::string name;
                            explicit CameraHeadTargetMissmatch (const std::string &name_) :
                                name (name_) {}
                        };
                        virtual bool Execute ();
                    };

                    struct _LIB_THEKOGANS_MEX_3DS_IO_COMMAND_DECL KeyframerValidateBeforeSaveCommand :
                            public thekogans::mex::command::CompoundCommand {
                        THEKOGANS_MEX_COMMAND_DECLARE_COMMAND (KeyframerValidateBeforeSaveCommand)

                        io::Keyframer &keyframer;

                        KeyframerValidateBeforeSaveCommand (io::Keyframer &keyframer_) :
                            keyframer (keyframer_) {}

                        virtual bool Execute ();

                    private:
                        void SetID (ext::Node &node, util::ui16 &id);
                    };

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_command_keyframer_KeyframerCommands_h)
