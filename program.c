#include <stdio.h>
#include <stdbool.h>

#define WIDTH  20
#define HEIGHT 20

/* Monde : chaque case contient 2 etats
   unite   = etat actuel (0 morte, 1 vivante)
   dizaine = etat futur  (0 morte, 1 vivante)
   Valeurs possibles : 0, 1, 10, 11 */
int world[HEIGHT][WIDTH] = {0};

/* true si la cellule est vivante MAINTENANT (unite = 1) */
bool cell_cur_state(int world[HEIGHT][WIDTH], int x, int y)
{
    return world[y][x] % 10 == 1;
}

/* true si la cellule sera vivante a la PROCHAINE generation (dizaine = 1) */
bool cell_next_state(int world[HEIGHT][WIDTH], int x, int y)
{
    return world[y][x] / 10 == 1;
}

/* Stable : bloc 2x2 */
void place_bloc(int x, int y)
{
    world[y][x]         = 1;
    world[y][x + 1]     = 1;
    world[y + 1][x]     = 1;
    world[y + 1][x + 1] = 1;
}

/* Oscillateur : clignotant (3 cellules alignees) */
void place_clignotant(int x, int y)
{
    world[y][x]     = 1;
    world[y][x + 1] = 1;
    world[y][x + 2] = 1;
}

/* Vaisseau : planeur */
void place_planeur(int x, int y)
{
    world[y][x + 1]     = 1;
    world[y + 1][x + 2] = 1;
    world[y + 2][x]     = 1;
    world[y + 2][x + 1] = 1;
    world[y + 2][x + 2] = 1;
}

/* Affichage du monde (etat ACTUEL des cellules) */
void world_print(void)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (cell_cur_state(world, x, y))
                printf("# ");
            else
                printf(". ");
        }
        printf("\n");
    }
}

int main(void)
{
    place_bloc(2, 2);
    place_clignotant(10, 3);
    place_planeur(3, 10);

    world_print();
    return 0;
}