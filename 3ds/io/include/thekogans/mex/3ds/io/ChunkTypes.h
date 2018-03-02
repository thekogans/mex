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

#if !defined (__thekogans_mex_3ds_io_ChunkTypes_h)
#define __thekogans_mex_3ds_io_ChunkTypes_h

#include "thekogans/util/Types.h"
#include "thekogans/mex/3ds/io/Config.h"

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {

                // common
                const util::ui16 MASTER_SCALE           = 0x0100;
                const util::ui16 VIEWPORT_LAYOUT        = 0x7001;
                const util::ui16 VIEWPORT_DATA          = 0x7011;
                const util::ui16 VIEWPORT_DATA_3        = 0x7012;
                const util::ui16 VIEWPORT_SIZE          = 0x7020;

                // 3ds
                const util::ui16 M3DMAGIC               = 0x4D4D;
                const util::ui16 M3D_VERSION            = 0x0002;
                const util::ui16 MDATA                  = 0x3D3D;
                const util::ui16 MESH_VERSION           = 0x3D3E;
                const util::ui16 FLOAT_PERCENTAGE       = 0x0031;
                const util::ui16 BIT_MAP                = 0x1100;
                const util::ui16 USE_BIT_MAP            = 0x1101;
                const util::ui16 SOLID_BGND             = 0x1200;
                const util::ui16 USE_SOLID_BGND         = 0x1201;
                const util::ui16 V_GRADIENT             = 0x1300;
                const util::ui16 USE_V_GRADIENT         = 0x1301;
                const util::ui16 LO_SHADOW_BIAS         = 0x1400;
                const util::ui16 HI_SHADOW_BIAS         = 0x1410;
                const util::ui16 SHADOW_MAP_SIZE        = 0x1420;
                const util::ui16 SHADOW_SAMPLES         = 0x1430;
                const util::ui16 SHADOW_RANGE           = 0x1440;
                const util::ui16 SHADOW_FILTER          = 0x1450;
                const util::ui16 RAY_BIAS               = 0x1460;
                const util::ui16 FOG                    = 0x2200;
                const util::ui16 USE_FOG                = 0x2201;
                const util::ui16 FOG_BGND               = 0x2210;
                const util::ui16 DISTANCE_CUE           = 0x2300;
                const util::ui16 USE_DISTANCE_CUE       = 0x2301;
                const util::ui16 LAYER_FOG              = 0x2302;
                const util::ui16 USE_LAYER_FOG          = 0x2303;
                const util::ui16 DCUE_BGND              = 0x2310;
                const util::ui16 O_CONSTS               = 0x1500;
                const util::ui16 AMBIENT_LIGHT          = 0x2100;
                const util::ui16 DEFAULT_VIEW           = 0x3000;
                const util::ui16 VIEW_TOP               = 0x3010;
                const util::ui16 VIEW_BOTTOM            = 0x3020;
                const util::ui16 VIEW_LEFT              = 0x3030;
                const util::ui16 VIEW_RIGHT             = 0x3040;
                const util::ui16 VIEW_FRONT             = 0x3050;
                const util::ui16 VIEW_BACK              = 0x3060;
                const util::ui16 VIEW_USER              = 0x3070;
                const util::ui16 VIEW_CAMERA            = 0x3080;
                const util::ui16 NAMED_OBJECT           = 0x4000;
                const util::ui16 OBJ_HIDDEN             = 0x4010;
                const util::ui16 OBJ_VIS_LOFTER         = 0x4011;
                const util::ui16 OBJ_DOESNT_CAST        = 0x4012;
                const util::ui16 OBJ_DONT_RCVSHADOW     = 0x4017;
                const util::ui16 OBJ_MATTE              = 0x4013;
                const util::ui16 OBJ_FAST               = 0x4014;
                const util::ui16 OBJ_PROCEDURAL         = 0x4015;
                const util::ui16 OBJ_FROZEN             = 0x4016;
                const util::ui16 N_TRI_OBJECT           = 0x4100;
                const util::ui16 POINT_ARRAY            = 0x4110;
                const util::ui16 POINT_FLAG_ARRAY       = 0x4111;
                const util::ui16 EDGE_ARRAY             = 0x4112;
                const util::ui16 FACE_ARRAY             = 0x4120;
                const util::ui16 MSH_MAT_GROUP          = 0x4130;
                const util::ui16 TEX_VERTS              = 0x4140;
                const util::ui16 NORMALS                = 0x4141;
                const util::ui16 VERTEX_NORMALS         = 0x4142;
                const util::ui16 VERTEX_NORMAL          = 0x4143;
                const util::ui16 VERTEX_NORMALS2        = 0x4144;
                const util::ui16 FACE_NORMALS           = 0x4145;
                const util::ui16 SMOOTH_GROUP           = 0x4150;
                const util::ui16 MESH_MATRIX            = 0x4160;
                const util::ui16 MESH_COLOR             = 0x4165;
                const util::ui16 MESH_TEXTURE_INFO      = 0x4170;
                const util::ui16 PROC_NAME              = 0x4181;
                const util::ui16 PROC_DATA              = 0x4182;
                const util::ui16 MSH_BOXMAP             = 0x4190;
                const util::ui16 N_DIRECT_LIGHT         = 0x4600;
                const util::ui16 DL_SPOTLIGHT           = 0x4610;
                const util::ui16 DL_OFF                 = 0x4620;
                const util::ui16 DL_ATTENUATE           = 0x4625;
                const util::ui16 DL_RAYSHAD             = 0x4627;
                const util::ui16 DL_SHADOWED            = 0x4630;
                const util::ui16 DL_LOCAL_SHADOW2       = 0x4641;
                const util::ui16 DL_SEE_CONE            = 0x4650;
                const util::ui16 DL_SPOT_RECTANGULAR    = 0x4651;
                const util::ui16 DL_SPOT_OVERSHOOT      = 0x4652;
                const util::ui16 DL_PROJECTOR           = 0x4653;
                const util::ui16 DL_EXCLUDE             = 0x4654;
                const util::ui16 DL_RANGE               = 0x4655;
                const util::ui16 DL_SPOT_ROLL           = 0x4656;
                const util::ui16 DL_SPOT_ASPECT         = 0x4657;
                const util::ui16 DL_RAY_BIAS            = 0x4658;
                const util::ui16 DL_INNER_RANGE         = 0x4659;
                const util::ui16 DL_OUTER_RANGE         = 0x465A;
                const util::ui16 DL_MULTIPLIER          = 0x465B;
                const util::ui16 N_CAMERA               = 0x4700;
                const util::ui16 CAM_SEE_CONE           = 0x4710;
                const util::ui16 CAM_RANGES             = 0x4720;
                const util::ui16 XDATA_SECTION          = 0x8000;

                // Keyframer Chunk IDs
                const util::ui16 KFDATA                 = 0xB000;
                const util::ui16 KFHDR                  = 0xB00A;
                const util::ui16 AMBIENT_NODE_TAG       = 0xB001;
                const util::ui16 OBJECT_NODE_TAG        = 0xB002;
                const util::ui16 CAMERA_NODE_TAG        = 0xB003;
                const util::ui16 TARGET_NODE_TAG        = 0xB004;
                const util::ui16 LIGHT_NODE_TAG         = 0xB005;
                const util::ui16 L_TARGET_NODE_TAG      = 0xB006;
                const util::ui16 SPOTLIGHT_NODE_TAG     = 0xB007;
                const util::ui16 KFSEG                  = 0xB008;
                const util::ui16 KFCURTIME              = 0xB009;
                const util::ui16 NODE_HDR               = 0xB010;
                const util::ui16 INSTANCE_NAME          = 0xB011;
                const util::ui16 PRESCALE               = 0xB012;
                const util::ui16 PIVOT                  = 0xB013;
                const util::ui16 BOUNDBOX               = 0xB014;
                const util::ui16 MORPH_SMOOTH           = 0xB015;
                const util::ui16 POS_TRACK_TAG          = 0xB020;
                const util::ui16 ROT_TRACK_TAG          = 0xB021;
                const util::ui16 SCL_TRACK_TAG          = 0xB022;
                const util::ui16 FOV_TRACK_TAG          = 0xB023;
                const util::ui16 ROLL_TRACK_TAG         = 0xB024;
                const util::ui16 COL_TRACK_TAG          = 0xB025;
                const util::ui16 MORPH_TRACK_TAG        = 0xB026;
                const util::ui16 HOT_TRACK_TAG          = 0xB027;
                const util::ui16 FALL_TRACK_TAG         = 0xB028;
                const util::ui16 HIDE_TRACK_TAG         = 0xB029;
                const util::ui16 NODE_ID                = 0xB030;

                // shp
                const util::ui16 SMAGIC                 = 0x2D2D;
                const util::ui16 POLY_2D                = 0x5000;
                const util::ui16 SHAPE_OK               = 0x5010;
                const util::ui16 SHAPE_NOT_OK           = 0x5011;
                const util::ui16 SHAPE_HOOK             = 0x5020;

                // mli
                const util::ui16 COLOR_F                = 0x0010;
                const util::ui16 COLOR_24               = 0x0011;
                const util::ui16 LIN_COLOR_24           = 0x0012;
                const util::ui16 LIN_COLOR_F            = 0x0013;
                const util::ui16 INT_PERCENTAGE         = 0x0030;
                const util::ui16 MLIBMAGIC              = 0x3DAA;
                const util::ui16 MAT_ENTRY              = 0xAFFF;
                const util::ui16 MAT_NAME               = 0xA000;
                const util::ui16 MAT_AMBIENT            = 0xA010;
                const util::ui16 MAT_DIFFUSE            = 0xA020;
                const util::ui16 MAT_SPECULAR           = 0xA030;
                const util::ui16 MAT_SHININESS          = 0xA040;
                const util::ui16 MAT_SHIN2PCT           = 0xA041;
                const util::ui16 MAT_SHIN3PCT           = 0xA042;
                const util::ui16 MAT_TRANSPARENCY       = 0xA050;
                const util::ui16 MAT_XPFALL             = 0xA052;
                const util::ui16 MAT_REFBLUR            = 0xA053;
                const util::ui16 MAT_SELF_ILLUM         = 0xA080;
                const util::ui16 MAT_TWO_SIDE           = 0xA081;
                const util::ui16 MAT_DECAL              = 0xA082;
                const util::ui16 MAT_ADDITIVE           = 0xA083;
                const util::ui16 MAT_SELF_ILPCT         = 0xA084;
                const util::ui16 MAT_WIRE               = 0xA085;
                const util::ui16 MAT_SUPERSMP           = 0xA086;
                const util::ui16 MAT_WIRESIZE           = 0xA087;
                const util::ui16 MAT_FACEMAP            = 0xA088;
                const util::ui16 MAT_XPFALLIN           = 0xA08A;
                const util::ui16 MAT_PHONGSOFT          = 0xA08C;
                const util::ui16 MAT_WIREABS            = 0xA08E;
                const util::ui16 MAT_SHADING            = 0xA100;
                const util::ui16 MAT_TEXMAP             = 0xA200;
                const util::ui16 MAT_OPACMAP            = 0xA210;
                const util::ui16 MAT_REFLMAP            = 0xA220;
                const util::ui16 MAT_BUMPMAP            = 0xA230;
                const util::ui16 MAT_SPECMAP            = 0xA204;
                const util::ui16 MAT_USE_XPFALL         = 0xA240;
                const util::ui16 MAT_USE_REFBLUR        = 0xA250;
                const util::ui16 MAT_BUMP_PERCENT       = 0xA252;
                const util::ui16 MAT_MAPNAME            = 0xA300;
                const util::ui16 MAT_ACUBIC             = 0xA310;
                const util::ui16 MAT_SXP_TEXT_DATA      = 0xA320;
                const util::ui16 MAT_SXP_TEXT2_DATA     = 0xA321;
                const util::ui16 MAT_SXP_OPAC_DATA      = 0xA322;
                const util::ui16 MAT_SXP_BUMP_DATA      = 0xA324;
                const util::ui16 MAT_SXP_SPEC_DATA      = 0xA325;
                const util::ui16 MAT_SXP_SHIN_DATA      = 0xA326;
                const util::ui16 MAT_SXP_SELFI_DATA     = 0xA328;
                const util::ui16 MAT_SXP_TEXT_MASKDATA  = 0xA32A;
                const util::ui16 MAT_SXP_TEXT2_MASKDATA = 0xA32C;
                const util::ui16 MAT_SXP_OPAC_MASKDATA  = 0xA32E;
                const util::ui16 MAT_SXP_BUMP_MASKDATA  = 0xA330;
                const util::ui16 MAT_SXP_SPEC_MASKDATA  = 0xA332;
                const util::ui16 MAT_SXP_SHIN_MASKDATA  = 0xA334;
                const util::ui16 MAT_SXP_SELFI_MASKDATA = 0xA336;
                const util::ui16 MAT_SXP_REFL_MASKDATA  = 0xA338;
                const util::ui16 MAT_TEX2MAP            = 0xA33A;
                const util::ui16 MAT_SHINMAP            = 0xA33C;
                const util::ui16 MAT_SELFIMAP           = 0xA33D;
                const util::ui16 MAT_TEXMASK            = 0xA33E;
                const util::ui16 MAT_TEX2MASK           = 0xA340;
                const util::ui16 MAT_OPACMASK           = 0xA342;
                const util::ui16 MAT_BUMPMASK           = 0xA344;
                const util::ui16 MAT_SHINMASK           = 0xA346;
                const util::ui16 MAT_SPECMASK           = 0xA348;
                const util::ui16 MAT_SELFIMASK          = 0xA34A;
                const util::ui16 MAT_REFLMASK           = 0xA34C;
                const util::ui16 MAT_MAP_TILINGOLD      = 0xA350;
                const util::ui16 MAT_MAP_TILING         = 0xA351;
                const util::ui16 MAT_MAP_TEXBLUR_OLD    = 0xA352;
                const util::ui16 MAT_MAP_TEXBLUR        = 0xA353;
                const util::ui16 MAT_MAP_USCALE         = 0xA354;
                const util::ui16 MAT_MAP_VSCALE         = 0xA356;
                const util::ui16 MAT_MAP_UOFFSET        = 0xA358;
                const util::ui16 MAT_MAP_VOFFSET        = 0xA35A;
                const util::ui16 MAT_MAP_ANG            = 0xA35C;
                const util::ui16 MAT_MAP_COL1           = 0xA360;
                const util::ui16 MAT_MAP_COL2           = 0xA362;
                const util::ui16 MAT_MAP_RCOL           = 0xA364;
                const util::ui16 MAT_MAP_GCOL           = 0xA366;
                const util::ui16 MAT_MAP_BCOL           = 0xA368;

                // vp
                const util::ui16 VPDATA                 = 0xd000;
                const util::ui16 P_QUEUE_ENTRY          = 0xd100;
                const util::ui16 P_QUEUE_IMAGE          = 0xd110;
                const util::ui16 P_QUEUE_USEIGAMMA      = 0xd114;
                const util::ui16 P_QUEUE_PROC           = 0xd120;
                const util::ui16 P_QUEUE_SOLID          = 0xd130;
                const util::ui16 P_QUEUE_GRADIENT       = 0xd140;
                const util::ui16 P_QUEUE_KF             = 0xd150;
                const util::ui16 P_QUEUE_MOTBLUR        = 0xd152;
                const util::ui16 P_QUEUE_MB_REPEAT      = 0xd153;
                const util::ui16 P_QUEUE_NONE           = 0xd160;
                const util::ui16 P_QUEUE_RESIZE         = 0xd180;
                const util::ui16 P_QUEUE_OFFSET         = 0xd185;
                const util::ui16 P_QUEUE_ALIGN          = 0xd190;
                const util::ui16 P_CUSTOM_SIZE          = 0xd1a0;
                const util::ui16 P_ALPH_NONE            = 0xd210;
                const util::ui16 P_ALPH_PSEUDO          = 0xd220; // Old chunk
                const util::ui16 P_ALPH_OP_PSEUDO       = 0xd221; // Old chunk
                const util::ui16 P_ALPH_BLUR            = 0xd222; // Replaces pseudo
                const util::ui16 P_ALPH_PCOL            = 0xd225;
                const util::ui16 P_ALPH_C0              = 0xd230;
                const util::ui16 P_ALPH_OP_KEY          = 0xd231;
                const util::ui16 P_ALPH_KCOL            = 0xd235;
                const util::ui16 P_ALPH_OP_NOCONV       = 0xd238;
                const util::ui16 P_ALPH_IMAGE           = 0xd240;
                const util::ui16 P_ALPH_ALPHA           = 0xd250;
                const util::ui16 P_ALPH_QUES            = 0xd260;
                const util::ui16 P_ALPH_QUEIMG          = 0xd265;
                const util::ui16 P_ALPH_CUTOFF          = 0xd270;
                const util::ui16 P_ALPHANEG             = 0xd280;
                const util::ui16 P_TRAN_NONE            = 0xd300;
                const util::ui16 P_TRAN_IMAGE           = 0xd310;
                const util::ui16 P_TRAN_FRAMES          = 0xd312;
                const util::ui16 P_TRAN_FADEIN          = 0xd320;
                const util::ui16 P_TRAN_FADEOUT         = 0xd330;
                const util::ui16 P_TRANNEG              = 0xd340;
                const util::ui16 P_RANGES               = 0xd400;
                const util::ui16 P_PROC_DATA            = 0xd500;

                // lft
                const util::ui16 LMAGIC                 = 0x2D3D;
                const util::ui16 PATH_3D                = 0x6000;
                const util::ui16 PATH_MATRIX            = 0x6005;
                const util::ui16 SHAPE_2D               = 0x6010;
                const util::ui16 M_SCALE                = 0x6020;
                const util::ui16 M_TWIST                = 0x6030;
                const util::ui16 M_TEETER               = 0x6040;
                const util::ui16 M_FIT                  = 0x6050;
                const util::ui16 M_BEVEL                = 0x6060;
                const util::ui16 XZ_CURVE               = 0x6070;
                const util::ui16 YZ_CURVE               = 0x6080;
                const util::ui16 INTERPCT               = 0x6090;
                const util::ui16 DEFORM_LIMIT           = 0x60A0;
                const util::ui16 USE_CONTOUR            = 0x6100;
                const util::ui16 USE_TWEEN              = 0x6110;
                const util::ui16 USE_SCALE              = 0x6120;
                const util::ui16 USE_TWIST              = 0x6130;
                const util::ui16 USE_TEETER             = 0x6140;
                const util::ui16 USE_FIT                = 0x6150;
                const util::ui16 USE_BEVEL              = 0x6160;

                // prj
                const util::ui16 CMAGIC                 = 0xC23D;
                const util::ui16 C_MDRAWER              = 0xC010;
                const util::ui16 C_TDRAWER              = 0xC020;
                const util::ui16 C_SHPDRAWER            = 0xC030;
                const util::ui16 C_MODDRAWER            = 0xC040;
                const util::ui16 C_RIPDRAWER            = 0xC050;
                const util::ui16 C_TXDRAWER             = 0xC060;
                const util::ui16 C_PDRAWER              = 0xC062;
                const util::ui16 C_MTLDRAWER            = 0xC064;
                const util::ui16 C_FLIDRAWER            = 0xC066;
                const util::ui16 C_CUBDRAWER            = 0xC067;
                const util::ui16 C_MFILE                = 0xC070;
                const util::ui16 C_SHPFILE              = 0xC080;
                const util::ui16 C_MODFILE              = 0xC090;
                const util::ui16 C_RIPFILE              = 0xC0A0;
                const util::ui16 C_TXFILE               = 0xC0B0;
                const util::ui16 C_PFILE                = 0xC0B2;
                const util::ui16 C_MTLFILE              = 0xC0B4;
                const util::ui16 C_FLIFILE              = 0xC0B6;
                const util::ui16 C_PALFILE              = 0xC0B8;
                const util::ui16 C_TX_STRING            = 0xC0C0;
                const util::ui16 C_CONSTS               = 0xC0D0;
                const util::ui16 C_SNAPS                = 0xC0E0;
                const util::ui16 C_GRIDS                = 0xC0F0;
                const util::ui16 C_ASNAPS               = 0xC100;
                const util::ui16 C_GRID_RANGE           = 0xC110;
                const util::ui16 C_RENDTYPE             = 0xC120;
                const util::ui16 C_PROGMODE             = 0xC130;
                const util::ui16 C_PREVMODE             = 0xC140;
                const util::ui16 C_MODWMODE             = 0xC150;
                const util::ui16 C_MODMODEL             = 0xC160;
                const util::ui16 C_ALL_LINES            = 0xC170;
                const util::ui16 C_BACK_TYPE            = 0xC180;
                const util::ui16 C_MD_CS                = 0xC190;
                const util::ui16 C_MD_CE                = 0xC1A0;
                const util::ui16 C_MD_SML               = 0xC1B0;
                const util::ui16 C_MD_SMW               = 0xC1C0;
                const util::ui16 C_LOFT_WITH_TEXTURE    = 0xC1C3;
                const util::ui16 C_LOFT_L_REPEAT        = 0xC1C4;
                const util::ui16 C_LOFT_W_REPEAT        = 0xC1C5;
                const util::ui16 C_LOFT_UV_NORMALIZE    = 0xC1C6;
                const util::ui16 C_WELD_LOFT            = 0xC1C7;
                const util::ui16 C_MD_PDET              = 0xC1D0;
                const util::ui16 C_MD_SDET              = 0xC1E0;
                const util::ui16 C_RGB_RMODE            = 0xC1F0;
                const util::ui16 C_RGB_HIDE             = 0xC200;
                const util::ui16 C_RGB_MAPSW            = 0xC202;
                const util::ui16 C_RGB_TWOSIDE          = 0xC204;
                const util::ui16 C_RGB_SHADOW           = 0xC208;
                const util::ui16 C_RGB_AA               = 0xC210;
                const util::ui16 C_RGB_OVW              = 0xC220;
                const util::ui16 C_RGB_OVH              = 0xC230;
                const util::ui16 C_RGB_PICTYPE          = 0xC240;
                const util::ui16 C_RGB_OUTPUT           = 0xC250;
                const util::ui16 C_RGB_TODISK           = 0xC253;
                const util::ui16 C_RGB_COMPRESS         = 0xC254;
                const util::ui16 C_JPEG_COMPRESSION     = 0xC255;
                const util::ui16 C_RGB_DISPDEV          = 0xC256;
                const util::ui16 C_RGB_HARDDEV          = 0xC259;
                const util::ui16 C_RGB_PATH             = 0xC25A;
                const util::ui16 C_BITMAP_DRAWER        = 0xC25B;
                const util::ui16 C_RGB_FILE             = 0xC260;
                const util::ui16 C_RGB_OVASPECT         = 0xC270;
                const util::ui16 C_RGB_ANIMTYPE         = 0xC271;
                const util::ui16 C_RENDER_ALL           = 0xC272;
                const util::ui16 C_REND_FROM            = 0xC273;
                const util::ui16 C_REND_TO              = 0xC274;
                const util::ui16 C_REND_NTH             = 0xC275;
                const util::ui16 C_PAL_TYPE             = 0xC276;
                const util::ui16 C_RND_TURBO            = 0xC277;
                const util::ui16 C_RND_MIP              = 0xC278;
                const util::ui16 C_BGND_METHOD          = 0xC279;
                const util::ui16 C_AUTO_REFLECT         = 0xC27A;
                const util::ui16 C_VP_FROM              = 0xC27B;
                const util::ui16 C_VP_TO                = 0xC27C;
                const util::ui16 C_VP_NTH               = 0xC27D;
                const util::ui16 C_REND_TSTEP           = 0xC27E;
                const util::ui16 C_VP_TSTEP             = 0xC27F;
                const util::ui16 C_SRDIAM               = 0xC280;
                const util::ui16 C_SRDEG                = 0xC290;
                const util::ui16 C_SRSEG                = 0xC2A0;
                const util::ui16 C_SRDIR                = 0xC2B0;
                const util::ui16 C_HETOP                = 0xC2C0;
                const util::ui16 C_HEBOT                = 0xC2D0;
                const util::ui16 C_HEHT                 = 0xC2E0;
                const util::ui16 C_HETURNS              = 0xC2F0;
                const util::ui16 C_HEDEG                = 0xC300;
                const util::ui16 C_HESEG                = 0xC310;
                const util::ui16 C_HEDIR                = 0xC320;
                const util::ui16 C_QUIKSTUFF            = 0xC330;
                const util::ui16 C_SEE_LIGHTS           = 0xC340;
                const util::ui16 C_SEE_CAMERAS          = 0xC350;
                const util::ui16 C_SEE_3D               = 0xC360;
                const util::ui16 C_MESHSEL              = 0xC370;
                const util::ui16 C_MESHUNSEL            = 0xC380;
                const util::ui16 C_POLYSEL              = 0xC390;
                const util::ui16 C_POLYUNSEL            = 0xC3A0;
                const util::ui16 C_SHPLOCAL             = 0xC3A2;
                const util::ui16 C_MSHLOCAL             = 0xC3A4;
                const util::ui16 C_NUM_FORMAT           = 0xC3B0;
                const util::ui16 C_ARCH_DENOM           = 0xC3C0;
                const util::ui16 C_IN_DEVICE            = 0xC3D0;
                const util::ui16 C_MSCALE               = 0xC3E0;
                const util::ui16 C_COMM_PORT            = 0xC3F0;
                const util::ui16 C_TAB_BASES            = 0xC400;
                const util::ui16 C_TAB_DIVS             = 0xC410;
                const util::ui16 C_MASTER_SCALES        = 0xC420;
                const util::ui16 C_SHOW_1STVERT         = 0xC430;
                const util::ui16 C_SHAPER_OK            = 0xC440;
                const util::ui16 C_LOFTER_OK            = 0xC450;
                const util::ui16 C_EDITOR_OK            = 0xC460;
                const util::ui16 C_KEYFRAMER_OK         = 0xC470;
                const util::ui16 C_PICKSIZE             = 0xC480;
                const util::ui16 C_MAPTYPE              = 0xC490;
                const util::ui16 C_MAP_DISPLAY          = 0xC4A0;
                const util::ui16 C_TILE_XY              = 0xC4B0;
                const util::ui16 C_MAP_XYZ              = 0xC4C0;
                const util::ui16 C_MAP_SCALE            = 0xC4D0;
                const util::ui16 C_MAP_MATRIX_OLD       = 0xC4E0;
                const util::ui16 C_MAP_MATRIX           = 0xC4E1;
                const util::ui16 C_MAP_WID_HT           = 0xC4F0;
                const util::ui16 C_OBNAME               = 0xC500;
                const util::ui16 C_CAMNAME              = 0xC510;
                const util::ui16 C_LTNAME               = 0xC520;
                const util::ui16 C_CUR_MNAME            = 0xC525;
                const util::ui16 C_CURMTL_FROM_MESH     = 0xC526;
                const util::ui16 C_GET_SHAPE_MAKE_FACES = 0xC527;
                const util::ui16 C_DETAIL               = 0xC530;
                const util::ui16 C_VERTMARK             = 0xC540;
                const util::ui16 C_MSHAX                = 0xC550;
                const util::ui16 C_MSHCP                = 0xC560;
                const util::ui16 C_USERAX               = 0xC570;
                const util::ui16 C_SHOOK                = 0xC580;
                const util::ui16 C_RAX                  = 0xC590;
                const util::ui16 C_STAPE                = 0xC5A0;
                const util::ui16 C_LTAPE                = 0xC5B0;
                const util::ui16 C_ETAPE                = 0xC5C0;
                const util::ui16 C_KTAPE                = 0xC5C8;
                const util::ui16 C_SPHSEGS              = 0xC5D0;
                const util::ui16 C_GEOSMOOTH            = 0xC5E0;
                const util::ui16 C_HEMISEGS             = 0xC5F0;
                const util::ui16 C_PRISMSEGS            = 0xC600;
                const util::ui16 C_PRISMSIDES           = 0xC610;
                const util::ui16 C_TUBESEGS             = 0xC620;
                const util::ui16 C_TUBESIDES            = 0xC630;
                const util::ui16 C_TORSEGS              = 0xC640;
                const util::ui16 C_TORSIDES             = 0xC650;
                const util::ui16 C_CONESIDES            = 0xC660;
                const util::ui16 C_CONESEGS             = 0xC661;
                const util::ui16 C_NGPARMS              = 0xC670;
                const util::ui16 C_PTHLEVEL             = 0xC680;
                const util::ui16 C_MSCSYM               = 0xC690;
                const util::ui16 C_MFTSYM               = 0xC6A0;
                const util::ui16 C_MTTSYM               = 0xC6B0;
                const util::ui16 C_SMOOTHING            = 0xC6C0;
                const util::ui16 C_MODICOUNT            = 0xC6D0;
                const util::ui16 C_FONTSEL              = 0xC6E0;
                const util::ui16 C_TESS_TYPE            = 0xC6f0;
                const util::ui16 C_TESS_TENSION         = 0xC6f1;
                const util::ui16 C_SEG_START            = 0xC700;
                const util::ui16 C_SEG_END              = 0xC705;
                const util::ui16 C_CURTIME              = 0xC710;
                const util::ui16 C_ANIMLENGTH           = 0xC715;
                const util::ui16 C_PV_FROM              = 0xC720;
                const util::ui16 C_PV_TO                = 0xC725;
                const util::ui16 C_PV_DOFNUM            = 0xC730;
                const util::ui16 C_PV_RNG               = 0xC735;
                const util::ui16 C_PV_NTH               = 0xC740;
                const util::ui16 C_PV_TYPE              = 0xC745;
                const util::ui16 C_PV_METHOD            = 0xC750;
                const util::ui16 C_PV_FPS               = 0xC755;
                const util::ui16 C_VTR_FRAMES           = 0xC765;
                const util::ui16 C_VTR_HDTL             = 0xC770;
                const util::ui16 C_VTR_HD               = 0xC771;
                const util::ui16 C_VTR_TL               = 0xC772;
                const util::ui16 C_VTR_IN               = 0xC775;
                const util::ui16 C_VTR_PK               = 0xC780;
                const util::ui16 C_VTR_SH               = 0xC785;

                // Material chunks
                const util::ui16 C_WORK_MTLS            = 0xC790; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_2          = 0xC792; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_3          = 0xC793; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_4          = 0xC794; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_5          = 0xCB00; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_6          = 0xCB01; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_7          = 0xCB02; // Old-style -- now ignored
                const util::ui16 C_WORK_MTLS_8          = 0xCB03; // Old-style -- now ignored
                const util::ui16 C_WORKMTL              = 0xCB04;
                const util::ui16 C_SXP_TEXT_DATA        = 0xCB10;
                const util::ui16 C_SXP_TEXT2_DATA       = 0xCB20;
                const util::ui16 C_SXP_OPAC_DATA        = 0xCB11;
                const util::ui16 C_SXP_BUMP_DATA        = 0xCB12;
                const util::ui16 C_SXP_SPEC_DATA        = 0xCB24;
                const util::ui16 C_SXP_SHIN_DATA        = 0xCB13;
                const util::ui16 C_SXP_SELFI_DATA       = 0xCB28;
                const util::ui16 C_SXP_TEXT_MASKDATA    = 0xCB30;
                const util::ui16 C_SXP_TEXT2_MASKDATA   = 0xCB32;
                const util::ui16 C_SXP_OPAC_MASKDATA    = 0xCB34;
                const util::ui16 C_SXP_BUMP_MASKDATA    = 0xCB36;
                const util::ui16 C_SXP_SPEC_MASKDATA    = 0xCB38;
                const util::ui16 C_SXP_SHIN_MASKDATA    = 0xCB3A;
                const util::ui16 C_SXP_SELFI_MASKDATA   = 0xCB3C;
                const util::ui16 C_SXP_REFL_MASKDATA    = 0xCB3E;
                const util::ui16 C_BGTYPE               = 0xC7A1;
                const util::ui16 C_MEDTILE              = 0xC7B0;

                // Contrast
                const util::ui16 C_LO_CONTRAST          = 0xC7D0;
                const util::ui16 C_HI_CONTRAST          = 0xC7D1;

                // 3d frozen display
                const util::ui16 C_FROZ_DISPLAY         = 0xC7E0;

                // Booleans
                const util::ui16 C_BOOLWELD             = 0xc7f0;
                const util::ui16 C_BOOLTYPE             = 0xc7f1;
                const util::ui16 C_ANG_THRESH           = 0xC900;
                const util::ui16 C_SS_THRESH            = 0xC901;
                const util::ui16 C_TEXTURE_BLUR_DEFAULT = 0xC903;
                const util::ui16 C_MAPDRAWER            = 0xCA00;
                const util::ui16 C_MAPDRAWER1           = 0xCA01;
                const util::ui16 C_MAPDRAWER2           = 0xCA02;
                const util::ui16 C_MAPDRAWER3           = 0xCA03;
                const util::ui16 C_MAPDRAWER4           = 0xCA04;
                const util::ui16 C_MAPDRAWER5           = 0xCA05;
                const util::ui16 C_MAPDRAWER6           = 0xCA06;
                const util::ui16 C_MAPDRAWER7           = 0xCA07;
                const util::ui16 C_MAPDRAWER8           = 0xCA08;
                const util::ui16 C_MAPDRAWER9           = 0xCA09;
                const util::ui16 C_MAPDRAWER_ENTRY      = 0xCA10;

                // System options
                const util::ui16 C_BACKUP_FILE          = 0xCA20;
                const util::ui16 C_DITHER_256           = 0xCA21;
                const util::ui16 C_SAVE_LAST            = 0xCA22;
                const util::ui16 C_USE_ALPHA            = 0xCA23;
                const util::ui16 C_TGA_DEPTH            = 0xCA24;
                const util::ui16 C_REND_FIELDS          = 0xCA25;
                const util::ui16 C_REFLIP               = 0xCA26;
                const util::ui16 C_SEL_ITEMTOG          = 0xCA27;
                const util::ui16 C_SEL_RESET            = 0xCA28;
                const util::ui16 C_STICKY_KEYINF        = 0xCA29;
                const util::ui16 C_WELD_THRESHOLD       = 0xCA2A;
                const util::ui16 C_ZCLIP_POINT          = 0xCA2B;
                const util::ui16 C_ALPHA_SPLIT          = 0xCA2C;
                const util::ui16 C_KF_SHOW_BACKFACE     = 0xCA30;
                const util::ui16 C_OPTIMIZE_LOFT        = 0xCA40;
                const util::ui16 C_TENS_DEFAULT         = 0xCA42;
                const util::ui16 C_CONT_DEFAULT         = 0xCA44;
                const util::ui16 C_BIAS_DEFAULT         = 0xCA46;
                const util::ui16 C_DXFNAME_SRC          = 0xCA50;
                const util::ui16 C_AUTO_WELD            = 0xCA60;
                const util::ui16 C_AUTO_UNIFY           = 0xCA70;
                const util::ui16 C_AUTO_SMOOTH          = 0xCA80;
                const util::ui16 C_DXF_SMOOTH_ANG       = 0xCA90;
                const util::ui16 C_SMOOTH_ANG           = 0xCAA0;

                // Special network-use chunks
                const util::ui16 C_NET_USE_VPOST        = 0xCC00;
                const util::ui16 C_NET_USE_GAMMA        = 0xCC10;
                const util::ui16 C_NET_FIELD_ORDER      = 0xCC20;
                const util::ui16 C_BLUR_FRAMES          = 0xCD00;
                const util::ui16 C_BLUR_SAMPLES         = 0xCD10;
                const util::ui16 C_BLUR_DUR             = 0xCD20;
                const util::ui16 C_HOT_METHOD           = 0xCD30;
                const util::ui16 C_HOT_CHECK            = 0xCD40;
                const util::ui16 C_PIXEL_SIZE           = 0xCD50;
                const util::ui16 C_DISP_GAMMA           = 0xCD60;
                const util::ui16 C_FBUF_GAMMA           = 0xCD70;
                const util::ui16 C_FILE_OUT_GAMMA       = 0xCD80;
                const util::ui16 C_FILE_IN_GAMMA        = 0xCD82;
                const util::ui16 C_GAMMA_CORRECT        = 0xCD84;
                const util::ui16 C_APPLY_DISP_GAMMA     = 0xCD90; // OBSOLETE
                const util::ui16 C_APPLY_FBUF_GAMMA     = 0xCDA0; // OBSOLETE
                const util::ui16 C_APPLY_FILE_GAMMA     = 0xCDB0; // OBSOLETE
                const util::ui16 C_FORCE_WIRE           = 0xCDC0;
                const util::ui16 C_RAY_SHADOWS          = 0xCDD0;
                const util::ui16 C_MASTER_AMBIENT       = 0xCDE0;
                const util::ui16 C_SUPER_SAMPLE         = 0xCDF0;
                const util::ui16 C_OBJECT_MBLUR         = 0xCE00;
                const util::ui16 C_MBLUR_DITHER         = 0xCE10;
                const util::ui16 C_DITHER_24            = 0xCE20;
                const util::ui16 C_SUPER_BLACK          = 0xCE30;
                const util::ui16 C_SAFE_FRAME           = 0xCE40;
                const util::ui16 C_VIEW_PRES_RATIO      = 0xCE50;
                const util::ui16 C_BGND_PRES_RATIO      = 0xCE60;
                const util::ui16 C_NTH_SERIAL_NUM       = 0xCE70;

                // Chunk id + length
                const util::ui16 CHUNK_HEADER_LENGTH    = util::UI16_SIZE + util::UI32_SIZE;

            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans

#endif // !defined (__thekogans_mex_3ds_io_ChunkTypes_h)
