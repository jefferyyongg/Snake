/*
    Snake Spel
    -------------------------

    [Jeffery Yong (16385179)]

    De speler bestuurt een slang ('🚀') die appels ('🌙') moet eten om te groeien.
    Het spel eindigt wanneer de slang tegen zichzelf of de muren ('🚧') botst.

    Gebruik: ./snake

    Functionaliteit:
    - Beweging van de slang met pijltjestoetsen
    - Automatisch groeien bij het eten van appels
    - Score bijhouden op basis van gegeten appels
    - Botsingsdetectie met muren en eigen lichaam
    - Afsluiten met ESC toets
 */

#define _POSIX_C_SOURCE 199309L
#define _XOPEN_SOURCE 600
#define _XOPEN_SOURCE_EXTENDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>
#include "snake.h"

int collision(pos a, pos b)
{
    if (a.x == b.x && a.y == b.y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int collision_snake_body(pos p, pos *body, int score)
{
    for (int i = 0; i < score; i++)
    {
        if (collision(p, body[i]))
        {
            return 1;
        }
    }
    return 0;
}

int verwerk_input(WINDOW *win, pos *direction)
{
    int input = wgetch(win);
    switch (input)
    {
    case KEY_LEFT:
        if (direction->x == 1)
        {
            break;
        }
        direction->x = -1;
        direction->y = 0;
        break;
    case KEY_RIGHT:
        if (direction->x == -1)
        {
            break;
        }
        direction->x = 1;
        direction->y = 0;
        break;
    case KEY_UP:
        if (direction->y == 1)
        {
            break;
        }
        direction->x = 0;
        direction->y = -1;
        break;
    case KEY_DOWN:
        if (direction->y == -1)
        {
            break;
        }
        direction->x = 0;
        direction->y = 1;
        break;
    case 27:
        return 0;
    }
    return 1;
}

void update_snake(pos *head, pos direction, pos *apple, pos *body, int *score,
                  int screen_width, int screen_height)
{
    if (head->x == apple->x && head->y == apple->y)
    {
        (*score)++;
        apple->x = rand() % (screen_width - 1);
        apple->y = rand() % (screen_height - 1);
    }

    for (int i = *score; i > 0; i--)
    {
        body[i] = body[i - 1];
    }
    body[0] = *head;
    head->x += direction.x;
    head->y += direction.y;
}

state check_game_over(pos head, pos *body, int score, int screen_width, int screen_height)
{
    if (collision_snake_body(head, body, score) ||
        (head.x < 1 || head.y < 1) ||
        (head.x > screen_width - 2 || head.y > screen_height - 2))
    {
        return DEAD;
    }
    return PLAYING;
}

void teken_spel(pos head, pos apple, pos *body, int score,
                int screen_width, int screen_height)
{
    erase();
    mvprintw(41, 36, "SCORE: %d", score);
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            if (i == 0 || j == 0 || i == screen_height - 1 || j == screen_width - 1)
            {
                mvaddwstr(i, j * 2, L"🚧");
            }
        }
    }

    mvaddwstr(apple.y, apple.x * 2, L"🌙");
    for (int i = 0; i < score; i++)
    {
        mvaddwstr(body[i].y, body[i].x * 2, L"🔥");
    }
    mvaddwstr(head.y, head.x * 2, L"🚀");
}

void speel_snake(WINDOW *win)
{
    int screen_height = 40;
    int screen_width = 40;
    int score = 0;
    pos body[1521];
    state game_state = PLAYING;

    pos head = {20, 20};
    pos direction = {1, 0};
    pos apple = {rand() % (screen_width - 1), rand() % (screen_height - 1)};

    while (game_state == PLAYING)
    {
        if (!verwerk_input(win, &direction))
        {
            return;
        }

        update_snake(&head, direction, &apple, body, &score, screen_width, screen_height);
        game_state = check_game_over(head, body, score, screen_width, screen_height);
        teken_spel(head, apple, body, score, screen_width, screen_height);
        usleep(80000);
    }
}

int main(void)
{
    WINDOW *win = initscr();
    setlocale(LC_ALL, "");
    keypad(win, true);
    nodelay(win, true);
    curs_set(0);

    speel_snake(win);

    endwin();
    return 0;
}