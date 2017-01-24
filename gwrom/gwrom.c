#include <gwrom.h>

#include <string.h>
#include <errno.h>

/* internal flags (1 << 16 to 1 << 23) */
#define GWROM_FREE_DATA ( 1 << 16 )

/******************************************************************************
zlib
******************************************************************************/

#ifdef GWROM_USE_GZIP

#include <zlib.h>

/* use gwrom allocation routines */
void* zalloc( void* opaque, uInt items, uInt size )
{
  (void)opaque;
  return gwrom_malloc( items * size );
}

/* use gwrom allocation routines */
void zfree( void* opaque, void* addr )
{
  (void)opaque;
  gwrom_free( addr );
}

static int identify_gzip( const void* data, size_t size )
{
  /* basic header check */
  const uint8_t* magic = (const uint8_t*)data;
  
  if ( magic[ 0 ] != 0x1f || magic[ 1 ] != 0x8b || magic[ 2 ] != 8 )
  {
    return GWROM_INVALID_ROM;
  }
  
  /* TODO check the signature at the end of the data */
  return GWROM_OK;
}

static int decompress_gzip( void** new_data, size_t* new_size, void* data, size_t size )
{
  /* find the decompressed size */
  /* TODO is this always little-endian in a gzipped file? */
  *new_size = *(uint32_t*)( (char*)data + size - 4 );
  
  /* allocate buffer */
  *new_data = gwrom_malloc( *new_size );
  
  if ( !*new_data )
  {
    return GWROM_NO_MEMORY;
  }
  
  /* decompress */
  uLong dest_len = *new_size;
  int res = uncompress( *new_data, &dest_len, data, size );
  
  if ( res != Z_OK )
  {
    gwrom_free( *new_data );
    return GWROM_INVALID_ROM;
  }
  
  return GWROM_OK;
}
  
#endif /* GWROM_USE_GZIP */

/******************************************************************************
bzip2
******************************************************************************/

#ifdef GWROM_USE_BZIP2

#include <bzlib.h>
#include <bzlib_private.h>

/* needed because of -DBZ_NO_STDIO, which should be defined for compilation */
void bz_internal_error( int errcode )
{
  (void)errcode;
}

/* use gwrom allocation routines */
void* bzalloc( void* opaque, Int32 items, Int32 size )
{
  (void)opaque;
  return gwrom_malloc( items * size );
}

/* use gwrom allocation routines */
void bzfree( void* opaque, void* addr )
{
  (void)opaque;
  gwrom_free( addr );
}

static int identify_bzip2( const void* data, size_t size )
{
  /* basic header check */
  const char* magic = (const char*)data;
  
  if ( magic[ 0 ] != 'B' || magic[ 1 ] != 'Z' || magic[ 2 ] != 'h' ||
       magic[ 3 ] < '0' || magic[ 3 ] > '9' )
  {
    return GWROM_INVALID_ROM;
  }
  
  /* TODO check the signature at the end of the data */
  return GWROM_OK;
}

