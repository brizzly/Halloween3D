
#if 0

#include <stdio.h>
#include "jpeglib.h"


void put_scanline_someplace (JSAMPLE* ba, int row_stride)
{
  static int height;
    int i;

    for (i=0; i < row_stride; i++)
         printf("%c", (char)ba[i]);
}

unsigned char * jpg_read(char * filename, int *w, int *h)
{
  	struct 		jpeg_decompress_struct cinfo;
  	FILE 		*infile;
  	JSAMPARRAY 	buffer;
  	int 		row_stride;
	unsigned char 	*image;
	unsigned char 	*ptr;

  	if ((infile = fopen(filename, "rb")) == NULL)
  	{
    		m_ConsPrint("Can't open %s\n", filename);
    		return 0;
  	}

  	// Step 1: allocate and initialize JPEG decompression object
  	jpeg_create_decompress(&cinfo);

  	// Step 2: specify data source (eg, a file)
  	jpeg_stdio_src(&cinfo, infile);

  	// Step 3: read file parameters with jpeg_read_header()
        jpeg_read_header(&cinfo, TRUE);

  	// Step 4: set parameters for decompression

  	// Step 5: Start decompressor
  	jpeg_start_decompress(&cinfo);

  	row_stride = cinfo.output_width * cinfo.output_components;
  	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  	*w = (int)cinfo.output_width;
  	*h = (int)cinfo.output_height;
  	image = (unsigned char*)s_malloc(4 * row_stride * (*h) * sizeof(unsigned char));
	//memset(image, 0, row_stride * (*h) * sizeof(unsigned char));


  	// Step 6: while (scan lines remain to be read)    jpeg_read_scanlines(...);
  	ptr = image;
  	while (cinfo.output_scanline < cinfo.output_height)
  	{
    		jpeg_read_scanlines(&cinfo, buffer, 1);

		memcpy(ptr, buffer, row_stride);
		ptr += row_stride;
 		//put_scanline_someplace(buffer[0], row_stride);  // hack
  	}

  	// Step 7: Finish decompression
  	jpeg_finish_decompress(&cinfo);

  	// Step 8: Release JPEG decompression object
  	jpeg_destroy_decompress(&cinfo);
  	fclose(infile);
  	return image;
}

#endif

