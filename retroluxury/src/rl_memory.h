#ifndef RL_MEMORY_H
#define RL_MEMORY_H

#include <rl_config.h>

#include <stddef.h>

void* rl_malloc( size_t size );
void* rl_realloc( void* pointer, size_t size );
void  rl_free( void* pointer );

#endif /* RL_MEMORY_H */
