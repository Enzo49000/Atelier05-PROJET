#include <stdio.h>

#define WIDTH  20
#define HEIGHT 20

/* Monde : 0 = cellule morte, 1 = cellule vivante */
int world[HEIGHT][WIDTH] = {0};

/* Stable : bloc 2x2 */
void place_bloc(int x, int y)
{
    world[y][x]         = 1;
    world[y][x + 1]     = 1;
    world[y + 1][x]     = 1;
    world[y + 1][x + 1] = 1;
}

/* Oscillateur : clignotant (3 cellules alignées) */
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

/* Affichage du monde */
void world_print(void)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (world[y][x] == 1)
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

