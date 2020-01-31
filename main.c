#include "stdlib.h"
#include <ncurses.h>

#include <string.h>
#include <unistd.h>
#include <time.h>

#define COL_RED 1
#define COL_GREEN 2
#define COL_GREY 3
#define COL_BLACK 4
#define COL_RED_BLACK 5

int RUNNING = 1;
int ERROR = 0;
int GENERATING = 1;
int GENERATE_DELAY = 1000;
int _LINES;
int _COLS;

int random_between(int min, int max)
{
    return rand() % ((max + 1) - min) + min;
}

struct position
{
    int x;
    int y;
};
struct neighbor
{
    int n;
    struct position paths[8];
};
struct position pos = {4, 3};
struct position prev_pos = {4, 3};
struct position end_point;

char *map;
char get_tile(int _x, int _y)
{
    return map[_y * _COLS + _x];
}

int is_odd(int n)
{
    if (n % 2 == 0)
    {
        return 1;
    }
    return 0;
}

int can_move_v(struct position *p)
{
    if (is_odd(p->x))
    {
        return 1;
    }
    return 0;
}
int can_move_h(struct position *p)
{
    if (is_odd(p->y))
    {
        return 0;
    }
    return 1;
}

void draw_map()
{
    int _x = 0;
    int _y = 0;
    for (_y = 0; _y < _LINES; _y++)
    {
        for (_x = 0; _x < _COLS; _x++)
        {
            move(_y, _x * 2);
            if (has_colors && get_tile(_x, _y) == '+')
            {
                if (_x == 0 || _y == 0 || _x == _COLS - 1 || _y == _LINES - 1)
                {
                }
                else
                {
                    attron(COLOR_PAIR(COL_BLACK));
                    addch(' ');
                    attroff(COLOR_PAIR(COL_BLACK));
                }
            }
            else if (has_colors && get_tile(_x, _y) == ' ')
            {
                attron(COLOR_PAIR(COL_GREY));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR(COL_GREY));
            }
            else
            {
                addch(get_tile(_x, _y));
            }
            move(_y, _x * 2 + 1);
            if (has_colors && get_tile(_x, _y) == '+')
            {
                if (_x == 0 || _y == 0 || _x == _COLS - 1 || _y == _LINES - 1)
                {
                }
                else
                {
                    attron(COLOR_PAIR(COL_BLACK));
                    addch(' ');
                    attroff(COLOR_PAIR(COL_BLACK));
                }
            }
            else if (has_colors && get_tile(_x, _y) == ' ')
            {
                attron(COLOR_PAIR(COL_GREY));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR(COL_GREY));
            }
            else
            {
                addch(get_tile(_x, _y));
            }
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
    if (pos.y >= _LINES)
    {
        pos.y = _LINES - 1;
    }
    if (pos.x >= _COLS)
    {
        pos.x = _COLS - 1;
    }
    attron(COLOR_PAIR(COL_RED_BLACK));
    move(0, 0);
    hline(ACS_HLINE, COLS - 1);
    vline(ACS_VLINE, LINES - 1);
    addch(ACS_ULCORNER);
    move(LINES - 1, 0);
    hline(ACS_HLINE, COLS - 1);
    addch(ACS_LLCORNER);
    move(0, COLS - 1);
    vline(ACS_VLINE, LINES - 1);
    addch(ACS_URCORNER);
    move(LINES - 1, _COLS - 1);
    addch(ACS_LRCORNER);
    attroff(COLOR_PAIR(COL_RED_BLACK));
    if (!GENERATING)
    {
        move(pos.y, pos.x * 2);
        if (has_colors)
        {
            attron(COLOR_PAIR(COL_RED));
            printw("  ");
            attroff(COLOR_PAIR(COL_RED));
        }
        else
        {
            addch('O');
        }
        move(end_point.y, end_point.x * 2);
        if (has_colors)
        {
            attron(COLOR_PAIR(COL_GREEN));
            printw("  ");
            attroff(COLOR_PAIR(COL_GREEN));
        }
        else
        {
            addch('O');
        }
    }
    move(0, 5);
    if (has_colors)
    {
        attron(COLOR_PAIR(5));
        printw("MazeCurses by Fran6nd.");
        attroff(COLOR_PAIR(5));
    }
    else
    {
        printw("MazeCurses by Fran6nd.");
    }
    move(LINES - 1, COLS - 20);
    if (has_colors)
    {
        attron(COLOR_PAIR(5));
        printw("Press [q] to exit.");
        attroff(COLOR_PAIR(5));
    }
    else
    {
        printw("Press [q] to exit.");
    }
}
int is_on_map(struct position p)
{
    if (p.x >= 0 && p.x < _COLS && p.y >= 0 && p.y < _LINES)
    {
        return 1;
    }
    return 0;
}

