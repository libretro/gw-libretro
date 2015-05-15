INCLUDES=-I. -I../../src
DEFINES=

CFLAGS=-m32 -O3 --std=c99 $(INCLUDES) $(DEFINES)
LDFLAGS=-m32 -L../../src
LIBS=-lretroluxury

OBJS=test.o
HEADERS=button_x.h block.h sketch008.h tick.h tile_x.h city_tls.h city_ims.h city_map.h

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

%.h: %.rle
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.ogg
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.pcm
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.tls
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.ims
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

%.h: %.map
	xxd -i $< | sed "s/unsigned/const unsigned/g" > $@

all: test.dll

test.dll: $(OBJS) ../../src/libretroluxury.a
	gcc -shared $(LDFLAGS) -o $@ $+ $(LIBS)

test.o: $(HEADERS)

button_x.rle: button_x.png
	../../etc/luai/luai ../../etc/rlrle.lua --margin 32 $<

button_x.h: button_x.rle

block.rle: block.png
	../../etc/luai/luai ../../etc/rlrle.lua --margin 32 $<

block.h: block.rle

sketch008.h: sketch008.ogg

tick.h: tick.pcm

tick.pcm: tick.wav
	sox $< -B -t s16 -c 1 -r 44100 $@

tile_x.h: tile_x.png
	../../etc/luai/luai ../../etc/rltile.lua $<

clean:
	rm -rf test.dll button_x.rle block.rle tick.pcm $(OBJS) $(HEADERS)
