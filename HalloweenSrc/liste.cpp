
#include "alloween.cpp"


pFace liste_Delete(pFace List)
{
	pFace	Face;

	while(List)
	{
		Face = List;
		List = List->Next;
		s_free(Face);
	}
	return NULL;
}

