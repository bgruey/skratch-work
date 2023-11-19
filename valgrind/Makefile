# Build Puppet Program
.RECIPEPREFIX = >
CC=/usr/bin/gcc
CFLAGS=-ggdb3 -Wall -I./lib
LD=/usr/bin/ld
LDFLAGS=-L./lib -lpuppet

default: test

test: build
>	LD_LIBRARY_PATH=./lib valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./puppet.x

run: build
>	LD_LIBRARY_PATH=./lib ./puppet.x

libpuppet.so:
>	$(MAKE) -C ./lib

build: libpuppet.so
>	$(CC) $(CFLAGS) -o puppet.x main.c $(LDFLAGS)

.PHONY: clean
clean:
>	rm *.x