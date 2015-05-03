#include <rl_sound.h>
#include <rl_memory.h>
#include <rl_config.h>

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <rl_endian.c>

/*---------------------------------------------------------------------------*/
#ifdef RL_OGG_VORBIS

// #define STB_VORBIS_NO_CRT
#define STB_VORBIS_NO_STDIO
#define STB_VORBIS_NO_PUSHDATA_API

// #define assert( x )
// #define malloc  rl_malloc
// #define realloc rl_realloc
// #define free    rl_free
// #define pow     pow
// #define floor   floor
#define alloca( a ) 0

#include <stb_vorbis.c>

#endif
/*---------------------------------------------------------------------------*/

typedef struct
{
  const rl_sound_t* sound;
  rl_soundstop_t    stop_cb;
  
  int position;
  int repeat;
}
voice_t;

static int16_t audio_buffer[ RL_SAMPLES_PER_FRAME * 2 ];
static voice_t voices[ RL_MAX_VOICES ];

#ifdef RL_OGG_VORBIS
static stb_vorbis*      ogg_stream;
static stb_vorbis_alloc ogg_alloc;
static int16_t          ogg_pcm[ 4096 ];
static int              ogg_position;
static int              ogg_available;
static int              ogg_repeat;
static rl_soundstop_t   ogg_stop_cb;
#endif

void rl_sound_init( void )
{
  for ( int i = 0; i < RL_MAX_VOICES; i++ )
  {
    voices[ i ].sound = NULL;
  }
  
#ifdef RL_OGG_VORBIS
  ogg_stream = NULL;
#endif
}

void rl_sound_done( void )
{
#ifdef RL_OGG_VORBIS
  if ( ogg_stream )
  {
    stb_vorbis_close( ogg_stream );
    rl_free( ogg_alloc.alloc_buffer );
  }
#endif
}

rl_sound_t* rl_sound_create( const void* data, size_t size, int stereo )
{
  union
  {
    const void*     restrict v;
    const uint16_t* restrict u16;
  }
  ptr;
  
  rl_sound_t* sound = (rl_sound_t*)rl_malloc( sizeof( rl_sound_t ) + size );
  
  if ( sound )
  {
    size /= 2;
    
    sound->samples = size;
    sound->stereo  = stereo;
    
    uint16_t* restrict pcm = (uint16_t*)( (uint8_t*)sound + sizeof( *sound ) );
    ptr.v = data;
    
    const uint16_t* restrict end = pcm + size;
    
    while ( pcm < end )
    {
      *pcm++ = ne16( *ptr.u16++ );
    }
    
    return sound;
  }
  
  return NULL;
}

int rl_sound_play( const rl_sound_t* sound, int repeat, rl_soundstop_t stop_cb )
{
  voice_t* restrict voice = voices;
  const voice_t* restrict end = voices + RL_MAX_VOICES;
  
  do
  {
    if ( !voice->sound )
    {
      voice->sound    = sound;
      voice->stop_cb  = stop_cb;
      voice->position = 0;
      voice->repeat   = repeat;
      
      return voice - voices;
    }
    
    voice++;
  }
  while ( voice < end );
  
  return -1;
}

void rl_sound_stop( int index )
{
  voice_t* voice = voices + index;
  
  if ( voice->stop_cb && voice->sound )
  {
    voice->stop_cb( voice->sound );
  }
  
  voice->sound = NULL;
}

void rl_sound_stop_all( void )
{
  voice_t* restrict voice = voices;
  const voice_t* restrict end = voices + RL_MAX_VOICES;
  
  while ( voice < end )
  {
    if ( voice->stop_cb && voice->sound )
    {
      voice->stop_cb( voice->sound );
    }
    
    voice->sound = NULL;
    voice++;
  }
}

#ifdef RL_OGG_VORBIS
int rl_sound_play_ogg( const void* data, size_t size, int repeat, rl_soundstop_t stop_cb )
{
  if ( !ogg_stream )
  {
    // This scheme is failing with a SIGSEGV when the buffer size reaches 128Kb
    // so we just allocate 256Kb for now which seems to work.
    
    /*
    ogg_alloc.alloc_buffer = NULL;
    ogg_alloc.alloc_buffer_length_in_bytes = 0;
    
    int res;
    
    do
    {
      ogg_alloc.alloc_buffer_length_in_bytes += RL_OGG_INCREMENT;
      
      void* new_buffer = rl_realloc( ogg_alloc.alloc_buffer, ogg_alloc.alloc_buffer_length_in_bytes );
      
      if ( !new_buffer )
      {
        rl_free( ogg_alloc.alloc_buffer );
        return -1;
      }
      
      ogg_alloc.alloc_buffer = (char*)new_buffer;
      ogg_stream = stb_vorbis_open_memory( (const unsigned char*)data, size, &res, &ogg_alloc );
    }
    while ( !ogg_stream && res == VORBIS_outofmem );
    */
    
    ogg_alloc.alloc_buffer = (char*)rl_malloc( 256 * 1024 );
    
    if ( ogg_alloc.alloc_buffer )
    {
      ogg_alloc.alloc_buffer_length_in_bytes = 256 * 1024;
      
      int res;
      ogg_stream = stb_vorbis_open_memory( (const unsigned char*)data, size, &res, &ogg_alloc );
      
      if ( ogg_stream )
      {
        ogg_position = ogg_available = 0;
        ogg_repeat = repeat;
        ogg_stop_cb = stop_cb;
        return 0;
      }
    
      rl_free( ogg_alloc.alloc_buffer );
    }
  }
  
  ogg_stream = NULL;
  return -1;
}

