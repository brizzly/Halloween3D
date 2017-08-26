
#include "alloween.h"


#ifdef H_WINDOWS
#include <io.h>
#endif

#ifdef H_MAC
#endif


#ifdef H_LINUX
#include "dirlib.h"
#endif



#define nPTR	10240
//#define MEMORYPRINT


hBool	IsScreenShot=false;
hBool	IsConsoleActive=false;
hBool	gKey_Console=false;
uint	gMemoryUsage=0;

struct Ptrtag
{
	hBool	used;
	void	*ptr;
	uint	size;

} hPtr_t[nPTR];


void fs_launchURL(char *url)
{
#ifdef H_MAC
	/*
    //#include <InternetConfig.h>
    
    long start, end;
    int len;
    ICInstance anInstance;
                    
    if(!url)
        return;
                    
    if(ICStart(&anInstance, 'HALL') == noErr)
    {
        len = strlen(url);
        start = 0;
        end = len;
        ICLaunchURL(anInstance, NULL, url, len, &start, &end );
        ICStop(anInstance);
    }*/
#endif

#ifdef H_WINDOWS
    ShellExecute( NULL , "open" , url, 0, 0, SW_SHOW);
#endif

#ifdef H_LINUX
#endif
}

void fs_makedir(char *dirname)
{
    if(dirname == NULL)
        return;
    
    #ifdef H_WINDOWS
        _mkdir(dirname);
    #else
        //mkdir(dirname);
    #endif
}

char *fs_RenameFileExtension(char *name, char *extension)
{
#ifdef H_WINDOWS
	static	char	renamed[260];
	int	Ptr=0;

	if(!name || !extension)
		return false;

	strcpy ( renamed, name ) ;

	while(renamed[Ptr] != '.')
	{
		if(renamed[Ptr] == 0)
		{
			renamed[Ptr] = '.';
	//		renamed[Ptr+1] = '_';
	//		renamed[Ptr+2] = '_';
	//		renamed[Ptr+3] = '_';
			break;
			//return false;
		}
		Ptr++;
	}
//	if(name[Ptr+1] == '\0')
//		return false;
//	if(!strncmp(&name[Ptr+1],extension,3))
//		return true;
	strncpy(&renamed[Ptr+1],extension,3);
	renamed[Ptr+4] = 0;

	m_ConsPrint("rename function enorme, renamed: %s\n", renamed);

//	strcpy ( renamed, name ) ;
	return (char*)renamed;
//	return (char*)name;

#else

	long	l ;
	int		hPtr = 0;
	static	char	renamed[260];
	
	strcpy ( renamed, name ) ;
	 
	if(!name || !extension)
			return name;
	
	l = strlen( renamed ) ;
	
	renamed[l-3] = extension[0];
	renamed[l-2] = extension[1];
	renamed[l-1] = extension[2];
	
	m_ConsPrint("%s\n", renamed);

	return (char*)renamed;
#endif
}

hBool fs_IsFileExtension(char *name, char *extension)
{
	int	hPtr=0;

	while(name[hPtr]!='.')
	{
		if(name[hPtr] == '\0')
			return false;
		hPtr++;
	}
	if(!strncmp(&name[hPtr+1],extension,3))
		return true;
	return false;
}

/*
====================================
sauve une capture d'écran en TGA
====================================
*/
hBool f_ScreenShot(char *nom)
{
	FILE			*fichier;
	char			path[255];
	char			*p;
	byte 			*pixels;
	int				ScreenshotSize=0;
	byte			temp;
	int				i;

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

	// swap rgb to bgr
	for (i=18 ; i<ScreenshotSize ; i+=3)
	{
		temp = pixels[i];
		pixels[i] = pixels[i+2];
		pixels[i+2] = temp;
	}

	sprintf(path,"%s%s",SYSDIR,nom);
	
	#ifdef H_MAC
	p = KMiscTools::makeFilePath(path);
	fichier = fopen(p,"wt");
	#else
	fichier = fopen(path,"wt");
	#endif
	
	if(!fichier)
		return false;
	fwrite(pixels,ScreenshotSize,1,fichier);
	fclose(fichier);
	s_free(pixels);

	return true;
}

