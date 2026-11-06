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
- À chaque tour, chaque joueur pose 3 cellules, l'un après l'autre, en tapant ses coordonnées
  (x puis y).
- Le joueur 1 pose dans la moitié gauche de la grille (x de 0 à 9), le joueur 2 dans la moitié
  droite (x de 10 à 19). On ne peut pas poser sur une case déjà occupée.
- Quand les deux joueurs ont posé, la grille évolue d'une génération.
- Après le dernier tour, le joueur qui a le plus de cellules vivantes gagne. En cas d'égalité,
  la partie est nulle.

## Règles de Conway utilisées

- Une cellule morte qui a exactement 3 voisines vivantes naît.
- Une cellule vivante qui a 2 ou 3 voisines vivantes survit.
- Dans tous les autres cas, une cellule vivante meurt (isolement ou surpopulation).
- Une cellule qui naît appartient au joueur majoritaire parmi ses 3 parents. Une cellule qui
  survit reste à son propriétaire.

## Choix de conception

- Grille de 20 colonnes sur 10 lignes (constantes `WIDTH` et `HEIGHT`).
- Grille torique : le bord droit est relié au bord gauche, le haut est relié au bas.
- Chaque case stocke deux états dans un entier : l'unité est l'état actuel (0 = morte,
  1 = joueur 1, 2 = joueur 2) et la dizaine est l'état futur. Cela permet de mettre toute la
  grille à jour en même temps.
- Toutes les constantes (nombre de tours, cellules par tour, taille de la grille) sont des
  `#define` en début de `program.c`.

## Règle spéciale

(à définir)