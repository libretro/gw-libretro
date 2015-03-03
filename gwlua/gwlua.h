#ifndef GWLUA_H
#define GWLUA_H

#include <stddef.h>
#include <stdint.h>

/*---------------------------------------------------------------------------*/
/* Registry value types for gwlua_(load|save)_value */

#define GWLUA_NULL    0
#define GWLUA_BOOLEAN 1
#define GWLUA_NUMBER  2
#define GWLUA_STRING  3

/*---------------------------------------------------------------------------*/
/* Controller buttons */

#define GWLUA_UP     1
#define GWLUA_DOWN   2
#define GWLUA_LEFT   3
#define GWLUA_RIGHT  4
#define GWLUA_A      5
#define GWLUA_B      6
#define GWLUA_X      7
#define GWLUA_Y      8
#define GWLUA_L1     9
#define GWLUA_R1     10
#define GWLUA_L2     11
#define GWLUA_R2     12
#define GWLUA_L3     13
#define GWLUA_R3     14
#define GWLUA_SELECT 15
#define GWLUA_START  16

/*---------------------------------------------------------------------------*/
/*
define these structures the way you need to properly handle states, pictures
and sound
*/

#include <gwrom.h>

typedef struct gwlua_picture_t gwlua_picture_t;
typedef struct gwlua_sound_t gwlua_sound_t;
typedef struct gwlua_state_t gwlua_state_t;

struct gwlua_picture_t
{
  unsigned width, height, pitch;
  uint16_t* pixels;
  const gwlua_picture_t* parent;
};

struct gwlua_sound_t
{
  int16_t* data;
  size_t size;
};

struct gwlua_state_t
{
  gwrom_t gwrom;
  const gwlua_picture_t* bg;
  gwlua_picture_t screen;
  int first_frame;
  int updated;
  
  const gwlua_sound_t* playing;
  size_t position;
  int16_t sound[ 735 * 2 ];
};

/*---------------------------------------------------------------------------*/
/* user-provided functions (must be defined elsewhere) */

/* memory management */
void* gwlua_malloc( size_t size );
void  gwlua_free( void* pointer );
void* gwlua_realloc( void* pointer, size_t size );

/* picture */
int  gwlua_load_picture( gwlua_state_t* state, gwlua_picture_t* picture, const char* name );
int  gwlua_sub_picture( gwlua_state_t* state, gwlua_picture_t* picture, const gwlua_picture_t* parent, int x0, int y0, unsigned width, unsigned height );
void gwlua_destroy_picture( gwlua_state_t* state, gwlua_picture_t* picture );
void gwlua_blit_picture( gwlua_state_t* state, const gwlua_picture_t* picture, int x, int y );
void gwlua_unblit_picture( gwlua_state_t* state, const gwlua_picture_t* picture, int x, int y );

/* sound */
int  gwlua_load_sound( gwlua_state_t* state, gwlua_sound_t* sound, const char* name );
void gwlua_destroy_sound( gwlua_state_t* state, gwlua_sound_t* sound );
void gwlua_play_sound( gwlua_state_t* state, const gwlua_sound_t* sound );
void gwlua_stop_all_sounds( gwlua_state_t* state );

/* registry */
const char* gwlua_load_value( gwlua_state_t* state, const char* key, int* type );
void        gwlua_save_value( gwlua_state_t* state, const char* key, const char* value, int type );

/* control */
int gwlua_set_bg( gwlua_state_t* state, const gwlua_picture_t* bg );

/*---------------------------------------------------------------------------*/
/* api */

int  gwlua_create( gwlua_state_t* state, const void* main, size_t size );
void gwlua_destroy( gwlua_state_t* state );
void gwlua_reset( gwlua_state_t* state );

void gwlua_tick( gwlua_state_t* state, int64_t now );
void gwlua_button_down( gwlua_state_t* state, unsigned controller_ndx, int button );
void gwlua_button_up( gwlua_state_t* state, unsigned controller_ndx, int button );

#endif /* GWLUA_H */
