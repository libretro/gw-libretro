#include <libretro.h>

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>

#include <rl_backgrnd.h>
#include <rl_image.h>
#include <rl_sprite.h>
#include <rl_sound.h>
#include <rl_map.h>
#include <rl_rand.h>
#include <rl_version.h>

#include <button_x.h>
#include <block.h>
#include <tick.h>
#include <sketch008.h>
#include <tile_x.h>
#include <city_tls.h>
#include <city_ims.h>
#include <city_map.h>

/*---------------------------------------------------------------------------*/

#define WIDTH  400
#define HEIGHT 300

/*---------------------------------------------------------------------------*/

static void dummy_log( enum retro_log_level level, const char* fmt, ... )
{
  (void)level;
  (void)fmt;
}

static retro_log_printf_t log_cb = dummy_log;
static retro_environment_t env_cb;
static retro_video_refresh_t video_cb;
static retro_audio_sample_batch_t audio_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;
static struct retro_perf_callback perf_cb;

#define MAX_PADS 2
static unsigned input_devices[ MAX_PADS ];

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
  // TODO: Is this needed?
  { 255, 255, 255, 255, "" }
};

static bool input_state[ MAX_PADS ][ sizeof( input_descriptors ) / sizeof( input_descriptors[ 0 ] ) - 1 ];
 
#ifdef LOG_PERFORMANCE
#define RETRO_PERFORMANCE_INIT(name)  static struct retro_perf_counter name = {#name}; if (!name.registered) perf_cb.perf_register(&(name))
#define RETRO_PERFORMANCE_START(name) perf_cb.perf_start(&(name))
#define RETRO_PERFORMANCE_STOP(name)  perf_cb.perf_stop(&(name))
#else
#define RETRO_PERFORMANCE_INIT(name)
#define RETRO_PERFORMANCE_START(name)
#define RETRO_PERFORMANCE_STOP(name)
#endif

/*---------------------------------------------------------------------------*/

typedef struct
{
  rl_image_t*    image;
  rl_image_t*    block;
  rl_sprite_t*   sprite;
  rl_sprite_t*   sprite2;
  rl_sound_t*    sound;
  rl_tileset_t*  tileset;
  rl_imageset_t* imageset;
  rl_map_t*      map;
  
  float xx, yy, dx, dy;
  int count, reset;
}
testscr_t;

static int testscr_init( testscr_t* s )
{
  rl_sound_init();
  rl_sprite_init();
  
  s->image = rl_image_create( button_x_rle, button_x_rle_len );
  
  if ( !s->image )
  {
  error0:
    return -1;
  }
  
  s->block = rl_image_create( block_rle, block_rle_len );
  
  if ( !s->block )
  {
  error100:
    rl_image_destroy( s->image );
    goto error0;
  }
  
  s->sprite = rl_sprite_create();
  
  if ( !s->sprite )
  {
  error1:
    rl_image_destroy( s->block );
    goto error100;
  }
  
  s->sprite->image = s->image;
  s->sprite->layer = 0;
  
  s->sprite2 = rl_sprite_create();
  
  if ( !s->sprite2 )
  {
  error2:
    rl_sprite_destroy( s->sprite );
    goto error1;
  }
  
  s->sprite2->image = s->image;
  s->sprite2->layer = 1;
  
  s->sound = rl_sound_create( tick_pcm, tick_pcm_len, 0 );
  
  if ( !s->sound )
  {
  error3:
    rl_sprite_destroy( s->sprite2 );
    goto error2;
  }
  
  s->tileset = rl_tileset_create( city_tls_tls, city_tls_tls_len );
  
  if ( !s->tileset )
  {
  error4:
    rl_sound_destroy( s->sound );
    goto error3;
  }
  
  s->imageset = rl_imageset_create( city_ims_ims, city_ims_ims_len );
  
  if ( !s->imageset )
  {
  error5:
    rl_tileset_destroy( s->tileset );
    goto error4;
  }
  
  s->map = rl_map_create( city_map_map, city_map_map_len, s->tileset, s->imageset );
  
  if ( !s->map )
  {
  error6:
    rl_imageset_destroy( s->imageset );
    goto error5;
  }
  
  if ( rl_backgrnd_create( WIDTH, HEIGHT ) )
  {
  error7:
    rl_map_destroy( s->map );
    goto error6;
  }
  
  rl_srand( perf_cb.get_time_usec() );
  
  s->reset = 0;
  rl_sound_play_ogg( sketch008_ogg, sketch008_ogg_len, 1, NULL );
  
  return 0;
}

