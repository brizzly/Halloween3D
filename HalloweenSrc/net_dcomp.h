#ifndef NETDCOMP_INCLUDED
#define NETDCOMP_INCLUDED

#include "alloween.h"



float	net_DelCmp_PacketPrecision(int ptype);
float	net_Float2Short(float val, float step);
//short	net_Float2Short(float val, float step);
float	net_Short2Float(float val, float step);
void	net_SendDeltaCompressedPos(int entpos, int entid, vect_t delta_v, int packet_type, hBool force3val);
void	net_UpdateEntity_DeltaCompressed(pEntity Ent, int packet_type, vect_t dc_vec, int nval);
void	net_GetDeltaCompressedPos(vect_t v, int packet_type, float Step, int nval, vect_t *vec_out);



#endif // NETDCOMP_INCLUDED
