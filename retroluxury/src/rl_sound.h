#ifndef RL_SOUND_H
#define RL_SOUND_H

#include <rl_userdata.h>

#include <stdint.h>
#include <stddef.h>

/* Number of 16-bit stereo samples per frame. DO NOT CHANGE! */
#define RL_SAMPLES_PER_FRAME 735

typedef struct
{
  rl_userdata_t ud;
  int           samples;
  int           stereo;
  const int16_t pcm[ 0 ];
}
rl_sound_t;

typedef void ( *rl_soundstop_t )( const rl_sound_t* );

void rl_sound_init( void );
void rl_sound_done( void );

rl_sound_t* rl_sound_create( const void* data, size_t size, int stereo );
#define     rl_sound_destroy( sound ) do { rl_free( sound ); } while ( 0 )

int  rl_sound_play( const rl_sound_t* sound, int repeat, rl_soundstop_t stop_cb );
void rl_sound_stop( int index );

void rl_sound_stop_all( void );

int  rl_sound_play_ogg( const void* data, size_t size, int repeat, rl_soundstop_t stop_cb );
void rl_sound_stop_ogg( void );

const int16_t* rl_sound_mix( void );

#endif /* RL_SOUND_H */
