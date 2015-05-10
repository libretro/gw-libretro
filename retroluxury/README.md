# retroluxury

**retroluxury** is a game library (wannabe) built on top of the [libretro](http://www.libretro.com/) API. It provides functions to blit images to the frame buffer, and to play [PCM audio](http://en.wikipedia.org/wiki/Pulse-code_modulation) and [Ogg Vorbis](http://en.wikipedia.org/wiki/Vorbis) files.

On top of that, **retroluxury** provides higher level functions to deal with sprites and draw maps created with the [Tiled](http://www.mapeditor.org/) map editor.

## API

### Background

The background is a 2D array of 16-bit pixels which represent what is seen on the screen. It's usually called **framebuffer**.

#### `int rl_backgrnd_create( int width, int height )`

Creates a framebuffer of `width` by `height` 16-bit pixels. Returns `0` on success or `-1` on error (which can only mean a memory allocation error for now.)

#### `void rl_backgrnd_destroy( void )`

Releases the memory allocated by the `rl_backgrnd_create` function.

#### `void rl_backgrnd_clear( uint16_t color )`

Clears the entire framebuffer to the given color. The 16-bit color can be calculated from 8-bit red, green and blue values with the following code:

    int r = r * 31 / 255;
    int g = g * 63 / 255;
    int b = b * 31 / 255;
    uint16_t color = ( r << 11 ) | ( g << 5 ) | b;


#### `void rl_backgrnd_scroll( int dx, int dy )`

Scrolls the framebuffer by the given pixels. `dx` scrolls horizontally, and `dy` scrolls vertically. `rl_backgrnd_scroll( 0, -1 )` scrolls the framebuffer one pixel up.

`rl_backgrnd_scroll` doesn't do anything with lines "created" by the scroll. I.e., if you scroll the framebuffer one pixel left, the last column on the right will contain garbagge. It's up to the application to draw meaningful pixels in these places after a scroll.

#### `uint16_t* rl_backgrnd_fb( int* width, int* height )`

Returns a pointer to the top-left pixel of the framebuffer, along with its width and height. Note that the framebuffer can have lines greater than `width` pixels (usually called *pitch*.) To calculate how much pixels to add to a pointer to the framebuffer in order to go one line down, use `width + RL_BACKGRND_MARGIN`.

### Images

Images are collections of [run-length encoded](http://en.wikipedia.org/wiki/Run-length_encoding) pixels, that can be drawn onto the framebuffer. Images also provide means to save the framebuffer pixels that are overwritten when they're drawn, and to restore those saved pixels back to framebuffer afterwards. This scheme allows for higher level contructions such as sprites.

Images have per-pixel transparency of 0, 25, 50, 75 and 100%.

#### `rl_image_t* rl_image_create( const void* data, size_t size )`

Creates and returns an image based on the given data. This data can be created with the `rlrle.lua` tool.

#### `rl_image_destroy( image )` (macro)

Frees the given image.

#### `rl_imageset_t* rl_imageset_create( const void* data, size_t size )`

Creates a collection of images (imageset) from the given data.

#### `void rl_imageset_destroy( const rl_imageset_t* imageset )`

Frees the given imageset.

#### `void rl_image_blit_nobg( const rl_image_t* image, int x, int y )`

Drawn an image to the framebuffer at position (x, y). `rl_image_blit_nobg` doesn't save overwritten pixels on the framebuffer, so it's kind of a permanent operation, which will be visible on the screen until you overwrite the image with something else, i.e. by clearing the framebuffer.

#### `uint16_t* rl_image_blit( const rl_image_t* image, int x, int y, uint16_t* bg )`

Save as `rl_image_blit_nobg`, but saves overwritten pixels on the framebuffer to the `bg` pointer. Make sure you allocate at least `image->used * sizeof(uint16_t)` bytes to `bg`.

#### `void rl_image_unblit( const rl_image_t* image, int x, int y, const uint16_t* bg )`

Restores the framebuffer as it was before the image was drawn. The `bg` argument should be the same as the `rl_image_blit` call.

### Tiles

Tiles are similar to images, but they are always a rectangular block of 100% opaque pixels.

#### `rl_tileset_t* rl_tileset_create( const void* data, size_t size )`

Creates a tileset (a collection of tiles having the same width and height) from the given data. Each tile is identified by its 0-based index in the tileset.

#### `rl_tileset_destroy( tileset)` (macro)

Destroys the tileset.

#### `void rl_tileset_blit_nobg( const rl_tileset_t* tileset, int index, int x, int y )`

Similar to `rl_image_blit_nobg`, but draws the tile identified by `index` in the given tileset.

#### `uint16_t* rl_tileset_blit( const rl_tileset_t* tileset, int index, int x, int y, uint16_t* bg )`

Similar to `rl_image_blit`.

#### `void rl_tileset_unblit( const rl_tileset_t* tileset, int x, int y, const uint16_t* bg )`

Similar to `rl_image_unblit`.

#### `void rl_tile_blit_nobg( int width, int height, const uint16_t* pixels, int x, int y )`

Similar to `rl_image_blit_nobg`, but draws a rectangular array of `width` times `height` pixels located at `pixels`.

#### `uint16_t* rl_tile_blit( int width, int height, const uint16_t* pixels, int x, int y, uint16_t* bg )`

Similar to `rl_image_blit`.

#### `void rl_tile_unblit( int width, int height, int x, int y, const uint16_t* bg )`

Similar to `rl_image_unblit`.

### Sound

**retro luxury** supports stero, 44,100 Hz audio. All audio must be supplied at 44,100 Hz. Mono audio will be duplicated in both left and right channels.

Sounds are just PCM data which are mixed toghether. The `RL_MAX_VOICES` macro defines at compile time how much sounds can be played at the same time (usually called *voices*.) One Ogg Vorbis file can be played in addition to the voices.

#### `void rl_sound_init( void )`

Initializes internal data.

#### `void rl_sound_done( void )`

Stops all sounds and releases all memory used by the sound system.

#### `rl_sound_t* rl_sound_create( const void* data, size_t size, int stereo )`

Creates a sound from the given data. The data is just `size / 2` signed 16-bit PCM data. If `stereo` is `true`, the sound is stereo and data is [Interleave](http://en.wikipedia.org/wiki/Interleave_sequence), with 16-bits for the left channel followed by 16-bits for the right channel and so forth: LRLRLR...

#### `rl_sound_destroy( sound )` (macro)

Frees the given sound. Do **not** free a sound that's still playing.

#### `int rl_sound_play( const rl_sound_t* sound, int repeat, rl_soundstop_t stop_cb )`

Plays a sound. If `repeat` is true, the sound will be repeated *ad infinitum* or until manually stopped. If `stop_cb` is not `NULL`, it'll be called when the sound stops, either because of calls to `rl_sound_stop` or `rl_sound_stop_all` or because it ended naturally.

The returned integer is an internal voice identifier which can be used in calls to `rl_sound_stop`.

#### `void rl_sound_stop( int index )`

Stops a sound (voice) being played.

#### `void rl_sound_stop_all( void )`

Mutes all voices. The Ogg Vorbis music is not affected.

#### `int rl_sound_play_ogg( const void* data, size_t size, int repeat, rl_soundstop_t stop_cb )`

Starts playing the Ogg Vorbis file contained in `data`. If `repeat` is true, the music will restart automatically. If `stop_cb` is not `NULL`, it'll be called when the music either finishes or `rl_sound_stop_ogg` is called.

#### `void rl_sound_stop_ogg( void )`

Interrupts the Ogg Vorbis music.

#### `const int16_t* rl_sound_mix( void )`

Mixes all the active voices and the Ogg Vorbis music, if any, and returns a pointer to the stereo, 16-bit PCM audio data buffer that can be sent directly to a properly configured audio device. The data buffer has `RL_SAMPLES_PER_FRAME` stereo samples, meaning `RL_SAMPLES_PER_FRAME * 2` samples, which equals to `RL_SAMPLES_PER_FRAME * 2 * 2` bytes.

### Sprites

TODO

### Maps

TODO


### Misc

TODO (memory management, endianess conversion, userdata and compile-time configuration.)

## Conversion Tools

### rlrle.lua

TODO

### rlmap.lua

TODO

### rltile.lua

TODO

### rltileset.lua

TODO

## A Minimum libretro Core

TODO, but see `test/libretro/test.c`.

## License

Released under the zlib/libpng license.
