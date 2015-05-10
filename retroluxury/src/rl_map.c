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
    
    if ( map->imageset )
    {
      rl_imageset_destroy( (void*)map->imageset );
    }
    
    if ( map->tileset )
    {
      rl_tileset_destroy( (void*)map->tileset );
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

rl_map_t* rl_map_create( const void* data, size_t size )
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
  
  rl_map_t* map = (rl_map_t*)alloc_zero( sizeof( rl_map_t ) + ( num_layers - 1 ) * sizeof( rl_layern_t ) );
  
  if ( !map )
  {
    return NULL;
  }
  
  map->width = width;
  map->height = height;
  map->num_layers = num_layers;
  
  size_t tileset_size = ne32( *ptr.u32++ );
  map->tileset = rl_tileset_create( ptr.v, tileset_size );
  
  if ( !map->tileset )
  {
    return destroy( map );
  }
  
  ptr.u8 += tileset_size;
  
  size_t imageset_size = ne32( *ptr.u32++ );
  map->imageset = rl_imageset_create( ptr.v, imageset_size );
  
  if ( !map->imageset )
  {
    return destroy( map );
  }
  
  ptr.u8 += imageset_size;
  
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
  
  return map;
}

void rl_map_destroy( const rl_map_t* map )
{
  destroy( map );
}

static void render_layer0( const rl_map_t* map, int x, int y )
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

static void render_layern( const rl_map_t* map, int index, int x, int y )
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

void rl_map_render_layer( const rl_map_t* map, int index, int x, int y )
{
  if ( index )
  {
    render_layern( map, index, x, y );
  }
  else
  {
    render_layer0( map, x, y );
  }
}
