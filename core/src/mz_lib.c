#include <stdlib.h>
#include <string.h>

#include "mz_lib.h"

/* To check wether a number is odd or not. */
static int is_odd(int n) {
  if (n % 2 == 0)
    return 1;
  return 0;
}

char mz_get_tile(mz_maze *maze_ptr, mz_position p) {
  return maze_ptr->grid[p.y * maze_ptr->x + p.x];
}

void mz_set_tile(mz_maze *maze_ptr, mz_position p, char tile) {
  maze_ptr->grid[p.y * maze_ptr->x + p.x] = tile;
}

/* To check wether we can path vertically or not. */
static int can_path_v(mz_position *p) {
  if (is_odd(p->x))
    return 1;
  return 0;
}

/* To check wether we can path horizontally or not. */
static int can_path_h(mz_position *p) {
  if (is_odd(p->y))
    return 0;
  return 1;
}

int random_between(int min, int max) {
  return rand() % ((max + 1) - min) + min;
}

int mz_is_on_grid(mz_maze *maze_ptr, mz_position p) {
  if (p.x >= 0 && p.x < maze_ptr->x && p.y >= 0 && p.y < maze_ptr->y)
    return 1;
  return 0;
}

mz_neighbor mz_get_neighboors(mz_maze *maze_ptr, mz_position p) {
  mz_position up, down, right, left;
  up.x = p.x;
  up.y = p.y - 1;
  down.x = p.x;
  down.y = p.y + 1;
  right.y = p.y;
  right.x = p.x + 1;
  left.y = p.y;
  left.x = p.x - 1;
  mz_neighbor nb;
  nb.n = 0;
  if (mz_is_on_grid(maze_ptr, up)) {
    nb.paths[nb.n] = up;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, down)) {
    nb.paths[nb.n] = down;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, right)) {
    nb.paths[nb.n] = right;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, left)) {
    nb.paths[nb.n] = left;
    nb.n++;
  }
  return nb;
}
mz_neighbor mz_get_advanced_neighboors(mz_maze *maze_ptr, mz_position p) {
  mz_position up, down, right, left;
  up.x = p.x;
  up.y = p.y - 1;
  down.x = p.x;
  down.y = p.y + 1;
  right.y = p.y;
  right.x = p.x + 1;
  left.y = p.y;
  left.x = p.x - 1;
  mz_position up_right = {p.x + 1, p.y - 1}, down_right = {p.x + 1, p.y + 1},
              down_left = {p.x - 1, p.y + 1}, up_left = {p.x - 1, p.y - 1};

  mz_neighbor nb;
  nb.n = 0;
  if (mz_is_on_grid(maze_ptr, up)) {
    nb.paths[nb.n] = up;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, down)) {
    nb.paths[nb.n] = down;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, right)) {
    nb.paths[nb.n] = right;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, left)) {
    nb.paths[nb.n] = left;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, up_left)) {
    nb.paths[nb.n] = up_left;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, up_right)) {
    nb.paths[nb.n] = up_right;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, down_left)) {
    nb.paths[nb.n] = down_left;
    nb.n++;
  }
  if (mz_is_on_grid(maze_ptr, down_right)) {
    nb.paths[nb.n] = down_right;
    nb.n++;
  }
  return nb;
}