/*
==============================================================
Transforme un emplacement systeme d'un fichier WIN vers LINUX
Ex: C:\dir\file -> C:/dir/file
==============================================================
*/
void str2linux(char str[])
{
	int i=0;

	while(str[i]!='\0')
	{
		if(str[i]==92) //'\'
			str[i]='/';
		i++;
	}
}

void fs_GetFileNameFromPath(char *path, char *name)
{
	int		i, j;

	if(!name)
		return;

	memset(name,0,255);

	for(i=0,j=-1 ; path[i] ; i++)
	{
		if(path[i] == 92)
			j=i+1;
	}
	if(j!=-1)
	{
		for(i=j ; path[i] ; i++)
			name[i-j] = path[i];
	}
	else
		strcpy(name,path);
}

void *s_malloc(uint size)
{
	void	*p;
	
	#ifdef MEMORYPRINT
	int		i=0, ptrpos=0;
	#endif

	p = malloc(size);

#ifdef MEMORYPRINT

	if(gMemoryUsage==0)
	{
		for(i=0 ; i<nPTR ; i++)
		{
			hPtr_t[i].ptr = NULL;
			hPtr_t[i].size = 0;
			hPtr_t[i].used = false;
		}
	}

	gMemoryUsage += size;
	ptrpos=0;
	while(hPtr_t[ptrpos].used)
	{
		ptrpos++;
		if(ptrpos>=nPTR)
		{
			m_ConsPrint("ptr tab overflow\n");
			return p;
		}
	}
	hPtr_t[ptrpos].ptr = p;
	hPtr_t[ptrpos].size = size;
	hPtr_t[ptrpos].used=true;

	m_ConsPrint("mem: %d\n", gMemoryUsage);

#endif

	return p;
}

void s_free(void *mem)
{
#ifdef MEMORYPRINT

	int	i;

	i=0;
	while(hPtr_t[i].ptr != mem)
	{
		i++;
		if(i>=nPTR)
		{
			m_ConsPrint("ptr tab overflow\n");
			free(mem);
			return;
		}
	}
	gMemoryUsage -= hPtr_t[i].size;
	hPtr_t[i].used=false;
	m_ConsPrint("free: %d\n",gMemoryUsage);

#endif

	if(mem)
	{
		free(mem);
		mem = NULL;
	}
	else
	{
		if(developer.value)
			m_ConsPrint("warning : Free(NULL)\n");
	}
}

hBool fs_GetFileNames(char *out, int n)
{
#ifdef H_WINDOWS
	struct _finddata_t data;
	long	idfile;
	int		i;

	if(n<1)
		return false;
	memset(data.name,0,FILENAME_MAX);

	if(n==1)
	{
		_findfirst("system/maps/*.map",&data);
		strcpy(out,data.name);
	}
	else
	{
		for(i=0; i<n ; i++)
		{
			if(i==0)
				idfile = _findfirst("system/maps/*.map",&data);
			else
			{
				if( _findnext(idfile,&data) == -1 )
					return false;
			}
		}
		strcpy(out,data.name);
		//m_ConsPrint("map : %s\n",data.name);
	}

	for(i=0 ; i<MAX_MAPFILENAME ; i++)
	{
		if(out[i] == '\0')
			out[i] = ' ';
	}

	return true;
#endif

#ifdef H_LINUX

	return true;
#endif

#ifdef H_MAC

	return true;
#endif
}

hBool fs_OrderName(char *nameA, char *nameB)
{
	int		i;
	char	temp[FILENAME_MAX];

	if(!nameA || !nameB)
		return false;

	for(i=0 ; i<FILENAME_MAX ; i++)
	{
		if(nameB[i] == nameA[i])
			continue;
		if(nameB[i] < nameA[i])
		{
			memset(temp,0,FILENAME_MAX);
			strcpy(temp,nameA);
			memset(nameA,0,FILENAME_MAX);
			strcpy(nameA,nameB);
			memset(nameB,0,FILENAME_MAX);
			strcpy(nameB,temp);
			return true;
		}
	}
	return false;
}
