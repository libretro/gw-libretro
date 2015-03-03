#include <gwlua.h>

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/*---------------------------------------------------------------------------*/

typedef struct
{
  gwlua_state_t user;
  
  lua_State* L;
  int        bg_ref;
  int        tick_ref;
  int        button_down_ref;
  int        button_up_ref;
  int64_t    now;
  uint64_t   seed;
  int        first_frame;
}
state_t;

#ifdef NDEBUG
void  lua_setstateud( lua_State* L, void* ud );
void* lua_getstateud( lua_State* L );
#else
static void* s_ud = NULL;
void  lua_setstateud( lua_State* L, void* ud ) { s_ud = ud; }
void* lua_getstateud( lua_State* L ) { return s_ud; }
#endif

static state_t* state_get( lua_State* L )
{
  return (state_t*)lua_getstateud( L );
}

/*---------------------------------------------------------------------------*/

#include <stdio.h>

static void dump_stack( lua_State* L )
{
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
}

/*---------------------------------------------------------------------------*/

static const char* str_int64( char* str_end, int64_t i64 )
{
  int neg = 0;
  
  if ( i64 < 0 )
  {
    neg = 1;
    i64 = -i64;
  }
  
  uint64_t u64 = (uint64_t)i64;
  *--str_end = 0;
  
  do
  {
    *--str_end = u64 % 10 + '0';
    u64 /= 10;
  }
  while ( u64 );
  
  if ( neg )
  {
    *--str_end = '-';
  }
  
  return str_end;
}

/*---------------------------------------------------------------------------*/

static int traceback( lua_State* L )
{
  luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
  return 1;
}

/*---------------------------------------------------------------------------*/

typedef void* (*ud_checker)( lua_State*, int );

static void* ref_create( lua_State* L, int index, int* ref, ud_checker checker, int empty_ok )
{
  if ( empty_ok && lua_isnoneornil( L, index ) )
  {
    *ref = LUA_NOREF;
    return NULL;
  }
  
  lua_pushvalue( L, index );
  *ref = luaL_ref( L, LUA_REGISTRYINDEX );
  return checker( L, index );
}

static void ref_destroy( lua_State* L, int* ref )
{
  if ( *ref != LUA_NOREF )
  {
    luaL_unref( L, LUA_REGISTRYINDEX, *ref );
    *ref = LUA_NOREF;
  }
}

static void* ref_new( lua_State* L, int index, int* ref, ud_checker checker, int empty_ok )
{
  ref_destroy( L, ref );
  return ref_create( L, index, ref, checker, empty_ok );
}

static void ref_get( lua_State* L, int ref )
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

static void* function_check( lua_State* L, int index )
{
  luaL_checktype( L, index, LUA_TFUNCTION );
  return NULL;
}

/*---------------------------------------------------------------------------*/

static uint32_t djb2( const char* str )
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

typedef struct
{
  gwlua_picture_t user;
  int parent_ref;
}
picture_t;

static picture_t* picture_check( lua_State* L, int index )
{
  return (picture_t*)luaL_checkudata( L, index, "picture" );
}

static int picture_gc( lua_State* L )
{
  picture_t* self = (picture_t*)lua_touserdata( L, 1 );
  ref_destroy( L, &self->parent_ref );
  gwlua_destroy_picture( (gwlua_state_t*)state_get( L ), (gwlua_picture_t*)self );
  return 0;
}

static int picture_push( lua_State* L )
{
  /* the userdata object must be on the top of the stack */
  
  if ( luaL_newmetatable( L, "picture" ) != 0 )
  {
    static const luaL_Reg methods[] =
    {
      { "__gc", picture_gc },
      { NULL, NULL }
    };
    
    luaL_setfuncs( L, methods, 0 );
  }
  
  lua_setmetatable( L, -2 );
  return 1;
}

