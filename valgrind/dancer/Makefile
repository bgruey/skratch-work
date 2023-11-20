# Make the Dancer Library!
.RECIPEPREFIX = >
CC=/usr/bin/gcc
CFLAGS=-g -O0 -shared -fPIC -ggdb3 -Wall
LD=/usr/bin/ld
LDFLAGS=-shared -fPIC

srcfiles = $(wildcard *.c)
objfiles = $(srcfiles:.c=.o)

default: libdancer.so

libdancer.so: $(objfiles)
>   $(LD) $(LDFLAGS) -o $@ $^;

.PHONY: clean
clean:
>   rm *.o libdancer.so;