static int decompress_bzip2( void** new_data, size_t* new_size, void* data, size_t size )
{
  bz_stream stream;
  
  /* setup the decompression stream */
  stream.bzalloc = bzalloc;
  stream.bzfree = bzfree;
  
  int res = BZ2_bzDecompressInit( &stream, 0, 0 );
  
  if ( res != BZ_OK )
  {
    return GWROM_INVALID_ROM;
  }
  
  stream.next_in = (char*)data;
  stream.avail_in = (unsigned)size;
  
#ifdef GWROM_NO_REALLOC
  /* first decompression run: evaluate size of decompressed data */
  for ( ;; )
  {
    char buffer[ GWROM_DECOMP_BUFFER ];
    stream.next_out = buffer;
    stream.avail_out = sizeof( buffer );
    
    res = BZ2_bzDecompress( &stream );
    
    if ( res == BZ_STREAM_END )
    {
      break;
    }
    
    if ( res != BZ_OK )
    {
      BZ2_bzDecompressEnd( &stream );
      return GWROM_INVALID_ROM;
    }
  }
  
  /* basic check for when size_t can't hold 64-bit values */
  if ( sizeof( size_t ) > 4 )
  {
    *new_size = stream.total_out_hi32;
    *new_size = *new_size << 32 | stream.total_out_lo32;
  }
  else
  {
    if ( stream.total_out_hi32 != 0 )
    {
      BZ2_bzDecompressEnd( &stream );
      return GWROM_NO_MEMORY;
    }
    
    *new_size = stream.total_out_lo32;
  }
  
  BZ2_bzDecompressEnd( &stream );
  *new_data = gwrom_malloc( *new_size );
  
  if ( !*new_data )
  {
    return GWROM_NO_MEMORY;
  }
  
  /* second decompression run: decompress data to the allocated buffer */
  unsigned dest_len = *new_size;
  res = BZ2_bzBuffToBuffDecompress( (char*)*new_data, &dest_len, (char*)data, size, 0, 0 );
  
  if ( res != BZ_OK )
  {
    return GWROM_INVALID_ROM;
  }
#else
  *new_data = NULL;
  *new_size = 0;
  
  /* decompress while reallocating the decompressed data as necessary */
  for ( ;; )
  {
    char buffer[ GWROM_DECOMP_BUFFER ];
    stream.next_out = buffer;
    stream.avail_out = sizeof( buffer );
    
    res = BZ2_bzDecompress( &stream );
    
    if ( res != BZ_OK && res != BZ_STREAM_END )
    {
      BZ2_bzDecompressEnd( &stream );
      gwrom_free( *new_data );
      return GWROM_INVALID_ROM;
    }
    
    size_t count = sizeof( buffer ) - stream.avail_out;
    
    if ( count )
    {
      char* realloc_data = gwrom_realloc( *new_data, *new_size + count );
      
      if ( realloc_data == NULL )
      {
        gwrom_free( *new_data );
        return GWROM_NO_MEMORY;
      }
      
      *new_data = realloc_data;
      memcpy( (void*)( (char*)*new_data + *new_size ), (void*)buffer, count );
      *new_size += count;
    }
    
    if ( res == BZ_STREAM_END )
    {
      break;
    }
  }
  
  BZ2_bzDecompressEnd( &stream );
#endif

  return GWROM_OK;
}

/* use the default destroy method */
#define destroy_bzip2 default_destroy

/* bzip2 roms don't have any entries in them */
#define find_bzip2 default_find

#endif /* GWROM_USE_BZIP2 */

/******************************************************************************
uncompressed
******************************************************************************/

static int identify_uncompressed( const void* data, size_t size )
{
  /* uncompressed data is always identified */
  return GWROM_OK;
}

static int inflate_uncompressed( void** new_data, size_t* new_size, void* data, size_t size )
{
  /*
  returns the same data, the caller must check that it wasn't inflated to a new
  buffer if it wants to copy the data into a new buffer
  */
  *new_data = data;
  *new_size = size;
  return GWROM_OK;
}

/******************************************************************************
decompress methods
******************************************************************************/

typedef struct
{
  /* returns GWROM_OK if the decompression method is identified */
  int (*identify)( const void*, size_t );
  
  /* decompresses the rom into a new buffer */
  int (*decompress)( void**, size_t*, void*, size_t );
}
decompress_t;

/* all inflate algorithms must have an entry here */
static const decompress_t decompress[] =
{
#ifdef GWROM_USE_GZIP
  { identify_gzip, decompress_gzip },
#endif

#ifdef GWROM_USE_BZIP2
  { identify_bzip2, decompress_bzip2 },
#endif

  /* add new inflate methods here */
  { identify_uncompressed, inflate_uncompressed },
};

/******************************************************************************
default rom methods
******************************************************************************/

static int default_init( gwrom_t* gwrom )
{
  (void)gwrom;
  return GWROM_OK;
}

static void default_destroy( gwrom_t* gwrom )
{
  (void)gwrom;
}

static int default_find( gwrom_entry_t* file, gwrom_t* gwrom, const char* file_name )
{
  (void)file;
  (void)gwrom;
  (void)file_name;
  return GWROM_ENTRY_NOT_FOUND;
}

/******************************************************************************
tar archive, use --format=v7 with gnu tar
******************************************************************************/

#ifdef GWROM_USE_TAR_V7

