#include "stdlib.h"
#include <ncurses.h>

#include <string.h>
#include <time.h>

int running = 1;
int error  = 0;

struct position
{
    int x;
    int y;
};
struct neighbor
{
    int n;
    struct position paths[4];
};
struct position pos = {0, 0};
struct position prev_pos = {0, 0};

char *map;
char get_tile(int _x, int _y)
{
    return map[_y * COLS + _x];
}

int is_odd(int n)
{
    if (n % 2 == 0)
    {
        return 1;
    }
    return 0;
}
void fix_map()
{
    int _x = 0;
    int _y = 0;
    for (_y = 0; _y < LINES; _y++)
    {
        for (_x = 0; _x < COLS; _x++)
        {

            if (get_tile(_x, _y) == '+')
            {
            }
        }
    }
}
int can_move_h(struct position *p)
{
    if (is_odd(p->x))
    {
        return 1;
    }
    return 0;
}
int can_move_v(struct position *p)
{
    if (is_odd(p->y))
    {
        return 0;
    }
    return 1;
}

void draw_map()
{
    clear();
    int _x = 0;
    int _y = 0;
    for (_y = 0; _y < LINES; _y++)
    {
        for (_x = 0; _x < COLS; _x++)
        {
            //mvprintw(_y, _x, get_tile(_x, _y));
            move(_y, _x);
            addch(get_tile(_x, _y));
        }
    }
    if (pos.y < 0)
    {
        pos.y = 0;
    }
    if (pos.x < 0)
    {
        pos.x = 0;
    }
    if (pos.y >= LINES)
    {
        pos.y = LINES - 1;
    }
    if (pos.x >= COLS)
    {
        pos.x = COLS - 1;
    }
    move(pos.y, pos.x);
    addch('O');
}
int is_on_map(struct position p)
{
    if (p.x >= 0 && p.x < COLS && p.y >= 0 && p.y < LINES)
    {
        return 1;
    }
    return 0;
}

void set_tile(int _x, int _y, char _c)
{
    map[_y * COLS + _x] = _c;
}
struct neighbor get_neighboors(struct position p)
{
    struct position up, down, right, left;
    up.x = p.x;
    up.y = p.y - 1;
    down.x = p.x;
    down.y = p.y + 1;
    right.y = right.y;
    right.x = right.x + 1;
    left.y = p.y;
    left.x = left.x - 1;
    struct neighbor nb;
    nb.n = 0;
    if (is_on_map(up))
    {
        nb.paths[nb.n] = up;
        nb.n++;
    }
    if (is_on_map(down))
    {
        nb.paths[nb.n] = down;
        nb.n++;
    }
    if (is_on_map(right))
    {
        nb.paths[nb.n] = right;
        nb.n++;
    }
    if (is_on_map(left))
    {
        nb.paths[nb.n] = left;
        nb.n++;
    }
}
int is_pathable(struct position p)
{
    if (is_on_map(p))
    {
        struct neighbor nb = get_neighboors(p);
        int i;
        int j = 0;
        for (i = 0; i < nb.n; i++)
        {
            if (get_tile(nb.paths[i].x, nb.paths[i].y) != ' ')
            {
                j++;
            }
        }
        if (j > 1)
        {
            return 0;
        }
    }
    return 1;
}
struct neighbor get_pathables(struct position p)
{
    struct position up, down, right, left;
    up.x = p.x;
    up.y = p.y - 1;
    down.x = p.x;
    down.y = p.y + 1;
    right.y = right.y;
    right.x = right.x + 1;
    left.y = p.y;
    left.x = left.x - 1;
    struct neighbor nb;
    nb.n = 0;
    if (is_pathable(up) && can_move_h(&p))
    {
        nb.paths[nb.n] = up;
        nb.n++;
    }
    if (is_pathable(down) && can_move_h(&p))
    {
        nb.paths[nb.n] = down;
        nb.n++;
    }
    if (is_pathable(right) && can_move_v(&p))
    {
        nb.paths[nb.n] = right;
        nb.n++;
    }
    if (is_pathable(left) && can_move_v(&p))
    {
        nb.paths[nb.n] = left;
        nb.n++;
    }
}
void _generate(struct position point)
{
    set_tile(point.x, point.y, ' ');
    struct neighbor nb = get_pathables(point);
    int i;
    for (i = 0; i < nb.n; i++)
    {
        _generate(nb.paths[i]);
    }
    if (nb.n < 0)
    {
        set_tile(2, 2, '*');
        running = 0;
        error = nb.n;
    }
}
void generate()
{
    struct position prev_point = {0, 0};
    struct position point = {15, 15};
    _generate(point);
}

int random_between(int min, int max)
{
    return rand() % ((max + 1) - min) + min;
}

int main(void)
{
    srand(time(NULL));
    WINDOW *boite;

    initscr();
    curs_set(0);
    map = malloc(((LINES * COLS)) * sizeof(char));
    memset(map, '+', LINES * COLS);
    generate();
    //memset(map, (LINES * COLS), 'z');

    while (running)
    {
        prev_pos.x = pos.x;
        prev_pos.y = pos.y;

        //printw("Le terminal actuel comporte %d lignes et %d colonnes\n", LINES, COLS);
        draw_map();
        refresh(); // Rafraîchit la fenêtre par défaut (stdscr) afin d'afficher le message
        char c = getch();
        //if (c != 410) // 410 est le code de la touche générée lorsqu'on redimensionne le terminal
        //    break;

        if (c == '\033')
        {            // if the first value is esc
            getch(); // skip the [
            switch (getch())
            { // the real value
            case 'A':
                pos.y--;
                break;
            case 'B':
                pos.y++;
                break;
            case 'C':
                pos.x++;
                break;
            case 'D':
                pos.x--;
                break;
            }
        }
        if (get_tile(pos.x, pos.y) != ' ')
        {
            pos.x = prev_pos.x;
            pos.y = prev_pos.y;
        }
    }
    free(map);
    endwin();

    return error;
}