static int picture_load( lua_State* L )
{
  const char* name = luaL_checkstring( L, 1 );
  picture_t* self = (picture_t*)lua_newuserdata( L, sizeof( picture_t ) );
  
  if ( !gwlua_load_picture( (gwlua_state_t*)state_get( L ), (gwlua_picture_t*)self, name ) )
  {
    return picture_push( L );
  }
  
  return luaL_error( L, "error loading picture %s", name );
}

static int picture_sub( lua_State* L )
{
  picture_t* parent = picture_check( L, 1 );
  int x0 = luaL_checkinteger( L, 2 );
  int y0 = luaL_checkinteger( L, 3 );
  unsigned width = luaL_checkinteger( L, 4 );
  unsigned height = luaL_checkinteger( L, 5 );
  
  picture_t* self = (picture_t*)lua_newuserdata( L, sizeof( picture_t ) );
  
  if ( !gwlua_sub_picture( (gwlua_state_t*)state_get( L ), (gwlua_picture_t*)self, (gwlua_picture_t*)parent, x0, y0, width, height ) )
  {
    ref_create( L, 1, &self->parent_ref, (ud_checker)picture_check, 0 );
    return picture_push( L );
  }
  
  return luaL_error( L, "error creating sub picture" );
}

/*---------------------------------------------------------------------------*/

typedef struct
{
  int x;
  int y;
  int is_visible;
  
  picture_t* picture;
  int picture_ref;
}
sprite_t;

static sprite_t* sprite_check( lua_State* L, int index )
{
  return (sprite_t*)luaL_checkudata( L, index, "sprite" );
}

static int sprite_gc( lua_State* L )
{
  sprite_t* self = (sprite_t*)lua_touserdata( L, 1 );
  ref_destroy( L, &self->picture_ref );
  return 0;
}

