#ifndef RL_CONFIG_H
#define RL_CONFIG_H

/*
The margin to use when blitting sprites, must be a power of 2. The exact same
value must be used when rle-encoding images with rlrle.lua.
*/
#ifndef RL_BACKGRND_MARGIN
#define RL_BACKGRND_MARGIN 0
#endif

/* The maximum *total* number of sprites. */
#ifndef RL_MAX_SPRITES
#define RL_MAX_SPRITES 512
#endif

/* The maximum number of saved pixels when blitting sprites. */
#ifndef RL_BG_SAVE_SIZE
#define RL_BG_SAVE_SIZE ( 192 * 1024 )
#endif

/* The maximum number of simultaneous *playing* sounds. */
#ifndef RL_MAX_VOICES
#define RL_MAX_VOICES 8
#endif

/* The buffer increment to use when opening ogg files. */
#ifndef RL_OGG_INCREMENT
#define RL_OGG_INCREMENT 32768
#endif

/* Undef to remove ogg-vorbis support. */
#undef RL_OGG_VORBIS

/* The number of userdata entries in other structures. */
#ifndef RL_USERDATA_COUNT
#define RL_USERDATA_COUNT 1
#endif

/* Undef to use custom memory functions. */
#define rl_malloc  malloc
#define rl_realloc realloc
#define rl_free    free

#endif /* RL_CONFIG_H */
