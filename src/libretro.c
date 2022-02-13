#include <libretro.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <gwlua.h>

/*---------------------------------------------------------------------------*/

static bool libretro_supports_persistent_buffer = false;
static bool libretro_supports_bitmasks          = false;

static void dummy_log( enum retro_log_level level, const char* fmt, ... ) { }

#define SRAM_MAX 8

typedef struct
{
  char types[ SRAM_MAX ];
  char keys[ SRAM_MAX ][ 32 ];
  char values[ SRAM_MAX ][ 64 ];
  char count;
}
sram_t;

retro_log_printf_t log_cb = dummy_log;
retro_environment_t env_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_batch_t audio_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

static int     init;
static gwrom_t rom;
static gwlua_t state;
static sram_t  sram;
static int     offset;
static int     soft_width;
static int     soft_height;

static struct retro_input_descriptor input_descriptors[] =
{
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP, "Up" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN, "Down" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT, "Left" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT, "Right" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A, "A" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B, "B" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X, "X" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y, "Y" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L, "L1" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R, "R1" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L2, "L2" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R2, "R2" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_L3, "L3" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_R3, "R3" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT, "Select" },
  { 0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START, "Start" },
  /* TODO: Is this needed? */
  { 255, 255, 255, 255, NULL }
};

/*---------------------------------------------------------------------------*/
/* gwlua user-defined functions */

static int find_key( const char* key )
{
   for ( int i = 0; i < sram.count; i++ )
   {
      if ( !strcmp( sram.keys[ i ], key ) )
         return i;
   }

   return -1;
}

const char* gwlua_load_value( gwlua_t* state, const char* key, int* type )
{
   int i = find_key( key );

   if (i != -1)
   {
      *type = sram.types[ i ];
      return sram.values[ i ];
   }

   return NULL;
}

void gwlua_save_value( gwlua_t* state, const char* key, const char* value, int type )
{
   int i = find_key( key );

   if ( i == -1 )
   {
      if ( sram.count == SRAM_MAX )
      {
         /* TODO: return an error when SRAM is full */
         log_cb( RETRO_LOG_ERROR, "Out of space writing <%s, %s> to SRAM\n", key, value );
         return;
      }

      i = sram.count++;
   }

   sram.types[ i ] = type;

   strncpy( sram.keys[ i ], key, sizeof( sram.keys[ i ] ) );
   sram.keys[ i ][ sizeof( sram.keys[ i ] ) - 1 ] = 0;

   strncpy( sram.values[ i ], value, sizeof( sram.values[ i ] ) );
   sram.values[ i ][ sizeof( sram.values[ i ] ) - 1 ] = 0;
}

int gwlua_set_fb( unsigned width, unsigned height )
{
   struct retro_game_geometry geometry;

   geometry.base_width   = width;
   geometry.base_height  = height;
   geometry.max_width    = width;
   geometry.max_height   = height;
   geometry.aspect_ratio = 0.0f;

   env_cb( RETRO_ENVIRONMENT_SET_GEOMETRY, &geometry );

   offset      = 0;
   soft_width  = width;
   soft_height = height;

   return 0;
}

void gwlua_zoom( gwlua_t* state, int x0, int y0, int width, int height )
{
  struct retro_game_geometry geometry;
  
  if ( x0 >= 0 )
  {
    geometry.base_width  = width;
    geometry.base_height = height;
    soft_width           = width;
    soft_height          = height;
    offset               = y0 * state->width + x0;
  }
  else
  {
    geometry.base_width  = state->width;
    geometry.base_height = state->height;
    soft_width           = state->width;
    soft_height          = state->height;
    offset               = 0;
  }
  
  geometry.max_width     = state->width;
  geometry.max_height    = state->height;
  geometry.aspect_ratio  = 0.0f;
  
  env_cb( RETRO_ENVIRONMENT_SET_GEOMETRY, &geometry );
}

void gwlua_vlog( const char* format, va_list args )
{
  char buffer[ 8192 ]; /* should be enough */
  
  vsnprintf( buffer, sizeof( buffer ), format, args );
  buffer[ sizeof( buffer ) - 1 ] = 0;
  log_cb( RETRO_LOG_ERROR, "%s", buffer );
}

/*---------------------------------------------------------------------------*/
/* compatibility functions */

#ifdef __PS3__
char* getenv( const char* name ) { return NULL; }
#endif

/*---------------------------------------------------------------------------*/

extern const char* gw_version;
extern const char* gw_hithash;

void retro_get_system_info( struct retro_system_info* info )
{
  info->library_name     = "Game & Watch";
  info->library_version  = gw_version;
  info->need_fullpath    = false;
  info->block_extract    = false;
  info->valid_extensions = "mgw";
}

