#include <stddef.h>
#include <malloc.h>
#include <bsreader.h>
#include <bstree.h>

#define MAX_LITERAL_SIZE 512

typedef struct
{
  unsigned char* byte;
  unsigned char  bit;
  char           space;
  char           literal[ MAX_LITERAL_SIZE ];
}
stream_t;

static int getbit( stream_t* stream )
{
  int bit = *stream->byte & stream->bit ? 1 : 0;
  stream->bit >>= 1;
  
  if ( !stream->bit )
  {
    stream->bit = 128;
    stream->byte++;
  }
  
  return bit;
}

static int getbyte( stream_t* stream )
{
  int byte = 0, i;
  
  for ( i = 0; i < 8; i++ )
  {
    byte = byte << 1 | getbit( stream );
  }
  
  return byte;
}

static size_t getliteral( stream_t* stream, char* literal, size_t size )
{
  int byte;
  char* begin = literal;
  char* end = literal + size;
  
  if ( stream->bit == 128 )
  {
    getbit( stream );
  }
  
  do
  {
    byte = getbyte( stream );
    *literal++ = byte;
  }
  while ( byte && literal < end );
  
  return literal - begin - 1;
}

const char* bsread( lua_State* L, void* data, size_t* size )
{
  stream_t* stream = (stream_t*)data;
  int bit;
  const char* literal;
  
  (void)L;
  
  stream->space = !stream->space;
  
  if ( !stream->space )
  {
    *size = 1;
    return " ";
  }
  
  const node_t* node = ROOT;
  
  while ( node->token == -1 )
  {
    bit = getbit( stream );
    node = bit ? node->right : node->left;
  }
  
  if ( node->token == LITERAL )
  {
    literal = stream->literal;
    *size = getliteral( stream, stream->literal, sizeof( stream->literal ) );
  }
  else if ( node->token != EOF )
  {
    literal = tokens[ node->token ].literal;
    *size = tokens[ node->token ].len;
  }
  else
  {
    literal = NULL;
    *size = 0;
  }
  
  return literal;
}

void* bsnew( void* data )
{
  stream_t* stream = (stream_t*)malloc( sizeof( *stream ) );
  stream->byte = (unsigned char*)data;
  stream->bit = 128;
  stream->space = 0;
  return (void*)stream;
}
