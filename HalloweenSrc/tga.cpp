
#include "alloween.h"


#ifdef H_MAC
#include "KMiscTools.h"
#endif


unsigned int 	texFormat;
int				imageWidth, imageHeight;

int checkSize (int x)
{
	if (x == 2	 || x == 4 || 
		x == 8	 || x == 16 || 
		x == 32  || x == 64 ||
		x == 128 || x == 256 ||
                x == 512 || x == 1024)
		return 1;
	else return 0;
}

unsigned char *getRGBA(FILE *s, int size, int picmip)
{
	unsigned char 	*rgba;
	unsigned char 	temp;
	int 		bread;
	int 		i;
	int		RealSize;
//#ifdef H_MAC
//        int		pos, pos3, pos4;
//        unsigned char 	*rgb;
//#endif

	RealSize = size * 4;

	rgba = (unsigned char*)s_malloc(RealSize*sizeof(unsigned char));
	if (rgba == NULL)
		return NULL;

	bread = fread(rgba, sizeof(unsigned char), RealSize, s);
	
	if (bread != RealSize)
	{
		s_free(rgba);
		return NULL;
	}

        /*
        // on charge tout en 16bits sur mac
        #ifdef H_MAC
	rgb = (unsigned char*)s_malloc(3*size*sizeof(unsigned char));
	if(rgb == NULL) return NULL;
        for(pos=0; pos<size; pos++)
	{
		pos4 = pos*4;
		pos3 = pos*3;
		rgb[pos3+0] = rgba[pos4+2];
		rgb[pos3+1] = rgba[pos4+1];
		rgb[pos3+2] = rgba[pos4+0];
        }
        texFormat = GL_RGB;
        s_free(rgba);
        return rgb;
        #endif
        */

	for(i=0 ; i<RealSize; i+=4 )
	{
		temp = rgba[i];
		rgba[i] = rgba[i+2];
		rgba[i+2] = temp;
	}

	/*
	picmip_step = pow(2,picmip-1);

	for(i=0 ; i<imageWidth ; i+=picmip_step)
	for(j=0 ; j<imageHeight ; j+=picmip_step)
	{
		pos = 4 * (j*imageWidth + i);
		pos2 = 4 * ((j/picmip_step)*(imageWidth/picmip_step) + (i/picmip_step));

		rgba_lod[pos2+0] = rgba[pos+0];
		rgba_lod[pos2+1] = rgba[pos+1];
		rgba_lod[pos2+2] = rgba[pos+2];
		rgba_lod[pos2+3] = rgba[pos+3];
	}
*/

	texFormat = GL_RGBA; // 8
	return rgba;
}

unsigned char *getRGB(FILE *s, int size, int imageWidth, int imageHeight, int picmip)
{
	unsigned char *rgb;
	unsigned char *rgba;
	unsigned char *rgba_lod;
	int bread;
	int i, pos, pos2, pos3, pos4;
	int	j;
	int	picmip_size;
	int	picmip_step;

        /*
        // on charge tout en 16bits sur le mac
        #ifdef H_MAC
        rgb = (unsigned char*)s_malloc (size * 3 * sizeof(unsigned char));
	if(!rgb) return NULL;
        bread = fread (rgb, sizeof (unsigned char), size * 3, s);
	if(bread != size * 3)
	{
		s_free(rgb);
		return NULL;
	}
        texFormat = GL_RGB;
        return rgb;
        #endif
        */

	rgb = (unsigned char*)s_malloc (size * 3 * sizeof(unsigned char));
	if(!rgb)
		return NULL;

	rgba = (unsigned char*)s_malloc (size * 4 * sizeof(unsigned char));
	if(!rgba)
		return NULL;

	picmip_step = (picmip - 1) * 2;
	picmip_size = 4 * size / (int)pow(2,(double)picmip_step);
	rgba_lod = (unsigned char*)s_malloc(picmip_size * sizeof(unsigned char));
	if(!rgba_lod)
		return NULL;
	memset(rgba_lod, 0, picmip_size);

	bread = fread (rgb, sizeof (unsigned char), size * 3, s);

	if(bread != size * 3)
	{
		s_free(rgb);
		s_free(rgba);
		s_free(rgba_lod);
		return NULL;
	}

	// Gestion automatique du channel alpha
	for(pos=0; pos<size; pos++)
	{
		pos4 = pos*4;
		pos3 = pos*3;
		rgba[pos4+2] = rgb[pos3];
		rgba[pos4+1] = rgb[pos3+1];
		rgba[pos4] = rgb[pos3+2];

		if((rgb[pos3] + rgb[pos3+1] + rgb[pos3+2])*0.333f < 10 )
			rgba[pos4+3] = 0;
		else
			rgba[pos4+3] = 255;
  	}

	picmip_step = (int)pow(2,(double)picmip-1);

	if(picmip_step == 1)
	{
		texFormat = GL_RGBA;
		s_free(rgb);
		s_free(rgba_lod);
		return rgba;
	}

	for(i=0 ; i<imageWidth ; i += picmip_step)
	for(j=0 ; j<imageHeight ; j += picmip_step)
	{
		pos = 4 * (j*imageWidth + i);
		pos2 = 4 * ((j/picmip_step)*(imageWidth/picmip_step) + (i/picmip_step));

		rgba_lod[pos2+0] = rgba[pos+0];
		rgba_lod[pos2+1] = rgba[pos+1];
		rgba_lod[pos2+2] = rgba[pos+2];
		rgba_lod[pos2+3] = rgba[pos+3];
	}

	texFormat = GL_RGBA; // 8
	s_free(rgb);
	s_free(rgba);

	return rgba_lod;
}

