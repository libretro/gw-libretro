#include <rl_sprite.h>
#include <rl_memory.h>
#include <rl_config.h>

#include <stdlib.h>

typedef struct
{
  rl_sprite_t* sprite;
  uint16_t*    bg;
}
spt_t;

static spt_t     sprites[ RL_MAX_SPRITES + 1 ];
static uint16_t  saved_backgrnd[ RL_BG_SAVE_SIZE ];
static uint16_t* saved_ptr;
static int       num_sprites, num_visible;
static int       x0, y0;

void rl_sprite_init( void )
{
  num_sprites = num_visible = 0;
  x0 = y0 = 0;
}

rl_sprite_t* rl_sprite_create( void )
{
  if ( num_sprites < RL_MAX_SPRITES )
  {
    rl_sprite_t* sprite = (rl_sprite_t*)rl_malloc( sizeof( rl_sprite_t ) );
    
    if ( sprite )
    {
      sprites[ num_sprites++ ].sprite = sprite;
      
      sprite->layer = sprite->flags = 0;
      sprite->x = sprite->y = 0;
      sprite->image = NULL;
      
      return sprite;
    }
  }
  
  return NULL;
}

void rl_sprites_translate( int x, int y )
{
  x0 = x;
  y0 = y;
}

static int compare( const void* e1, const void* e2 )
{
  const spt_t* s1 = (const spt_t*)e1;
  const spt_t* s2 = (const spt_t*)e2;
  
  int32_t c1 = s1->sprite->flags;
  int32_t c2 = s2->sprite->flags;
  
  c1 = c1 << 16 | s1->sprite->layer;
  c2 = c2 << 16 | s2->sprite->layer;
  
  return c1 - c2;
}

void rl_sprites_render( void )
{
  spt_t* sptptr = sprites;
  const spt_t* endptr = sprites + num_sprites;
  
  if ( sptptr < endptr )
  {
    do
    {
      sptptr->sprite->flags &= ~RL_SPRITE_TEMP_INV;
      sptptr->sprite->flags |= sptptr->sprite->image == NULL;
      sptptr++;
    }
    while ( sptptr < endptr );
  }
  
  qsort( (void*)sprites, num_sprites, sizeof( spt_t ), compare );
  
  rl_sprite_t guard;
  guard.flags = RL_SPRITE_UNUSED;
  sprites[ num_sprites ].sprite = &guard; /* guard */
  
  sptptr = sprites;
  
  /* Iterate over active and visible sprites and blit them */
  /* flags & 0x0007U == 0 */
  if ( sptptr->sprite->flags == 0 )
  {
    do
    {
      rl_image_blit_nobg( sptptr->sprite->image, x0 + sptptr->sprite->x, y0 + sptptr->sprite->y );
      sptptr++;
    }
    while ( sptptr->sprite->flags == 0 );
  }
  
  num_visible = sptptr - sprites;
  
  /* Jump over active but invisible sprites */
  /* flags & 0x0004U == 0x0000U */
  if ( ( sptptr->sprite->flags & RL_SPRITE_UNUSED ) == 0 )
  {
    do
    {
      sptptr++;
    }
    while ( ( sptptr->sprite->flags & RL_SPRITE_UNUSED ) == 0 );
  }
  
  num_sprites = sptptr - sprites;
}

void rl_sprites_begin( void )
{
  spt_t* sptptr = sprites;
  const spt_t* endptr = sprites + num_sprites;
  
  if ( sptptr < endptr )
  {
    do
    {
      sptptr->sprite->flags &= ~RL_SPRITE_TEMP_INV;
      sptptr->sprite->flags |= sptptr->sprite->image == NULL;
      sptptr++;
    }
    while ( sptptr < endptr );
  }
  
  qsort( (void*)sprites, num_sprites, sizeof( spt_t ), compare );
  
  rl_sprite_t guard;
  guard.flags = RL_SPRITE_UNUSED;
  sprites[ num_sprites ].sprite = &guard; /* guard */
  
  sptptr    = sprites;
  saved_ptr = saved_backgrnd;
  
  /* Iterate over active and visible sprites and blit them */
  /* flags & 0x0007U == 0 */
  if ( sptptr->sprite->flags == 0 )
  {
    do
    {
      sptptr->bg = saved_ptr;
      saved_ptr = rl_image_blit( sptptr->sprite->image, x0 + sptptr->sprite->x, y0 + sptptr->sprite->y, saved_ptr );
      sptptr++;
    }
    while ( sptptr->sprite->flags == 0 );
  }
  
  num_visible = sptptr - sprites;
  
  /* Jump over active but invisible sprites */
  /* flags & 0x0004U == 0x0000U */
  if ( ( sptptr->sprite->flags & RL_SPRITE_UNUSED ) == 0 )
  {
    do
    {
      sptptr++;
    }
    while ( ( sptptr->sprite->flags & RL_SPRITE_UNUSED ) == 0 );
  }
  
  num_sprites = sptptr - sprites;
}

void rl_sprites_end( void )
{
  spt_t* sptptr = sprites + num_visible - 1;
  
  /* Unblit the sprites in reverse order */
  if ( sptptr >= sprites )
  {
    do
    {
      rl_image_unblit( sptptr->sprite->image, x0 + sptptr->sprite->x, y0 + sptptr->sprite->y, sptptr->bg );
      sptptr--;
    }
    while ( sptptr >= sprites );
  }
}
