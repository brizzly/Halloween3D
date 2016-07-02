
#include "alloween.cpp"
#include "net_dcomp.h"

#include <OpenPlay.h>





float net_DelCmp_PacketPrecision(int ptype)
{
	switch(ptype)
	{
		// update de POSITION
		case PTYPE_5:
		return SHORT2FLOATPREC; // 

		// update de VELOCITE
		case PTYPE_6:
		return SHORT2FLOATPREC4;

		// update de ROTATION
		case PTYPE_7:
		return SHORT2FLOATPREC0;

		// update de ACCELERATION
		case PTYPE_8:
		return SHORT2FLOATPREC4;
	}
	return 1;
}

float net_Float2Short(float val, float step)
//short net_Float2Short(float val, float step)
{
	return val;
//	return (short)(val * step);
}

float net_Short2Float(float val, float step)
{
	return val;
//	return val / step;
}

void net_SendDeltaCompressedPos(int entpos, int entid, vect_t delta_v, int packet_type, hBool force3val)
{
	char			buffer[40];
	Net_Ent_1val	NetEnt1;
	Net_Ent_2val	NetEnt2;
	Net_Ent_3val	NetEnt3;
	byte			type;
	float			Step;


	type = 0;
	type = type | packet_type;
	Step = net_DelCmp_PacketPrecision(packet_type);

	if(force3val)
		goto envoi_3val;

	if(!delta_v.X && !delta_v.Y && !delta_v.Z)
		return;

	if(!delta_v.X)
	{
		if(!delta_v.Y)
		{
			// envoi Z
			NetEnt1.type = type;
			NetEnt1.Ent_Pos = (byte)entpos;
			NetEnt1.Ent_ID = (byte)entid;
			NetEnt1.val = net_Float2Short(delta_v.Z, Step);
			memcpy(buffer,&NetEnt1,sizeof(Net_Ent_1val));
			sv_Send2(buffer, sizeof(Net_Ent_1val), PacketType_UpdateEnt_1val, !false);
		}
		else
		{
			if(!delta_v.Z)
			{
				// envoi Y
				type = type | PTYPE_2;
				NetEnt1.type = type;
				NetEnt1.Ent_Pos = (byte)entpos;
				NetEnt1.Ent_ID = (byte)entid;
				NetEnt1.val = net_Float2Short(delta_v.Y, Step);
				memcpy(buffer,&NetEnt1,sizeof(Net_Ent_1val));
				sv_Send2(buffer, sizeof(Net_Ent_1val), PacketType_UpdateEnt_1val, !false);
			}
			else
			{
				// envoi YZ
				type = type | PTYPE_2;
				NetEnt2.type = type;
				NetEnt2.Ent_Pos = (byte)entpos;
				NetEnt2.Ent_ID = (byte)entid;
				NetEnt2.val[0] = net_Float2Short(delta_v.Y, Step);
				NetEnt2.val[1] = net_Float2Short(delta_v.Z, Step);
				memcpy(buffer,&NetEnt2,sizeof(Net_Ent_2val));
				sv_Send2(buffer, sizeof(Net_Ent_2val), PacketType_UpdateEnt_2val, !false);
			}
		}
	}
	else
	{
		if(!delta_v.Y)
		{
			if(!delta_v.Z)
			{
				// envoi X
				type = type | PTYPE_1;
				NetEnt1.type = type;
				NetEnt1.Ent_Pos = (byte)entpos;
				NetEnt1.Ent_ID = (byte)entid;
				NetEnt1.val = net_Float2Short(delta_v.X, Step);
				memcpy(buffer,&NetEnt1,sizeof(Net_Ent_1val));
				sv_Send2(buffer, sizeof(Net_Ent_1val), PacketType_UpdateEnt_1val, !false);
			}
			else
			{
				// envoi XZ
				type = type | PTYPE_1;
				NetEnt2.type = type;
				NetEnt2.Ent_Pos = (byte)entpos;
				NetEnt2.Ent_ID = (byte)entid;
				NetEnt2.val[0] = net_Float2Short(delta_v.X, Step);
				NetEnt2.val[1] = net_Float2Short(delta_v.Z, Step);
				memcpy(buffer,&NetEnt2,sizeof(Net_Ent_2val));
				sv_Send2(buffer, sizeof(Net_Ent_2val), PacketType_UpdateEnt_2val, !false);
			}
		}
		else
		{
			if(!delta_v.Z)
			{
				// envoi XY
				type = type | PTYPE_1;
				type = type | PTYPE_4;
				NetEnt2.type = type;
				NetEnt2.Ent_Pos = (byte)entpos;
				NetEnt2.Ent_ID = (byte)entid;
				NetEnt2.val[0] = net_Float2Short(delta_v.X, Step);
				NetEnt2.val[1] = net_Float2Short(delta_v.Z, Step);
				memcpy(buffer,&NetEnt2,sizeof(Net_Ent_2val));
				sv_Send2(buffer, sizeof(Net_Ent_2val), PacketType_UpdateEnt_2val, !false);
			}
			else
			{
envoi_3val:
				// envoi XYZ
				NetEnt3.type = type;
				NetEnt3.Ent_Pos = (byte)entpos;
				NetEnt3.Ent_ID = (byte)entid;
				NetEnt3.val[0] = net_Float2Short(delta_v.X, Step);
				NetEnt3.val[1] = net_Float2Short(delta_v.Y, Step);
				NetEnt3.val[2] = net_Float2Short(delta_v.Z, Step);
				memcpy(buffer,&NetEnt3,sizeof(Net_Ent_3val));
				sv_Send2(buffer, sizeof(Net_Ent_3val), PacketType_UpdateEnt_3val, true);
			}
		}
	}
}

