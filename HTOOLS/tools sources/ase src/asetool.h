
#ifndef __ASETOOL__
#define	__ASETOOL__


typedef struct
{
	float	X;
	float	Y;
	float	Z;

} vect_t;

typedef struct
{
	float	X;
	float	Y;
	float	Z;
	float	U;
	float	V;
	float	NX;
	float	NY;
	float	NZ;
	bool	Used_Opacity;
	bool	Used;
	bool	GroupUsed;

} point_t;

typedef struct
{
	vect_t	VecteurNormal;
	vect_t	PointOnPlane;
} plan_t;

typedef struct tagrect *portal_t;
typedef struct tagrect *face_t;
typedef struct tagrect *pFace;
typedef struct	tagrect
{
	int		IsDetailObjet;
	int		BrushID;
	int		ActionNumber;
	bool	Used;
	bool	BSP_used;
	point_t	*point;
	int		NumberOfVertices;
	int		textId;
	plan_t	Plan;
	int		NumberOfLeaf;
	int		LeafOwnerArray[4];
	int		nodes[2];
	bool	Removed;
	struct	tagrect	*Next;

} RECTANGLE;

typedef struct tag_bbox
{
	vect_t		BoxMin;
	vect_t		BoxMax;

} BOUNDINGBOX;

typedef struct ENTITYTAG
{
	vect_t	pos;
	int		id;
	int		angleY;

} ENTITY;

typedef struct LIGHTTAG
{
	vect_t	pos;
	int		R;
	int		G;
	int		B;
	float	I;
	int		type;
	
} LIGHT;

typedef struct objtag
{
	int			type;
	RECTANGLE	*liste;

} OBJ;

pFace		AddListes(pFace PortalList, pFace AddList);
face_t		AllocFace(int Nb_Points, bool track);
pFace		AddBoundFaces(pFace List);
void		ComputeNormals(RECTANGLE *gFaceList);
void		Overlaps(RECTANGLE *gFaceList);
bool		IsFacesOverlap(RECTANGLE *Fa, RECTANGLE *Fb);
bool		IsRectangle(RECTANGLE *Face);
bool		CutSemiOverlapFaces(RECTANGLE *Fa, RECTANGLE *Fb);
void		Spliting(RECTANGLE *Poly, plan_t *Plan, RECTANGLE *FrontSplit, RECTANGLE *BackSplit);
bool		IsVertexEgal(vect_t v, vect_t a);
bool		IsVertexEgalFace(vect_t v, RECTANGLE *Face);
bool		IsVertexOnEdgeFace(vect_t v, RECTANGLE *Face);
bool		IsVertexOnEdge(vect_t v, vect_t a, vect_t b);
bool		IsVertexOnFace(vect_t V, RECTANGLE *Face);
bool		IsVertexOnFace_2(vect_t V, RECTANGLE *Face);
RECTANGLE *	RemoveFaceFromList(RECTANGLE *racine, RECTANGLE *remove);

bool		PutPolyToFile(char *path, RECTANGLE *RectList);
RECTANGLE	*Load_XYZ(char *nom, char *data);
face_t		GroupFaces(face_t PolyList);
int			CountFaces(face_t facelist);
void		AddObject(RECTANGLE *list);
bool		PutObjectToFile(char *path);
face_t		SplitToTriangle(face_t PolyList);


pFace		gPolyList;
ENTITY		Entities2[255];
OBJ			Objets[255];
LIGHT		Lights[255];



#endif __ASETOOL__
