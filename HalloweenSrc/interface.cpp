
#include "alloween.cpp"





int int_GetButonId(int *anim_time, int modulo, float speed)
{
	int			anim;

	if(*anim_time == -1)
	{
		*anim_time = disp_GetClock();
		return 0;
	}
	anim = (int)((disp_GetClock() - *anim_time) * tflame.value);
	anim %= modulo;
	return anim;
}

void int_DrawTitle()
{
	int			id;
	static int	anim_time = -1;

	if(gMenu_InitFirstSel)
		return;

	id = int_GetButonId(&anim_time,30/*25*/,0);

	disp_DrawSprite_2(txid_NEWTITLE[id],48,460,728,128,false);

	if(	ProgramState != PS_GREETINGS	&&
		ProgramState != PS_ORDER		&&
		ProgramState != PS_STARTMAP)
	{
		disp_DrawSprite_2(txid_TNN[0],48,396,512,64,true);
		disp_DrawSprite_2(txid_TNN[1],560,396,512,64,true);
	}
}

void int_DrawBackground()
{
	if(gMenu_InitFirstSel)
		return;

	/*
	if(ProgramState == PS_SELECTMAP)
	{
		Color_t	col = {1, 0.392f, 0, 1};
		gl_SetClearColor(col);

	//	disp_DrawSprite(txid_CONSOLE,0,0,640,480,false);
		return;
	}
	else
	{
		Color_t	col = {0, 0, 0, 1};
		gl_SetClearColor(col);
	}
	*/	

	if(ProgramState != PS_GREETINGS && ProgramState != PS_SELECTMAP)
		disp_DrawSprite_2(txid_BG1,0,-100,128,512,false);
	
	if(ProgramState == PS_PRES)
		disp_DrawSprite_2(txid_BG2,128,-100,512,512,false);
	else
		disp_DrawSprite_2(txid_BG2_G,128,-100,512,512,false);

	if(ProgramState == PS_SETSLOTGAME)
	{
		disp_DrawSprite_2(txid_SELECTGAMESLOT,312,374,512,32,true);
	}
}


