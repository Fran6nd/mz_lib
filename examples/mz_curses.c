#include "stdlib.h"
#include <ncurses.h>

#include <string.h>
#include <unistd.h>
#include <time.h>

#include "mz_lib.h"

/* Defining all collors used. */

#define COL_RED 1
#define COL_GREEN 2
#define COL_GREY 3
#define COL_BLACK 4
#define COL_RED_BLACK 5

/* Here are globals. */

int RUNNING = 1;
int ERROR = 0;
int GENERATING = 1;
int GENERATE_DELAY = 1000;
int _LINES;
int _COLS;

mz_position pos = {4, 3};
mz_position prev_pos = {4, 3};
mz_position end_point;

mz_maze maze;

void draw_map()
{
    int _x = 0;
    int _y = 0;
    for (_y = 0; _y < _LINES; _y++)
    {
        for (_x = 0; _x < _COLS; _x++)
        {
            move(_y, _x * 2);
            mz_position p1 = {_x, _y};
            if (has_colors && mz_get_tile(&maze, p1) == MZ_WALL)
            {
                if (!(_x == 0 || _y == 0 || _x == _COLS - 1 || _y == _LINES - 1))
                {
                    attron(COLOR_PAIR(COL_BLACK));
                    addch(' ');
                    attroff(COLOR_PAIR(COL_BLACK));
                }
            }
            else if (has_colors && mz_get_tile(&maze, p1) == MZ_PATH)
            {
                attron(COLOR_PAIR(COL_GREY));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR(COL_GREY));
            }
            else
            {
                addch(mz_get_tile(&maze, p1));
            }
            move(_y, _x * 2 + 1);
            if (has_colors && mz_get_tile(&maze, p1) == MZ_WALL)
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
            else if (has_colors && mz_get_tile(&maze, p1) == MZ_PATH)
            {
                attron(COLOR_PAIR(COL_GREY));
                addch(ACS_CKBOARD);
                attroff(COLOR_PAIR(COL_GREY));
            }
            else
            {
                addch(mz_get_tile(&maze, p1));
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
    move(LINES - 1, COLS - 1);
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

void during_generation(mz_maze *maze)
{
    if (GENERATE_DELAY > 0)
    {
        draw_map();
        usleep(GENERATE_DELAY);
        refresh();
    }
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
            printf("./MazeCurses --delay [int] to set the maze's generation's "
                   "delay (us).\n");
            printf("./MazeCurses --nodelay to set the maze's geneartion as"
                   " fast as possible.\n");
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

begin:
    mz_new(&maze, _COLS, _LINES);
    GENERATING = 1;
    mz_generate(&maze, &during_generation);
    GENERATING = 0;
    pos = maze.start_pos;
    end_point = maze.end_pos;
    prev_pos.x = pos.x;
    prev_pos.y = pos.y;

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
        if (mz_get_tile(&maze, pos) != MZ_PATH)
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
    mz_free(&maze);
    goto begin;
quit:
    mz_free(&maze);
    endwin();

    return ERROR;
}
