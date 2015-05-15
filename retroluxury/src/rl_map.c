#include <rl_map.h>
#include <rl_memory.h>
#include <rl_backgrnd.h>

#include <string.h>

#include <rl_endian.c>

static rl_map_t* destroy( const rl_map_t* map )
{
  if ( map )
  {
    for ( int i = map->num_layers - 1; i >= 0; --i )
    {
      rl_free( (void*)map->layers[ i ] );
    }
    
    if ( map->layer0 )
    {
      rl_free( (void*)map->layer0 );
    }
    
    rl_free( (void*)map );
  }
  
  return NULL;
}

static void* alloc_zero( size_t size )
{
  void* ptr = rl_malloc( size );
  
  if ( ptr )
  {
    memset( ptr, 0, size );
  }
  
  return ptr;
}

rl_map_t* rl_map_create( const void* data, size_t size, const rl_tileset_t* tileset, const rl_imageset_t* imageset )
{
  union
  {
    const void*     restrict v;
    const uint8_t*  restrict u8;
    const uint16_t* restrict u16;
    const uint32_t* restrict u32;
  }
  ptr;
  
  ptr.v = data;
  
  int width      = ne16( *ptr.u16++ );
  int height     = ne16( *ptr.u16++ );
  int num_layers = ne16( *ptr.u16++ );
  int flags      = ne16( *ptr.u16++ );
  
  rl_map_t* map = (rl_map_t*)alloc_zero( sizeof( rl_map_t ) + ( num_layers - 1 ) * sizeof( rl_layern_t* ) );
  
  if ( !map )
  {
    return NULL;
  }
  
  map->width      = width;
  map->height     = height;
  map->num_layers = num_layers;
  map->flags      = flags;
  map->tileset    = tileset;
  map->imageset   = imageset;
  
  map->layer0 = (rl_layer0_t*)alloc_zero( width * height * sizeof( uint16_t ) );
  
  if ( !map->layer0 )
  {
    return destroy( map );
  }
  
  uint16_t* restrict ndx = (uint16_t*)( (uint8_t*)map->layer0 + sizeof( rl_layer0_t ) );
  const uint16_t* restrict end = ndx + width * height;
  
  while ( ndx < end )
  {
    *ndx++ = ne16( *ptr.u16++ );
  }
  
  for ( int i = 1; i < num_layers; i++ )
  {
    map->layers[ i - 1 ] = (rl_layern_t*)alloc_zero( width * height * sizeof( uint16_t ) );
    
    if ( !map->layers[ i - 1 ] )
    {
      return destroy( map );
    }
    
    ndx = (uint16_t*)( (uint8_t*)map->layers[ i - 1 ] + sizeof( rl_layern_t ) );
    end = ndx + width * height;
    
    while ( ndx < end )
    {
      *ndx++ = ne16( *ptr.u16++ );
    }
  }
  
  int numqw = ( width * height + 31 ) / 32;
  uint32_t* restrict collision = (uint32_t*)rl_malloc( numqw * sizeof( uint32_t ) );
  
  if ( !collision )
  {
    return destroy( map );
  }
  
  map->collision = collision;
  const uint32_t* restrict coll_end = collision + numqw;
  
  while ( collision < coll_end )
  {
    *collision++ = ne32( *ptr.u32++ );
  }
  
  return map;
}

void rl_map_destroy( const rl_map_t* map )
{
  destroy( map );
}

void rl_map_blit0_nobg( const rl_map_t* map, int x, int y )
{
  int bg_width, bg_height;
  rl_backgrnd_fb( &bg_width, &bg_height );
  
  const rl_tileset_t* tileset = map->tileset;
  
  int ts_width  = tileset->width;
  int ts_height = tileset->height;
  
  int dx = -( x % ts_width );
  int dy = -( y % ts_height );
  
  int max_x = dx + bg_width + ts_width;
  int max_y = dy + bg_height + ts_height;
  
  x /= ts_width;
  y /= ts_height;
  
  int pitch = map->width;
  const uint16_t* restrict ndx = map->layer0->indices + y * pitch + x;
  
  for ( y = dy; y < max_y; y += ts_height )
  {
    const uint16_t* restrict next = ndx + pitch;
    
    for ( x = dx; x < max_x; x += ts_width )
    {
      rl_tileset_blit_nobg( tileset, *ndx++, x, y );
    }
    
    ndx = next;
  }
}

void rl_map_blitn_nobg( const rl_map_t* map, int index, int x, int y )
{
  int bg_width, bg_height;
  rl_backgrnd_fb( &bg_width, &bg_height );
  
  const rl_imageset_t* imageset = map->imageset;
  
  int ts_width  = map->tileset->width;
  int ts_height = map->tileset->height;
  
  int dx = -( x % ts_width );
  int dy = -( y % ts_height );
  
  int max_x = dx + bg_width + ts_width;
  int max_y = dy + bg_height + ts_height;
  
  x /= ts_width;
  y /= ts_height;
  
  int pitch = map->width;
  const uint16_t* restrict ndx = map->layers[ --index ]->indices + y * pitch + x;
  
  for ( y = dy; y < max_y; y += ts_height )
  {
    const uint16_t* restrict next = ndx + pitch;
    
    for ( x = dx; x < max_x; x += ts_width )
    {
      index = *ndx++;
      
      if ( index )
      {
        rl_image_blit_nobg( imageset->images[ index - 1 ], x, y );
      }
    }
    
    ndx = next;
  }
}
