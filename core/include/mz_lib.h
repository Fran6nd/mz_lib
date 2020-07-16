#ifndef MZ_LIB_H
#define MZ_LIB_H

#define MZ_WALL '#'
#define MZ_PATH ' '

typedef struct Maze mz_maze;

typedef struct Position mz_position;

struct Position {
  int x;
  int y;
};

struct Maze {
  int size;
  int x;
  int y;
  char *grid;
  mz_position start_pos;
  mz_position end_pos;
};

typedef struct Neighbor mz_neighbor;

struct Neighbor {
  int n;
  mz_position paths[8];
};

mz_maze mz_new(int x, int y);
int mz_free(mz_maze *maze_ptr);
void mz_generate(mz_maze *maze_ptr, void (*callback)(mz_maze *maze_ptr));
char mz_get_tile(mz_maze *maze_ptr, mz_position pos);
void mz_set_tile(mz_maze *maze_ptr, mz_position pos, char tile);
void mz_iterate(mz_maze *maze_ptr,
                void (*callback)(mz_maze *maze_ptr, mz_position pos));
char mz_get_tile(mz_maze *maze_ptr, mz_position p);

void mz_set_tile(mz_maze *maze_ptr, mz_position p, char tile);

#endif
