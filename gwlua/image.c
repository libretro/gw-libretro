#include <gwlua.h>
#include <picture.h>

#include <stdint.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define get_state( L ) ( ( gwlua_t* )lua_touserdata( L, lua_upvalueindex( 1 ) ) )

static int blit( gwlua_image_t* image )
{
  gwlua_picture_t* picture = image->picture;
  
  if ( image->is_visible && picture && image->state->screen )
  {
    if ( picture->used > image->used )
    {
      image->bg = (uint16_t*)gwlua_realloc( image->bg, picture->used * sizeof( uint16_t ) );
      
      if ( !image->bg )
      {
        return -1;
      }
      
      image->used = picture->used;
    }
    
    blit_picture( picture, image->x, image->y, image->bg );
    image->blitted = 1;
    image->state->updated = 1;
  }
  
  return 0;
}

static void unblit( gwlua_image_t* image )
{
  if ( image->blitted )
  {
    unblit_picture( image->picture, image->x, image->y, image->bg );
    image->blitted = 0;
    image->state->updated = 1;
  }
}

static int l_gc( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_touserdata( L, 1 );
  gwlua_ref_destroy( L, &self->picture_ref );
  gwlua_free( self->bg );
  return 0;
}

static int l_index( lua_State* L )
{
  gwlua_image_t* self = (gwlua_image_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( gwlua_djb2( key ) )
  {
  case 0x7c618d53U: // visible
    lua_pushboolean( L, self->is_visible );
    return 1;
    
  case 0xad68f281U: // picture
    gwlua_ref_get( L, self->picture_ref );
    return 1;
    
  case 0x7c97e438U: // hint
    lua_pushliteral( L, "" );
    return 1;
    
  case 0x10a3b0a5U: // width
    if ( self->picture )
    {
      lua_pushinteger( L, self->picture->width );
    }
    else
    {
      lua_pushnil( L );
    }
    
    return 1;
    
  case 0x01d688deU: // height
    if ( self->picture )
    {
      lua_pushinteger( L, self->picture->height );
    }
    else
    {
      lua_pushnil( L );
    }
    
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
  case 0x7c9a03b0U: // left
    self->x = luaL_checkinteger( L, 3 );
    return 0;
    
  case 0x0b88af18U: // top
    self->y = luaL_checkinteger( L, 3 );
    return 0;
    
  case 0x10a3b0a5U: // width
  case 0x01d688deU: // height
  case 0xc6a99383U: // onmousedown
  case 0xc6ae82c2U: // onmousemove
  case 0x6e576d97U: // transparent
  case 0xf7536923U: // cursor
  case 0x7c97e438U: // hint
  case 0x6d2e9883U: // parentshowhint
  case 0xee7f6ff9U: // showhint
  case 0xca597e70U: // onmouseup
  case 0x45b29eb9U: // autosize
  case 0xaf079762U: // stretch
  case 0xf62fb286U: // center
  case 0x6c13e8e8U: // onclick
  case 0x164d61e1U: // dragcursor
    return 0;
    
  case 0x7c618d53U: // visible
    unblit( self );
    self->is_visible = lua_toboolean( L, 3 );
    
    if ( blit( self ) )
    {
      return luaL_error( L, "out of memory" );
    }
    
    return 0;
    
  case 0xad68f281U: // picture
    unblit( self );
    
    if ( !lua_isnoneornil( L, 3 ) )
    {
      self->picture = (gwlua_picture_t*)luaL_checkudata( L, 3, "picture" );
      gwlua_ref_new( L, 3, &self->picture_ref );
    }
    else
    {
      self->picture = NULL;
      gwlua_ref_destroy( L, &self->picture_ref );
    }
    
    if ( blit( self ) )
    {
      return luaL_error( L, "out of memory" );
    }
    
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
  gwlua_t* state = get_state( L );
  gwlua_image_t* self = (gwlua_image_t*)lua_newuserdata( L, sizeof( gwlua_image_t ) );
  
  self->state = state;
  self->x = self->y = 0;
  self->is_visible = 1;
  self->blitted = 0;
  
  lua_getglobal( L, "system" );
  lua_getfield( L, -1, "newpicture" );
  
  lua_call( L, 0, 1 );
  
  self->picture = (gwlua_picture_t*)lua_touserdata( L, -1 );
  gwlua_ref_create( L, -1, &self->picture_ref );
  
  self->bg = (uint16_t*)gwlua_malloc( self->picture->used * sizeof( uint16_t ) );
  
  if ( !self->bg )
  {
    return luaL_error( L, "out of memory" );
  }
  
  self->used = self->picture->used;
  
  lua_pop( L, 2 );
  
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
