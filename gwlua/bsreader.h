#ifndef BSREADER_H
#define BSREADER_H

#include <lua.h>

void* bsnew( void* data );
const char* bsread( lua_State* L, void* data, size_t* size );

#endif /* BSREADER_H */
