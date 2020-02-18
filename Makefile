LIB_INC = core/include
LIB_SRC =  core/src
DIST = dist
EXAMPLES = examples

.DEFAULT_GOAL := mz_lib

mz_lib.o: $(LIB_SRC)/mz_lib.c
	[ -d $(DIST) ] || mkdir -p $(DIST)
	gcc -O -c $(LIB_SRC)/mz_lib.c -std=c99 -o $(LIB_SRC)/mz_lib.o -I$(LIB_INC)
mz_lib.a: mz_lib.o
	ar rcs $(DIST)/mz_lib.a $(LIB_SRC)/mz_lib.o
mz_curses.o:  mz_lib.o $(EXAMPLES)/mz_curses.c
	gcc -D _DEFAULT_SOURCE -O -c $(EXAMPLES)/mz_curses.c -std=c99 -o $(EXAMPLES)/mz_curses.o -I$(LIB_INC)
mz_curses: mz_curses.o mz_lib.o
	gcc $(EXAMPLES)/mz_curses.o $(LIB_SRC)/mz_lib.o -I$(LIB_INC) -lncurses -o $(DIST)/mz_curses
all: mz_curses mz_lib.a 
clean:
	rm $(DIST)/*
	rm $(EXAMPLES)/*.o
	rm $(LIB_SRC)/*.o

#PREFIX is environment variable, but if it is not set, then set default value
ifeq ($(PREFIX),)
    PREFIX := /usr
endif

install: mz_lib.a
	install -d /usr/lib/
	install -m 644 $(DIST)/mz_lib.a /usr/lib
	install -d $(PREFIX)/include/mz_lib
	install -m 644 $(LIB_INC)/mz_lib.h $(PREFIX)/include/mz_lib
