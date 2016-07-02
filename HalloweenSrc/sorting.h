#ifndef SORTING_INCLUDED
#define SORTING_INCLUDED

#include "alloween.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef union
{
    int     ValeurEntiere;
    float	ValeurFlottante;

} Float;

typedef struct tabsort
{
	pEntity	Ent[MAX_TABSORT];
	/*int*/ Float		zvalue[MAX_TABSORT];
	int		id[MAX_TABSORT];
	int		sortedid[MAX_TABSORT];

}	TABSORT;


void sort_Process();
byte sort_getoctet(int num, int pos);
void sort_do(TABSORT *tabsort, /*int*/Float *tabtri, int size);
void sort2_do(Facetab_t *tabsort, int size);


#endif // SORTING_INCLUDED
