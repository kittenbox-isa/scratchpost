#finds all files in engine/source in all directories that have the .c suffix
SRCPATHS = src/*/*.c src/*/*/*.c

CSRC = $(wildcard $(SRCPATHS))

OBJECTS = $(CSRC:.c=.o)

#LCC is our linux compiler
LCC = gcc

#LCFLAGS are our compile time flags for linux
LCFLAGS = -lm -Iinclude -fcommon

DEBUGFLAGS = -g -DDEBUG -lSDL2 -D__NOCOL__
ASANFLAGS = -fsanitize=undefined -fsanitize=address
DONTBEAFUCKINGIDIOT = -Werror -Wno-missing-field-initializers -Wno-format-zero-length 

%.o: %.c
	$(LCC) -c -o $@ $< $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

a.out : $(OBJECTS)
	$(LCC) $(OBJECTS) $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

clean: 
	rm -f src/*/*.o
	rm -f src/*/*/*.o
	rm -f a.out