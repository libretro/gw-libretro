#include <rl_map.h>
#include <rl_memory.h>
#include <rl_backgrnd.h>

#include <rl_endian.c>

static rl_map_t* destroy( rl_map_t* map )
{
  if ( map )
  {
    if ( map->tileset )
    {
      rl_tileset_destroy( map->tileset );
    }
    
    if ( map->layer0 )
    {
      rl_free( map->layer0 );
    }
    
    rl_free( map );
  }
  
  return NULL;
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
  ptr.u16++; /* padding */
  
  rl_map_t* map = (rl_map_t*)rl_malloc( sizeof( rl_map_t ) );
  
  if ( map )
  {
    map->width = width;
    map->height = height;
    map->num_layers = num_layers;
    
    size_t tileset_size = ne32( *ptr.u32++ );
    map->tileset = rl_tileset_create( ptr.v, tileset_size );
    
    if ( map->tileset )
    {
      ptr.u8 += tileset_size;
      map->layer0 = (rl_layer0_t*)rl_malloc( width * height * sizeof( uint16_t ) );
      
      if ( map->layer0 )
      {
        uint16_t* restrict ndx = map->layer0->indices;
        const uint16_t* restrict end = ndx + width * height;
        
        while ( ndx < end )
        {
          *ndx++ = ne16( *ptr.u16++ );
        }
        
        return map;
      }
    }
  }
  
  return destroy( map );
}

void rl_map_destroy( rl_map_t* map )
{
  destroy( map );
}

void rl_map_render_layer( rl_map_t* map, int ndx, int x, int y )
{
  int width, height;
  rl_backgrnd_fb( &width, &height );
  
  if ( ndx == 0 )
  {
    int wt = ( width + map->tileset->width - 1 ) / map->tileset->width + 1;
    int ht = ( width + map->tileset->height - 1 ) / map->tileset->height;
    
    int dx = -( x % map->tileset->width );
    int dy = -( y % map->tileset->height );
    
    x /= map->tileset->width;
    y /= map->tileset->height;
    
    for ( int yy = 0; yy < ht; yy++ )
    {
      for ( int xx = 0; xx < wt; xx++ )
      {
        rl_tileset_blit_nobg( map->tileset, map->layer0->indices[ ( y + yy ) * map->width + ( x + xx ) ], dx + xx * map->tileset->width, dy + yy * map->tileset->height );
      }
    }
  }
}