char *getData (FILE *s, int sz, int imageWidth, int imageHeight, int iBits, int picmip)
{
	char *data;

	if (iBits == 32)
		return (char *)getRGBA (s, sz, picmip);
	else if (iBits == 24)
	{
		data = (char *)getRGB (s, sz, imageWidth, imageHeight, picmip);
		return data;
	}

	return NULL;
}

int tga_Load(char *name, int id, int picmip, hBool map_tex)
{
	unsigned char	type[4];
	unsigned char	info[7];
	unsigned char	*imageData = NULL;
	int		imageBits, size, k;
	FILE		*s;
	char		fullname[255];
	hBool		picmip_locked;

	if(notextures.value)
	if( strncmp("font",&name[strlen(name)-1-3-4],4) != 0)
		return 1;

	if(gIsServer && net_dedicated.value)
		return 1;

	sprintf(fullname,"%s%s%s",SYSDIR,TEXDIR,name);

#ifdef H_MAC
	char * fullpath = KMiscTools::makeFilePath(fullname);
	s = fopen(fullpath, "r"); // r+bt
#else
	s = fopen(fullname, "r+b"); // r+bt
#endif

	if(!s)
	{
		m_ConsPrint("TGA_FILE_NOT_FOUND( %s )\n", fullname);
		return -1;
	}

	fread(&type, sizeof (char), 3, s);
	fseek(s, 12, SEEK_SET);
	fread(&info, sizeof (char), 6, s);

	if(type[1] != 0 || type[2] != 2)
	{
		fclose(s);
        m_ConsPrint("TGA_BAD_IMAGE_TYPE\n");
		return -1;
	}

	imageWidth = info[0] + info[1] * 256;
	imageHeight = info[2] + info[3] * 256;
	imageBits =	info[4];

	size = imageWidth * imageHeight;

	if(!checkSize (imageWidth) || !checkSize (imageHeight))
	{
		fclose(s);
		m_ConsPrint("TGA_BAD_DIMENSION( %d %d)\n", imageWidth, imageHeight);
		return -1;
	}

	if(imageBits != 32 && imageBits != 24)
	{
		fclose(s);
		m_ConsPrint("TGA_BAD_BITS\n");
		return -1;
	}

	if(picmip == -1)
	{
		picmip = 1;
		picmip_locked = true;
	}
	else
		picmip_locked = false;

	if(picmip < 1)
		picmip = 1;
	if(picmip > 6)
		picmip = 6;

	imageData = (unsigned char *)getData(s,size,imageWidth,imageHeight,imageBits,picmip);

	imageWidth /= (int)pow(2.0,picmip-1);
	imageHeight /= (int)pow(2.0,picmip-1);

	fclose(s);

	if (imageData == NULL)
        {
            m_ConsPrint("TGA_BAD_DATA\n");
            return -1;
        }

	if(id == -1)
	{
		k = 1;
        while(TextureList[k].used)
		{
			if(!strncmp(TextureList[k].name,name,strlen(name)))
			{
				s_free(imageData);
                //m_ConsPrint("texture already loaded( %s )\n", name);
				return TextureList[k].id;
			}
			k++;
			if(k >= TEXTURELOADED)
				break;
		}
	}
	else
   	{
		k = id;
   	}
        
	if(k >= TEXTURELOADED)
	{
		s_free(imageData);
		m_ConsPrint("too much loaded texture. max : %d", TEXTURELOADED);
		return -1;
	}

	TextureList[k].used = true;
	TextureList[k].type = TEX_TGA;
	TextureList[k].map_tex = map_tex;
	TextureList[k].id = k;
	strcpy(TextureList[k].name,name);
	TextureList[k].picmip_locked = picmip_locked;

	gl_LoadSurfaceTexture(imageData, GL_UNSIGNED_BYTE, texFormat, texFormat, imageWidth, imageHeight, k);
	
	//printf("TGA loaded ok: %s\n", name);
	
	return k;
}

