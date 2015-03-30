#include <gwlua.h>

#include <stdint.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/*---------------------------------------------------------------------------*/
/* stb_image config and inclusion */

#define STBI_ASSERT( x )

#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*---------------------------------------------------------------------------*/

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static void assign_data( lua_State* L, gwlua_picture_t* picture, void* data, size_t size )
{
  static const char* signatures[] =
  {
    "TBitmap",
    "TJPEGImage",
    NULL
  };
  
  if ( picture->pixels )
  {
    luaL_error( L, "data reassign in sound" );
    return;
  }
  
  int i, offset = 0;
  
  for ( i = 0; signatures[ i ]; i++ )
  {
    size_t len = strlen( signatures[ i ] );
    size_t len2 = *(uint8_t*)data;
    
    if ( len == len2 && !strncmp( signatures[ i ], (char*)data + 1, len ) )
    {
      offset = len + 5;
      break;
    }
  }
  
  int width, height;
  uint32_t* abgr32 = (uint32_t*)stbi_load_from_memory( (const stbi_uc*)data + offset, (int)size - offset, &width, &height, NULL, STBI_rgb_alpha );
  
  if ( !abgr32 )
  {
    luaL_error( L, "error loading image" );
    return;
  }
  
  uint32_t* to_free = abgr32;
  uint16_t* rgab16 = (uint16_t*)gwlua_malloc( width * height * sizeof( uint16_t ) );
  picture->pixels = rgab16;
  
  if ( !rgab16 )
  {
    gwlua_free( abgr32 );
    luaL_error( L, "out of memory" );
    return;
  }
  
  const uint32_t* end = abgr32 + width * height;
  
  picture->pixels = rgab16;
  picture->width = width;
  picture->height = height;
  
  while ( abgr32 < end )
  {
    uint32_t p = *abgr32++;
    *rgab16++ = ( ( p & 0xf8 ) << 8 ) | ( ( p & 0xf800 ) >> 5 ) | ( ( p & 0xf80000 ) >> 19 ) | ( ( ( p & 0xff000000U ) != 0 ) << 5 );
  }
  
  gwlua_free( to_free );
}

static int l_gc( lua_State* L )
{
  gwlua_picture_t* self = (gwlua_picture_t*)lua_touserdata( L, 1 );
  gwlua_free( self->pixels );
  return 0;
}

static int l_index( lua_State* L )
{
  gwlua_picture_t* self = (gwlua_picture_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c95915fU: // data
    lua_pushlightuserdata( L, (void*)self->pixels );
    return 1;
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
  self->width = self->height = 0;
  self->pixels = NULL;
  
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
