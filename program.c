#include <stdio.h>
#include <stdbool.h>

/* ---------- CONSTANTES ---------- */

#define WIDTH                20
#define HEIGHT               10

/* Etats d'une cellule (et numeros des joueurs) */
#define DEAD                 0
#define PLAYER_1             1
#define PLAYER_2             2
#define NB_PLAYERS           2
#define NB_STATES            3
#define NO_WINNER            0

/* Regles de Conway */
#define BIRTH_NEIGHBORS      3
#define SURVIVE_MIN          2
#define SURVIVE_MAX          3
#define NEIGHBOR_OFFSET      1   /* les voisines vont de -1 a +1 autour de la cellule */

/* Regles du jeu */
#define NB_TURNS             10
#define CELLS_PER_TURN       3
#define GENERATIONS_PER_TURN 1
#define ZONE_WIDTH           (WIDTH / NB_PLAYERS)   /* largeur de la zone de chaque joueur */

/* Codage : unite = etat actuel, dizaine = etat futur */
#define ENCODING_BASE        10

/* Symbole affiche pour chaque etat : DEAD, PLAYER_1, PLAYER_2 */
const char symbols[NB_STATES] = {'.', 'X', 'O'};

/* Monde : world[y][x] = etat futur * 10 + etat actuel */
int world[HEIGHT][WIDTH] = {0};

/* ---------- LOGIQUE DU JEU ---------- */

/* Etat ACTUEL de la cellule : DEAD, PLAYER_1 ou PLAYER_2 */
int cell_cur_state(int x, int y)
{
    return world[y][x] % ENCODING_BASE;
}

/* Etat FUTUR de la cellule */
int cell_next_state(int x, int y)
{
    return world[y][x] / ENCODING_BASE;
}

/* Calcule l'etat futur d'une cellule selon l'etat ACTUEL de ses 8 voisines.
   Monde torique : les bords se rejoignent.
   Une naissance prend le proprietaire majoritaire parmi les 3 parents. */
void cell_compute_next_state(int x, int y)
{
    int count[NB_STATES] = {0};   /* count[s] = nombre de voisines dans l'etat s */

    for (int dy = -NEIGHBOR_OFFSET; dy <= NEIGHBOR_OFFSET; dy++)
    {
        for (int dx = -NEIGHBOR_OFFSET; dx <= NEIGHBOR_OFFSET; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;   /* on ne compte pas la cellule elle-meme */

            int nx = (x + dx + WIDTH) % WIDTH;
            int ny = (y + dy + HEIGHT) % HEIGHT;

            count[cell_cur_state(nx, ny)]++;
        }
    }

    int neighbors = count[PLAYER_1] + count[PLAYER_2];
    int current = cell_cur_state(x, y);
    int next = DEAD;

    if (current != DEAD)
    {
        if (neighbors >= SURVIVE_MIN && neighbors <= SURVIVE_MAX)
            next = current;          /* survie : garde son proprietaire */
    }
    else if (neighbors == BIRTH_NEIGHBORS)
    {
        if (count[PLAYER_1] > count[PLAYER_2])
            next = PLAYER_1;         /* naissance : proprietaire majoritaire */
        else
            next = PLAYER_2;
    }

    /* on garde l'unite (etat actuel) et on ecrit le futur dans la dizaine */
    world[y][x] = current + ENCODING_BASE * next;
}

/* Phase 1 : calcul du futur de toutes les cellules */
void world_compute_next_state(void)
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            cell_compute_next_state(x, y);
}

/* Phase 2 : le futur devient l'actuel */
void world_apply_next_state(void)
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            world[y][x] = cell_next_state(x, y);
}

/* Fait evoluer la grille de GENERATIONS_PER_TURN generations */
void world_evolve(void)
{
    for (int g = 0; g < GENERATIONS_PER_TURN; g++)
    {
        world_compute_next_state();
        world_apply_next_state();
    }
}

/* true si la colonne x fait partie de la zone du joueur */
bool x_in_player_zone(int x, int player)
{
    return x / ZONE_WIDTH == player - PLAYER_1;
}