typedef union
{
  struct
  {
    char name[ 100 ];
    char mode[ 8 ];
    char owner[ 8 ];
    char group[ 8 ];
    char size[ 12 ];
    char modification[ 12 ];
    char checksum[ 8 ];
    char type;
    char linked[ 100 ];
    
    /*
    a space for the user to store things related to the entry, i.e. data has
    been converted to little endian
    CAUTION: things stored in user_flags are *not* persistent!
    */
    uint32_t user_flags;
    
    /*
    a space for the user to store things related to the entry, i.e. a different
    representation of the data
    CAUTION: things stored in user_flags are *not* persistent!
    */
    void* user_data;
  } s;
  
  char fill[ 512 ];
}
entry_tar_v7;

static int identify_tar_v7( const void* data, size_t size )
{
  /* tar size is always a multiple of 512 */
  if ( size & 511 )
  {
    return GWROM_INVALID_ROM;
  }
  
  entry_tar_v7* entry = (entry_tar_v7*)data;
  char* end = (char*)data + size - 512;
  
  /* iterate over the entries and do a basic chack on each on of them */
  while ( (char*)entry <= end && entry->s.name[ 0 ] )
  {
    char* endptr;
    long entry_size = strtol( entry->s.size, &endptr, 8 );
    
    /* Check for a valid entry size */
    if ( *endptr != 0 || errno == ERANGE )
    {
      return GWROM_INVALID_ROM;
    }
    
    char* name = entry->s.name;
    char* endname = name + 100;
    
    /* Check for a valid entry name */
    do
    {
      if ( *name++ < 32 )
      {
        return GWROM_INVALID_ROM;
      }
    }
    while ( *name && name < endname );
    
    /* go to the next entry */
    entry_size = ( entry_size + 511 ) / 512 + 1;
    entry += entry_size;
  }
  
  /* the last entry must be followed by one or more empty entries */
  if ( (char*)entry >= end )
  {
    return GWROM_INVALID_ROM;
  }
  
  /* check for empty entries */
  do
  {
    int i;
    
    for ( i = 0; i < 512; i++ )
    {
      if ( ( (char*)entry )[ i ] != 0 )
      {
        return GWROM_INVALID_ROM;
      }
    }
    
    entry++;
  }
  while ( (char*)entry < end );
  
  return GWROM_OK;
}

static int init_tar_v7( gwrom_t* gwrom )
{
  entry_tar_v7* entry = (entry_tar_v7*)gwrom->data;
  
  while ( entry->s.name[ 0 ] )
  {
    long entry_size = strtol( entry->s.size, NULL, 8 );
    
    /* zero user space */
    entry->s.user_flags = 0;
    entry->s.user_data = NULL;
    
    /* go to the next entry */
    entry_size = ( entry_size + 511 ) / 512 + 1;
    entry += entry_size;
  }
  
  return GWROM_OK;
}

static int find_tar_v7( gwrom_entry_t* file, gwrom_t* gwrom, const char* file_name )
{
  entry_tar_v7* entry = (entry_tar_v7*)gwrom->data;
  
  while ( entry->s.name[ 0 ] )
  {
    long entry_size = strtol( entry->s.size, NULL, 8 );
    
    if ( !strcmp( entry->s.name, file_name ) )
    {
      /* found the entry, fill in gwrom_entry_t* */
      file->name = entry->s.name;
      file->data = (void*)( entry + 1 );
      file->size = entry_size;
      file->user_flags = &entry->s.user_flags;
      
      return GWROM_OK;
    }
    
    /* go to the next entry */
    entry_size = ( entry_size + 511 ) / 512 + 1;
    entry += entry_size;
  }
  
  return GWROM_ENTRY_NOT_FOUND;
}

static void iterate_tar_v7( gwrom_t* gwrom, int (*callback)( gwrom_entry_t*, gwrom_t* ) )
{
  entry_tar_v7* entry = (entry_tar_v7*)gwrom->data;
  gwrom_entry_t file;
  
  while ( entry->s.name[ 0 ] )
  {
    long entry_size = strtol( entry->s.size, NULL, 8 );
    
    file.name = entry->s.name;
    file.data = (void*)( entry + 1 );
    file.size = entry_size;
    file.user_flags = &entry->s.user_flags;
    file.user_data = &entry->s.user_data;
    
    if ( !callback( &file, gwrom ) )
    {
      return;
    }
    
    /* go to the next entry */
    entry_size = ( entry_size + 511 ) / 512 + 1;
    entry += entry_size;
  }
}

