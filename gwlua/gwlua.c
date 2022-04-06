#include <gwlua.h>

#include <string.h>

#include <gwrom.h>
#include <bsreader.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static void* l_alloc( void* ud, void* ptr, size_t osize, size_t nsize )
{
  (void)ud;
  (void)osize;
  
  if ( nsize == 0 )
  {
    if ( ptr )
    {
      gwlua_free( ptr );
    }
    
    return NULL;
  }

  return gwlua_realloc( ptr, nsize );
}

static int l_traceback( lua_State* L )
{
  luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
  return 1;
}

static int l_pcall( lua_State* L, int nargs, int nres )
{
  int errndx = lua_gettop( L ) - nargs;
  int ret;
  lua_pushcfunction( L, l_traceback );
  lua_insert( L, errndx );
  
  ret = lua_pcall( L, nargs, nres, errndx );
  lua_remove(L, errndx);
  
  if ( ret != LUA_OK )
  {
    gwlua_log( "\n==============================\n%s\n------------------------------\n", lua_tostring( L, -1 ) );
    lua_pop( L, 1 );
    return -1;
  }
  
  return 0;
}

void register_functions( lua_State* L, gwlua_t* state );

static int l_create( lua_State* L )
{
  gwlua_t* state = (gwlua_t*)lua_touserdata( L, 1 );
  gwrom_entry_t entry;
  int error;
  void* bs;

  register_functions( L, state );
  
  error = gwrom_find( &entry, state->rom, "main.bs" );
  
  if ( error != GWROM_OK )
  {
    return luaL_error( L, "%s", gwrom_error_message( error ) );
  }
  
  bs = bsnew( entry.data );
  
  if ( !bs )
  {
    return luaL_error( L, "out of memory allocating the bs reader" );
  }
  
  if ( lua_load( L, bsread, bs, "main.lua", "t" ) != LUA_OK )
  {
    free( bs );
    return lua_error( L );
  }
  
  free( bs );
  
  lua_call( L, 0, 1 );
  gwlua_ref_create( L, -1, &state->tick_ref );
  return 0;
}

int gwlua_create( gwlua_t* state, gwrom_t* rom )
{
  int i;

  static const luaL_Reg lualibs[] =
  {
    { "_G", luaopen_base },
    { LUA_LOADLIBNAME, luaopen_package },
    { LUA_COLIBNAME, luaopen_coroutine },
    { LUA_TABLIBNAME, luaopen_table },
    // { LUA_IOLIBNAME, luaopen_io }, // remove because of tmpfile
    // { LUA_OSLIBNAME, luaopen_os }, // remove because of system
    { LUA_STRLIBNAME, luaopen_string },
    { LUA_MATHLIBNAME, luaopen_math },
    { LUA_UTF8LIBNAME, luaopen_utf8 },
    { LUA_DBLIBNAME, luaopen_debug },
  };

  state->L = lua_newstate( l_alloc, NULL );
  
  if ( !state->L )
  {
    return -1;
  }
  
#ifndef NDEBUG
  lua_pushboolean( state->L, 1 );
  lua_setglobal( state->L, "_DEBUG" );
#endif
    
  for ( i = 0; i < sizeof( lualibs ) / sizeof( lualibs[ 0 ] ); i++ )
  {
    luaL_requiref( state->L, lualibs[ i ].name, lualibs[ i ].func, 1 );
    lua_pop( state->L, 1 );
  }
  
  state->rom = rom;
  state->width = state->height = 0;
  state->zoom.x0 = state->zoom.y0 = state->zoom.w = state->zoom.h = -1;
  state->screen = NULL;
  state->now = 0;
  memset( (void*)state->input, 0, sizeof( state->input ) );
  state->layer = 16383;
  state->tick_ref = LUA_NOREF;
  
  lua_pushcfunction( state->L, l_create );
  lua_pushlightuserdata( state->L, (void*)state );
  
  if ( l_pcall( state->L, 1, 0 ) )
  {
    lua_close( state->L );
    state->L = NULL;
    return -1;
  }
  
  return 0;
}

void gwlua_destroy( gwlua_t* state )
{
  if ( state->L )
  {
    lua_close( state->L );
    state->L = NULL;
  }
}

int gwlua_reset( gwlua_t* state )
{
  gwrom_t* rom = state->rom;
  gwlua_destroy( state );
  return gwlua_create( state, rom );
}

/*---------------------------------------------------------------------------*/

void gwlua_set_button( gwlua_t* state, int port, int button, int pressed )
{
  state->input[ port ][ button ] = pressed;
}

void gwlua_set_pointer( gwlua_t* state, int x, int y, int pressed )
{
  state->pointer.x = x;
  state->pointer.y = y;
  state->pointer.pressed = pressed;
}

/*---------------------------------------------------------------------------*/

void gwlua_tick( gwlua_t* state )
{
  gwlua_ref_get( state->L, state->tick_ref );
  l_pcall( state->L, 0, 1 );

  if ( lua_toboolean( state->L, -1 ) )
  {
    state->now += 16666;
  }

  lua_pop( state->L, 1 );

  lua_gc( state->L, LUA_GCSTEP, 0 );
}

/*---------------------------------------------------------------------------*/

uint32_t gwlua_djb2( const char* str )
{
  const uint8_t* aux = (const uint8_t*)str;
  uint32_t hash = 5381;

  while ( *aux )
  {
    hash = ( hash << 5 ) + hash + *aux++;
  }

  return hash;
}

/*---------------------------------------------------------------------------*/

void gwlua_log( const char* format, ... )
{
  va_list args;
  va_start( args, format );
  gwlua_vlog( format, args );
  va_end( args );
}
