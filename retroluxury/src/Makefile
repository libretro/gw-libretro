INCLUDES=-I.
DEFINES=

CFLAGS=-m32 -O3 --std=c99 $(INCLUDES) $(DEFINES)

OBJS=rl_backgrnd.o rl_image.o rl_map.o rl_rand.o rl_sound.o rl_sprite.o rl_tile.o rl_version.o

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

all: libretroluxury.a

libretroluxury.a: $(OBJS)
	ar rcs $@ $+

rl_version.c: FORCE
	cat ../etc/version.c.templ | sed s/HASH/`git rev-parse HEAD | tr -d "\n"`/g > $@

clean:
	rm -f libretroluxury.a $(OBJS)

.PHONY: clean FORCE
