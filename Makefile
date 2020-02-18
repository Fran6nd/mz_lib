mz_lib: mz_lib.c mz_lib.h
	gcc -shared -o mz_lib.so -fPIC mz_lib.c
mz_curses: main.c mz_lib.c mz_lib.h
	gcc main.c mz_lib.c -lncurses -o mz_curses
all: mz_lib  mz_curses