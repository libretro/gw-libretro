#include <gwlua.h>

#include <stdint.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static int l_pic_index( lua_State* L )
{
  const char* key = luaL_checkstring( L, 2 );
  return luaL_error( L, "%s not found in picture", key );
}

static int l_pic_newindex( lua_State* L )
{
  rl_image_t*** self = (rl_image_t***)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );
  const char* data;
  size_t len;

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c95915fU: // data
    data = luaL_checklstring( L, 3, &len );
    **self = rl_image_create( data, len );
    
    if ( !**self )
    {
      return luaL_error( L, "out of memory creating the image" );
    }
    
    return 0;
  }

  return luaL_error( L, "%s not found in picture", key );
}

static int l_pic_tostring( lua_State* L )
{
  rl_image_t*** self = (rl_image_t***)lua_touserdata( L, 1 );
  lua_pushfstring( L, "picture@%p", **self );
  return 1;
}

static int l_pic_push( lua_State* L, rl_image_t** pic )
{
  rl_image_t*** self = (rl_image_t***)lua_newuserdata( L, sizeof( rl_image_t** ) );
  
  *self = pic;
  
  if ( luaL_newmetatable( L, "picture" ) != 0 )
  {
    static const luaL_Reg methods[] =
    {
      { "__index",    l_pic_index },
      { "__newindex", l_pic_newindex },
      { "__tostring", l_pic_tostring },
      { NULL, NULL }
    };
    
    luaL_setfuncs( L, methods, 0 );
  }

  lua_setmetatable( L, -2 );
  return 1;
}

static int l_gc( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_touserdata( L, 1 );
  rl_sprite_destroy( self->sprite );
  gwlua_ref_destroy( L, &self->mousedown_ref );
  gwlua_ref_destroy( L, &self->mouseup_ref );
  return 0;
}

static int l_index( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c618d53U: // visible
    lua_pushboolean( L, ( self->sprite->flags & RL_SPRITE_INVISIBLE ) == 0 );
    return 1;
    
  case 0xad68f281U: // picture
    l_pic_push( L, &self->sprite->image );
    return 1;
    
  case 0xc6a99383U: // onmousedown
    gwlua_ref_get( L, self->mousedown_ref );
    return 1;

  case 0xca597e70U: // onmouseup
    gwlua_ref_get( L, self->mouseup_ref );
    return 1;
  
  case 0x7c9a03b0U: // left
    lua_pushinteger( L, self->sprite->x );
    return 1;
    
  case 0x0b88af18U: // top
    lua_pushinteger( L, self->sprite->y );
    return 1;
    
  case 0x10a3b0a5U: // width
    lua_pushinteger( L, self->width ? self->width : self->sprite->image ? self->sprite->image->width : 0 );
    return 1;
    
  case 0x01d688deU: // height
    lua_pushinteger( L, self->height ? self->height : self->sprite->image ? self->sprite->image->height : 0 );
    return 1;
  }

  return luaL_error( L, "%s not found in image", key );
}

static int l_newindex( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );
  
  switch ( gwlua_djb2( key ) )
  {
  case 0x7c618d53U: // visible
    if ( lua_toboolean( L, 3 ) )
    {
      self->sprite->flags &= ~RL_SPRITE_INVISIBLE;
    }
    else
    {
      self->sprite->flags |= RL_SPRITE_INVISIBLE;
    }
    
    return 0;
    
  case 0xad68f281U: // picture
    self->sprite->image = NULL;
  
    if ( !lua_isnoneornil( L, 3 ) )
    {
      rl_image_t*** picture = (rl_image_t***)luaL_checkudata( L, 3, "picture" );
      self->sprite->image = **picture;
    }
    
    return 0;

  case 0xc6a99383U: // onmousedown
    if ( !lua_isnoneornil( L, 3 ) )
    {
      luaL_checktype( L, 3, LUA_TFUNCTION );
    }
    
    gwlua_ref_new( L, -1, &self->mousedown_ref );
    return 0;

  case 0xca597e70U: // onmouseup
    if ( !lua_isnoneornil( L, 3 ) )
    {
      luaL_checktype( L, 3, LUA_TFUNCTION );
    }
    
    gwlua_ref_new( L, -1, &self->mouseup_ref );
    return 0;
  
  case 0x7c9a03b0U: // left
    self->sprite->x = luaL_checkinteger( L, 3 );
    return 0;
    
  case 0x0b88af18U: // top
    self->sprite->y = luaL_checkinteger( L, 3 );
    return 0;
    
  case 0x0fd89782U: // layer
    self->sprite->layer = luaL_checkinteger( L, 3 );
    return 0;

  case 0x10a3b0a5U: // width
    self->width = luaL_checkinteger( L, 3 );
    return 0;

  case 0x01d688deU: // height
    self->height = luaL_checkinteger( L, 3 );
    return 0;

  case 0xc6ae82c2U: // onmousemove
  case 0x6e576d97U: // transparent
  case 0xf7536923U: // cursor
  case 0x6d2e9883U: // parentshowhint
  case 0xee7f6ff9U: // showhint
  case 0x45b29eb9U: // autosize
  case 0xaf079762U: // stretch
  case 0xf62fb286U: // center
  case 0x6c13e8e8U: // onclick
  case 0x164d61e1U: // dragcursor
  case 0x7c97e438U: // hint
    return 0;
  }

  return luaL_error( L, "%s not found in image", key );
}

static int l_tostring( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_touserdata( L, 1 );
  lua_pushfstring( L, "image@%p", self );
  return 1;
}

static int l_new( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_newuserdata( L, sizeof( gwlua_image_t ) );
  
  self->sprite = rl_sprite_create();
  
  if ( !self->sprite )
  {
    return luaL_error( L, "sprite limit reached" );
  }

  gwlua_t* state = get_state(L);
  
  self->sprite->layer = state->layer--;
  self->width = 0;
  self->height = 0;
  self->mousedown_ref = LUA_NOREF;
  self->mouseup_ref = LUA_NOREF;
  
  if ( luaL_newmetatable( L, "image" ) != 0 )
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

void register_image( lua_State* L, gwlua_t* state )
{
  static const luaL_Reg statics[] =
  {
    { "newimage", l_new },
    { NULL, NULL }
  };
  
  lua_pushlightuserdata( L, (void*)state );
  luaL_setfuncs( L, statics, 1 );
}
