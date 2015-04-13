#include <stddef.h>
#include <malloc.h>
#include <bsreader.h>

#define MAX_LITERAL_SIZE 512

typedef struct node_t node_t;

struct node_t
{
  const node_t* left;
  const node_t* right;
  char          token;
};

static const node_t _007faa60 = { NULL, NULL, 0 }; /* "literal" 0 */
static const node_t _007faa88 = { NULL, NULL, 1 }; /* "[" 100000 */
static const node_t _00848b48 = { NULL, NULL, 2 }; /* "]" 100001 */
static const node_t _0059a720 = { &_007faa88, &_00848b48, -1 };
static const node_t _00848df0 = { NULL, NULL, 3 }; /* "in" 10001000 */
static const node_t _00834420 = { NULL, NULL, 4 }; /* "false" 10001001 */
static const node_t _0083e698 = { &_00848df0, &_00834420, -1 };
static const node_t _00848e40 = { NULL, NULL, 5 }; /* "not" 1000101 */
static const node_t _0059a630 = { &_0083e698, &_00848e40, -1 };
static const node_t _007fab00 = { NULL, NULL, 6 }; /* "function" 100011 */
static const node_t _0059a748 = { &_0059a630, &_007fab00, -1 };
static const node_t _007f1a28 = { &_0059a720, &_0059a748, -1 };
static const node_t _007f56c8 = { NULL, NULL, 7 }; /* "(" 1001 */
static const node_t _007f1af0 = { &_007f1a28, &_007f56c8, -1 };
static const node_t _007f5718 = { NULL, NULL, 8 }; /* "." 10100 */
static const node_t _00848c10 = { NULL, NULL, 9 }; /* ";" 101010 */
static const node_t _007fab50 = { NULL, NULL, 10 }; /* "==" 1010110 */
static const node_t _007faab0 = { NULL, NULL, 11 }; /* "and" 10101110 */
static const node_t _00834448 = { NULL, NULL, 12 }; /* "for" 10101111 */
static const node_t _0083e6c0 = { &_007faab0, &_00834448, -1 };
static const node_t _0059a658 = { &_007fab50, &_0083e6c0, -1 };
static const node_t _0059a770 = { &_00848c10, &_0059a658, -1 };
static const node_t _007f1a50 = { &_007f5718, &_0059a770, -1 };
static const node_t _007f56f0 = { NULL, NULL, 13 }; /* ")" 1011 */
static const node_t _007f1b18 = { &_007f1a50, &_007f56f0, -1 };
static const node_t _007ec708 = { &_007f1af0, &_007f1b18, -1 };
static const node_t _007f5768 = { NULL, NULL, 14 }; /* "," 1100 */
static const node_t _007faad8 = { NULL, NULL, 15 }; /* "self" 1101000 */
static const node_t _007f56a0 = { NULL, NULL, 16 }; /* "+" 110100100 */
static const node_t _007f5830 = { NULL, NULL, 17 }; /* "<=" 11010010100 */
static const node_t _007faa38 = { NULL, NULL, 18 }; /* "until" 11010010101 */
static const node_t _00843a80 = { &_007f5830, &_007faa38, -1 };
static const node_t _00848d78 = { NULL, NULL, 19 }; /* "..." 1101001011 */
static const node_t _0083e5a8 = { &_00843a80, &_00848d78, -1 };
static const node_t _0083e620 = { &_007f56a0, &_0083e5a8, -1 };
static const node_t _007f57e0 = { NULL, NULL, 20 }; /* "else" 11010011 */
static const node_t _0083e6e8 = { &_0083e620, &_007f57e0, -1 };
static const node_t _0059a680 = { &_007faad8, &_0083e6e8, -1 };
static const node_t _00848bc0 = { NULL, NULL, 21 }; /* "return" 110101 */
static const node_t _0059a798 = { &_0059a680, &_00848bc0, -1 };
static const node_t _00848be8 = { NULL, NULL, 22 }; /* ":" 1101100 */
static const node_t _00848c60 = { NULL, NULL, 23 }; /* "do" 11011010 */
static const node_t _007fa970 = { NULL, NULL, 24 }; /* "or" 11011011 */
static const node_t _0083e710 = { &_00848c60, &_007fa970, -1 };
static const node_t _0059a6a8 = { &_00848be8, &_0083e710, -1 };
static const node_t _007fabc8 = { NULL, NULL, 25 }; /* "#" 11011100 */
static const node_t _007fab28 = { NULL, NULL, 26 }; /* "while" 11011101000 */
static const node_t _007f5808 = { NULL, NULL, 27 }; /* "break" 11011101001 */
static const node_t _0083e558 = { &_007fab28, &_007f5808, -1 };
static const node_t _00848d28 = { NULL, NULL, 28 }; /* "repeat" 11011101010 */
static const node_t _007f5678 = { NULL, NULL, 29 }; /* "*" 1101110101100000 */
static const node_t _007f5740 = { NULL, NULL, 30 }; /* "/" 1101110101100001 */
static const node_t _00843918 = { &_007f5678, &_007f5740, -1 };
static const node_t _007fa948 = { NULL, NULL, 31 }; /* "goto" 11011101011000100 */
static const node_t _007fa998 = { NULL, NULL, 32 }; /* "eof" 11011101011000101 */
static const node_t _00848e68 = { &_007fa948, &_007fa998, -1 };
static const node_t _007fa9c0 = { NULL, NULL, 33 }; /* ">=" 11011101011000110 */
static const node_t _007fa9e8 = { NULL, NULL, 34 }; /* "::" 11011101011000111 */
static const node_t _00848e90 = { &_007fa9c0, &_007fa9e8, -1 };
static const node_t _00843940 = { &_00848e68, &_00848e90, -1 };
static const node_t _008439b8 = { &_00843918, &_00843940, -1 };
static const node_t _007faa10 = { NULL, NULL, 35 }; /* "//" 11011101011001000 */
static const node_t _007faba0 = { NULL, NULL, 36 }; /* ">>" 11011101011001001 */
static const node_t _00843878 = { &_007faa10, &_007faba0, -1 };
static const node_t _007f5588 = { NULL, NULL, 37 }; /* "<<" 11011101011001010 */
static const node_t _007f55b0 = { NULL, NULL, 38 }; /* "|" 11011101011001011 */
static const node_t _008438a0 = { &_007f5588, &_007f55b0, -1 };
static const node_t _00843968 = { &_00843878, &_008438a0, -1 };
static const node_t _007f55d8 = { NULL, NULL, 39 }; /* "&" 11011101011001100 */
static const node_t _007f5600 = { NULL, NULL, 40 }; /* "~" 11011101011001101 */
static const node_t _008438c8 = { &_007f55d8, &_007f5600, -1 };
static const node_t _007f5628 = { NULL, NULL, 41 }; /* "^" 11011101011001110 */
static const node_t _007f5650 = { NULL, NULL, 42 }; /* "%" 11011101011001111 */
static const node_t _008438f0 = { &_007f5628, &_007f5650, -1 };
static const node_t _00843990 = { &_008438c8, &_008438f0, -1 };
static const node_t _008439e0 = { &_00843968, &_00843990, -1 };
static const node_t _00843a08 = { &_008439b8, &_008439e0, -1 };
static const node_t _00848cd8 = { NULL, NULL, 43 }; /* "<" 1101110101101 */
static const node_t _00843a30 = { &_00843a08, &_00848cd8, -1 };
static const node_t _007f5790 = { NULL, NULL, 44 }; /* "-" 110111010111 */
static const node_t _00843a58 = { &_00843a30, &_007f5790, -1 };
static const node_t _0083e580 = { &_00848d28, &_00843a58, -1 };
static const node_t _0083e5d0 = { &_0083e558, &_0083e580, -1 };
static const node_t _007f57b8 = { NULL, NULL, 45 }; /* "elseif" 1101110110 */
static const node_t _00848c88 = { NULL, NULL, 46 }; /* ">" 1101110111 */
static const node_t _0083e5f8 = { &_007f57b8, &_00848c88, -1 };
static const node_t _0083e648 = { &_0083e5d0, &_0083e5f8, -1 };
static const node_t _0083e738 = { &_007fabc8, &_0083e648, -1 };
static const node_t _00848b98 = { NULL, NULL, 47 }; /* "true" 11011110 */
static const node_t _007fab78 = { NULL, NULL, 48 }; /* "~=" 110111110 */
static const node_t _00848cb0 = { NULL, NULL, 49 }; /* "nil" 110111111 */
static const node_t _0083e670 = { &_007fab78, &_00848cb0, -1 };
static const node_t _0083e760 = { &_00848b98, &_0083e670, -1 };
static const node_t _0059a6d0 = { &_0083e738, &_0083e760, -1 };
static const node_t _0059a7c0 = { &_0059a6a8, &_0059a6d0, -1 };
static const node_t _007f1a78 = { &_0059a798, &_0059a7c0, -1 };
static const node_t _007f1b40 = { &_007f5768, &_007f1a78, -1 };
static const node_t _00848c38 = { NULL, NULL, 50 }; /* "end" 11100 */
static const node_t _00848e18 = { NULL, NULL, 51 }; /* "if" 111010 */
static const node_t _00848b70 = { NULL, NULL, 52 }; /* "then" 111011 */
static const node_t _0059a7e8 = { &_00848e18, &_00848b70, -1 };
static const node_t _007f1aa0 = { &_00848c38, &_0059a7e8, -1 };
static const node_t _00848d00 = { NULL, NULL, 53 }; /* "=" 11110 */
static const node_t _00848d50 = { NULL, NULL, 54 }; /* "local" 111110 */
static const node_t _007fa920 = { NULL, NULL, 55 }; /* ".." 1111110 */
static const node_t _00848da0 = { NULL, NULL, 56 }; /* "{" 11111110 */
static const node_t _00848dc8 = { NULL, NULL, 57 }; /* "}" 11111111 */
static const node_t _0059a608 = { &_00848da0, &_00848dc8, -1 };
static const node_t _0059a6f8 = { &_007fa920, &_0059a608, -1 };
static const node_t _0059a810 = { &_00848d50, &_0059a6f8, -1 };
static const node_t _007f1ac8 = { &_00848d00, &_0059a810, -1 };
static const node_t _007ec6e0 = { &_007f1aa0, &_007f1ac8, -1 };
static const node_t _007ec730 = { &_007f1b40, &_007ec6e0, -1 };
static const node_t _007ec758 = { &_007ec708, &_007ec730, -1 };
static const node_t _007ec780 = { &_007faa60, &_007ec758, -1 };

