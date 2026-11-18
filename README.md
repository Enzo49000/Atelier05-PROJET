# Le jeu de la vie : version à deux joueurs

Jeu en tour par tour pour la console, basé sur le jeu de la vie de John Conway.

## Lancer le jeu

```
gcc -Wall -Wextra -o program program.c
./program
```

## Principe

Deux joueurs (joueur 1 = `X`, joueur 2 = `O`) posent des cellules sur une grille. Après chaque
tour, la grille évolue selon les règles de Conway. Le joueur qui a le plus de cellules à la fin
de la partie gagne.

## Règles du jeu

- La partie dure 10 tours.
- À chaque tour, chaque joueur pose 3 cellules (4 lors d'un tour de renfort, voir plus bas),
  l'un après l'autre, en tapant ses coordonnées (x puis y).
- Le joueur 1 pose dans la moitié gauche de la grille (x de 0 à 9), le joueur 2 dans la moitié
  droite (x de 10 à 19). On ne peut pas poser sur une case déjà occupée. Les lignes (y) vont
  de 0 à 9.
- Quand les deux joueurs ont posé, la grille évolue d'une génération (deux lors d'un tour
  accéléré, voir plus bas).
- Après le dernier tour, le joueur qui a le plus de cellules vivantes gagne. En cas d'égalité,
  la partie est nulle.

## Règles de Conway utilisées

- Une cellule morte qui a exactement 3 voisines vivantes naît.
- Une cellule vivante qui a 2 ou 3 voisines vivantes survit.
- Dans tous les autres cas, une cellule vivante meurt (isolement ou surpopulation).
- Une cellule qui naît appartient au joueur majoritaire parmi ses 3 parents (il y a toujours
  une majorité, car 3 parents ne peuvent pas se partager à égalité entre 2 joueurs). Une
  cellule qui survit reste à son propriétaire.
- Le voisinage d'une cellule est constitué des 8 cellules qui l'entourent (horizontalement,
  verticalement et en diagonale). Toutes les cellules sont mises à jour simultanément.

## Règles spéciales

- **Tour accéléré** : tous les 3 tours (tours 3, 6 et 9), la grille évolue de 2 générations
  au lieu d'une. Les joueurs doivent donc anticiper plus loin. Le jeu l'annonce au début du
  tour concerné.
- **Ordre de jeu** : le joueur 1 commence les tours impairs, le joueur 2 commence les tours
  pairs. Celui qui joue en dernier voit les cellules de l'autre avant de poser les siennes,
  c'est un avantage : l'alternance le partage équitablement sur les 10 tours.
- **Tour de renfort** : tous les 4 tours (tours 4 et 8), chaque joueur pose 1 cellule de plus,
  soit 4 cellules au lieu de 3. Les joueurs peuvent donc garder une bonne position en réserve
  pour ces tours. Le jeu l'annonce au début du tour concerné. Les tours de renfort ne coïncident
  jamais avec les tours accélérés (3, 6 et 9).

Sans ces règles spéciales, l'évolution de la grille est exactement celle de Conway.

## Choix de conception

- Grille de 20 colonnes sur 10 lignes (constantes `WIDTH` et `HEIGHT`).
- Grille torique : le bord droit est relié au bord gauche, le haut est relié au bas.
- Chaque case stocke deux états dans un entier : l'unité est l'état actuel (0 = morte,
  1 = joueur 1, 2 = joueur 2) et la dizaine est l'état futur. Cela permet de mettre toute la
  grille à jour en même temps.
- Toutes les constantes (nombre de tours, cellules par tour, taille de la grille, fréquence des
  tours accélérés et des tours de renfort) sont des `#define` en début de `program.c`.
- La logique du jeu (calcul de la grille, validité d'une pose, score) est séparée des
  entrées/sorties (affichage et saisie) dans `program.c`.

## Contenu du dépôt

- `program.c` : le code source du jeu
- `README.md` : les règles du jeu et les choix de conception
- `LLM.md` : l'utilisation de l'IA dans le projet
- `flows/` : les logigrammes (Flow-C) des fonctions qui contiennent de la logique de jeu :
  `world_evolve`, `world_compute_next_state`, `world_apply_next_state`,
  `cell_compute_next_state`, `can_place`, `get_winner`, `generations_for_turn`,
  `cells_for_turn` et `world_print`