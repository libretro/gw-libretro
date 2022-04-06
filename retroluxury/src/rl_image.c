#include <rl_image.h>
#include <rl_memory.h>
#include <rl_backgrnd.h>

#include <string.h>

#include <rl_endian.c>

rl_image_t* rl_image_create( const void* data, size_t size )
{
  union
  {
    const void*     restrict v;
    const uint16_t* restrict u16;
    const uint32_t* restrict u32;
  }
  ptr;
  int width;
  int height;
  // structure size
  size_t mem = sizeof( rl_image_t );
  rl_image_t* image;

  ptr.v = data;
  
  width  = ne16( *ptr.u16++ );
  height = ne16( *ptr.u16++ );
  
  // rows array size
  mem += height * sizeof( uint32_t );
  // rle size
  size -= 2 * sizeof( uint16_t ) /* width & height */
          + sizeof(uint32_t) /* used */
          + height * sizeof( uint32_t ) /* row pointers */;

  mem += size;
  
  image = (rl_image_t*)rl_malloc( mem );
  
  if ( image )
  {
    uint32_t* restrict rows = (uint32_t*)( image->data );
    uint16_t* restrict rle = (uint16_t*)( rows + height );

    image->width  = width;
    image->height = height;
    image->used   = ne32( *ptr.u32++ );
    
    image->rows = rows;
        
    for ( int i = 0; i < height; i++ )
    {
      *rows++ = height * sizeof( uint32_t ) + ne32( *ptr.u32++ );
    }
    
    for ( int i = 0; i < size; i += 2 )
    {
      *rle++ = ne16( *ptr.u16++ );
    }
    
    return image;
  }
  
  return NULL;
}

rl_imageset_t* rl_imageset_create( const void* data, size_t size )
{
  union
  {
    const void*     restrict v;
    const uint8_t*  restrict u8;
    const uint16_t* restrict u16;
    const uint32_t* restrict u32;
  }
  ptr;
  int num_images;
  
  rl_imageset_t* imageset;
  
  ptr.v = data;
  
  num_images = ne16( *ptr.u16++ );
  
  imageset = (rl_imageset_t*)rl_malloc( sizeof( rl_imageset_t ) + num_images * sizeof( rl_image_t* ) );
  
  if ( imageset )
  {
    imageset->num_images = num_images;
    
    for ( int i = 0; i < num_images; i++ )
    {
      size_t image_size = ne32( *ptr.u32++ );
      imageset->images[ i ] = rl_image_create( ptr.v, image_size );
      
      if ( !imageset->images[ i ] )
      {
        for ( int j = i - 1; j >= 0; --j )
        {
          rl_image_destroy( (void*)imageset->images[ j ] );
        }
        
        rl_free( imageset );
        return NULL;
      }
      
      ptr.u8 += image_size;
    }
    
    return imageset;
  }
  
  return NULL;
}

void rl_imageset_destroy( const rl_imageset_t* imageset )
{
  for ( int i = imageset->num_images - 1; i >= 0; --i )
  {
    rl_image_destroy( imageset->images[ i ] );
  }
  
  rl_free( (void*)imageset );
}

