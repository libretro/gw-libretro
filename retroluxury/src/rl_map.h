#ifndef RL_MAP_H
#define RL_MAP_H

#include <rl_userdata.h>
#include <rl_tile.h>

#include <stdint.h>
#include <stddef.h>

typedef struct
{
  uint16_t indices[ 0 ]; /* width * height *tile* indices */
}
rl_layer0_t;

typedef struct
{
  rl_userdata_t ud;
  
  int width;
  int height;
  int num_layers;
  
  rl_tileset_t* tileset;
  rl_layer0_t*  layer0;
  
  //rl_layern_t* layers[ 0 ];
}
rl_map_t;

rl_map_t* rl_map_create( const void* data, size_t size );
void      rl_map_destroy( rl_map_t* map );

void rl_map_render_layer( rl_map_t* map, int ndx, int x, int y );

#endif /* RL_MAP_H */