void set_tile(int _x, int _y, char _c)
{
    struct position p = {_x, _y};
    map[_y * _COLS + _x] = _c;
}
struct neighbor get_neighboors(struct position p)
{
    struct position up, down, right, left;
    up.x = p.x;
    up.y = p.y - 1;
    down.x = p.x;
    down.y = p.y + 1;
    right.y = p.y;
    right.x = p.x + 1;
    left.y = p.y;
    left.x = p.x - 1;
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
    return nb;
}
struct neighbor get_advanced_neighboors(struct position p)
{
    struct position up, down, right, left;
    up.x = p.x;
    up.y = p.y - 1;
    down.x = p.x;
    down.y = p.y + 1;
    right.y = p.y;
    right.x = p.x + 1;
    left.y = p.y;
    left.x = p.x - 1;
    struct position up_right = {p.x + 1, p.y - 1}, down_right = {p.x + 1, p.y + 1}, down_left = {p.x - 1, p.y + 1}, up_left = {p.x - 1, p.y - 1};

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
    if (is_on_map(up_left))
    {
        nb.paths[nb.n] = up_left;
        nb.n++;
    }
    if (is_on_map(up_right))
    {
        nb.paths[nb.n] = up_right;
        nb.n++;
    }
    if (is_on_map(down_left))
    {
        nb.paths[nb.n] = down_left;
        nb.n++;
    }
    if (is_on_map(down_right))
    {
        nb.paths[nb.n] = down_right;
        nb.n++;
    }
    return nb;
}