static void testscr_destroy( testscr_t* s )
{
  rl_backgrnd_destroy();
  rl_map_destroy( s->map );
  rl_imageset_destroy( s->imageset );
  rl_tileset_destroy( s->tileset );
  rl_sound_destroy( s->sound );
  rl_sprite_destroy( s->sprite2 );
  rl_sprite_destroy( s->sprite );
  rl_image_destroy( s->block );
  rl_image_destroy( s->image );
  rl_sound_done();
}

static void testscr_update( testscr_t* s )
{
  if ( s->reset-- == 0 )
  {
    rl_backgrnd_clear( 0 );
    
    //s->reset = WIDTH * 6;
    s->count = 4;
    // s->xx = rl_random( 0, WIDTH - 1 );
    // s->yy = rl_random( 0, HEIGHT - 1 );
    // s->dx = ( rl_rand() & 1 ) * 2 - 1;
    // s->dy = ( rl_rand() & 1 ) * 2 - 1;
    s->xx = 123.0f;
    s->yy = 73.0f;
    s->dx = 0.25f;
    s->dy = -0.25f;
  }
  
  int width, height;
  uint16_t* fb = rl_backgrnd_fb( &width, &height );
  
  s->xx += s->dx;
  s->yy += s->dy;
  
  if ( s->xx < 0 || s->xx >= width )
  {
    s->dx = -s->dx;
    s->xx += s->dx;
    rl_sound_play( s->sound, 0, NULL );
  }
  
  if ( s->yy < 0 || s->yy >= height )
  {
    s->dy = -s->dy;
    s->yy += s->dy;
    rl_sound_play( s->sound, 0, NULL );
  }
  
  s->sprite->x = s->xx - s->image->width / 2;
  s->sprite->y = s->yy - s->image->height / 2;
  
  s->sprite2->x = ( width - s->xx ) - s->image->width / 2;
  s->sprite2->y = ( height - s->yy ) - s->image->height / 2;
    
  if ( 0 && --s->count == 0 )
  {
    int pitch = width + RL_BACKGRND_MARGIN;
    int i;
    
    s->count = 4;
    
    rl_backgrnd_scroll( -s->dx, -s->dy );
    
    if ( s->dy == 1 )
    {
      for ( i = 0; i < width; i++ )
      {
        fb[ i ] = 0;
      }
    }
    else
    {
      for ( i = 0; i < width; i++ )
      {
        fb[ ( height - 1 ) * pitch + i ] = 0;
      }
    }
    
    if ( s->dx == 1 )
    {
      for ( i = 0; i < height; i++ )
      {
        fb[ i * pitch ] = 0;
      }
    }
    else
    {
      for ( i = 0; i < height; i++ )
      {
        fb[ i * pitch + width - 1 ] = 0;
      }
    }
    
    fb[ ( height / 2 ) * pitch + width / 2 ] = ( rl_rand() & 0x0f ) << 12 | ( rl_rand() & 0x1f ) << 6 | ( rl_rand() & 0x0f ) << 1;
  }
  else
  {
    int map_w = s->map->width * s->map->tileset->width;
    int map_h = s->map->height * s->map->tileset->height;
    
    int max_x = map_w - width - s->map->tileset->width;
    int max_y = map_h - height - s->map->tileset->height;
    
    int x = s->xx * max_x / width;
    int y = s->yy * max_y / height;
    
    rl_map_blit0_nobg( s->map, x, y );
    rl_sprites_blit_nobg();
    rl_map_blitn_nobg( s->map, 1, x, y );
    
    {
      int bg_width, bg_height;
      rl_backgrnd_fb( &bg_width, &bg_height );
      
      int ts_width  = s->map->tileset->width;
      int ts_height = s->map->tileset->height;
      
      int dx = -( x % ts_width );
      int dy = -( y % ts_height );
      
      int max_x = dx + bg_width + ts_width;
      int max_y = dy + bg_height + ts_height;
      
      x /= ts_width;
      y /= ts_height;
      
      int pitch = s->map->width;
      int ndx = y * pitch + x;
      
      for ( y = dy; y < max_y; y += ts_height )
      {
        int next = ndx + pitch;
        
        for ( x = dx; x < max_x; x += ts_width, ndx++ )
        {
          if ( s->map->collision[ ndx / 32 ] & ( 1 << ( ndx & 31 ) ) )
          {
            rl_image_blit_nobg( s->block, x, y );
          }
        }
        
        ndx = next;
      }
    }
  }
}

static testscr_t testscr;

/*---------------------------------------------------------------------------*/

