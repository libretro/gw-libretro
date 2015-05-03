#ifndef RL_TILE_H
#define RL_TILE_H

#include <rl_userdata.h>

#include <stdint.h>
#include <stddef.h>

/*
A tile is a rectangular area of 100% opaque pixels pixels. A tileset is a
collection of tiles that share the same dimensions.
*/

typedef struct
{
  rl_userdata_t ud;
  
  int width;     /* the width of each tile */
  int height;    /* the height of each tile */
  int size;      /* width * height */
  int num_tiles; /* the number of tiles in this tileset */

  const uint16_t data[ 0 ]; /* the tiles' pixels */
}
rl_tileset_t;

/* Creates a tileset given the data produced by rltileset. */
rl_tileset_t* rl_tileset_create( const void* data, size_t size );
/* Destroys a tileset. */
#define rl_tileset_destroy( tileset) do { rl_free( tileset ); } while ( 0 )

/* Blits a tile from a tileset onto the background. */
void rl_tileset_blit_nobg( const rl_tileset_t* tileset, int index, int x, int y );
/* Blits a tile from a tileset onto the background, saving overwritten pixels in bg. */
uint16_t* rl_tileset_blit( const rl_tileset_t* tileset, int index, int x, int y, uint16_t* bg );
/* Erases a tile from a tileset from the background, restoring overwritten pixels from bg. */
void rl_tileset_unblit( const rl_tileset_t* tileset, int x, int y, const uint16_t* bg );

/* Blits a tile onto the background. */
void rl_tile_blit_nobg( int width, int height, const uint16_t* pixels, int x, int y );
/* Blits a tile onto the background, saving overwritten pixels in bg. */
uint16_t* rl_tile_blit( int width, int height, const uint16_t* pixels, int x, int y, uint16_t* bg );
/* Erases a tile from the background, restoring overwritten pixels from bg. */
void rl_tile_unblit( int width, int height, int x, int y, const uint16_t* bg );

#endif /* RL_TILE_H */
