#ifndef GWLUA_H
#define GWLUA_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <lua.h>

#include <gwrom.h>

#include <rl_image.h>
#include <rl_sprite.h>
#include <rl_sound.h>

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

typedef struct gwlua_t gwlua_t;

typedef struct
{
  gwlua_t* state;
  
  int64_t interval;
  int64_t expiration;
  int     is_enabled;
  int     callback_ref;
}
gwlua_timer_t;

struct gwlua_t
{
  /* contents */
  gwrom_t* rom;
  
  /* screen */
  int width, height;
  uint16_t* screen;
  
  /* control */
  lua_State* L;
  int help;
  int64_t now;
  
  /* input */
  char input[ GWLUA_START + 1 ];
  
  /* references */
  int tick_ref;
};

/*---------------------------------------------------------------------------*/
/* user-provided functions (must be defined elsewhere) */

/* memory management */
void* gwlua_malloc( size_t size );
void  gwlua_free( void* pointer );
void* gwlua_realloc( void* pointer, size_t size );

/* registry */
const char* gwlua_load_value( gwlua_t* state, const char* key, int* type );
void        gwlua_save_value( gwlua_t* state, const char* key, const char* value, int type );

/* control */
int  gwlua_set_fb( unsigned width, unsigned height );
void gwlua_zoom( gwlua_t* state, int x0, int y0, int width, int height );

/* log */
void gwlua_vlog( const char* format, va_list args );

/*---------------------------------------------------------------------------*/
/* api */

int  gwlua_create( gwlua_t* state, gwrom_t* rom, int64_t now );
void gwlua_destroy( gwlua_t* state );
int  gwlua_reset( gwlua_t* state );

void gwlua_set_button( gwlua_t* state, int button, int pressed );
void gwlua_tick( gwlua_t* state, int64_t now );

void gwlua_ref_create( lua_State* L, int index, int* ref );
void gwlua_ref_destroy( lua_State* L, int* ref );
void gwlua_ref_new( lua_State* L, int index, int* ref );
void gwlua_ref_get( lua_State* L, int ref );

uint32_t gwlua_djb2( const char* str );
void     gwlua_log( const char* format, ... );

#endif /* GWLUA_H */
