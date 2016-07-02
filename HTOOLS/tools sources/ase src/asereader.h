#ifndef __ASEREADER__
#define	__ASEREADER__


#define	STARTPLAYER				"start"
#define	STOPPLAYER				"stop"
#define	ACTIONBRUSH				"action"
#define	DETAILOBJET				"detail"
#define	NOBSP					"nobsp"
#define	GUN1					"gun1"
#define	GUN2					"gun2"
#define	GUN3					"gun3"
#define	AMO1					"amo1"
#define	AMO2					"amo2"
#define	AMO3					"amo3"
#define	PUMPKIN					"pumpkin"
#define	MONSTRE					"monstre"
#define	MORBAK					"morbak"
#define	ZEBAT					"zebat"
#define	SUBZERAT				"subzerat"
#define	ZEMUMMY					"zemummy"
#define	LAVETAG					"lava"
#define	LIFTTAG					"lift"
#define	DOORTAG					"door"
#define	FIOLE1					"fiole1"
#define	FIOLE2					"fiole2"
#define	FIOLE3					"fiole3"
#define	LAMPE					"lampe"
#define	MLAMPE					"mlampe"
#define	INVISIBLE				"invisible"
#define	MOREINVISIBLE			"moreinvisible"
#define	letterH					"[H]"
#define	letterA					"[A]"
#define	letterL					"[L]"
#define	letterO					"[O]"
#define	letterW					"[W]"
#define	letterE					"[E]"
#define	letterN					"[N]"


#define	SUBMAT					"*SUBMATERIAL"
#define	NSUBMAT					"*NUMSUBMTLS"
#define	MATLIST					"*MATERIAL_LIST"
#define NTEXTURES				"*MATERIAL_COUNT"
#define TEXTID					"*MATERIAL"
#define	MATNAME					"*MATERIAL_NAME"
#define	MATCLASS				"*MATERIAL_CLASS"
#define TEXTURE					"*BITMAP"
#define UTILE					"*UVW_U_TILING"
#define VTILE					"*UVW_V_TILING"
#define UOFFSET					"*UVW_U_OFFSET"
#define VOFFSET					"*UVW_V_OFFSET"

#define	NEWOBJET				"*GEOMOBJECT"
#define NODENAME				"*NODE_NAME"
#define	NODEPARENT				"*NODE_PARENT"
#define NUM_VERTEX				"*MESH_NUMVERTEX"
#define NUM_FACES				"*MESH_NUMFACES"
#define VERTEX_LIST				"*MESH_VERTEX_LIST"
#define ASEVERTEX				"*MESH_VERTEX"
#define FACE_LIST				"*MESH_FACE_LIST"
#define ASEFACE					"*MESH_FACE"
#define NUM_TVERTEX				"*MESH_NUMTVERTEX"
#define	TVERTEX_LIST			"*MESH_TVERTLIST"
#define TVERTEX					"*MESH_TVERT"
#define NUM_TFACES				"*MESH_NUMTVFACES"
#define TVFACE_LIST				"*MESH_TFACELIST"
#define TFACE					"*MESH_TFACE"
#define	MATID					"*MESH_MTLID"
#define OBJETTEXTID				"*MATERIAL_REF"

#define NORMALS					"*MESH_NORMALS"
#define FACE_NORMAL				"*MESH_FACENORMAL"
#define NVERTEX					"*MESH_VERTEXNORMAL"


#define	MAXVERTEX				51200
#define	MAXFACE					51200
#define	MAXTVERTEX				51200
#define	MAXTFACE				51200

#define	TEXTURENAMEMAX			255
#define TEXTURELOADED			255


typedef struct
{
	int		used;
	int		id;
	char	name[TEXTURENAMEMAX];
}
TEXLIST;

typedef struct
{
	float	x;
	float	y;
	float	z;
}
VERTEX_TYPE;

typedef struct
{
	int		id_vertex1;
	int		id_vertex2;
	int		id_vertex3;
	int		mat_id;
	int		brush_id;
}
FACE_TYPE;

typedef struct
{
	float	u;
	float	v;
}
TVERTEX_TYPE;

typedef struct
{
	int		id_tface1;
	int		id_tface2;
	int		id_tface3;
}
TFACE_TYPE;

typedef struct
{
	VERTEX_TYPE	Entity_Pos;
	int			Entity_ID;
	int			Entity_RotY;
}
TOBJET_TYPE;

VERTEX_TYPE		vertex[MAXVERTEX];
FACE_TYPE		face[MAXVERTEX];
TVERTEX_TYPE	tvertex[MAXVERTEX];
TFACE_TYPE		tface[MAXVERTEX];
TEXLIST			TextureList[TEXTURELOADED];

enum
{
	WALL,
	DETAIL,
	BSPLESS,
	LAVA,
	DOOR,
	INVI,
	ACTION,
	INVI2
};

// ENTITY ID
enum
{
	IDMONSTRE1=1,
	IDGUN1,
	IDAMO1,
	IDGUN2,
	IDAMO2,
	IDLIFT,
	IDGUN3,
	IDDOOR,
	IDAMO3,
	IDFIOLE1,
	IDLAMPE,
	IDMORBAK,
	IDPUMPKIN,
	IDMLAMPE,
	IDH,
	IDA,
	IDL,
	IDO,
	IDW,
	IDE,
	IDN,
	IDZEBAT,
	IDFIOLE2,
	IDFIOLE3,
	IDSUBZERAT,
	IDZEMUMMY,
	IDSTOP

};


bool f_ChargerASE(char *nom, int objet);
void ComputeEntityPosition(int EntityPos, int angle, int ID);


#endif __ASEREADER__
