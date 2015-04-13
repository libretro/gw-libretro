#include <gwlua.h>

#include <string.h>

#include <gwrom.h>
#include <bsreader.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static void dump_stack( lua_State* L, const char* title )
{
  printf( "================================\n%s\n", title );
  int top = lua_gettop( L );
  int i;
  
  for ( i = 1; i <= top; i++ )
  {
    printf( "%2d %3d ", i, i - top - 1 );
    
    lua_pushvalue( L, i );
    
    switch ( lua_type( L, -1 ) )
    {
    case LUA_TNIL:
      printf( "nil\n" );
      break;
    case LUA_TNUMBER:
      printf( "%e\n", lua_tonumber( L, -1 ) );
      break;
    case LUA_TBOOLEAN:
      printf( "%s\n", lua_toboolean( L, -1 ) ? "true" : "false" );
      break;
    case LUA_TSTRING:
      printf( "\"%s\"\n", lua_tostring( L, -1 ) );
      break;
    case LUA_TTABLE:
      printf( "table\n" );
      break;
    case LUA_TFUNCTION:
      printf( "function\n" );
      break;
    case LUA_TUSERDATA:
      printf( "userdata\n" );
      break;
    case LUA_TTHREAD:
      printf( "thread\n" );
      break;
    case LUA_TLIGHTUSERDATA:
      printf( "light userdata\n" );
      break;
    default:
      printf( "?\n" );
      break;
    }
  }
  
  lua_settop( L, top );
}

static void* l_alloc( void* ud, void* ptr, size_t osize, size_t nsize )
{
  (void)ud;
  (void)osize;
  
  if ( nsize == 0 )
  {
    gwlua_free( ptr );
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
  lua_pushcfunction( L, l_traceback );
  lua_insert( L, -nargs - 2 );
  
  if ( lua_pcall( L, nargs, nres, -nargs - 2 ) != LUA_OK )
  {
    gwlua_log( "==============================\n%s\n------------------------------\n", lua_tostring( L, -1 ) );
    return -1;
  }
  
  return 0;
}

void register_functions( lua_State* L, gwlua_t* state );
void register_image( lua_State* L, gwlua_t* state );
void register_picture( lua_State* L, gwlua_t* state );
void register_sound( lua_State* L, gwlua_t* state );
void register_timer( lua_State* L, gwlua_t* state );

static int l_create( lua_State* L )
{
  gwlua_t* state = (gwlua_t*)lua_touserdata( L, 1 );
  
  lua_newtable( L );
  
  register_functions( L, state );
  register_image( L, state );
  register_picture( L, state );
  register_sound( L, state );
  register_timer( L, state );
  
  lua_setglobal( L, "system" );
  
#ifndef NDEBUG
  lua_pushboolean( L, 1 );
  lua_setglobal( L, "_DEBUG" );
#endif
  
  gwrom_entry_t entry;
  int error = gwrom_find( &entry, state->rom, "main.bs" );
  
  if ( error != GWROM_OK )
  {
    return luaL_error( L, "%s", gwrom_error_message( error ) );
  }
  
  void* bs = bsnew( entry.data );
  
  if ( !bs )
  {
    return luaL_error( L, "out of memory" );
  }
  
  if ( lua_load( L, bsread, bs, "main.lua", "t" ) != LUA_OK )
  {
    free( bs );
    return lua_error( L );
  }
  
  free( bs );
  lua_call( L, 0, 0 );
  return 0;
}

int gwlua_create( gwlua_t* state, gwrom_t* rom, int64_t now )
{
  state->L = lua_newstate( l_alloc, NULL );
  
  if ( !state->L )
  {
    return -1;
  }
  
  luaL_openlibs( state->L );
  
  state->rom = rom;
  state->bg = NULL;
  state->first_frame = 1;
  state->now = now;
  state->playing = NULL;
  
  lua_pushcfunction( state->L, l_create );
  lua_pushlightuserdata( state->L, (void*)state );
  
  if ( l_pcall( state->L, 1, 0 ) )
  {
    lua_close( state->L );
    return -1;
  }
  
  return 0;
}

void gwlua_destroy( gwlua_t* state )
{
  lua_close( state->L );
}

int gwlua_reset( gwlua_t* state )
{
  gwrom_t* rom = state->rom;
  int64_t now = state->now;
  gwlua_destroy( state );
  return gwlua_create( state, rom, now );
}

/*---------------------------------------------------------------------------*/

void gwlua_tick( gwlua_t* state, int64_t now )
{
  state->now = now;
  
  gwlua_ref_get( state->L, state->tick_ref );
  l_pcall( state->L, 0, 0 );
  
  state->first_frame = 0;
}

static const char* button_name( int button )
{
  switch ( button )
  {
  case GWLUA_UP:     return "up";
  case GWLUA_DOWN:   return "down";
  case GWLUA_LEFT:   return "left";
  case GWLUA_RIGHT:  return "right";
  case GWLUA_A:      return "a";
  case GWLUA_B:      return "b";
  case GWLUA_X:      return "x";
  case GWLUA_Y:      return "y";
  case GWLUA_L1:     return "l1";
  case GWLUA_R1:     return "r1";
  case GWLUA_L2:     return "l2";
  case GWLUA_R2:     return "r2";
  case GWLUA_L3:     return "l3";
  case GWLUA_R3:     return "r3";
  case GWLUA_SELECT: return "select";
  case GWLUA_START:  return "start";
  default:           return "?";
  }
}

void gwlua_button_down( gwlua_t* state, unsigned controller_ndx, int button )
{
  gwlua_ref_get( state->L, state->keydown_ref );
  lua_pushstring( state->L, button_name( button ) );
  lua_pushinteger( state->L, controller_ndx );
  
  l_pcall( state->L, 2, 0 );
}

void gwlua_button_up( gwlua_t* state, unsigned controller_ndx, int button )
{
  gwlua_ref_get( state->L, state->keyup_ref );
  lua_pushstring( state->L, button_name( button ) );
  lua_pushinteger( state->L, controller_ndx );
  
  l_pcall( state->L, 2, 0 );
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
