#include <gwlua.h>

#include <stdint.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static int is_little_endian( void )
{
  union
  {
    uint16_t u16;
    uint8_t u8[ 2 ];
  }
  u;
  
  u.u16 = 1;
  return u.u8[ 0 ];
}

static void assign_data( lua_State* L, gwlua_sound_t* sound, void* data, size_t size )
{
  if ( sound->pcm16 )
  {
    luaL_error( L, "data reassign in sound" );
    return;
  }
  
  sound->pcm16 = (int16_t*)gwlua_malloc( size );
  
  if ( !sound->pcm16 )
  {
    luaL_error( L, "out of memory" );
    return;
  }
  
  sound->size = size / 2;
  
  if ( is_little_endian() )
  {
    uint16_t* src = (uint16_t*)data;
    uint16_t* dest = (uint16_t*)sound->pcm16;
    const uint16_t* end = src + sound->size;
    
    while ( src < end )
    {
      uint16_t sample = *src++;
      sample = ( sample >> 8 ) | ( sample << 8 );
      *dest++ = sample;
    }
  }
  else
  {
    memcpy( (void*)sound->pcm16, data, size );
  }
}

static int l_gc( lua_State* L )
{
  gwlua_sound_t* self = (gwlua_sound_t*)lua_touserdata( L, 1 );
  gwlua_free( self->pcm16 );
  return 0;
}

static int l_index( lua_State* L )
{
  gwlua_sound_t* self = (gwlua_sound_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c95915fU: // data
    lua_pushlightuserdata( L, (void*)self->pcm16 );
    return 1;
  }

  return luaL_error( L, "%s not found in sound", key );
}

static int l_newindex( lua_State* L )
{
  gwlua_sound_t* self = (gwlua_sound_t*)lua_touserdata( L, 1 );
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

  return luaL_error( L, "%s not found in sound", key );
}

static int l_tostring( lua_State* L )
{
  gwlua_sound_t* self = (gwlua_sound_t*)lua_touserdata( L, 1 );
  lua_pushfstring( L, "sound@%p", self );
  return 1;
}

static int l_new( lua_State* L )
{
  gwlua_t* state = get_state( L );
  gwlua_sound_t* self = (gwlua_sound_t*)lua_newuserdata( L, sizeof( gwlua_sound_t ) );
  
  self->state = state;
  self->pcm16 = NULL;
  self->size = 0;
  
  if ( luaL_newmetatable( L, "sound" ) != 0 )
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

void register_sound( lua_State* L, gwlua_t* state )
{
  static const luaL_Reg statics[] =
  {
    { "newsound", l_new },
    { NULL, NULL }
  };
  
  lua_pushlightuserdata( L, (void*)state );
  luaL_setfuncs( L, statics, 1 );
}
