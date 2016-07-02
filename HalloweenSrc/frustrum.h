#ifndef FRUSTRUM___
#define FRUSTRUM___

#include "alloween.h"


void	frustrum_DispMatrix();
void	frustrum_Disp(Plan_t Planes[]);
void	frustrum_ExtractPlanes(Plan_t Planes[]);
hBool	frustrum_LeafIn(Plan_t Plane[], int Leaf);
hBool	frustrum_BoxIn(Plan_t Plane[], BOUNDINGBOX bbox);


#endif //FRUSTRUM___
