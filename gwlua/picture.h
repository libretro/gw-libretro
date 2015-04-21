#ifndef PICTURE_H
#define PICTURE_H

#include <gwlua.h>

void blit_picture( const gwlua_picture_t* picture, int x, int y, uint16_t* bg );
void unblit_picture( const gwlua_picture_t* picture, int x, int y, uint16_t* bg );

#endif /* PICTURE_H */
