#include <gwlua.h>

#include <stdint.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static uint16_t ne16( uint16_t x )
{
  static const union
  {
    uint16_t u16;
    uint8_t u8[ 2 ];
  }
  u = { 1 };
  
  return u.u8[ 0 ] ? x >> 8 | x << 8 : x;
}

static uint32_t ne32( uint32_t x )
{
  static const union
  {
    uint16_t u16;
    uint8_t u8[ 2 ];
  }
  u = { 1 };
  
  if ( u.u8[ 0 ] )
  {
    return ne16( x ) << 16 | ne16( x >> 16 );
  }
  else
  {
    return x;
  }
}

static void assign_data( lua_State* L, gwlua_picture_t* picture, void* data, size_t size )
{
  union { void* v; uint16_t* u16; uint32_t* u32; } ptr;
  uint16_t* rle;
  
  if ( picture->rle )
  {
    luaL_error( L, "data reassign in picture" );
    return;
  }
  
  size -= 8; /* width, height, used */
  picture->rle = (uint16_t*)gwlua_malloc( size );
  
  if ( !picture->rle )
  {
    luaL_error( L, "out of memory" );
    return;
  }
  
  ptr.v = data;
  
  picture->width = ne16( *ptr.u16++ );
  picture->height = ne16( *ptr.u16++ );
  picture->used = ne32( *ptr.u32++ );
  
  rle = picture->rle;
  
  while ( size )
  {
    *rle++ = ne16( *ptr.u16++ );
    size -= 2;
  }
}

void blit_picture( const gwlua_picture_t* picture, int x, int y, uint16_t* bg )
{
  if ( picture->rle )
  {
    uint16_t* fb = picture->state->screen + y * picture->state->width + x;
    unsigned  pitch = picture->state->width - picture->width;
    uint16_t* rle = picture->rle;
    int       code, count;
    uint32_t  c1, c2;
    
    y = picture->height;
    
    if ( bg )
    {
      do
      {
        /* runs in this row */
        x = *rle++;
        
        do
        {
          /* decode */
          code   = *rle++;
          count  = code & 0x1fff;
          code >>= 13;
          
          switch ( code & 7 )
          {
          case 4: /* opaque */
            memcpy( (void*)bg, (void*)fb, count * 2 );
            memcpy( (void*)fb, (void*)rle, count * 2 );
            rle += count, fb += count, bg += count;
            break;
          
          case 0: /* transparent */
            fb += count;
            break;
          
          case 2: /* 50% */
            do
            {
              c1    = *fb;
              *bg++ = c1;
              c1   &= 0xf7de;
              c2    = *rle++ & 0xf7de;
              *fb++ = ( c1 + c2 ) >> 1;
            }
            while ( --count );
            break;
          
          case 1: /* 25% */
            do
            {
              c1    = *fb;
              *bg++ = c1;
              c1   &= 0xe79c;
              c2    = *rle++ & 0xe79c;
              *fb++ = ( 3 * c1 + c2 ) >> 2;
            }
            while ( --count );
            break;
          
          case 3: /* 75% */
            do
            {
              c1    = *fb;
              *bg++ = c1;
              c1   &= 0xe79c;
              c2    = *rle++ & 0xe79c;
              *fb++ = ( c1 + 3 * c2 ) >> 2;
            }
            while ( --count );
            break;
          
          /* these are just to avoid gcc adding a cmp + ja */
          case 5:
            rle++;
          case 6:
            rle++;
          case 7:
            rle++;
            break;
          }
        }
        while ( --x );
        
        fb += pitch;
      }
      while ( --y );
    }
    else
    {
      do
      {
        /* runs in this row */
        x = *rle++;
        
        do
        {
          /* decode */
          code   = *rle++;
          count  = code & 0x1fff;
          code >>= 13;
          
          switch ( code & 7 )
          {
          case 4: /* opaque */
            memcpy( (void*)fb, (void*)rle, count * 2 );
            rle += count, fb += count;
            break;
          
          case 0: /* transparent */
            fb += count;
            break;
          
          case 2: /* 50% */
            do
            {
              c1    = *fb & 0xf7de;
              c2    = *rle++ & 0xf7de;
              *fb++ = ( c1 + c2 ) >> 1;
            }
            while ( --count );
            break;
          
          case 1: /* 25% */
            do
            {
              c1    = *fb & 0xe79c;
              c2    = *rle++ & 0xe79c;
              *fb++ = ( 3 * c1 + c2 ) >> 2;
            }
            while ( --count );
            break;
          
          case 3: /* 75% */
            do
            {
              c1    = *fb & 0xe79c;
              c2    = *rle++ & 0xe79c;
              *fb++ = ( c1 + 3 * c2 ) >> 2;
            }
            while ( --count );
            break;
          
          /* these are just to avoid gcc adding a cmp + ja */
          case 5:
            rle++;
          case 6:
            rle++;
          case 7:
            rle++;
            break;
          }
        }
        while ( --x );
        
        fb += pitch;
      }
      while ( --y );
    }
  }
}

