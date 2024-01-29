# Make the Pin Thread Library!
.RECIPEPREFIX = >
CC=/usr/bin/gcc
CFLAGS=-g -O0 -shared -fPIC -ggdb3 -Wall
LD=/usr/bin/ld
LDFLAGS=-shared -fPIC -L../base_puppet -lpuppet

srcfiles = $(wildcard *.c)
objfiles = $(srcfiles:.c=.o)

default: libpinthreads.so

libpinthreads.so: $(objfiles)
>   $(LD) $(LDFLAGS) -o $@ $^;

.PHONY: clean
clean:
>   rm -f *.o libpinthreads.so;
