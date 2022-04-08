#include <rl_map.h>
#include <rl_memory.h>
#include <rl_backgrnd.h>

#include <string.h>

#include <rl_endian.c>

static rl_map_t* destroy( const rl_map_t* map )
{
  if ( map )
  {
    int i;
    for ( i = map->num_layers - 1; i >= 0; --i )
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
  int width;
  int height;
  int num_layers;
  int flags;
  rl_map_t* map;
  uint16_t* restrict ndx;
  const uint16_t* restrict end;
  int numqw;
  uint32_t* restrict collision;
  const uint32_t* restrict coll_end;
  int i;

  ptr.v = data;
  
  width      = ne16( *ptr.u16++ );
  height     = ne16( *ptr.u16++ );
  num_layers = ne16( *ptr.u16++ );
  flags      = ne16( *ptr.u16++ );
  
  map = (rl_map_t*)alloc_zero( sizeof( rl_map_t ) + ( num_layers - 1 ) * sizeof( rl_layern_t* ) );
  
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
  
  ndx = (uint16_t*)( (uint8_t*)map->layer0 + sizeof( rl_layer0_t ) );
  end = ndx + width * height;
  
  while ( ndx < end )
  {
    *ndx++ = ne16( *ptr.u16++ );
  }
  
  for ( i = 1; i < num_layers; i++ )
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
  
  numqw = ( width * height + 31 ) / 32;
  collision = (uint32_t*)rl_malloc( numqw * sizeof( uint32_t ) );
  
  if ( !collision )
  {
    return destroy( map );
  }
  
  map->collision = collision;
  coll_end = collision + numqw;
  
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
  const rl_tileset_t* tileset;
  
  int ts_width;
  int ts_height;
  
  int dx;
  int dy;
  
  int max_x;
  int max_y;

  int pitch;
  const uint16_t* restrict ndx;

  rl_backgrnd_fb( &bg_width, &bg_height );
  
  tileset = map->tileset;
  
  ts_width  = tileset->width;
  ts_height = tileset->height;
  
  dx = -( x % ts_width );
  dy = -( y % ts_height );
  
  max_x = dx + bg_width + ts_width;
  max_y = dy + bg_height + ts_height;
  
  x /= ts_width;
  y /= ts_height;
  
  pitch = map->width;
  ndx = map->layer0->indices + y * pitch + x;
  
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

  const rl_imageset_t* imageset;
  
  int ts_width;
  int ts_height;
  
  int dx;
  int dy;
  
  int max_x;
  int max_y;

  int pitch = map->width;
  const uint16_t* restrict ndx = map->layers[ --index ]->indices + y * pitch + x;

  rl_backgrnd_fb( &bg_width, &bg_height );
  
  imageset = map->imageset;
  
  ts_width  = map->tileset->width;
  ts_height = map->tileset->height;
  
  dx = -( x % ts_width );
  dy = -( y % ts_height );
  
  max_x = dx + bg_width + ts_width;
  max_y = dy + bg_height + ts_height;
  
  x /= ts_width;
  y /= ts_height;
  
  pitch = map->width;
  ndx = map->layers[ --index ]->indices + y * pitch + x;
  
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
