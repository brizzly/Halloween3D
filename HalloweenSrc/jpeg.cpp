
#include "alloween.cpp"


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
	
	
#ifdef H_MAC
	return LoadFromJPEG(Name, KMiscTools::makeFilePath(fullname), id, map_tex);
	// hack m_ConsPrint("charge TGA #######################\n");
	//Name = fs_RenameFileExtension(Name, "tga");
#endif
	
#ifdef H_LINUX
	// hack :
	//m_ConsPrint("charge TGA #######################\n");
	Name = fs_RenameFileExtension(Name, "tga");
	return tga_Load(Name, id, picmip, map_tex);

	img_data = jpg_read(fullname, &w, &h);
	if(img_data == NULL)
	{
		return 0;
	}
	else
	{
	//	printf("ok - %d %d\n", w, h);
	}
#endif
	
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

#ifdef H_MAC

struct my_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

// Here's the routine that will replace the standard error_exit method:
METHODDEF(void)my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	
	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);
	
	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}


int LoadFromJPEG(char *Name, char *filename, int text_id, hBool map_tex)
{
	/* This struct contains the JPEG decompression parameters and pointers to
	 * working space (which is allocated as needed by the JPEG library).
	 */
	struct jpeg_decompress_struct cinfo;
	
	/* We use our private extension JPEG error handler.
	 * Note that this struct must live as long as the main JPEG parameter
	 * struct, to avoid dangling-pointer problems.
	 */
	struct my_error_mgr jerr;
	
	/* More stuff */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;			/* physical row width in output buffer */
	
	/* In this example we want to open the input file before doing anything else,
	 * so that the setjmp() error recovery below can assume the file is open.
	 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	 * requires it in order to read binary files.
	 */
	
	FILE * infile = fopen(filename, "r"); // rb
	if(infile == NULL)
	{
		//fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}
	
	/* Step 1: allocate and initialize JPEG decompression object */
	
	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer))
	{
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}
	/* Now we can initialize the JPEG decompression object. */
	jpeg_create_decompress(&cinfo);
	
	/* Step 2: specify data source (eg, a file) */
	jpeg_stdio_src(&cinfo, infile);
	
	/* Step 3: read file parameters with jpeg_read_header() */
	(void) jpeg_read_header(&cinfo, TRUE);
	
	/* We can ignore the return value from jpeg_read_header since
	 *   (a) suspension is not possible with the stdio data source, and
	 *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	 * See libjpeg.doc for more info.
	 */
	
	/* Step 4: set parameters for decompression */
	
	/* In this example, we don't need to change any of the defaults set by
	 * jpeg_read_header(), so we do nothing here.
	 */
	
	/* Step 5: Start decompressor */
	
	(void) jpeg_start_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */
	
	/* We may need to do some setup of our own at this point before reading
	 * the data.  After jpeg_start_decompress() we have the correct scaled
	 * output image dimensions available, as well as the output colormap
	 * if we asked for color quantization.
	 * In this example, we need to make an output work buffer of the right size.
	 */
	/* JSAMPLEs per row in output buffer */
	row_stride = cinfo.output_width * cinfo.output_components;
	
	/* Make a one-row-high sample array that will go away when done with image */
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	
	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */
	
	/* Here we use the library's state variable cinfo.output_scanline as the
	 * loop counter, so that we don't have to keep track ourselves.
	 */
	
	int jpeg_w = (int)cinfo.output_width;
	int jpeg_h = (int)cinfo.output_height;
	unsigned char *jpeg_buffer = new unsigned char [jpeg_w * jpeg_h * row_stride];
	memset(jpeg_buffer, 0, jpeg_w * jpeg_h * row_stride);
	
	while (cinfo.output_scanline < cinfo.output_height)
	{
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		 * Here the array is only one element long, but you could ask for
		 * more than one scanline at a time if that's more convenient.
		 */
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		
		/* Assume put_scanline_someplace wants a pointer and sample count. */
  //  put_scanline_someplace(buffer[0], row_stride);
		memcpy(jpeg_buffer+(((int)cinfo.output_scanline-1)*jpeg_w*3), buffer[0], row_stride);
	}
	
	/* Step 7: Finish decompression */
	
	(void) jpeg_finish_decompress(&cinfo);
	/* We can ignore the return value since suspension is not possible
	 * with the stdio data source.
	 */
	
	/* Step 8: Release JPEG decompression object */
	
	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_decompress(&cinfo);
	
	/* After finish_decompress, we can close the input file.
	 * Here we postpone it until after no more JPEG errors are possible,
	 * so as to simplify the setjmp error logic above.  (Actually, I don't
	 * think that jpeg_destroy can do an error exit, but why assume anything...)
	 */
	fclose(infile);
	
	/* At this point you may want to check to see whether any corrupt-data
	 * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	 */
	
	
	
	//int texture_id = GraphicAPI->UploadBitsToGPU(jpeg_w, jpeg_h, 24, jpeg_buffer, params);
	
	
	int	k;
	
	if(jpeg_buffer == NULL) {
		return 0;
	}
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

#endif




