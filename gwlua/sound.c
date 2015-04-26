#include <gwlua.h>

#include <stdint.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static int l_gc( lua_State* L )
{
  rl_sound_t* self = *(rl_sound_t**)lua_touserdata( L, 1 );
  rl_sound_destroy( self );
  return 0;
}

static int l_index( lua_State* L )
{
  const char* key = luaL_checkstring( L, 2 );
  return luaL_error( L, "%s not found in sound", key );
}

static int l_newindex( lua_State* L )
{
  rl_sound_t** self = (rl_sound_t**)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );
  const char* data;
  size_t len;

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c95915fU: // data
    data = luaL_checklstring( L, 3, &len );
    *self = rl_sound_create( data, len, 0 );
    
    if ( !*self )
    {
      return luaL_error( L, "out of memory" );
    }
    
    return 0;
  }

  return luaL_error( L, "%s not found in sound", key );
}

static int l_tostring( lua_State* L )
{
  rl_sound_t* self = *(rl_sound_t**)lua_touserdata( L, 1 );
  lua_pushfstring( L, "sound@%p", self );
  return 1;
}

static int l_new( lua_State* L )
{
  rl_sound_t** self = (rl_sound_t**)lua_newuserdata( L, sizeof( rl_sound_t* ) );
  
  *self = NULL;
  
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
