#include "stdlib.h"
#include <ncurses.h>

#include <string.h>
#include <unistd.h>
#include <time.h>

int running = 1;
int error = 0;
int generating = 1;

char msg[100];

int random_between(int min, int max)
{
    return rand() % ((max + 1) - min) + min;
}

void set_msg(char *s)
{
    memset(msg, 0, 100);
    memcpy(msg, s, strlen(s));
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
    if (!generating)
    {
        move(pos.y, pos.x);
        if (has_colors)
        {
            attron(COLOR_PAIR(1));
            addch('O');
            attroff(COLOR_PAIR(1));
        }
        else
        {
            addch('O');
        }
        move(end_point.y, end_point.x);
        if (has_colors)
        {
            attron(COLOR_PAIR(2));
            addch('O');
            attroff(COLOR_PAIR(2));
        }
        else
        {
            addch('O');
        }
    }
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
    struct position p = {_x, _y};
    //if(is_on_map(p))
    map[_y * COLS + _x] = _c;
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
void fix_map()
{
    int _x = 0;
    int _y = 0;
    for (_y = 0; _y < LINES; _y++)
    {
        for (_x = 0; _x < COLS; _x++)
        {

            if (get_tile(_x, _y) == ' ')
            {
                set_tile(_x, _y, 254);
            }
            if (get_tile(_x, _y) == '+')
            {
                set_tile(_x, _y, ' ');
            }

            /*
                struct neighbor nb = get_advanced_neighboors(p);
                struct neighbor wall;
                wall.n = 0;
                int i = 0;
                int j = 0;
                for(; i<nb.n; i++)
                {
                    struct position p1;
                    p1 = nb.paths[i];
                    char c = get_tile(p1.x, p1.y);
                    if(c == '+' || c == '-' || c=='|'){
                        wall.paths[wall.n] = p1;
                        wall.n++;

                    }
                }
                {
                    if (wall.n == 1)
                    {
                        if (wall.paths[0].x == p.x)
                        {
                            set_tile(_x, _y, '-');
                        }
                        else
                        {
                            set_tile(_x, _y, '|');
                        }
                    }
                    else if (wall.n == 2)
                    {
                        int v = 0;
                        int h = 0;
                        if (wall.paths[0].x != p.x)
                        {
                            h++;
                        }
                        if (wall.paths[1].x != p.x)
                        {
                            h++;
                        }
                        if (wall.paths[0].y != p.y)
                        {
                            v++;
                        }
                        if (wall.paths[1].y != p.y)
                        {
                            v++;
                        }
                        if(v == 2){
                            set_tile(_x, _y, '|');
                        }
                        if(h == 2){
                            set_tile(_x, _y, '-');
                        }
                    }
                }*/
        }
    }
}
void _generate(struct position point)
{
    if (is_pathable(point))
    {
        set_msg("Started!");
        set_tile(point.x, point.y, ' ');
        struct neighbor nb = get_pathables(point);
        //randomize_neighbor(&nb);
        draw_map();
        refresh();
        usleep(3000);

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
struct position find_random_path()
{
    struct position p;
    do{
    
        p.x = random_between(0, COLS - 2) + 1;
    p.y = random_between(0, LINES - 2) + 1;
    }while(get_tile(p.x, p.y) == '+');
    return p;

}

void generate()
{
    struct position point;
    point.x = random_between(0, COLS - 2) + 1;
    point.y = random_between(0, LINES - 2) + 1;
    _generate(point);
}

int main(void)
{
    initscr();
    curs_set(0);
    if (has_colors)
    {
        start_color();
        init_pair(1, COLOR_RED, COLOR_RED);
        init_pair(2, COLOR_GREEN, COLOR_GREEN);
    }
    else
    {
        return 0;
    }
    srand(time(NULL));
    WINDOW *boite;

    map = malloc(((LINES * COLS)) * sizeof(char));
    begin:
    memset(map, '+', LINES * COLS);
    generate();
    pos = find_random_path();
    prev_pos.x = pos.x;
    prev_pos.y = pos.y;
    end_point = find_random_path();
    generating = 0;
    fix_map();

    //memset(map, (LINES * COLS), 'z');

    while (running)
    {
        prev_pos.x = pos.x;
        prev_pos.y = pos.y;

        //printw("Le terminal actuel comporte %d lignes et %d colonnes\n", LINES, COLS);
        draw_map();
        doupdate(); // Rafraîchit la fenêtre par défaut (stdscr) afin d'afficher le message
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
        if (get_tile(pos.x, pos.y) == ' ')
        {
            pos.x = prev_pos.x;
            pos.y = prev_pos.y;
        }
        if(pos.x == end_point.x && pos.y == end_point.y){
            running = 0;
        }
    }
    running = 1;
    goto begin;
    free(map);
    endwin();

    return error;
}