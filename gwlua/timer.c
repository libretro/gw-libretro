#include <gwlua.h>

#include <stdint.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static gwlua_timer_t* l_check( lua_State* L, int index )
{
  return (gwlua_timer_t*)luaL_checkudata( L, index, "timer" );
}

static int l_gc( lua_State* L )
{
  gwlua_timer_t* self = (gwlua_timer_t*)lua_touserdata( L, 1 );
  gwlua_ref_destroy( L, &self->callback_ref );
  return 0;
}

static int l_tick( lua_State* L )
{
  gwlua_timer_t* self = l_check( L, 1 );
  
  if ( self->is_enabled && self->state->now >= self->expiration && self->callback_ref != LUA_NOREF )
  {
    self->expiration = self->interval + self->state->now;
    
    gwlua_ref_get( L, self->callback_ref );
    lua_pushvalue( L, 1 );
    
    lua_call( L, 1, 0 );
  }
  
  return 0;
}

static int l_index( lua_State* L )
{
  gwlua_timer_t* self = (gwlua_timer_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );
  
  switch ( gwlua_djb2( key ) )
  {
  case 0x8c344f2aU: // interval
    lua_pushinteger( L, self->interval / 1000 );
    return 1;
  case 0x6a23e990U: // enabled
    lua_pushboolean( L, self->is_enabled );
    return 1;
  case 0x6d45f5a3U: // ontimer
    gwlua_ref_get( L, self->callback_ref );
    return 1;
  case 0x7c9e7750U: // tick
    lua_pushcfunction( L, l_tick );
    return 1;
  }

  return luaL_error( L, "%s not found in timer", key );
}

static int l_newindex( lua_State* L )
{
  gwlua_timer_t* self = (gwlua_timer_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( gwlua_djb2( key ) )
  {
  case 0x0b88af18U: // top
  case 0x7c9a03b0U: // left
    return 0;
    
  case 0x8c344f2aU: // interval
    self->interval = luaL_checkinteger( L, 3 ) * 1000;
    self->expiration = self->interval + self->state->now;
    return 0;
    
  case 0x6a23e990U: // enabled
    self->is_enabled = lua_toboolean( L, 3 );
    self->expiration = self->interval + self->state->now;
    return 0;
    
  case 0x6d45f5a3U: // ontimer
    if ( !lua_isnoneornil( L, 3 ) )
    {
      luaL_checktype( L, 3, LUA_TFUNCTION );
    }
    
    gwlua_ref_new( L, 3, &self->callback_ref );
    return 0;
  }

  return luaL_error( L, "%s not found in timer", key );
}

static int l_tostring( lua_State* L )
{
  gwlua_timer_t* self = (gwlua_timer_t*)lua_touserdata( L, 1 );
  lua_pushfstring( L, "timer@%p", self );
  return 1;
}

static int l_new( lua_State* L )
{
  gwlua_t* state = get_state( L );
  gwlua_timer_t* self = (gwlua_timer_t*)lua_newuserdata( L, sizeof( gwlua_timer_t ) );
  
  self->state = state;
  self->interval = self->expiration = 0;
  self->is_enabled = 0;
  self->callback_ref = LUA_NOREF;
  
  if ( luaL_newmetatable( L, "timer" ) != 0 )
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

void register_timer( lua_State* L, gwlua_t* state )
{
  static const luaL_Reg statics[] =
  {
    { "newtimer", l_new },
    { NULL, NULL }
  };
  
  lua_pushlightuserdata( L, (void*)state );
  luaL_setfuncs( L, statics, 1 );
}
