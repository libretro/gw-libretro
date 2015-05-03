INCLUDES=-I.
DEFINES=-DGITHASH=\"`git rev-parse HEAD`\"

CFLAGS=-O3 --std=c99 $(INCLUDES) $(DEFINES)

OBJS=rl_backgrnd.o rl_image.o rl_map.o rl_sound.o rl_sprite.o rl_tile.o rl_version.o

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

all: libretroluxury.a

libretroluxury.a: $(OBJS)
	ar rcs $@ $+

clean:
	rm -f libretroluxury.a $(OBJS)
