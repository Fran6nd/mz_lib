#include "stdlib.h"
#include <ncurses.h>

#include <string.h>

struct position {
    int x;
    int y;
};
struct position pos = {0 , 0};
struct position prev_pos = {0 , 0};


char *map;
char get_tile(int _x, int _y)
{
    return map[_y * COLS + _x];
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
    addch('o');
}

void set_tile(int _x, int _y, char _c)
{
    map[_y * COLS + _x] = _c;
}

int main(void)
{
    WINDOW *boite;

    initscr();
    curs_set(0);
    map = malloc(((LINES * COLS)) * sizeof(char));
    memset(map, ' ', LINES * COLS);
    //memset(map, (LINES * COLS), 'z');
    set_tile(5, 5, '+');
    set_tile(5, 8, '/');
    while (1)
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
        if(get_tile(pos.x, pos.y) != ' '){
            pos.x = prev_pos.x;
            pos.y = prev_pos.y;
        }
    }
    free(map);
    endwin();

    return 0;
}