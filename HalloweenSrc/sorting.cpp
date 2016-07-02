
#include "alloween.cpp"

#define ALPHAPRECBIT	11
#define ALPHAPREC		(1 << ALPHAPRECBIT)
#define ALPHAMASK		(ALPHAPREC - 1)


int		gNbSort;
TABSORT	TabSort;


void sort_Process()
{
	int	i;
	Float	tabtrie[MAX_TABSORT];

	if(gNbSort<2)
		return;

	for(i=0 ; i<gNbSort ; i++)
	{
		TabSort.id[i] = i;
		TabSort.sortedid[i] = i;
		TabSort.zvalue[i].ValeurFlottante = m_Distance(TabSort.Ent[i]->ent_anim.Pos,gCamera.Pos);
	}

	sort_do(&TabSort,tabtrie,gNbSort);
}


byte sort_getoctet(int num, int pos)
{
	int		div=0;

	if(pos==1)
		div=8;
	else if(pos==2)
		div=16;
	else if(pos==3)
		div=32;

	return (num >> div); // % 256; // hack
}

void sort_do(TABSORT *tabsort, Float *tabtri, int size)
{
	Float	*tmp;
	int		*idtmp;
	Float	*tab;
	int		*idtab;
	int		*idsortedtab;
	int		tab2[256];
	int		i, pos, loop;
	byte	val;
	int		ZListIndex;

	tab = tabsort->zvalue;
	idtab = tabsort->id;
	idsortedtab = tabsort->sortedid;

	for(loop=0 ; loop<4 ; loop++)
	{
		for(i=0 ; i<256 ; i++)
			tab2[i] = 0;

		for(i=size-1 ; i>=0 ; i--)
		{
			ZListIndex = ((tab[i].ValeurEntiere >> 16) & ALPHAMASK);
			val = sort_getoctet(ZListIndex,loop);
			tab2[val]++;
		}

		for(i=1 ; i<256 ; i++)
			tab2[i] += tab2[i-1];

		for(i=size-1 ; i>=0 ; i--)
		{
			ZListIndex = ((tab[i].ValeurEntiere >> 16) & ALPHAMASK);
			val = sort_getoctet(ZListIndex,loop);
			pos = tab2[val];
			tab2[val] = tab2[val]-1;

			tabtri[pos-1] = tab[i];
			idsortedtab[pos-1] = idtab[i]; //
		}

		tmp = tab;
		tab = tabtri;
		tabtri = tmp;

		idtmp = idtab;
		idtab = idsortedtab;
		idsortedtab = idtmp;
	}
}



void sort2_do(Facetab_t *tabsort, int size)
{
/*
	Float	*tmp;
	int		*idtmp;
	Float	*tab;
	int		*idtab;
	int		*idsortedtab;
	int		tab2[256];
	int		i, pos, loop;
	byte	val;
	int		ZListIndex;

//	tab = tabsort->zvalue;
//	idtab = tabsort->id;
//	idsortedtab = tabsort->sortedid;

	for(loop=0 ; loop<4 ; loop++)
	{
		for(i=0 ; i<256 ; i++)
			tab2[i] = 0;

		for(i=size-1 ; i>=0 ; i--)
		{
			val = sort_getoctet(tabsort[i].id, loop);
			tab2[val]++;
		}

		for(i=1 ; i<256 ; i++)
			tab2[i] += tab2[i-1];

		for(i=size-1 ; i>=0 ; i--)
		{
			val = sort_getoctet(tabsort[i].id, loop);
			pos = tab2[val];
			tab2[val] = tab2[val]-1;

			tabsort[pos-1].sorted_id = tabsort[i].id;
		//	idsortedtab[pos-1] = idtab[i]; //
		}

	//	tmp = tab;
	//	tab = tabtri;
	//	tabtri = tmp;

	//	idtmp = idtab;
	//	idtab = idsortedtab;
	//	idsortedtab = idtmp;
	}
	*/
}