void rl_sound_stop_ogg( void )
{
  if ( ogg_stream )
  {
    stb_vorbis_close( ogg_stream );
    rl_free( ogg_alloc.alloc_buffer );
    
    ogg_stream = NULL;
  }
}

static void ogg_mix( int32_t* buffer )
{
  if ( ogg_stream )
  {
    int buf_free = RL_SAMPLES_PER_FRAME * 2;
    
    if ( ogg_position == ogg_available )
    {
    again:
      ogg_available = stb_vorbis_get_frame_short_interleaved( ogg_stream, 2, ogg_pcm, sizeof( ogg_pcm ) / sizeof( ogg_pcm[ 0 ] ) );
      
      if ( !ogg_available )
      {
        if ( ogg_repeat )
        {
          stb_vorbis_seek_start( ogg_stream );
          goto again;
        }
        else
        {
          if ( ogg_stop_cb )
          {
            ogg_stop_cb( NULL );
          }
          
          stb_vorbis_close( ogg_stream );
          ogg_stream = NULL;
          return;
        }
      }
      
      ogg_available *= 2;
      ogg_position   = 0;
    }
    
    const int16_t* pcm = ogg_pcm + ogg_position;
    
    if ( ogg_available < buf_free )
    {
      for ( int i = ogg_available; i != 0; --i )
      {
        *buffer++ += *pcm++;
      }
      
      buf_free -= ogg_available;
      goto again;
    }
    else
    {
      for ( int i = buf_free; i != 0; --i )
      {
        *buffer++ += *pcm++;
      }
      
      ogg_position  += buf_free;
      ogg_available -= buf_free;
    }
  }
}
#endif /* RL_OGG_VORBIS */

static void mix( int32_t* buffer, voice_t* voice )
{
  int buf_free = RL_SAMPLES_PER_FRAME * 2;
  const rl_sound_t* sound = voice->sound;
  
again:
  if ( sound->stereo )
  {
    int pcm_available = sound->samples - voice->position;
    const int16_t* pcm = sound->pcm + voice->position;
    
    if ( pcm_available < buf_free )
    {
      for ( int i = pcm_available; i != 0; --i )
      {
        *buffer++ += *pcm++;
      }
      
      if ( voice->repeat )
      {
        buf_free -= pcm_available;
        voice->position = 0;
        goto again;
      }
      
      if ( voice->stop_cb )
      {
        voice->stop_cb( voice->sound );
      }
      
      voice->sound = NULL;
    }
    else
    {
      for ( int i = buf_free; i != 0; --i )
      {
        *buffer++ += *pcm++;
      }
      
      voice->position += buf_free;
    }
  }
  else
  {
    int pcm_available = sound->samples - voice->position;
    const int16_t* pcm = sound->pcm + voice->position;
    
    buf_free /= 2;
    
    if ( pcm_available < buf_free )
    {
      for ( int i = pcm_available; i != 0; --i )
      {
        *buffer++ += *pcm;
        *buffer++ += *pcm++;
      }
      
      if ( voice->repeat )
      {
        buf_free -= pcm_available;
        voice->position = 0;
        goto again;
      }
      
      if ( voice->stop_cb )
      {
        voice->stop_cb( voice->sound );
      }
      
      voice->sound = NULL;
    }
    else
    {
      for ( int i = buf_free; i != 0; --i )
      {
        *buffer++ += *pcm;
        *buffer++ += *pcm++;
      }
      
      voice->position += buf_free;
    }
  }
}

const int16_t* rl_sound_mix( void )
{
  int32_t buffer[ RL_SAMPLES_PER_FRAME * 2 ];
  memset( buffer, 0, sizeof( buffer ) );
  
  voice_t* restrict voice = voices;
  const voice_t* restrict end   = voices + RL_MAX_VOICES;
  
  do
  {
    if ( voice->sound )
    {
      mix( buffer, voice );
    }
    
    voice++;
  }
  while ( voice < end );
  
#ifdef RL_OGG_VORBIS
  ogg_mix( buffer );
#endif
  
  for ( int i = 0; i < RL_SAMPLES_PER_FRAME * 2; i++ )
  {
    int32_t sample = buffer[ i ];
    audio_buffer[ i ] = sample < -32768 ? -32768 : sample > 32767 ? 32767 : sample;
  }
  
  return audio_buffer;
}