/* tar doesn't need destruction */
#define destroy_tar_v7 default_destroy

#endif /* GWROM_USE_TAR_V7 */

/******************************************************************************
rom methods
******************************************************************************/

typedef struct
{
  /* returns GWROM_OK if the rom type is identified */
  int (*identify)( const void*, size_t );
  
  /* initializes the rom */
  int (*init)( gwrom_t* );
  
  /* frees all memory allocated to the rom */
  void (*destroy)( gwrom_t* );
  
  /* finds an entry in the rom */
  int (*find)( gwrom_entry_t*, gwrom_t*, const char* );
  
  /* iterates over all rom entries */
  void (*iterate)( gwrom_t*, int (*)( gwrom_entry_t*, gwrom_t* ) );
}
methods_t;

/* all supported rom types must have an entry here */
static const methods_t methods[] =
{
#ifdef GWROM_USE_TAR_V7
  { identify_tar_v7, init_tar_v7, destroy_tar_v7, find_tar_v7, iterate_tar_v7 },
#endif

  /* add new rom types here */
};

/******************************************************************************
external API
******************************************************************************/

int gwrom_init( gwrom_t* gwrom, void* data, size_t size, uint32_t flags )
{
  void* new_data = data;
  size_t new_size = size;
  unsigned i;
  
  /* check for compressed roms first */
  const decompress_t* decomp = decompress;
  
  for ( i = 0; i < sizeof( decompress ) / sizeof( decompress[ 0 ] ); i++, decomp++ )
  {
    if ( decomp->identify( data, size ) == GWROM_OK )
    {
      if ( decomp->decompress( &new_data, &new_size, data, size ) != GWROM_OK )
      {
        return GWROM_INVALID_ROM;
      }
      
      /* check if data was copied into a new buffer */
      if ( new_data != data )
      {
        /* yes, set flags to free the data */
        flags |= GWROM_FREE_DATA;
      }
      else
      {
        /* no, check if the caller has asked to copy it */
        if ( flags & GWROM_COPY_ALWAYS )
        {
          /* yes, copy data into a new buffer */
          new_data = gwrom_malloc( size );
          
          if ( !new_data )
          {
            return GWROM_NO_MEMORY;
          }
          
          memcpy( new_data, data, size );
          new_size = size;
          
          /* set flags to free the data */
          flags |= GWROM_FREE_DATA;
        }
      }
      
      break;
    }
  }
  
  /* iterates over the supported types and compress algorithms */
  const methods_t* meth = methods;
  
  for ( i = 0; i < sizeof( methods ) / sizeof( methods[ 0 ] ); i++, meth++ )
  {
    if ( meth->identify( new_data, new_size ) == GWROM_OK )
    {
      /* type was identified, fill in gwrom and call its init method */
      gwrom->data = new_data;
      gwrom->size = new_size;
      gwrom->flags = flags;
      gwrom->destroy = meth->destroy;
      gwrom->find = meth->find;
      gwrom->iterate = meth->iterate;
      return meth->init( gwrom );
    }
  }
  
  /* rom not identified */
  return GWROM_INVALID_ROM;
}

void gwrom_destroy( gwrom_t* gwrom )
{
   if (!gwrom)
      return;

  /* calls the destroy method */
  if (gwrom->destroy)
     gwrom->destroy( gwrom );
  
  /* free data */
  if ( gwrom->flags & GWROM_FREE_DATA )
  {
    gwrom_free( gwrom->data );
  }
  
  /* zeroes gwrom data */
  gwrom->data = NULL;
  gwrom->size = 0;
  gwrom->user_data = NULL;
  gwrom->destroy = NULL;
  gwrom->find = NULL;
}

const char* gwrom_error_message( int error )
{
  switch ( error )
  {
  case GWROM_OK:              return "Ok";
  case GWROM_INVALID_ROM:     return "Invalid ROM (corrupted file?)";
  case GWROM_NO_MEMORY:       return "Out of memory";
  case GWROM_ENTRY_NOT_FOUND: return "Entry not found";
  }
  
  return "Unknown error";
}
