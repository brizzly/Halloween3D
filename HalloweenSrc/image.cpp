#include "alloween.h"

#ifdef H_LINUX
//#include <sys/types.h>
//#include <sys/dir.h>
//#define _mkdir mkdir
#endif

#ifdef H_MAC
#include "KMiscTools.h"
//#include <sys/types.h>
//#include <sys/dir.h>
//#include <dir.h>
//#define _mkdir mkdir
#endif


hBool im_Init()
{
	return true;
}

int im_Load(char *name, int id, int picmip, hBool map_tex)
{
	FILE	*file;
	char	path[255];
	char 	*tmpnametga;
	char 	*tmpname;

//	#ifdef DEV_NETWORK
//	if(gIsServer)
//	if(strncmp("console",name,6) != 0 && strncmp("font",name,4) != 0)
	if(notextures.value)
	if( strncmp("font",&name[strlen(name)-1-3-4],4) != 0)
		return 1;
//	#endif

	if(gIsServer && net_dedicated.value)
		return 1;                      
                                        
	m_ConsPrint("%s\n",name);
	sprintf(path,"%s%s%s",SYSDIR,TEXDIR,name);
	
	
#ifdef H_MAC
		file = fopen(KMiscTools::makeFilePath(path), "r"); // rb
#else
		file = fopen(path,"rb");
#endif	

	if(fs_IsFileExtension(name,"jpg"))
	{
		if(!file)
		{             
			tmpnametga = fs_RenameFileExtension(name,"tga");
			return tga_Load(tmpnametga,id,picmip,map_tex);
		}
		fclose(file);
		return jpeg_Load(name,id,picmip,map_tex);
	}
	else if(fs_IsFileExtension(name,"tga"))
	{
		if(!file)
		{
			tmpname = fs_RenameFileExtension(name,"jpg");
			return jpeg_Load(tmpname,id,picmip,map_tex);
		}
		fclose(file);
		return tga_Load(name,id,picmip,map_tex);
	}
	else
	{
		if(file)
			fclose(file);
		return -1;
	}
}

void im_ReloadTextures()
{
	int		i;
	int		ntext;
	char	name[TEXTURELOADED][TEXTURENAMEMAX];
	hBool	picmipbool[TEXTURELOADED];
	hBool	texmapbool[TEXTURELOADED];
	GLuint	tmpTextureId;

	ntext=0;
	for(i=0 ; i<TEXTURELOADED ; i++)
	if(TextureList[i].used)
	{
		strcpy(name[ntext],TextureList[i].name);
		picmipbool[ntext] = TextureList[i].picmip_locked;
		texmapbool[ntext] = TextureList[i].map_tex;
		ntext++;
	}

	g_ResetTextureStruct();

	tmpTextureId = game_Load2D("logo_restartsys.tga");
	DispText("",tmpTextureId);
	glDeleteTextures(1, &tmpTextureId); //
	g_ResetTextureStruct();

	for(i=0 ; i<ntext ; i++)
	{
		if(picmipbool[i] == false)
			im_Load(name[i],-1,(int)picmip.value,texmapbool[i]);
		else
			im_Load(name[i],-1,-1,texmapbool[i]);
	}

//	glDeleteTextures(1, &tmpTextureId);
}

void im_UnloadTextures()
{
	int	i;
	GLuint	tid;

	for(i=0 ; i<TEXTURELOADED ; i++)
	{
		if(TextureList && TextureList[i].name)
		{
			tid = (GLuint)TextureList[i].id;
			glDeleteTextures(1, &tid);
		}
	}
}

void im_UnloadMapTextures()
{
	int	i;
	GLuint	tid;

	for(i=0 ; i<TEXTURELOADED ; i++)
	{
		if(TextureList)
		if(TextureList[i].name)
		if(TextureList[i].map_tex)
		{
			tid = (GLuint)TextureList[i].id;
			glDeleteTextures(1, &tid);
			TextureList[i].used = false;
			TextureList[i].id = -1;
		}
	}
}

void im_ScreenShotTGA()
{
	FILE	*file;
	//int		id = 0;
	char	filename[255];
	int		idshot = 0;
	char	path[255];
#ifdef H_MAC
	char	*ptr;
#endif
	byte 	*pixels;
	byte	p;
	int		i;
	int		ScreenshotSize=0;

	sprintf(path,"%s%s",SYSDIR,SHTDIR);
	fs_makedir(path);

	while(1)
	{
		sprintf(filename,"%sshot%.4d.tga",path,idshot);
		//m_ConsPrint("%s\n",filename);
		
	#ifdef H_MAC
	ptr = KMiscTools::makeFilePath(filename);
	file = fopen(ptr,"r");
	#else
	file = fopen(filename,"r");
	#endif
		
		
		if(file)
		{
			fclose(file);
			if(idshot >= 9999)
			{
				m_ConsPrint("Too much screenshots!\n");
				return;
			}
			idshot++;
		}
		else
			break;
	}

	ScreenshotSize = 18+(ScreenX[(int)videomode.value]*ScreenY[(int)videomode.value]*3);
	pixels = (byte*)s_malloc(ScreenshotSize);
	memset(pixels, 0, 18);
	pixels[2] = 2;		// uncompressed type
	pixels[12] = (short)ScreenX[(int)videomode.value]&255;
	pixels[13] = (short)ScreenX[(int)videomode.value]>>8;
	pixels[14] = (short)ScreenY[(int)videomode.value]&255;
	pixels[15] = (short)ScreenY[(int)videomode.value]>>8;
	pixels[16] = 24;	// pixel size

	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,ScreenX[(int)videomode.value],ScreenY[(int)videomode.value],GL_RGB,GL_UNSIGNED_BYTE,pixels+18);

	for(i=18 ; i<ScreenshotSize ; i+=3)
	{
		p = pixels[i];
		pixels[i] = pixels[i+2];
		pixels[i+2] = p;
	}

	#ifdef H_MAC
	file = fopen(ptr,"w");
	#else
	file = fopen(filename,"w+b");
	#endif
	
	
	if(!file)
		return;
	
	sprintf(path, "Screenshot: %s\n", filename);
	m_ConsPrint("%s\n",path);
	game_catchedinit2(path);

	fwrite(pixels,ScreenshotSize,1,file);
	fclose(file);
	s_free(pixels);
}
