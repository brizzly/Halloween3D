
#include "alloween.cpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int jpeg_VidLoad(byte *data, int w, int h, int text_id, char *Name, hBool map_tex)
{
	int	k;

	if(data == NULL)
           return 0;
 	if(text_id == -1)
   	{
           k = 1; //0
           while(TextureList[k].used)
           {
	      if(!strncmp(TextureList[k].name,Name,strlen(Name)))
	      {
	        m_ConsPrint("Already loaded ! (id:%d)\n",TextureList[k].id);
	        s_free(data);
		 return TextureList[k].id;
		 }
		 k++;
		 if(k >= TEXTURELOADED)
	          break;
	   }
   	}
    	else
    	{
	    k = text_id;
    	}
    
    	if(k >= TEXTURELOADED)
    	{
       		m_ConsPrint("too much loaded texture. max : %d", TEXTURELOADED);
       		s_free(data);
       		return -1;
     	}
    
    	TextureList[k].used = true;
    	TextureList[k].type = TEX_JPG;
    	TextureList[k].map_tex = map_tex;
    	TextureList[k].id = k;
    	strcpy(TextureList[k].name,Name);
    	TextureList[k].picmip_locked = -1;
    	gl_LoadSurfaceTexture(data, GL_UNSIGNED_BYTE, GL_RGB, GL_BGRA_EXT, w, h, k);
		 
    	return k;
}

int jpeg_Load(char *Name, int id, int picmip, hBool map_tex)
{
	int		w;
	int		h;
	char	fullname[255];
#ifdef H_WINDOWS
	oglBitmap	*data;
	unsigned long	*img_data;
#else
	byte		*img_data;
#endif

	if(notextures.value)
		return 1;
	if(gIsServer && net_dedicated.value)
		return 1;
	sprintf(fullname,"%s%s%s",SYSDIR,TEXDIR,Name);

	return LoadFromJPEG(Name, KMiscTools::makeFilePath(fullname), id, map_tex);
	
#ifdef H_WINDOWS
	data = LoadPictureFromFile(fullname);
	if(data == NULL)
		return false;

	img_data = data->PixMapPtr;
	w = data->width;
	h = data->height;
#endif

	return jpeg_VidLoad((byte*)img_data, w, h, id, Name, map_tex);
}


// JPEG ----------------------------------------------------------

int LoadFromJPEG(char *Name, char *filename, int text_id, hBool map_tex)
{
	int jpeg_w, jpeg_h, channels;
	unsigned char * jpeg_buffer = stbi_load(filename, &jpeg_w, &jpeg_h, &channels, 0);
	if(jpeg_buffer == NULL) {
		printf("Error in loading the image: %s\n", filename);
		return 0;
	}

	
	int	k;
	
	if(text_id == -1)
	{
		k = 1; //0
		while(TextureList[k].used)
		{
			if(!strncmp(TextureList[k].name,Name,strlen(Name)))
			{
				m_ConsPrint("Already loaded ! (id:%d)\n",TextureList[k].id);
				return TextureList[k].id;
		 }
		 k++;
		 if(k >= TEXTURELOADED)
			 break;
		}
	}
	else
	{
		k = text_id;
	}
	
	if(k >= TEXTURELOADED)
	{
		m_ConsPrint("too much loaded texture. max : %d", TEXTURELOADED);
		s_free(jpeg_buffer);
		//delete [] jpeg_buffer;
		return -1;
	}
	
	TextureList[k].used = true;
	TextureList[k].type = TEX_JPG;
	TextureList[k].map_tex = map_tex;
	TextureList[k].id = k;
	strcpy(TextureList[k].name,Name);
	TextureList[k].picmip_locked = -1;
	gl_LoadSurfaceTexture(jpeg_buffer, GL_UNSIGNED_BYTE, GL_RGB, GL_RGB, jpeg_w, jpeg_h, k);
	

	//s_free(jpeg_buffer);
	return k;
}