/* true si le joueur peut poser en (x, y) : dans la grille, dans sa zone,
   sur une case morte */
bool can_place(int x, int y, int player)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return false;
    if (!x_in_player_zone(x, player))
        return false;
    return cell_cur_state(x, y) == DEAD;
}

/* Pose une cellule du joueur (on suppose can_place deja verifie) */
void place_cell(int x, int y, int player)
{
    world[y][x] = player;
}

/* Nombre de cellules vivantes d'un joueur */
int count_cells(int player)
{
    int total = 0;

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            if (cell_cur_state(x, y) == player)
                total++;

    return total;
}

/* Gagnant de la partie : PLAYER_1, PLAYER_2 ou NO_WINNER en cas d'egalite */
int get_winner(void)
{
    int score_1 = count_cells(PLAYER_1);
    int score_2 = count_cells(PLAYER_2);

    if (score_1 > score_2)
        return PLAYER_1;
    if (score_2 > score_1)
        return PLAYER_2;
    return NO_WINNER;
}

/* ---------- ENTREES / SORTIES ---------- */

/* Affichage du monde (etat ACTUEL des cellules) avec les numeros de colonnes et de lignes */
void world_print(void)
{
    printf("    ");
    for (int x = 0; x < WIDTH; x++)
        printf("%-3d", x);
    printf("\n");

    for (int y = 0; y < HEIGHT; y++)
    {
        printf("%2d  ", y);
        for (int x = 0; x < WIDTH; x++)
            printf("%c  ", symbols[cell_cur_state(x, y)]);
        printf("\n");
    }
}

void print_scores(void)
{
    printf("Score -> Joueur %d (%c) : %d   Joueur %d (%c) : %d\n",
           PLAYER_1, symbols[PLAYER_1], count_cells(PLAYER_1),
           PLAYER_2, symbols[PLAYER_2], count_cells(PLAYER_2));
}

/* Demande un nombre entier ; recommence tant que la saisie n'est pas un nombre */
int ask_number(int player, char axis)
{
    int value;

    printf("Joueur %d, %c ? > ", player, axis);
    while (scanf("%d", &value) != 1)
    {
        int c;
        do
        {
            c = getchar();   /* on vide la ligne mal saisie */
        } while (c != '\n' && c != EOF);
        printf("Entrez un nombre entier : ");
    }
    return value;
}

/* Fait poser CELLS_PER_TURN cellules a un joueur */
void player_place_cells(int player)
{
    int first_x = (player - PLAYER_1) * ZONE_WIDTH;

    for (int i = 1; i <= CELLS_PER_TURN; i++)
    {
        int x, y;
        bool valid;

        do
        {
            printf("\nJoueur %d (%c) : cellule %d/%d\n", player, symbols[player], i, CELLS_PER_TURN);
            x = ask_number(player, 'x');
            y = ask_number(player, 'y');

            valid = can_place(x, y, player);
            if (!valid)
                printf("Impossible : choisis une case vide dans ta zone (x de %d a %d, y de 0 a %d).\n",
                       first_x, first_x + ZONE_WIDTH - 1, HEIGHT - 1);
        } while (!valid);

        place_cell(x, y, player);
        world_print();
    }
}

void print_end_of_game(void)
{
    int winner = get_winner();

    printf("\n===== Fin de la partie =====\n");
    world_print();
    print_scores();

    if (winner == NO_WINNER)
        printf("Egalite !\n");
    else
        printf("Le joueur %d gagne !\n", winner);
}

/* ---------- PROGRAMME PRINCIPAL ---------- */

int main(void)
{
    for (int turn = 1; turn <= NB_TURNS; turn++)
    {
        printf("\n===== Tour %d/%d =====\n", turn, NB_TURNS);
        world_print();
        print_scores();

        for (int player = PLAYER_1; player <= PLAYER_2; player++)
            player_place_cells(player);

        world_evolve();
        printf("\nLa grille evolue...\n");
    }

    print_end_of_game();
    return 0;
}