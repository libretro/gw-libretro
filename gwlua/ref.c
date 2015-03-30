#include <gwlua.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void gwlua_ref_create( lua_State* L, int index, int* ref )
{
  lua_pushvalue( L, index );
  *ref = luaL_ref( L, LUA_REGISTRYINDEX );
}

void gwlua_ref_destroy( lua_State* L, int* ref )
{
  if ( *ref != LUA_NOREF )
  {
    luaL_unref( L, LUA_REGISTRYINDEX, *ref );
    *ref = LUA_NOREF;
  }
}

void gwlua_ref_new( lua_State* L, int index, int* ref )
{
  gwlua_ref_destroy( L, ref );
  gwlua_ref_create( L, index, ref );
}

void gwlua_ref_get( lua_State* L, int ref )
{
  if ( ref != LUA_NOREF )
  {
    lua_rawgeti( L, LUA_REGISTRYINDEX, ref );
  }
  else
  {
    lua_pushnil( L );
  }
}