static struct{ const char* literal; size_t len; } tokens[] =
{
  { "literal", 7 }, /* 1839*/
  { "[", 1 }, /* 71*/
  { "]", 1 }, /* 71*/
  { "in", 2 }, /* 17*/
  { "false", 5 }, /* 18*/
  { "not", 3 }, /* 38*/
  { "function", 8 }, /* 75*/
  { "(", 1 }, /* 325*/
  { ".", 1 }, /* 161*/
  { ";", 1 }, /* 82*/
  { "==", 2 }, /* 40*/
  { "and", 3 }, /* 20*/
  { "for", 3 }, /* 22*/
  { ")", 1 }, /* 326*/
  { ",", 1 }, /* 345*/
  { "self", 4 }, /* 47*/
  { "+", 1 }, /* 11*/
  { "<=", 2 }, /* 2*/
  { "until", 5 }, /* 3*/
  { "...", 3 }, /* 6*/
  { "else", 4 }, /* 25*/
  { "return", 6 }, /* 98*/
  { ":", 1 }, /* 49*/
  { "do", 2 }, /* 25*/
  { "or", 2 }, /* 26*/
  { "#", 1 }, /* 26*/
  { "while", 5 }, /* 3*/
  { "break", 5 }, /* 3*/
  { "repeat", 6 }, /* 3*/
  { "*", 1 }, /* 0*/
  { "/", 1 }, /* 0*/
  { "goto", 4 }, /* 0*/
  { "eof", 3 }, /* 0*/
  { ">=", 2 }, /* 0*/
  { "::", 2 }, /* 0*/
  { "//", 2 }, /* 0*/
  { ">>", 2 }, /* 0*/
  { "<<", 2 }, /* 0*/
  { "|", 1 }, /* 0*/
  { "&", 1 }, /* 0*/
  { "~", 1 }, /* 0*/
  { "^", 1 }, /* 0*/
  { "%", 1 }, /* 0*/
  { "<", 1 }, /* 1*/
  { "-", 1 }, /* 2*/
  { "elseif", 6 }, /* 7*/
  { ">", 1 }, /* 7*/
  { "true", 4 }, /* 28*/
  { "~=", 2 }, /* 15*/
  { "nil", 3 }, /* 17*/
  { "end", 3 }, /* 225*/
  { "if", 2 }, /* 120*/
  { "then", 4 }, /* 130*/
  { "=", 1 }, /* 256*/
  { "local", 5 }, /* 134*/
  { "..", 2 }, /* 66*/
  { "{", 1 }, /* 34*/
  { "}", 1 }, /* 34*/
};

#define ROOT    ( &_007ec780 )
#define LITERAL 0
#define EOF     32

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