void retro_set_environment( retro_environment_t cb )
{
  static const struct retro_system_content_info_override content_overrides[] = {
     {
        "mgw",    /* extensions */
        false,    /* need_fullpath */
        true      /* persistent_data */
     },
     { NULL, false, false }
  };
  static const struct retro_variable vars[] = {
    { NULL, NULL },
  };
  
  static const struct retro_controller_description controllers[] = {
    { "Controller", RETRO_DEVICE_JOYPAD },
    /* TODO: Is this needed? */
    { NULL, 0 }
  };

  static const struct retro_controller_description pointers[] = {
    { "Pointer", RETRO_DEVICE_POINTER },
    /* TODO: Is this needed? */
    { NULL, 0 }
  };

  static const struct retro_controller_info ports[] = {
    { controllers, 1 },
    { controllers, 1 },
    { pointers, 1 },
    { NULL, 0 }
  };

  env_cb = cb;
  
  cb( RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars );
  cb( RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports );
  cb( RETRO_ENVIRONMENT_SET_CONTENT_INFO_OVERRIDE,
        (void*)content_overrides);
}

unsigned retro_api_version(void)
{
  return RETRO_API_VERSION;
}

void retro_init(void)
{
  struct retro_log_callback log;
  
  if ( env_cb( RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log ) )
    log_cb = log.log;

  if (env_cb(RETRO_ENVIRONMENT_GET_INPUT_BITMASKS, NULL))
     libretro_supports_bitmasks = true;
}

extern const char* gw_gitstamp;
extern const char* rl_gitstamp;

bool retro_load_game( const struct retro_game_info* info )
{
   int res;
   const struct retro_game_info_ext 
      *info_ext                = NULL;
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
   const uint8_t *rom_data     = NULL;
   uint32_t rom_flags          = 0;
   size_t rom_size             = 0;

   if (!info)
      return false;

   if ( !env_cb( RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt ) )
   {
      log_cb( RETRO_LOG_ERROR, "RGB565 is not supported\n" );
      return false;
   }


   log_cb( RETRO_LOG_INFO, "\n%s\n%s", gw_gitstamp, rl_gitstamp );

   if (env_cb(RETRO_ENVIRONMENT_GET_GAME_INFO_EXT, &info_ext) &&
         info_ext->persistent_data)
   {
      libretro_supports_persistent_buffer = true;
      rom_data                            = (const uint8_t*)info_ext->data;
      rom_size                            = info_ext->size;
   }
   else
   {
      libretro_supports_persistent_buffer = false;
      rom_data                            = (const uint8_t*)info->data;
      rom_size                            = info->size;
      /* If buffer is not persistent, we need to copy always */
      rom_flags                           = GWROM_COPY_ALWAYS;
   }

   res = gwrom_init( &rom, (void*)rom_data, rom_size, rom_flags);

   if ( res != GWROM_OK )
   {
      log_cb( RETRO_LOG_ERROR, "Error initializing the rom: ",
            gwrom_error_message( res ) );
      init = -1;
      return false;
   }

   env_cb( RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, input_descriptors );
   memset( (void*)&state, 0, sizeof( state ) );
   state.width  = 128;
   state.height = 128;
   init         = 0;
   return true;
}

size_t retro_get_memory_size( unsigned id )
{
  return id == RETRO_MEMORY_SAVE_RAM ? sizeof( sram ) : 0;
}

void* retro_get_memory_data( unsigned id )
{
  return id == RETRO_MEMORY_SAVE_RAM ? (void*)&sram : NULL;
}

void retro_set_video_refresh( retro_video_refresh_t cb )
{
  video_cb = cb;
}

void retro_set_audio_sample( retro_audio_sample_t cb )
{
  (void)cb;
}

void retro_set_audio_sample_batch( retro_audio_sample_batch_t cb )
{
  audio_cb = cb;
}

void retro_set_input_state( retro_input_state_t cb )
{
  input_state_cb = cb;
}

void retro_set_input_poll( retro_input_poll_t cb )
{
  input_poll_cb = cb;
}

void retro_get_system_av_info( struct retro_system_av_info* info )
{
  info->geometry.base_width = state.width;
  info->geometry.base_height = state.height;
  info->geometry.max_width = state.width;
  info->geometry.max_height = state.height;
  info->geometry.aspect_ratio = 0.0f;
  info->timing.fps = 60.0;
  info->timing.sample_rate = 44100.0;
}