int is_pathable(struct position p)
{
    if (is_on_map(p))
    {
        if (get_tile(p.x, p.y) == '+')
        {
            struct neighbor nb1 = get_neighboors(p);
            if (nb1.n == 4)
            {
                int i;
                int j = 0;
                for (i = 0; i < nb1.n; i++)
                {
                    if (get_tile(nb1.paths[i].x, nb1.paths[i].y) == ' ')
                    {
                        j++;
                    }
                }
                if (j <= 1)
                {
                    struct neighbor nb = get_advanced_neighboors(p);
                    if (nb.n == 8)
                    {
                        i = 0;
                        j = 0;
                        for (i = 0; i < nb.n; i++)
                        {
                            if (get_tile(nb.paths[i].x, nb.paths[i].y) == ' ')
                            {
                                j++;
                            }
                        }
                        if (j <= 3)
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
struct neighbor get_pathables(struct position p)
{
    struct position up, down, right, left;
    up.x = p.x;
    up.y = p.y - 1;
    down.x = p.x;
    down.y = p.y + 1;
    right.y = p.y;
    right.x = p.x + 1;
    left.y = p.y;
    left.x = p.x - 1;
    struct neighbor nb;
    nb.n = 0;
    if (is_pathable(up) && can_move_v(&p))
    {
        nb.paths[nb.n] = up;
        nb.n++;
    }
    if (is_pathable(down) && can_move_v(&p))
    {
        nb.paths[nb.n] = down;
        nb.n++;
    }
    if (is_pathable(right) && can_move_h(&p))
    {
        nb.paths[nb.n] = right;
        nb.n++;
    }
    if (is_pathable(left) && can_move_h(&p))
    {
        nb.paths[nb.n] = left;
        nb.n++;
    }
    return nb;
}
void randomize_neighbor(struct neighbor *nb)
{
    if (nb->n > 1)
    {
        int index = random_between(0, nb->n);
        int i2 = random_between(0, nb->n);
        struct position p1;
        struct position p2;
        p1 = nb->paths[index];
        struct position paths2[4];

        int i;
        for (i = 0; i < nb->n; i++)
        {
            paths2[i] = nb->paths[i];
        }
        for (i = index; i < nb->n - 1; i++)
        {
            nb->paths[i] = paths2[i + 1];
        }
        nb->paths[nb->n - 1] = p1;
    }
}

void _generate(struct position point)
{
    if (is_pathable(point))
    {
        set_tile(point.x, point.y, ' ');
        struct neighbor nb = get_pathables(point);
        if (GENERATE_DELAY)
        {
            draw_map();
            refresh();
        }

        /* Want it faster? Remove the following line :) */
        usleep(GENERATE_DELAY);

        int i;
        if (nb.n)
        {
            if (random_between(0, 1) == 1)
            {
                for (i = 0; i < nb.n; i++)
                {

                    _generate(nb.paths[i]);
                }
            }
            else
            {
                for (i = nb.n - 1; i >= 0; i--)
                {

                    _generate(nb.paths[i]);
                }
            }
        }
    }
}
void find_random_path(struct position * p)
{
    do
    {

        p->x = random_between(0, _COLS - 2) + 1;
        p->y = random_between(0, _LINES - 2) + 1;
    } while (get_tile(p->x, p->y) == '+');
}

void generate()
{
    struct position point;
    point.x = random_between(1, _COLS / 2 - 1) * 2;
    point.y = random_between(1, _LINES / 2 - 1) * 2 + 1;
    _generate(point);
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            printf("------------------------------------------\n");
            printf("MazeCurses, a maze game made with ncurses.\n");
            printf("------------------------------------------\n");
            printf("by Fran6nd\n\n");
            printf("Usage:\n");
            printf("./MazeCurses\n");
            printf("./MazeCurses -h to display help.\n");
            printf("./MazeCurses --delay [int] to set the maze's generation's delay (us).\n");
            printf("./MazeCurses --nodelay to set the maze's geneartion as fast as possible.\n");
            return 0;
        }
        else if (strcmp(argv[1], "--nodelay") == 0)
        {
            GENERATE_DELAY = 0;
        }
        else
        {
            printf("Unknown arg \"%s\" see -h for help.\n", argv[1]);
            return 0;
        }
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "--delay") == 0)
        {
            GENERATE_DELAY = atoi(argv[2]);
        }
        else
        {
            printf("Unknown arg \"%s\" see -h for help.\n", argv[1]);
            return 0;
        }
    }
    else if (argc > 3)
    {
        printf("Too many args... see -h for help.\n");
        return 0;
    }

    initscr();
    _COLS = COLS / 2;
    _LINES = LINES;
    curs_set(0);
    if (has_colors)
    {
        start_color();
        init_pair(COL_RED, COLOR_RED, COLOR_RED);
        init_pair(COL_GREEN, COLOR_GREEN, COLOR_GREEN);
        init_pair(COL_GREY, COLOR_BLACK, COLOR_WHITE);
        init_pair(COL_BLACK, COLOR_BLACK, COLOR_BLACK);
        init_pair(COL_RED_BLACK, COLOR_RED, COLOR_BLACK);
    }
    else
    {
        return 0;
    }
    srand(time(NULL));

    map = malloc(((_LINES * _COLS)) * sizeof(char));
begin:
    GENERATING = 1;
    memset(map, '+', _LINES * _COLS);
    generate();
    find_random_path(&pos);
    prev_pos.x = pos.x;
    prev_pos.y = pos.y;
    find_random_path(&end_point);
    GENERATING = 0;

    while (RUNNING)
    {
        prev_pos.x = pos.x;
        prev_pos.y = pos.y;

        draw_map();
        doupdate();
        char c = getch();

        if (c == '\033')
        {
            getch();
            switch (getch())
            {
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
        else if (c == 'q')
        {
            goto quit;
        }
        if (get_tile(pos.x, pos.y) != ' ')
        {
            pos.x = prev_pos.x;
            pos.y = prev_pos.y;
        }
        if (pos.x == end_point.x && pos.y == end_point.y)
        {
            RUNNING = 0;
        }
    }
    RUNNING = 1;
    goto begin;
quit:
    free(map);
    endwin();

    return ERROR;
}