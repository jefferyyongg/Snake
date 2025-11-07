#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>

/* Structuur voor het opslaan van posities */
typedef struct pos_snake
{
    int x;
    int y;
} pos;

/* Enum voor de spelstatus */
typedef enum
{
    PLAYING,
    DEAD
} state;

/* Controleert of twee posities gelijk zijn.

   - a, b: de te vergelijken posities

   Output: 1 als de posities gelijk zijn, anders 0
*/
int collision(pos a, pos b);

/* Controleert of een positie botst met het lichaam van de slang.

   - p: de te controleren positie
   - body: array met lichaamssegmenten van de slang
   - score: huidige lengte van de slang

   Output: 1 als er een botsing is, anders 0
*/
int collision_snake_body(pos p, pos *body, int score);

/* Verwerkt de gebruikersinvoer en past de richting aan.

   - win: het ncurses venster
   - direction: pointer naar de huidige richting

   Output: 0 als het spel moet stoppen (ESC), anders 1

   Side-effects:
   - Wijzigt de waarden van direction->x en direction->y op basis van toetsinvoer
   - Leest een karakter uit het ncurses venster (consumeert de input buffer)
*/
int verwerk_input(WINDOW *win, pos *direction);

/* Update de positie van de slang en controleert op appel.

   - head: pointer naar de koppositie
   - direction: de bewegingsrichting
   - apple: pointer naar de appelpositie
   - body: array met lichaamssegmenten
   - score: pointer naar de huidige score
   - screen_width, screen_height: afmetingen van het speelveld

   Side-effects:
   - Wijzigt head->x en head->y op basis van de richting
   - Verschuift alle lichaamssegmenten in de body array
   - Verhoogt *score bij het eten van een appel
   - Verplaatst de appel naar een nieuwe willekeurige positie bij het eten
*/
void update_snake(pos *head, pos direction, pos *apple, pos *body, int *score,
                  int screen_width, int screen_height);

/* Controleert of het spel voorbij is.

   - head: de koppositie
   - body: array met lichaamssegmenten
   - score: huidige score
   - screen_width, screen_height: afmetingen van het speelveld

   Output: DEAD als het spel voorbij is, anders PLAYING
*/
state check_game_over(pos head, pos *body, int score, int screen_width, int screen_height);

/* Tekent het speelveld met slang, appel en rand.
   - head: de koppositie
   - apple: de appelpositie
   - body: array met lichaamssegmenten
   - score: huidige score
   - screen_width, screen_height: afmetingen van het speelveld

   Side-effects:
   - Wist het hele scherm met erase()
   - Tekent nieuwe graphics op het ncurses venster
   - Verplaatst de cursor voor het tekenen
*/
void teken_spel(pos head, pos apple, pos *body, int score,
                int screen_width, int screen_height);

/* Speelt het Snake spel tot de speler verliest of stopt.

   - win: het ncurses venster

   Side-effects:
   - Initialiseert en wijzigt spelstatus variabelen
   - Roept andere functies aan die het scherm updaten
   - Gebruikt usleep() om de game loop te vertragen
   - Genereert willekeurige posities voor de appel
*/
void speel_snake(WINDOW *win);

#endif