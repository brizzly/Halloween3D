/*
 *  GLTextureHelper.h
 *  OpenGL
 *
 // Released under BSD License
  Copyright (c) 2007, Richard S. Wright Jr.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, 
 are permitted provided that the following conditions are met:
 
 Redistributions of source code must retain the above copyright notice, this list 
 of conditions and the following disclaimer.
 
 Redistributions in binary form must reproduce the above copyright notice, this list 
 of conditions and the following disclaimer in the documentation and/or other 
 materials provided with the distribution.
 
 Neither the name of Richard S. Wright Jr. nor the names of other contributors may be used 
 to endorse or promote products derived from this software without specific prior 
 written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
 BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// This class wraps up and slightly modifies APPLE sample code


#include <OpenGL/OpenGL.h>
#import <OpenGL/gLu.h>


typedef struct
{
	char		*	filename;
	int				originalWidth;
	int				originalHeight;
	int				width;
	int				height;
	int				bpp;
	bool			useColorkey;
	int				R_colorkey;
	int				G_colorkey;
	int				B_colorkey;
	GLubyte		*	pixels;
	GLuint			textureID;
	GLuint			pboId;

} JadeImage_t;

class GLTextureHelper
	{
	public:
		GLTextureHelper();
		~GLTextureHelper(void);
		
		void	clearSurface(GLubyte * surface, int w, int h, int bpp, int R, int G, int B, int A);
		void	colorkeySurface(GLubyte * surface, int w, int h, int bpp, int ckR, int ckG, int ckB);
		bool	LoadWatermark();
		bool	isEyeRetina();
		void	listFonts();
		GLubyte * getDatas();
		bool	loadText(const char *text, const char * fontname, int fontsize);
		bool	LoadTexture(const char *szTextureFile, bool colorkey, unsigned char ckr, unsigned char ckg, unsigned char ckb);
		bool	getImageDataFromPath(const char *path);
		int		LoadFileTGA(JadeImage_t *textureObject);
		
		GLint	getHeight(void) { return _nHeight; }
		GLint	getWidth(void) { return _nWidth; }
		GLint	getTextureSize(void) { return _texSize; }
		bool	getEyeRetina(void) { return _eyeRetina; }
		
		
	protected:
		
		GLubyte *	_pData;
		GLint		_nWidth;
		GLint		_nHeight;
		GLint		_texSize;
		bool		_eyeRetina;
		
	};