void retro_run(void)
{
   unsigned id;
   int16_t inputs[2];
   int16_t x, y;
   static const struct { unsigned retro; int gw; } map[] =
   {
      { RETRO_DEVICE_ID_JOYPAD_UP,     GWLUA_UP },
      { RETRO_DEVICE_ID_JOYPAD_DOWN,   GWLUA_DOWN },
      { RETRO_DEVICE_ID_JOYPAD_LEFT,   GWLUA_LEFT },
      { RETRO_DEVICE_ID_JOYPAD_RIGHT,  GWLUA_RIGHT },
      { RETRO_DEVICE_ID_JOYPAD_A,      GWLUA_A },
      { RETRO_DEVICE_ID_JOYPAD_B,      GWLUA_B },
      { RETRO_DEVICE_ID_JOYPAD_X,      GWLUA_X },
      { RETRO_DEVICE_ID_JOYPAD_Y,      GWLUA_Y },
      { RETRO_DEVICE_ID_JOYPAD_L,      GWLUA_L1 },
      { RETRO_DEVICE_ID_JOYPAD_R,      GWLUA_R1 },
      { RETRO_DEVICE_ID_JOYPAD_L2,     GWLUA_L2 },
      { RETRO_DEVICE_ID_JOYPAD_R2,     GWLUA_R2 },
      { RETRO_DEVICE_ID_JOYPAD_L3,     GWLUA_L3 },
      { RETRO_DEVICE_ID_JOYPAD_R3,     GWLUA_R3 },
      { RETRO_DEVICE_ID_JOYPAD_SELECT, GWLUA_SELECT },
      { RETRO_DEVICE_ID_JOYPAD_START,  GWLUA_START },
   };

   input_poll_cb();

   if ( init == 0 )
   {
      /* Initialize game */
      if ( gwlua_create( &state, &rom ) )
      {
         log_cb( RETRO_LOG_ERROR, "Error inializing gwlua" );
         init = -1;
         return;
      }

      struct retro_system_av_info info;
      retro_get_system_av_info( &info );
      env_cb( RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &info );

      init = 1;
   }
   else if ( init == -1) /* Error, return */
      return;
   else /* erase sprites here to avoid blank screenshots */
      rl_sprites_unblit();

   inputs[0] = 0;
   inputs[1] = 0;

   if (libretro_supports_bitmasks)
   {
      int16_t ret  = input_state_cb(0, RETRO_DEVICE_JOYPAD, 0,
            RETRO_DEVICE_ID_JOYPAD_MASK);
      int16_t ret2 = input_state_cb(1, RETRO_DEVICE_JOYPAD, 0,
            RETRO_DEVICE_ID_JOYPAD_MASK);
      for ( id = 0; id < sizeof( map ) / sizeof( map [ 0 ] ); id++ )
      {
         if (ret  & (1 << map[id].retro))
            inputs[0] |= (1 << map[id].retro);
         if (ret2 & (1 << map[id].retro))
            inputs[1] |= (1 << map[id].retro);
      }
   }
   else
   {
      for ( id = 0; id < sizeof( map ) / sizeof( map [ 0 ] ); id++ )
      {
         if (input_state_cb( 0, RETRO_DEVICE_JOYPAD, 0, map[ id ].retro ))
            inputs[0] |= (1 << map[id].retro);
         if (input_state_cb( 1, RETRO_DEVICE_JOYPAD, 0, map[ id ].retro ))
            inputs[1] |= (1 << map[id].retro);
      }
   }

   /* Run game */
   for ( id = 0; id < sizeof( map ) / sizeof( map [ 0 ] ); id++ )
   {
      int16_t pressed1 = (inputs[0] & (1 << map[id].retro));
      int16_t pressed2 = (inputs[1] & (1 << map[id].retro));
      gwlua_set_button( &state, 0, map[ id ].gw, pressed1 != 0 );
      gwlua_set_button( &state, 1, map[ id ].gw, pressed2 != 0 );
   }

   x = input_state_cb( 2, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_X );
   y = input_state_cb( 2, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_Y );
   gwlua_set_pointer( &state, x, y, input_state_cb( 2, RETRO_DEVICE_POINTER, 0, RETRO_DEVICE_ID_POINTER_PRESSED ) != 0 );

   gwlua_tick( &state );
   rl_sprites_blit();

   video_cb( state.screen + offset, soft_width, soft_height, state.width * sizeof( uint16_t ) );
   audio_cb( rl_sound_mix(), RL_SAMPLES_PER_FRAME );
}

void retro_deinit(void) { libretro_supports_bitmasks = false; }
void retro_set_controller_port_device( unsigned port, unsigned device ) { }
void retro_reset(void) { gwlua_reset(&state); }
size_t retro_serialize_size(void) { return 0; }
bool retro_serialize( void* data, size_t size ) { return false; }
bool retro_unserialize( const void* data, size_t size ) { return false; }
void retro_cheat_reset(void) { }
void retro_cheat_set( unsigned a, bool b, const char* c ) { }

bool retro_load_game_special(unsigned a, const struct retro_game_info* b, size_t c) { return false; }

void retro_unload_game(void)
{
  gwlua_destroy(&state);
  gwrom_destroy(&rom);
  libretro_supports_persistent_buffer = false;
}

unsigned retro_get_region(void) { return RETRO_REGION_NTSC; }
