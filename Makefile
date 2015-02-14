CC=gcc
CFLAGS=-Wall -I.

DEPS = thread.h
OBJ = main.c thread.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hacksim: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f hacksim