void rl_image_blit_nobg( const rl_image_t* image, int x, int y )
{
  int x0 = 0;
  int y0 = 0;
  int x1 = image->width;
  int y1 = image->height;
  
  int width, height;
  uint16_t* fb = rl_backgrnd_fb( &width, &height );
  
  if ( x < -RL_BACKGRND_MARGIN )
  {
    x   = -x;
    x0 += x & ~( RL_BACKGRND_MARGIN - 1 );
    x1 -= x & ~( RL_BACKGRND_MARGIN - 1 );
    x   = -( x & ( RL_BACKGRND_MARGIN - 1 ) );
  }
  
  if ( x + x1 > width )
  {
    x1 -= x + x1 - width;
  }
  
  if ( y < 0 )
  {
    y0 -= y;
    y1 += y;
    y   = 0;
  }
  
  if ( y + y1 > height )
  {
    y1 -= y + y1 - height;
  }
  
  if ( y1 > 0 && x1 > 0 )
  {
    int       pitch  = width + RL_BACKGRND_MARGIN;
    uint16_t* save   = fb + y * pitch + x;
    
#if RL_BACKGRND_MARGIN == 0
    int runcnt = 1;
#else
    int runcnt = ( x1 + ( RL_BACKGRND_MARGIN - 1 ) ) / RL_BACKGRND_MARGIN;
    x0         = ( x0 + ( RL_BACKGRND_MARGIN - 1 ) ) / RL_BACKGRND_MARGIN;
#endif
    
    do
    {
      const uint16_t* restrict rle = (uint16_t*)( image->data + image->rows[ y0++ ] );
      uint16_t* restrict dest = save;
      int runs = runcnt;

      rle += rle[ x0 ];
      
      save += pitch;
      
      
      do
      {
        /* number of rle encodings in this chunk */
        int numrle = *rle++;
        
        do
        {
          /* decode */
          int code   = *rle++;
          int count  = code & 0x1fff;
	  uint32_t c1, c2;

          code >>= 13;
                    
          switch ( code & 7 )
          {
          case 4: /* opaque */
            memcpy( (void*)dest, (void*)rle, count * 2 );
            rle += count, dest += count;
            break;
          
          case 0: /* transparent */
            dest += count;
            break;
          
          case 2: /* 50% */
            do
            {
              c1      = *dest & 0xf7de;
              c2      = *rle++ & 0xf7de;
              *dest++ = ( c1 + c2 ) >> 1;
            }
            while ( --count );
            break;
          
          case 1: /* 25% */
            do
            {
              c1      = *dest & 0xe79c;
              c2      = *rle++ & 0xe79c;
              *dest++ = ( 3 * c1 + c2 ) >> 2;
            }
            while ( --count );
            break;
          
          case 3: /* 75% */
            do
            {
              c1      = *dest & 0xe79c;
              c2      = *rle++ & 0xe79c;
              *dest++ = ( c1 + 3 * c2 ) >> 2;
            }
            while ( --count );
            break;
          
          /* these are just to avoid gcc adding a cmp + ja */
          case 5:
            rle++;
          case 6:
            rle++;
          case 7:
            rle++;
            break;
          }
        }
        while ( --numrle );
      }
      while ( --runs );
    }
    while ( --y1 );
  }
}

uint16_t* rl_image_blit( const rl_image_t* image, int x, int y, uint16_t* bg_ )
{
  int x0 = 0;
  int y0 = 0;
  int x1 = image->width;
  int y1 = image->height;
  
  int width, height;
  uint16_t* fb = rl_backgrnd_fb( &width, &height );
  uint16_t* restrict bg = bg_;
  
  if ( x < -RL_BACKGRND_MARGIN )
  {
    x   = -x;
    x0 += x & ~( RL_BACKGRND_MARGIN - 1 );
    x1 -= x & ~( RL_BACKGRND_MARGIN - 1 );
    x   = -( x & ( RL_BACKGRND_MARGIN - 1 ) );
  }
  
  if ( x + x1 > width )
  {
    x1 -= x + x1 - width;
  }
  
  if ( y < 0 )
  {
    y0 -= y;
    y1 += y;
    y   = 0;
  }
  
  if ( y + y1 > height )
  {
    y1 -= y + y1 - height;
  }
    
  if ( y1 > 0 && x1 > 0 )
  {
    int       pitch  = width + RL_BACKGRND_MARGIN;
    uint16_t* save   = fb + y * pitch + x;
    
#if RL_BACKGRND_MARGIN == 0
    int runcnt = 1;
#else
    int runcnt = ( x1 + ( RL_BACKGRND_MARGIN - 1 ) ) / RL_BACKGRND_MARGIN;
    x0         = ( x0 + ( RL_BACKGRND_MARGIN - 1 ) ) / RL_BACKGRND_MARGIN;
#endif
    
    do
    {
      const uint16_t* restrict rle = (uint16_t*)( image->data + image->rows[ y0++ ] );
      uint16_t* restrict dest = save;
      int runs = runcnt;
      
      rle += rle[ x0 ];
      
      save += pitch;
      
      do
      {
        /* number of rle encodings in this chunk */
        int numrle = *rle++;
        
        do
        {
          /* decode */
          int code   = *rle++;
          int count  = code & 0x1fff;
	  uint32_t c1, c2;

          code >>= 13;
                    
          switch ( code & 7 )
          {
          case 4: /* opaque */
            memcpy( (void*)bg, (void*)dest, count * 2 );
            memcpy( (void*)dest, (void*)rle, count * 2 );
            rle += count, dest += count, bg += count;
            break;
          
          case 0: /* transparent */
            dest += count;
            break;
          
          case 2: /* 50% */
            do
            {
              c1      = *dest;
              *bg++   = c1;
              c1     &= 0xf7de;
              c2      = *rle++ & 0xf7de;
              *dest++ = ( c1 + c2 ) >> 1;
            }
            while ( --count );
            break;
          
          case 1: /* 25% */
            do
            {
              c1      = *dest;
              *bg++   = c1;
              c1     &= 0xe79c;
              c2      = *rle++ & 0xe79c;
              *dest++ = ( 3 * c1 + c2 ) >> 2;
            }
            while ( --count );
            break;
          
          case 3: /* 75% */
            do
            {
              c1      = *dest;
              *bg++   = c1;
              c1     &= 0xe79c;
              c2      = *rle++ & 0xe79c;
              *dest++ = ( c1 + 3 * c2 ) >> 2;
            }
            while ( --count );
            break;
          
          /* these are just to avoid gcc adding a cmp + ja */
          case 5:
            rle++;
          case 6:
            rle++;
          case 7:
            rle++;
            break;
          }
        }
        while ( --numrle );
      }
      while ( --runs );
    }
    while ( --y1 );
  }
  
  return bg;
}

