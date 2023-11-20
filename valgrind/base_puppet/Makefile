# Make the Puppet Library!
.RECIPEPREFIX = >
CC=/usr/bin/gcc
CFLAGS=-g -O0 -shared -fPIC -ggdb3 -Wall
LD=/usr/bin/ld
LDFLAGS=-shared -fPIC -lm 

srcfiles = $(wildcard *.c)
objfiles = $(srcfiles:.c=.o)

default: libpuppet.so

libpuppet.so: $(objfiles)
>   $(LD) $(LDFLAGS) -o $@ $^;

.PHONY: clean
clean:
>   rm *.o libpuppet.so;