void net_GetDeltaCompressedPos(vect_t v, int packet_type, float Step, int nval, vect_t *vec_out)
{
	// recupere les 1,2 ou 3 valeurs et les convertit de byte->float
	if(nval == 1)
	{
		if(packet_type & PTYPE_1)
		{
			vec_out->X = net_Short2Float(v.X, Step);
		}
		else if(packet_type & PTYPE_2)
		{
			vec_out->Y = net_Short2Float(v.X, Step);
		}
		else
		{
			vec_out->Z = net_Short2Float(v.X, Step);
		}
	}
	else if(nval == 2)
	{
		if(packet_type & PTYPE_1)
		{
			vec_out->X = net_Short2Float(v.X, Step);
		}
		else if(packet_type & PTYPE_2)
		{
			vec_out->Y = net_Short2Float(v.X, Step);
		}
		else
		{
			vec_out->Z = net_Short2Float(v.X, Step);
		}

		
		
		if(packet_type & PTYPE_4)
		{
			vec_out->Y = net_Short2Float(v.Y, Step);
		}
		else
		{
			vec_out->Z = net_Short2Float(v.Y, Step);
		}
	}
	else
	{
		vec_out->X = net_Short2Float(v.X, Step);
		vec_out->Y = net_Short2Float(v.Y, Step);
		vec_out->Z = net_Short2Float(v.Z, Step);
	}
}

void net_UpdateEntity_DeltaCompressed(pEntity Ent, int packet_type, vect_t dc_vec, int nval)
{
	vect_t	vec;
	float	Step;

	if(!Ent)
		return;

	if(nval < 1 || nval > 3)
		return;
	
	if(packet_type & PTYPE_5)
	{
		vec.X = 0;
		vec.Y = 0;
		vec.Z = 0;
		Step = net_DelCmp_PacketPrecision(PTYPE_5);
		net_GetDeltaCompressedPos(dc_vec, packet_type, Step, nval, &vec);

		Ent->ent_anim.Pos.X = Ent->ent_anim.net_OldPos.X + vec.X;
		Ent->ent_anim.Pos.Y = Ent->ent_anim.net_OldPos.Y + vec.Y;
		Ent->ent_anim.Pos.Z = Ent->ent_anim.net_OldPos.Z + vec.Z;
	}
	else if(packet_type & PTYPE_6)
	{
		vec.X = 0;
		vec.Y = 0;
		vec.Z = 0;
		Step = net_DelCmp_PacketPrecision(PTYPE_8);
		net_GetDeltaCompressedPos(dc_vec, packet_type, Step, nval, &vec);

		Ent->ent_anim.Pos_Vel.X = Ent->ent_anim.net_OldVel.X + vec.X;
		Ent->ent_anim.Pos_Vel.Z = Ent->ent_anim.net_OldVel.Z + vec.Z;
	}
	else if(packet_type & PTYPE_7)
	{
		vec.X = 0;
		vec.Y = 0;
		vec.Z = 0;
		Step = net_DelCmp_PacketPrecision(PTYPE_7);
		net_GetDeltaCompressedPos(dc_vec, packet_type, Step, nval, &vec);

		Ent->ent_anim.Rot.Y = cam_RotationCycle(Ent->ent_anim.net_OldRot.Y + vec.Y);
	}
	else if(packet_type & PTYPE_8)
	{
		vec.X = 0;
		vec.Y = 0;
		vec.Z = 0;
		Step = net_DelCmp_PacketPrecision(PTYPE_8);
		net_GetDeltaCompressedPos(dc_vec, packet_type, Step, nval, &vec);

		Ent->ent_anim.Pos_Acc.X = Ent->ent_anim.net_OldAcc.X + vec.X;
		Ent->ent_anim.Pos_Acc.Z = Ent->ent_anim.net_OldAcc.Z + vec.Z;
	}
}

