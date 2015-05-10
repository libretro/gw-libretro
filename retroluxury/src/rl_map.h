#ifndef RL_MAP_H
#define RL_MAP_H

#include <rl_userdata.h>
#include <rl_image.h>
#include <rl_tile.h>

#include <stdint.h>
#include <stddef.h>

typedef struct
{
  uint16_t indices[ 0 ]; /* width * height *tile* indices */
}
rl_layer0_t;

/* layern indices are *image* indices. */
typedef rl_layer0_t rl_layern_t;

typedef struct
{
  rl_userdata_t ud;
  
  int width;
  int height;
  int num_layers;
  
  const rl_tileset_t*  tileset;
  const rl_imageset_t* imageset;
  const rl_layer0_t*   layer0;
  
  rl_layern_t* layers[ 0 ];
}
rl_map_t;

rl_map_t* rl_map_create( const void* data, size_t size );
void      rl_map_destroy( const rl_map_t* map );

void rl_map_render_layer( const rl_map_t* map, int index, int x, int y );

#endif /* RL_MAP_H */
