#finds all files in engine/source in all directories that have the .c suffix
SRCPATHS = src/*/*.c

CSRC = $(wildcard $(SRCPATHS))

OBJECTS = $(CSRC:.c=.o)

#LCC is our linux compiler
LCC = gcc

#LCFLAGS are our compile time flags for linux
LCFLAGS = -lm -Iinclude

DEBUGFLAGS = -g -rdynamic -DDEBUG -lSDL2
ASANFLAGS = -fsanitize=undefined -fsanitize=address
DONTBEAFUCKINGIDIOT = -Werror -Wall -Wextra -Wno-missing-field-initializers -Wno-format-zero-length

%.o: %.c
	$(LCC) -c -o $@ $< $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

a.out : $(OBJECTS)
	$(LCC) $(OBJECTS) $(LCFLAGS) $(DEBUGFLAGS) $(DONTBEAFUCKINGIDIOT)

clean: 
	rm -f src/*/*.o
	rm -f a.out