void retro_get_system_info( struct retro_system_info* info )
{
  info->library_name = "Retro Luxury";
  info->library_version = "1.0";
  info->need_fullpath = false;
  info->block_extract = false;
  info->valid_extensions = "any";
}

void retro_set_environment( retro_environment_t cb )
{
  env_cb = cb;
  
  static const struct retro_variable vars[] = {
    { NULL, NULL },
  };
  
  static const struct retro_controller_description controllers[] = {
    { "Controller", RETRO_DEVICE_JOYPAD },
    { NULL, 0 }
  };
  
  static const struct retro_controller_info ports[] = {
    { controllers, 1 },
    { NULL, 0 }
  };
  
  cb( RETRO_ENVIRONMENT_SET_VARIABLES, (void*)vars );
  cb( RETRO_ENVIRONMENT_SET_CONTROLLER_INFO, (void*)ports );
  
  bool t = true;
  cb( RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, (void*)&t );
}

unsigned retro_api_version()
{
  return RETRO_API_VERSION;
}

void retro_init()
{
  struct retro_log_callback log;
  
  if ( env_cb( RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &log ) )
  {
    log_cb = log.log;
  }
  
  if ( !env_cb( RETRO_ENVIRONMENT_GET_PERF_INTERFACE, &perf_cb ) )
  {
    perf_cb.get_time_usec = NULL;
    log_cb( RETRO_LOG_WARN, "Could not get the perf interface\n" );
  }
}

void* constcast( const void* ptr );
extern const char* rl_gitstamp;

bool retro_load_game( const struct retro_game_info* info )
{
  log_cb( RETRO_LOG_ERROR, "\n%s", rl_gitstamp );
  
  if ( !perf_cb.get_time_usec )
  {
    log_cb( RETRO_LOG_ERROR, "Core needs the perf interface\n" );
    return false;
  }
  
  enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_RGB565;
  
  if ( !env_cb( RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt ) )
  {
    log_cb( RETRO_LOG_ERROR, "RGB565 is not supported\n" );
    return false;
  }
  
  env_cb( RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS, input_descriptors );
  memset( input_state, 0, sizeof( input_state ) );
  
  if ( testscr_init( &testscr ) )
  {
    log_cb( RETRO_LOG_ERROR, "Error initializing game" );
    return false;
  }
  
  return true;
}

size_t retro_get_memory_size( unsigned id )
{
  (void)id;
  return 0;
}

void* retro_get_memory_data( unsigned id )
{
  (void)id;
  return NULL;
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
  info->geometry.base_width = WIDTH;
  info->geometry.base_height = HEIGHT;
  info->geometry.max_width = WIDTH;
  info->geometry.max_height = HEIGHT;
  info->geometry.aspect_ratio = 0.0f;
  info->timing.fps = 60.0;
  info->timing.sample_rate = 44100.0;
}

void retro_run()
{
  input_poll_cb();
  
  testscr_update( &testscr );
  
  int width, height;
  uint16_t* fb = rl_backgrnd_fb( &width, &height );
  
  //rl_sprites_begin();
  video_cb( fb, width, height, ( width + RL_BACKGRND_MARGIN ) * sizeof( uint16_t ) );
  //rl_sprites_end();
  
  audio_cb( rl_sound_mix(), RL_SAMPLES_PER_FRAME );
}

void retro_deinit()
{
#ifdef LOG_PERFORMANCE
  perf_cb.perf_log();
#endif
}

void retro_set_controller_port_device( unsigned port, unsigned device )
{
  switch ( device )
  {
  default:
    device = RETRO_DEVICE_JOYPAD;
    // fallthrough
    
  case RETRO_DEVICE_JOYPAD:
    input_devices[ port ] = device;
    break;
  }
}

void retro_reset()
{
}

size_t retro_serialize_size()
{
  return 0;
}

bool retro_serialize( void* data, size_t size )
{
  (void)data;
  (void)size;
  return false;
}

bool retro_unserialize( const void* data, size_t size )
{
  (void)data;
  (void)size;
  return false;
}

void retro_cheat_reset()
{
}

void retro_cheat_set( unsigned a, bool b, const char* c )
{
  (void)a;
  (void)b;
  (void)c;
}

bool retro_load_game_special(unsigned a, const struct retro_game_info* b, size_t c)
{
  (void)a;
  (void)b;
  (void)c;
  return false;
}

void retro_unload_game()
{
  testscr_destroy( &testscr );
}

unsigned retro_get_region()
{
  return RETRO_REGION_NTSC;
}