void unblit_picture( const gwlua_picture_t* picture, int x, int y, uint16_t* bg )
{
  if ( picture->rle )
  {
    uint16_t* fb = picture->state->screen + y * picture->state->width + x;
    unsigned  pitch = picture->state->width - picture->width;
    uint16_t* rle = picture->rle;
    int       code, count;
    
    y = picture->height;
    
    do
    {
      /* runs in this row */
      x = *rle++;
      
      do
      {
        /* decode */
        code   = *rle++;
        count  = code & 0x1fff;
        code >>= 13;
        
        switch ( code & 7 )
        {
        case 4: /* opaque */
        case 2: /* 50% */
        case 1: /* 25% */
        case 3: /* 75% */
          memcpy( (void*)fb, (void*)bg, count * 2 );
          rle += count, fb += count, bg += count;
          break;
        
        case 0: /* transparent */
          fb += count;
          break;
        
        /* these are just to avoid gcc adding a cmp + ja */
        case 5:
          rle++;
        case 6:
          rle++;
        case 7:
          rle++;
          break;
        }
      }
      while ( --x );
      
      fb += pitch;
    }
    while ( --y );
  }
}

static int l_gc( lua_State* L )
{
  gwlua_picture_t* self = (gwlua_picture_t*)lua_touserdata( L, 1 );
  gwlua_free( self->rle );
  return 0;
}

static int l_index( lua_State* L )
{
  gwlua_picture_t* self = (gwlua_picture_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( gwlua_djb2( key ) )
  {
  // case 0x7c95915fU: // data
    // lua_pushlightuserdata( L, (void*)self->rle );
    // return 1;
  }

  return luaL_error( L, "%s not found in picture", key );
}

static int l_newindex( lua_State* L )
{
  gwlua_picture_t* self = (gwlua_picture_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );
  const char* data;
  size_t len;

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c95915fU: // data
    data = luaL_checklstring( L, 3, &len );
    assign_data( L, self, (void*)data, len );
    return 0;
  }

  return luaL_error( L, "%s not found in picture", key );
}

static int l_tostring( lua_State* L )
{
  gwlua_picture_t* self = (gwlua_picture_t*)lua_touserdata( L, 1 );
  lua_pushfstring( L, "picture@%p", self );
  return 1;
}

static int l_new( lua_State* L )
{
  gwlua_t* state = get_state( L );
  gwlua_picture_t* self = (gwlua_picture_t*)lua_newuserdata( L, sizeof( gwlua_picture_t ) );
  
  self->state = state;
  self->width = self->height = self->used = 0;
  self->rle = NULL;
  
  if ( luaL_newmetatable( L, "picture" ) != 0 )
  {
    static const luaL_Reg methods[] =
    {
      { "__index",    l_index },
      { "__newindex", l_newindex },
      { "__tostring", l_tostring },
      { "__gc",       l_gc },
      { NULL, NULL }
    };
    
    luaL_setfuncs( L, methods, 0 );
  }
  
  lua_setmetatable( L, -2 );
  return 1;
}

void register_picture( lua_State* L, gwlua_t* state )
{
  static const luaL_Reg statics[] =
  {
    { "newpicture", l_new },
    { NULL, NULL }
  };
  
  lua_pushlightuserdata( L, (void*)state );
  luaL_setfuncs( L, statics, 1 );
}
