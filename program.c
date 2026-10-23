#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define WIDTH          20
#define HEIGHT         20
#define NB_GENERATIONS 40

/* Monde : unite = etat actuel, dizaine = etat futur */
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

/* Calcule l'etat futur d'une cellule selon l'etat ACTUEL de ses 8 voisines.
   Monde torique : les bords se rejoignent. */
void cell_compute_next_state(int world[HEIGHT][WIDTH], int x, int y)
{
    int neighbors = 0;

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;   /* on ne compte pas la cellule elle-meme */

            int nx = (x + dx + WIDTH) % WIDTH;
            int ny = (y + dy + HEIGHT) % HEIGHT;

            if (cell_cur_state(world, nx, ny))
                neighbors++;
        }
    }

    int next;
    if (cell_cur_state(world, x, y))
        next = (neighbors == 2 || neighbors == 3);   /* survit */
    else
        next = (neighbors == 3);                      /* naissance */

    /* on garde l'unite (etat actuel) et on ecrit le futur dans la dizaine */
    world[y][x] = world[y][x] % 10 + 10 * next;
}

/* Phase 1 : calcul du futur de toutes les cellules */
void world_compute_next_state(void)
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            cell_compute_next_state(world, x, y);
}

/* Phase 2 : le futur devient l'actuel */
void world_apply_next_state(void)
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            world[y][x] = world[y][x] / 10;
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

    for (int gen = 0; gen < NB_GENERATIONS; gen++)
    {
        printf("\033[H\033[J");              /* efface le terminal */
        printf("Generation %d\n", gen);
        world_print();

        world_compute_next_state();          /* phase 1 */
        world_apply_next_state();            /* phase 2 */

        usleep(300000);                      /* pause 0,3 s */
    }
    return 0;
}