int mz_is_pathable(mz_maze *maze_ptr, mz_position p) {
  if (mz_is_on_grid(maze_ptr, p)) {
    if (mz_get_tile(maze_ptr, p) == MZ_WALL) {
      mz_neighbor nb1 = mz_get_neighboors(maze_ptr, p);
      if (nb1.n == 4) {
        int i;
        int j = 0;
        for (i = 0; i < nb1.n; i++) {
          mz_position p1 = {.x = nb1.paths[i].x, nb1.paths[i].y};
          if (mz_get_tile(maze_ptr, p1) == MZ_PATH) {
            j++;
          }
        }
        if (j <= 1) {
          mz_neighbor nb = mz_get_advanced_neighboors(maze_ptr, p);
          if (nb.n == 8) {
            i = 0;
            j = 0;
            for (i = 0; i < nb.n; i++) {
              mz_position p1 = {.x = nb.paths[i].x, nb.paths[i].y};
              if (mz_get_tile(maze_ptr, p1) == MZ_PATH) {
                j++;
              }
            }
            if (j <= 3) {
              return 1;
            }
          }
        }
      }
    }
  }
  return 0;
}
mz_neighbor mz_get_pathables(mz_maze *maze_ptr, mz_position p) {
  mz_position up, down, right, left;
  up.x = p.x;
  up.y = p.y - 1;
  down.x = p.x;
  down.y = p.y + 1;
  right.y = p.y;
  right.x = p.x + 1;
  left.y = p.y;
  left.x = p.x - 1;
  mz_neighbor nb;
  nb.n = 0;
  if (mz_is_pathable(maze_ptr, up) && can_path_v(&p)) {
    nb.paths[nb.n] = up;
    nb.n++;
  }
  if (mz_is_pathable(maze_ptr, down) && can_path_v(&p)) {
    nb.paths[nb.n] = down;
    nb.n++;
  }
  if (mz_is_pathable(maze_ptr, right) && can_path_h(&p)) {
    nb.paths[nb.n] = right;
    nb.n++;
  }
  if (mz_is_pathable(maze_ptr, left) && can_path_h(&p)) {
    nb.paths[nb.n] = left;
    nb.n++;
  }
  return nb;
}

mz_maze *mz_new(int x, int y) {
  mz_maze *maze_ptr;
  maze_ptr = malloc(sizeof(mz_maze));
  maze_ptr->size = x * y;
  maze_ptr->grid = malloc(((y * x)) * sizeof(char));
  memset(maze_ptr->grid, MZ_WALL, maze_ptr->size);
  maze_ptr->x = x;
  maze_ptr->y = y;
  maze_ptr->start_pos.x = -1;
  maze_ptr->start_pos.y = -1;
  maze_ptr->end_pos.x = -1;
  maze_ptr->end_pos.y = -1;
  return maze_ptr;
}

int mz_free(mz_maze *maze_ptr) {
  free(maze_ptr->grid);
  return 1;
}

void mz_iterate(mz_maze *maze_ptr,
                void (*callback)(mz_maze *maze_ptr, mz_position pos)) {
  int x;
  for (x = 0; x < maze_ptr->x; x++) {
    int y;
    for (y = 0; y < maze_ptr->y; y++) {
      mz_position pos = {.x = x, .y = y};
      callback(maze_ptr, pos);
    }
  }
}

void _generate(mz_maze *maze_ptr, mz_position point,
               void (*callback)(mz_maze *maze_ptr)) {
  if (mz_is_pathable(maze_ptr, point)) {
    mz_set_tile(maze_ptr, point, MZ_PATH);
    mz_neighbor nb = mz_get_pathables(maze_ptr, point);
    if (callback != NULL) {
      callback(maze_ptr);
    }

    int i;
    if (nb.n) {
      if (random_between(0, 1) == 1) {
        for (i = 0; i < nb.n; i++) {

          _generate(maze_ptr, nb.paths[i], callback);
        }
      } else {
        for (i = nb.n - 1; i >= 0; i--) {

          _generate(maze_ptr, nb.paths[i], callback);
        }
      }
    }
  }
}

void find_random_path(mz_maze *maze_ptr, mz_position *p) {
  do {

    p->x = random_between(0, maze_ptr->x - 1);
    p->y = random_between(0, maze_ptr->y - 1);
  } while (mz_get_tile(maze_ptr, *p) != MZ_PATH);
}
void mz_generate(mz_maze *maze_ptr, void (*callback)(mz_maze *maze_ptr)) {
  mz_position point;
  do {
    point.x = random_between(1, maze_ptr->x - 2);
    point.y = random_between(1, maze_ptr->y - 2);
  } while (!can_path_h(&point) || !can_path_v(&point));
  _generate(maze_ptr, point, callback);
  find_random_path(maze_ptr, &maze_ptr->end_pos);
  find_random_path(maze_ptr, &maze_ptr->start_pos);
}
