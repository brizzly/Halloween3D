
#ifndef OBJPROC_INCLUDED
#define OBJPROC_INCLUDED

#include "alloween.h"

void		FioleProcess(pEntity FioleEnt, pEntity PlayerEnt);
void		JadeProcess(pEntity JadeEnt);

pEntity		ExplosionProcess(pEntity ExplosionEnt);
pEntity		BombProcess(pEntity BombEnt, pEntity PlayerEnt);
pEntity		GibProcess(pEntity GibEnt, pEntity PlayerEnt);
pEntity		SabreProcess(pEntity SabreEnt);
pEntity		AmmoProcess(pEntity AmmoEnt);
pEntity		PumpkinProcess(pEntity PumpkinEnt);
pEntity		GunProcess(pEntity GunEnt);
pEntity		FlecheProcess(pEntity PlayerEnt, pEntity FlecheEnt);
pEntity		MonstreProcess(pEntity MonstreEnt);
pEntity		LampeProcess(pEntity LampeEnt, int *isanim);
pEntity		LettreProcess(pEntity LettreEnt);
pEntity		PangProcess(pEntity PangEnt, pEntity PlayerEnt);


#endif // OBJPROC_INCLUDED