void rl_image_unblit( const rl_image_t* image, int x, int y, const uint16_t* bg_ )
{
  int x0 = 0;
  int y0 = 0;
  int x1 = image->width;
  int y1 = image->height;
  
  int width, height;
  uint16_t* fb = rl_backgrnd_fb( &width, &height );
  const uint16_t* restrict bg = bg_;

  if ( x < -RL_BACKGRND_MARGIN )
  {
    x   = -x;
    x0 += x & ~( RL_BACKGRND_MARGIN - 1 );
    x1 -= x & ~( RL_BACKGRND_MARGIN - 1 );
    x   = -( x & ( RL_BACKGRND_MARGIN - 1 ) );
  }
  
  if ( x + x1 > width )
  {
    x1 -= x + x1 - width;
  }
  
  if ( y < 0 )
  {
    y0 -= y;
    y1 += y;
    y   = 0;
  }
  
  if ( y + y1 > height )
  {
    y1 -= y + y1 - height;
  }
    
  if ( y1 > 0 && x1 > 0 )
  {
    int       pitch  = width + RL_BACKGRND_MARGIN;
    uint16_t* save   = fb + y * pitch + x;
    
#if RL_BACKGRND_MARGIN == 0
    int runcnt = 1;
#else
    int runcnt = ( x1 + ( RL_BACKGRND_MARGIN - 1 ) ) / RL_BACKGRND_MARGIN;
    x0         = ( x0 + ( RL_BACKGRND_MARGIN - 1 ) ) / RL_BACKGRND_MARGIN;
#endif
    
    do
    {
      const uint16_t* restrict rle = (uint16_t*)( image->data + image->rows[ y0++ ] );
      uint16_t* restrict dest = save;
      int runs = runcnt;

      rle += rle[ x0 ];
      
      save += pitch;
            
      do
      {
        /* number of rle encodings in this chunk */
        int numrle = *rle++;
        
        do
        {
          /* decode */
          int code   = *rle++;
          int count  = code & 0x1fff;
          
          if ( code & 0xe000 )
          {
            memcpy( (void*)dest, (void*)bg, count * 2 );
            rle += count, dest += count, bg += count;
          }
          else
          {
            dest += count;
          }
        }
        while ( --numrle );
      }
      while ( --runs );
    }
    while ( --y1 );
  }
}
