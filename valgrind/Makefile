# Build Puppet Program
.RECIPEPREFIX = >
CC=/usr/bin/gcc
CFLAGS=-g -O0 -ggdb3 -Wall -I./base_puppet -I./dancer -I./pin_threads
LD=/usr/bin/ld
LDFLAGS=-L./base_puppet -L./dancer -L./pin_threads -lpuppet -lpinthreads -ldancer

default: test

test2: build
>       LD_LIBRARY_PATH=./base_puppet:./dancer:./pin_threads gdb ./puppet.x

test: clean build
>	LD_LIBRARY_PATH=./base_puppet:./dancer:./pin_threads \
        valgrind --leak-check=full \
                --show-leak-kinds=all \
                --verbose \
                --log-file=valgrind-out.txt \
                --track-origins=yes \
                --num-callers=10 \
                ./puppet.x

run: build
>	LD_LIBRARY_PATH=./base_puppet:./dancer:./pin_threads ./puppet.x

build: libpuppet.so libdancer.so libpinthreads.so
>	$(CC) $(CFLAGS) -o puppet.x main.c $(LDFLAGS)

libpuppet.so:
>	$(MAKE) -C ./base_puppet

libdancer.so: libpinthreads.so libpuppet.so
>       $(MAKE) -C ./dancer

libpinthreads.so: libpuppet.so
>       $(MAKE) -C ./pin_threads

.PHONY: clean
clean:
>	rm -f *.x
>	$(MAKE) clean -C ./base_puppet
>       $(MAKE) clean -C ./dancer
>       $(MAKE) clean -C ./pin_threads