static int sprite_index( lua_State* L )
{
  sprite_t* self = (sprite_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( djb2( key ) )
  {
  case 0x7c618d53U: // visible
    lua_pushboolean( L, self->is_visible );
    return 1;
    
  case 0xad68f281U: // picture
    ref_get( L, self->picture_ref );
    return 1;
    
  case 0x7c97e438U: // hint
    lua_pushliteral( L, "" );
    return 1;
  }

  return luaL_error( L, "%s not found in sprite", key );
}

static int sprite_newindex( lua_State* L )
{
  sprite_t* self = (sprite_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( djb2( key ) )
  {
  case 0x7c618d53U: // visible
    {
      int is_visible = lua_toboolean( L, 3 );
      state_t* s = state_get( L );
      
      if ( ( is_visible != self->is_visible || s->first_frame ) && self->picture )
      {
        if ( is_visible )
        {
          gwlua_blit_picture( (gwlua_state_t*)s, (gwlua_picture_t*)self->picture, self->x, self->y );
        }
        else
        {
          gwlua_unblit_picture( (gwlua_state_t*)s, (gwlua_picture_t*)self->picture, self->x, self->y );
        }
      }
      
      self->is_visible = is_visible;
      return 0;
    }
    
  case 0xad68f281U: // picture
    {
      picture_t* picture = ref_new( L, 3, &self->picture_ref, (ud_checker)picture_check, 1 );
      state_t* s = state_get( L );
      
      if ( ( picture != self->picture || s->first_frame ) && self->is_visible )
      {
        if ( self->picture )
        {
          gwlua_unblit_picture( (gwlua_state_t*)s, (gwlua_picture_t*)self->picture, self->x, self->y );
        }
        
        if ( picture )
        {
          gwlua_blit_picture( (gwlua_state_t*)s, (gwlua_picture_t*)picture, self->x, self->y );
        }
      }
      
      self->picture = picture;
      return 0;
    }
    
  case 0x7c97e438U: // hint
    return 0;
  }

  return luaL_error( L, "%s not found in sprite", key );
}

static int sprite_new( lua_State* L )
{
  int x = luaL_checkinteger( L, 1 );
  int y = luaL_checkinteger( L, 2 );
  
  if ( lua_gettop( L ) == 2 )
  {
    lua_pushnil( L );
  }
  
  sprite_t* self = (sprite_t*)lua_newuserdata( L, sizeof( sprite_t ) );
  
  self->x = x;
  self->y = y;
  self->is_visible = 1;
  self->picture = ref_create( L, 3, &self->picture_ref, (ud_checker)picture_check, 1 );
  
  if ( luaL_newmetatable( L, "sprite" ) != 0 )
  {
    static const luaL_Reg methods[] =
    {
      { "__index",    sprite_index },
      { "__newindex", sprite_newindex },
      { "__gc",       sprite_gc },
      { NULL, NULL }
    };
    
    luaL_setfuncs( L, methods, 0 );
  }

  lua_setmetatable( L, -2 );
  return 1;
}

/*---------------------------------------------------------------------------*/

typedef struct
{
  int64_t interval;
  int64_t expiration;
  int     is_enabled;
  int     callback_ref;
}
gwtimer_t;

static gwtimer_t* timer_check( lua_State* L, int index )
{
  return (gwtimer_t*)luaL_checkudata( L, index, "timer" );
}

static int timer_gc( lua_State* L )
{
  gwtimer_t* self = (gwtimer_t*)lua_touserdata( L, 1 );
  ref_destroy( L, &self->callback_ref );
  return 0;
}

static int timer_tick( lua_State* L )
{
  gwtimer_t* self = timer_check( L, 1 );
  state_t* state = state_get( L );
  
  if ( self->is_enabled && state->now >= self->expiration && self->callback_ref != LUA_NOREF )
  {
    self->expiration = self->interval + state->now;
    
    lua_pushcfunction( L, traceback );
    ref_get( L, self->callback_ref );
    lua_pushvalue( L, 1 );
    
    if ( lua_pcall( L, 1, 0, -3 ) != LUA_OK )
    {
      fprintf( stderr, "%s", lua_tostring( L, -1 ) );
    }
  }
  
  return 0;
}

static int timer_index( lua_State* L )
{
  gwtimer_t* self = (gwtimer_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( djb2( key ) )
  {
  case 0x8c344f2aU: // interval
    lua_pushinteger( L, self->interval / 1000 );
    return 1;
  case 0x6a23e990U: // enabled
    lua_pushboolean( L, self->is_enabled );
    return 1;
  case 0xd7deacd3U: // onExpired
    ref_get( L, self->callback_ref );
    return 1;
  case 0x7c9e7750U: // tick
    lua_pushcfunction( L, timer_tick );
    return 1;
  }

  return luaL_error( L, "%s not found in timer", key );
}

static int timer_newindex( lua_State* L )
{
  gwtimer_t* self = (gwtimer_t*)lua_touserdata( L, 1 );
  const char* key = luaL_checkstring( L, 2 );

  switch ( djb2( key ) )
  {
  case 0x8c344f2aU: // interval
    {
      self->interval = luaL_checkinteger( L, 3 ) * 1000;
      state_t* state = state_get( L );
      self->expiration = self->interval + state->now;
      
      return 0;
    }
    
  case 0x6a23e990U: // enabled
    {
      self->is_enabled = lua_toboolean( L, 3 );
      state_t* state = state_get( L );
      self->expiration = self->interval + state->now;
      
      return 0;
    }
    
  case 0xd7deacd3U: // onExpired
    ref_new( L, 3, &self->callback_ref, function_check, 1 );
    return 0;
  }

  return luaL_error( L, "%s not found in timer", key );
}

static int timer_tostring( lua_State* L )
{
  gwtimer_t* self = (gwtimer_t*)lua_touserdata( L, 1 );
  
  char buf1[ 128 ];
  const char* interval = str_int64( buf1 + sizeof( buf1 ), self->interval );
  
  char buf2[ 128 ];
  const char* expiration = str_int64( buf2 + sizeof( buf2 ), self->expiration );
  
  lua_pushfstring( L, "timer( interval=%s, expiration=%s, is_enabled=%s, callback_ref=%d )", interval, expiration, self->is_enabled ? "true" : "false", self->callback_ref );
  return 1;
}

static int timer_new( lua_State* L )
{
  gwtimer_t* self = (gwtimer_t*)lua_newuserdata( L, sizeof( gwtimer_t ) );
  
  self->is_enabled = 0;
  
  if ( luaL_newmetatable( L, "timer" ) != 0 )
  {
    static const luaL_Reg methods[] =
    {
      { "__index",    timer_index },
      { "__newindex", timer_newindex },
      { "__tostring", timer_tostring },
      { "__gc",       timer_gc },
      { NULL, NULL }
    };
    
    luaL_setfuncs( L, methods, 0 );
  }
  
  lua_setmetatable( L, -2 );
  return 1;
}

/*---------------------------------------------------------------------------*/

typedef struct
{
  gwlua_sound_t user;
}
sound_t;

static sound_t* sound_check( lua_State* L, int index )
{
  return (sound_t*)luaL_checkudata( L, index, "sound" );
}

static int sound_gc( lua_State* L )
{
  sound_t* self = (sound_t*)lua_touserdata( L, 1 );
  gwlua_destroy_sound( (gwlua_state_t*)state_get( L ), (gwlua_sound_t*)self );
  return 0;
}

static int sound_play( lua_State* L )
{
  sound_t* self = sound_check( L, 1 );
  gwlua_play_sound( (gwlua_state_t*)state_get( L ), (gwlua_sound_t*)self );
  return 0;
}

static int sound_index( lua_State* L )
{
  const char* key = luaL_checkstring( L, 2 );

  switch ( djb2( key ) )
  {
  case 0x7c9c525bU: // play
    lua_pushcfunction( L, sound_play );
    return 1;
  }

  return luaL_error( L, "%s not found in sound", key );
}

static int sound_stop( lua_State* L )
{
  gwlua_stop_all_sounds( (gwlua_state_t*)state_get( L ) );
  return 0;
}

static int sound_newindex( lua_State* L )
{
  const char* key = luaL_checkstring( L, 2 );
  return luaL_error( L, "%s not found in sound", key );
}

static int sound_load( lua_State* L )
{
  const char* name = luaL_checkstring( L, 1 );
  sound_t* self = (sound_t*)lua_newuserdata( L, sizeof( sound_t ) );
  
  if ( !gwlua_load_sound( (gwlua_state_t*)state_get( L ), (gwlua_sound_t*)self, name ) )
  {
    if ( luaL_newmetatable( L, "sound" ) != 0 )
    {
      static const luaL_Reg methods[] =
      {
        { "__index",    sound_index },
        { "__newindex", sound_newindex },
        { "__gc",       sound_gc },
        { NULL, NULL }
      };
      
      luaL_setfuncs( L, methods, 0 );
    }

    lua_setmetatable( L, -2 );
    return 1;
  }
  
  return luaL_error( L, "error loading sound %s", name );
}

/*---------------------------------------------------------------------------*/

static int value_load( lua_State* L )
{
  const char* key = luaL_checkstring( L, 1 );
  
  int type;
  const char* value = gwlua_load_value( (gwlua_state_t*)state_get( L ), key, &type );
  
  if ( value )
  {
    switch ( type )
    {
    case GWLUA_NULL:
    default:
      lua_pushnil( L );
      break;
      
    case GWLUA_BOOLEAN:
      lua_pushboolean( L, !strcmp( value, "true" ) );
      break;
      
    case GWLUA_NUMBER:
      if ( !lua_stringtonumber( L, value ) )
      {
        lua_pushinteger( L, 0 );
      }
      break;
      
    case GWLUA_STRING:
      lua_pushstring( L, value );
      break;
    }
  }
  else
  {
    lua_pushnil( L );
  }
  
  return 1;
}

static int value_save( lua_State* L )
{
  gwlua_state_t* state = (gwlua_state_t*)state_get( L );
  const char* key = luaL_checkstring( L, 1 );
  
  switch ( lua_type( L, 2 ) )
  {
  case LUA_TBOOLEAN:
    gwlua_save_value( state, key, lua_toboolean( L, 2 ) ? "true" : "false", GWLUA_BOOLEAN );
    break;
    
  case LUA_TNUMBER:
    gwlua_save_value( state, key, lua_tostring( L, 2 ), GWLUA_NUMBER );
    break;
    
  case LUA_TSTRING:
    gwlua_save_value( state, key, lua_tostring( L, 2 ), GWLUA_STRING );
    break;
    
  default:
    gwlua_save_value( state, key, NULL, GWLUA_NULL );
    break;
  }
  
  return 1;
}

/*---------------------------------------------------------------------------*/

static int array_new( lua_State* L )
{
  int top = lua_gettop( L ) - 2;
  int i, j, k, a, b, res;
  
  /* level 1 */
  lua_newtable( L );
  a = b = res = lua_gettop( L );
  
  /* level n */
  for ( i = 1; i <= top; i += 2 )
  {
    int begin = (int)luaL_checkinteger( L, i );
    int end = (int)luaL_checkinteger( L, i + 1 );
    
    for ( j = a; j <= b; j++ )
    {
      for ( k = begin; k <= end; k++ )
      {
        lua_newtable( L );
        lua_pushvalue( L, -1 );
        lua_rawseti( L, j, k );
      }
    }
    
    a = b + 1;
    b = lua_gettop( L );
  }
  
  lua_pushvalue( L, res );
  return 1;
}

static int bg_set( lua_State* L )
{
  picture_t* bg = picture_check( L, 1 );
  state_t* state = state_get( L );
  
  if ( state->bg_ref == LUA_NOREF )
  {
    gwlua_set_bg( (gwlua_state_t*)state, (gwlua_picture_t*)bg );
    ref_create( L, 1, &state->bg_ref, (ud_checker)picture_check, 0 );
    return 0;
  }
  
  return luaL_error( L, "background already set" );
}

static int randomize( lua_State* L )
{
  state_t* state = state_get( L );
  state->seed = time( NULL );
  return 0;
}

static int rnd( lua_State* L )
{
  state_t* state = state_get( L );
  state->seed = 6364136223846793005ULL * state->seed + 1;
  double frac = ( state->seed >> 11 ) / 9007199254740992.0;
  
  if ( lua_isnumber( L, 1 ) )
  {
    lua_pushinteger( L, (int64_t)( frac * lua_tointeger( L, 1 ) ) );
  }
  else
  {
    lua_pushnumber( L, frac );
  }
  
  return 1;
}

static int time_now( lua_State* L )
{
  lua_pushinteger( L, time( NULL ) );
  return 1;
}

static int time_split( lua_State* L )
{
  time_t tt = (time_t)luaL_checkinteger( L, 1 );
  struct tm* tm = localtime( &tt );
  
  lua_pushinteger( L, tm->tm_hour );
  lua_pushinteger( L, tm->tm_min );
  lua_pushinteger( L, tm->tm_sec );
  lua_pushinteger( L, 0 );
  lua_pushinteger( L, tm->tm_mday );
  lua_pushinteger( L, tm->tm_mon + 1 );
  lua_pushinteger( L, tm->tm_year + 1900 );
  return 7;
}

// ----------------------------------------------------------------------------

void gwlua_tick( gwlua_state_t* state, int64_t now )
{
  state_t* s = (state_t*)state;
  int top = lua_gettop( s->L );
  s->now = now;
  
  lua_pushcfunction( s->L, traceback );
  ref_get( s->L, s->tick_ref );
  
  if ( lua_pcall( s->L, 0, 0, -2 ) != LUA_OK )
  {
    fprintf( stderr, "%s", lua_tostring( s->L, -1 ) );
  }
  
  lua_settop( s->L, top );
  s->first_frame = 0;
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

void gwlua_button_down( gwlua_state_t* state, unsigned controller_ndx, int button )
{
  state_t* s = (state_t*)state;
  int top = lua_gettop( s->L );
  
  lua_pushcfunction( s->L, traceback );
  
  ref_get( s->L, s->button_down_ref );
  lua_pushstring( s->L, button_name( button ) );
  lua_pushinteger( s->L, controller_ndx );
  
  if ( lua_pcall( s->L, 2, 0, -4 ) != LUA_OK )
  {
    fprintf( stderr, "%s", lua_tostring( s->L, -1 ) );
  }
  
  lua_settop( s->L, top );
}

void gwlua_button_up( gwlua_state_t* state, unsigned controller_ndx, int button )
{
  state_t* s = (state_t*)state;
  int top = lua_gettop( s->L );
  
  lua_pushcfunction( s->L, traceback );
  
  ref_get( s->L, s->button_up_ref );
  lua_pushstring( s->L, button_name( button ) );
  lua_pushinteger( s->L, controller_ndx );
  
  if ( lua_pcall( s->L, 2, 0, -4 ) != LUA_OK )
  {
    fprintf( stderr, "%s", lua_tostring( s->L, -1 ) );
  }
  
  lua_settop( s->L, top );
}

/*---------------------------------------------------------------------------*/

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

int gwlua_create( gwlua_state_t* state, const void* main, size_t size )
{
  static const luaL_Reg statics[] =
  {
    { "loadPicture",   picture_load },
    { "newSubPicture", picture_sub },
    { "newSprite",     sprite_new },
    { "newTimer",      timer_new },
    { "loadSound",     sound_load },
    { "stopSounds",    sound_stop },
    { "loadValue",     value_load },
    { "saveValue",     value_save },
    { "array",         array_new },
    { "setBackground", bg_set },
    { "randomize",     randomize },
    { "random",        rnd },
    { "now",           time_now },
    { "splitTime",     time_split },
    { NULL, NULL }
  };
  
  state_t* s = (state_t*)state;
  s->L = lua_newstate( l_alloc, (void*)s );
  
  if ( s->L )
  {
    s->bg_ref = LUA_NOREF;
    s->tick_ref = LUA_NOREF;
    s->button_down_ref = LUA_NOREF;
    s->button_up_ref = LUA_NOREF;
    s->first_frame = 1;
    
    lua_setstateud( s->L, (void*)s );
    int top = lua_gettop( s->L );
    
    luaL_openlibs( s->L );
    luaL_newlib( s->L, statics );
    lua_setglobal( s->L, "gw" );
    
#ifndef NDEBUG
    lua_pushboolean( s->L, 1 );
    lua_setglobal( s->L, "_DEBUG" );
#endif
    
    lua_settop( s->L, top );
    lua_pushcfunction( s->L, traceback );
    
    switch ( luaL_loadbufferx( s->L, (const char*)main, size, "main.lua", "t" ) )
    {
    case LUA_OK:
      if ( lua_pcall( s->L, 0, 3, -2 ) == LUA_OK )
      {
        ref_create( s->L, -3, &s->tick_ref, function_check, 0 );
        ref_create( s->L, -2, &s->button_down_ref, function_check, 0 );
        ref_create( s->L, -1, &s->button_up_ref, function_check, 0 );
        lua_settop( s->L, 0 );
        return 0;
      }

      fprintf( stderr, "%s", lua_tostring( s->L, -1 ) );
      break;
      
    case LUA_ERRSYNTAX:
      fprintf( stderr, "%s", lua_tostring( s->L, -1 ) );
      break;
      
    case LUA_ERRMEM:
      fprintf( stderr, "out of memory" );
      break;
      
    case LUA_ERRGCMM:
      fprintf( stderr, "error running __gc" );
      break;
    }
  }
  else
  {
    fprintf( stderr, "main.lua not found" );
  }
    
  lua_close( s->L );
  return -1;
}

void gwlua_destroy( gwlua_state_t* state )
{
  state_t* s = (state_t*)state;
  lua_close( s->L );
}

void gwlua_reset( gwlua_state_t* state )
{
  /* TODO implement */
  (void)state;
}
