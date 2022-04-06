#include <rl_tile.h>
#include <rl_memory.h>
#include <rl_backgrnd.h>

#include <string.h>

#include <rl_endian.c>

rl_tileset_t* rl_tileset_create( const void* data, size_t size )
{
  union
  {
    const void*     restrict v;
    const uint16_t* restrict u16;
  }
  ptr;
  uint16_t* restrict pixel;
  const uint16_t* restrict end;
  rl_tileset_t* tileset;
  int width;
  int height;
  int num_tiles;
  
  ptr.v = data;
  
  width     = ne16( *ptr.u16++ );
  height    = ne16( *ptr.u16++ );
  num_tiles = ne16( *ptr.u16++ );
  
  size -= 3 * sizeof( uint16_t );
  
  tileset = (rl_tileset_t*)rl_malloc( sizeof( rl_tileset_t ) + size );
  
  if ( tileset )
  {
    tileset->width     = width;
    tileset->height    = height;
    tileset->size      = width * height;
    tileset->num_tiles = num_tiles;
    
    pixel = (uint16_t*)( (uint8_t*)tileset + sizeof( rl_tileset_t ) );
    
    end = pixel + size / 2;
    
    while ( pixel < end )
    {
      *pixel++ = ne16( *ptr.u16++ );
    }
    
    return tileset;
  }
  
  return NULL;
}

void rl_tileset_blit_nobg( const rl_tileset_t* tileset, int index, int x, int y )
{
  int width  = tileset->width;
  int height = tileset->height;
  int size   = tileset->size;
  
  const uint16_t* pixels = tileset->data + size * index;
  rl_tile_blit_nobg( width, height, pixels, x, y );
}

uint16_t* rl_tileset_blit( const rl_tileset_t* tileset, int index, int x, int y, uint16_t* bg )
{
  int width  = tileset->width;
  int height = tileset->height;
  int size   = tileset->size;
  
  const uint16_t* pixels = tileset->data + size * index;
  return rl_tile_blit( width, height, pixels, x, y, bg );
}

void rl_tileset_unblit( const rl_tileset_t* tileset, int x, int y, const uint16_t* bg )
{
  int width  = tileset->width;
  int height = tileset->height;
  
  rl_tile_unblit( width, height, x, y, bg );
}

void rl_tile_blit_nobg( int width, int height, const uint16_t* pixels, int x, int y )
{
  int d_width, d_height;
  uint16_t* restrict dest = rl_backgrnd_fb( &d_width, &d_height );
  
  const uint16_t* restrict source = pixels;
  
  int pitch   = width;
  int d_pitch = d_width + RL_BACKGRND_MARGIN;
  
  if ( x < 0 )
  {
    width  += x;
    source -= x;
    x = 0;
  }
  
  if ( x + width > d_width )
  {
    int out = x + width - d_width;
    width -= out;
  }
  
  if ( y < 0 )
  {
    height += y;
    source -= y * pitch;
    y = 0;
  }
  
  if ( y + height > d_height )
  {
    int out = y + height - d_height;
    height -= out;
  }
  
  if ( width > 0 && height > 0 )
  {
    dest  += y * d_pitch + x;
    width *= 2;
    
    for ( int y = height; y > 0; --y )
    {
      memcpy( (void*)dest, (void*)source, width );
      
      source += pitch;
      dest   += d_pitch;
    }
  }
}

uint16_t* rl_tile_blit( int width, int height, const uint16_t* pixels, int x, int y, uint16_t* bg )
{
  int d_width, d_height;
  uint16_t* restrict dest = rl_backgrnd_fb( &d_width, &d_height );
  
  const uint16_t* restrict source = pixels;
  
  int pitch   = width;
  int d_pitch = d_width + RL_BACKGRND_MARGIN;
  
  if ( x < 0 )
  {
    width  += x;
    source -= x;
    x = 0;
  }
  
  if ( x + width > d_width )
  {
    int out = x + width - d_width;
    width -= out;
  }
  
  if ( y < 0 )
  {
    height += y;
    source -= y * pitch;
    y = 0;
  }
  
  if ( y + height > d_height )
  {
    int out = y + height - d_height;
    height -= out;
  }
  
  if ( width > 0 && height > 0 )
  {
    dest += y * d_pitch + x;
    
    for ( int y = height; y > 0; --y )
    {
      memcpy( (void*)bg, (void*)dest, width * 2 );
      memcpy( (void*)dest, (void*)source, width * 2 );
      
      source += pitch;
      dest   += d_pitch;
    }
  }
  
  return bg;
}

void rl_tile_unblit( int width, int height, int x, int y, const uint16_t* bg )
{
  int d_width, d_height;
  uint16_t* restrict dest = rl_backgrnd_fb( &d_width, &d_height );
  
  int pitch   = width;
  int d_pitch = d_width + RL_BACKGRND_MARGIN;
  
  if ( x < 0 )
  {
    width += x;
    x = 0;
  }
  
  if ( x + width > d_width )
  {
    int out = x + width - d_width;
    width -= out;
  }
  
  if ( y < 0 )
  {
    height += y;
    y = 0;
  }
  
  if ( y + height > d_height )
  {
    int out = y + height - d_height;
    height -= out;
  }
  
  if ( width > 0 && height > 0 )
  {
    dest  += y * d_pitch + x;
    width *= 2;
    
    for ( int y = height; y > 0; --y )
    {
      memcpy( (void*)dest, (void*)bg, width );
      
      dest += d_pitch;
      bg   += width;
    }
  }
}
