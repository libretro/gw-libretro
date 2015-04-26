#ifndef RL_IMAGE_H
#define RL_IMAGE_H

#include <rl_backgrnd.h>
#include <rl_userdata.h>

#include <stdint.h>
#include <stddef.h>

/*
An image with RLE-encoded pixels and per-pixel alpha of 0, 25, 50, 75 and 100%.
Images save the background in the bg pointer when blit, and restore the
background with that information when unblit.
*/

typedef struct
{
  rl_userdata_t ud;
  
  int width;     /* the image width */
  int height;    /* the image height */
  int used;      /* number of overwritten pixels on the background */
  
  const uint32_t* rows; /* offsets to rle data for each row */
  
  uint8_t   data[ 0 ]; /* stored row offsets and rle data */
}
rl_image_t;

/* Creates an image given the RLE-encoded data produced by rlrle. */
rl_image_t* rl_image_create( const void* data, size_t size );
/* Destroys an image. */
#define rl_image_destroy( image ) do { rl_free( image ); } while ( 0 )

/* Blits an image to the given background. */
void rl_image_blit_nobg( const rl_image_t* image, int x, int y );
/* Blits an image to the given background, saving overwritten pixels in bg. */
uint16_t* rl_image_blit( const rl_image_t* image, int x, int y, uint16_t* bg );
/* Erases an image from the given background, restoring overwritten pixels from bg. */
void rl_image_unblit( const rl_image_t* image, int x, int y, uint16_t* bg );

#endif /* RL_IMAGE_H */
