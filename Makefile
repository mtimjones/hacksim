CC=gcc
CFLAGS=-Wall -I.

DEPS = thread.h
OBJ = main.c thread.c display.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hacksim: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) `sdl2-config --cflags --libs`

clean:
	rm -f hacksim
