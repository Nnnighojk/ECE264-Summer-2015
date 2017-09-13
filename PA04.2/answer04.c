#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer04.h"

#define FALSE 0
#define TRUE 1
#define perr(s) fprintf( stderr, "%s\n", s );


Image  *Image_load ( const char * filename )
{
   FILE *ptr = fopen(filename, "rb");
	size_t b;
	ImageHeader header;
	Image * size = NULL, *si = NULL;
	int error = FALSE;
        size_t size1;
        size_t size2;
        size_t size3;
	
	if(!error)	
	{
	  ptr = fopen(filename, "r");	
	  if(ptr == NULL)			
	{
	  fprintf(stderr,"Error, file %s is not there", filename);
			
	   error = TRUE;
	}
	}
	if(!error)	
	{
	  b = fread(&header, sizeof(ImageHeader),1, ptr);
		
		
	  if(b!= 1)
	  {
	    error=TRUE;
	    fprintf(stderr,"Error for file header");
			
	  }
	}
	if(!error)	
	{
	  if(header.magic_number!= ECE264_IMAGE_MAGIC_NUMBER)
		
	{
	  fprintf(stderr,"Magic number does not match");
			
	  error = TRUE;
	}
	}
	if(!error)	
	{
	    if(header.width==0 || header.height==0 || header.comment_len==0)
		
	    {
	      fprintf(stderr,"ERROR");
			
	      error = TRUE;
	    }
	}
	if(!error)	
	{
		size = malloc(sizeof(Image));
		if(size==NULL)		
		{
		  fprintf(stderr, "no space for image");
		  error = TRUE;
		}
	}
	if(!error)	
	{
	  size->width = (int)header.width;
	  size->height = (int)header.height;
	}
	if(!error)	
	{
	   size->comment = malloc(sizeof(char) * header.comment_len);
	   if(size->comment == NULL)
	   {
	     fprintf(stderr,"no space for comment");
	     error = TRUE;
	   }
	}

	if(!error)	
	{
	  size->data = malloc( sizeof(uint8_t) * (size->width) * (size->height) );
	  if(size->data==NULL)
	  {
	    fprintf(stderr, "no memory for data");
	    error = TRUE;
	   }
	}

	
	if(!error)	
	{
		size1 = fread(size->comment, sizeof(char), header.comment_len, ptr);
		if(size1!=header.comment_len)	
		{
			fprintf(stderr,"couldn't read comment\n");
			error = TRUE;
		}
		else if(size->comment[header.comment_len-1] != '\0' )
		{
			fprintf(stderr,"comment does not finish with '\\0'\n");
			error = TRUE;
		}
		else
		{
			printf("The comment is\n%s", size->comment);
		}
	}
	
	if(!error)	
	{
		size2 = fread(size->data, sizeof(uint8_t), ( (size->width) * (size->height) ), ptr );
		if(size2!= (( (size->width) * (size->height) )) )	
		{
			fprintf(stderr, "couldn't read data from file");
			error = TRUE;
		}
	}
	
	if(!error)	
	{
		uint8_t a_byte;
		size3 = fread(&a_byte, sizeof(uint8_t), 1, ptr);
		if(size3 != 0)	
		{
			fprintf(stderr, "stray bites found");
			error = TRUE;
		}
	}
	
	if(!error)	
	{
		si = size;
		size = NULL;
		
	}
	
	if(size != NULL)	
	{
		free(size->comment);
		free(size->data);
		free(size);
	}
	if(ptr!=NULL)
	{
		fclose(ptr);
	}
	
	return si;
}

int Image_save ( const char * filename, Image * image )
{
   int error = FALSE; 
    FILE * ptr = NULL;
    int * buffer = NULL;    
    size_t a = 0;

    
    ptr = fopen(filename, "wb");
    if(ptr == NULL) 
    {
      fprintf(stderr, "couldn't open '%s' for writing\n", filename);
      return FALSE;
    }

    
    size_t n_bytes = sizeof(int) * image->width * image->height;

    
    ImageHeader header;
    header.magic_number = ECE264_IMAGE_MAGIC_NUMBER;
    header.width = image->width;
    header.height = image->height;
    header.comment_len = strlen(image->comment) + 1;

    if(!error) 
    {  
    	
       a = fwrite(&header, sizeof(ImageHeader), 1, ptr);
       if(a != 1) 
       {
          fprintf(stderr, "wrote %zd of %zd of file header to '%s'\n", a, sizeof(ImageHeader), filename);
	  error = TRUE;	
	}
    }

    if(!error) 
    { 
    	
	buffer = malloc(n_bytes);
	if(buffer == NULL) 
	{
	  fprintf(stderr, "did not allocte buffer\n");
	   error = TRUE;
	} 
	
    }

	
    if(!error) 
    {
     	a = fwrite ( image->comment, sizeof( char ), header.comment_len, ptr );
     	if ( a != sizeof (char) * strlen( image->comment ) )
     	{
     		perr("could not write comment");
     		error = TRUE;
     	}
  	}
  	
  	
  	if ( !error )
  	{
  	  a = fwrite ( image->data, sizeof( int ), n_bytes, ptr );
     	  if ( a != sizeof ( n_bytes ) )
     	{
     	   perr("could not write data");
     	   error = TRUE;
     	}
  	}

  free(buffer);
  
     
    if(ptr)
    fclose(ptr);

    return !error;
}



void Image_free(Image *image)
{
    
       free(image->comment);
    	
    
       free(image->data);	
     
       free(image);			
   

}


void linearNormalization ( int width, int height, uint8_t * intensity )
{
  int dataSize = width * height;
  int i;
  int high = 0;
  int low = intensity[0];
  
 
   for ( i = 0; i < dataSize; i++ )
   {
     if ( low > intensity[i] )
      {
	low = intensity[i];
      }
    else if ( high < intensity[i] )
      {
	high = intensity[i];
      }
    }

   for ( i = 0; i < dataSize; i++ )
   {
     intensity[i] = ( intensity[i] - low ) * 255.0 / ( high - low ); 	
   }			
}
