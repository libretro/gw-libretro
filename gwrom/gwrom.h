#ifndef GWROM_H
#define GWROM_H

#include <stdlib.h>
#include <stdint.h>

/*---------------------------------------------------------------------------*/
/* must be implemented by the user */

/*
memory allocation routines
*/
void* gwrom_malloc( size_t size );
void* gwrom_realloc( void* ptr, size_t size );
void  gwrom_free( void* ptr );

/*---------------------------------------------------------------------------*/
/* compilation config */

/* use tar v7 archives */
#define GWROM_USE_TAR_V7

/* use zlib for decompression */
/*#define GWROM_USE_GZIP*/

/* use bzip2 for decompression */
#define GWROM_USE_BZIP2

/*
prevents memory reallocation during bzip2 decompression
note that in this case gwrom will do two decompression runs, one to evaluate
the decompressed size and another to decompress the data to the allocated
buffer, but memory will be entirely allocated in one go
*/
#define GWROM_NO_REALLOC 1

/* size of the buffer used during decompression */
#define GWROM_DECOMP_BUFFER 65536

/*---------------------------------------------------------------------------*/
/* flags to gwrom_init (1 << 0 to 1 << 15) */

/*
makes gwrom_init copy the rom into a new buffer, even if it's unecessary (i.e.
data wasn't compressed)
*/
#define GWROM_COPY_ALWAYS 1

/*---------------------------------------------------------------------------*/

/* everything went ok */
#define GWROM_OK 0

/* invalid rom format */
#define GWROM_INVALID_ROM -1

/* memory allocation error */
#define GWROM_NO_MEMORY -2

/* entry not found in the rom */
#define GWROM_ENTRY_NOT_FOUND -3

/*---------------------------------------------------------------------------*/

typedef struct gwrom_t gwrom_t;

typedef struct
{
  /* entry name */
  const char* name;
  /* pointer to entry data */
  void*       data;
  /* entry data size */
  size_t      size;
  /* internal gwrom flags for the entry */
  uint32_t    flags;
  
  /* persistent (between gwrom_init and gwrom_destroy) user flags */
  uint32_t* user_flags;
  
  /* available to the user */
  void** user_data;
}
gwrom_entry_t;

struct gwrom_t
{
  /* rom data */
  void*    data;
  /* rom data size */
  size_t   size;
  /* internal gwrom flags for the rom */
  uint32_t flags;
  
  /* persistent (between gwrom_init and gwrom_destroy) user flags */
  uint32_t user_flags;
  
  /* available to the user */
  void* user_data;
  
  /* frees all memory allocated to the rom */
  void (*destroy)( gwrom_t* );
  /* finds an entry in the rom */
  int  (*find)( gwrom_entry_t*, gwrom_t*, const char* );
  /* iterates over all rom entries */
  void (*iterate)( gwrom_t*, int (*)( gwrom_entry_t*, gwrom_t* ) );
};

/*---------------------------------------------------------------------------*/
/* external api */

/*
initializes an archive. if flags contain GWROM_COPY_ALWAYS, data will be copied
to another buffer. this buffer is automatically freed when gwrom_destroy is
called.
*/
int  gwrom_init( gwrom_t* gwrom, void* data, size_t size, uint32_t flags );

/* frees all memory allocated for the rom */
void gwrom_destroy( gwrom_t* );

/* returns a readable descriptions for errors */
const char* gwrom_error_message( int error );

/* finds an entry in the rom */
#define gwrom_find( entry, gwrom, file_name ) ( ( gwrom )->find( entry, gwrom, file_name ) )

/* iterates over all rom entries */
#define gwrom_iterate( gwrom, callback ) ( ( gwrom )->iterate( gwrom, callback ) )

/*---------------------------------------------------------------------------*/

#endif /* GWROM_H */
