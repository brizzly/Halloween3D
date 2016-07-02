
#ifndef FILESYS_INCLUDED
#define FILESYS_INCLUDED

#include "alloween.h"

void 	fs_launchURL(char *url);
void 	fs_makedir(char *dirname);
char	*fs_RenameFileExtension(char *name, char *extension);
hBool	fs_IsFileExtension(char *name, char *extension);
hBool	f_ScreenShot(char *nom);
void	str2linux(char str[]);
void	fs_GetFileNameFromPath(char *path, char *name);
void *	s_malloc(uint size);
void	s_free(void *mem);
hBool	fs_GetFileNames(char *out, int n);
hBool	fs_OrderName(char *nameA, char *nameB);


#endif //FILESYS_INCLUDED
