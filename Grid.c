#include "Grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static void init_bombs(Grid *grid, int x, int y);
static int valid_pos(Grid *grid, int x, int y);
static int gridIsMine(Grid *grid, int x, int y);

struct Cell_t {
    int isMine; 
    int isRevealed;
    int isFlagged;
};

typedef struct Cell_t Cell;

struct Grid_t {
  int width;
  int height;
  int mines_amount;
  int bombs_set; // 0 si les bombes ont été posées. 1 sinon.
  Cell **cells; // Contient les cases.
};

Grid *gridInit(int width, int height, int nbrBombs) {
  // Puisqu'on ne peut pas avoir de bombes dans un carré de 5x5, si la taille
  // n'est pas d'au moins 6 dans une direction, le jeu est injouable.
  // TODO vérifier cette condition, car joueur pourrait appuyer dans un coin.
  if (height <= 5 && width <= 5) {
    printf("Erreur : La taille donnéene permet pas de jouer. Arrêt du programme.\n");
    exit(-1);
    return NULL;
  }

  // On ne peut pas placer dans le carré de 5x5, donc height*width - 25
  // positions est le maximum de bombes plaçables.
  if (nbrBombs > height * width - 5 * 5) {
    printf("Erreur : Le nombre de bombes est trop élevée pour cette taille de grille. Arrêt du programme.\n");
    exit(-1);
    return NULL;
  }

  if (nbrBombs < 1) {
    printf("Erreur : Nombre de bombes nul ou négatif. Arrêt du programme.\n");
    exit(-1);
    return NULL;
  }

  // Evidemment, les dimensions ne peuvent pas être inférieures à 1...
  if (height < 1 || width < 1) {
    printf("Erreur : Dimensions incorrectes (négatives ou nulles). Arrêt du programme.");
    exit(-1);
    return NULL;
  }

  Grid *grid = malloc(sizeof(Grid));
  if (grid == NULL) {
    printf("Erreur d'allocation mémoire. Arrêt du programme.");
    exit(-1);
    return NULL;
  }

  grid->width = width;
  grid->height = height;
  grid->mines_amount = nbrBombs;
  grid->bombs_set = 0;

  /**
   * Ici, on alloue 3 tableaux 2D qui contiennent chacun une information pour
   * chaque case du tableau. Un tableau pour les mines, un pour les drapeaux, et
   * un pour si c'est révélé ou non.
   */
  grid->cells = malloc(width * sizeof(Cell *));
  if (grid->cells == NULL) {
    printf("Erreur d'allocation mémoire. Arrêt du programme.");
    exit(-1);
    return NULL;
  }

  for (int i = 0; i < width; i++) {
    grid->cells[i] = malloc(height * sizeof(Cell));
    if (grid->cells[i] == NULL) {
      printf("Erreur d'allocation mémoire. Arrêt du programme.");
      exit(-1);
      return NULL;
    }

    // On insère les valeurs par défaut.
    for (int j = 0; j < height; j++) {
		Cell c;
    	c.isMine = 0;
    	c.isFlagged = 0;
		c.isRevealed = 0;
		grid->cells[i][j] = c;
    }
  }

  return grid;
}

void gridFree(Grid *grid) {
	if(grid == NULL) {
		printf("Erreur : Pointeur nul passé en argument. Arrêt du programme.");
		exit(-1);
		return;
	}

  	for (int i = 0; i < grid->height; i++) {
    	free(grid->cells[i]);
	}
	free(grid->cells);
	free(grid);
}

void gridPrint(Grid *grid) {
	if(grid == NULL) {
		printf("Erreur : Pointeur nul passé en argument. Arrêt du programme.");
		exit(-1);
		return;
	}

	int width = gridWidth(grid);
	int height = gridHeight(grid);
	printf(" ");
    	for (int x = 0; x < 2 * width + 1; x++) { // Sépare les lignes.
			if(x % 2)
      		printf("%d ", (x - 1) / 2);
    	}
		printf("\n");
	for (int y = height - 1; y >= 0; y--) {
    	for (int x = 0; x < 2 * width + 1; x++) { // Sépare les lignes.
      		printf("-");
    	}
    	printf("\n");
    	for (int i = 0; i < 2 * width + 1; i++) {
      		if (i % 2 == 0) {
        		printf("|");
				if(i == 2 * width) {
					printf("    %d", y);
				}
      		} else {
				int x = (i - 1) / 2;
				if(gridWon(grid) == 0 || !gridIsMine(grid, x, y)) { // Partie en cours.
					if(gridIsExplored(grid, x, y)) {
						printf("%d", gridValue(grid, x, y));
					} else {
						if(gridIsFlagged(grid, x, y)) {
							printf("F");
						} else {
							printf(" ");
						}
					}
				} else {
					if(gridIsMine(grid, x, y)) {
						printf("X");
					}
				}
      		}
    	}
    	printf("\n");
  	}
  	for (int x = 0; x < 2 * width + 1; x++) { // Sépare les lignes.
    	printf("-");
  	}
    printf("\n");
}

int gridReveal(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridReveal : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}

	if(!valid_pos(grid, x, y)) {
		printf("\nErreur gridReveal : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	printf("Revealing: (%d, %d)\n", x, y);

	grid->cells[x][y].isRevealed = 1;

	if(gridIsMine(grid, x, y)) {
		// Partie perdue. Pas besoin de révéler les cases autour.
		return 1;
	}

	if(!grid->bombs_set) {
		// 1er coup de la partie -> initialiser les bombes.
		init_bombs(grid, x, y);
	}

	int bombs_around = gridValue(grid, x, y);
	if(bombs_around == 0) {
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(i == 0 && j == 0) continue;
				if(!valid_pos(grid, x + i, y + j)) continue;
				if(!gridIsExplored(grid, x + i, y + j)) {
					gridReveal(grid, x + i, y + j);
				}
			}
		}
	}
	return 0;
}

/**
 * Place aléatoirement les bombes là où c'est autorisé.
 * 
 * Paramètres :
 * grid : Grille
 * x, y : Position du 1er coup. On ne met rien dans le carré 5x5 centré en (x, y).
 */
static void init_bombs(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridReveal : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return;
	}

	// Sécurité, init_bombs étant appelé une seule fois en interne quand cette
	// condition n'est pas rempli, cela ne devrait jamais arriver. Mais jamais 
	// trop prudent !
	if(grid->bombs_set) {
		printf("Erreur init_bombs : Bombes déjà initialisées. Arrêt du programme.");
		exit(-1);
		return;
	}
	
	// On génère la seed du RNG
	srand(time(NULL));

	// Placement des bombs
	int i = grid->mines_amount;
	while(i > 0) {
		int rand_x = (int) (rand() % gridWidth(grid));
		int rand_y = (int) (rand() % gridHeight(grid));
		if(abs(rand_x - x) <= 2 && abs(rand_y - y) <= 2) continue; // trop près du départ.
		if(gridIsMine(grid, rand_x, rand_y)) continue; // Bombe déjà mise là.
		if(!valid_pos(grid, rand_x, rand_y)) continue;
		grid->cells[rand_x][rand_y].isMine = 1;
		i--;
	}

	grid->bombs_set = 1;
}

/**
 * Vérifie si la position est possible.
 * Renvoie :
 *    1 si la position est POSSIBLE,
 *    0 sinon.
 */
static int valid_pos(Grid *grid, int x, int y) {
	return !(x < 0 || y < 0 || x > gridWidth(grid) - 1 || y > gridHeight(grid) - 1);
}

int gridValue(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridValue : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}
	if(!valid_pos(grid, x, y)) {
		printf("\nErreur gridValue : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	if (gridIsExplored(grid, x, y)) { // Case révélée.
		int count = 0;
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(i == 0 && j == 0) continue;
				if(!valid_pos(grid, x + i, y + j)) continue;
				if(gridIsMine(grid, x + i, y + j)) {
					count++;
				}
			}
		}
		return count;
	} else { // Case cachée.
		return -1;
	}
}

int gridIsMine(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridIsExplored : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}
	if(!valid_pos(grid, x, y)) {
		printf("\nErreur gridIsExplored : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	return grid->cells[x][y].isMine;
}

int gridIsExplored(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridIsExplored : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}
	if(!valid_pos(grid, x, y)) {
		printf("\nErreur gridIsExplored : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	return grid->cells[x][y].isRevealed;
}

int gridWidth(Grid *g) {
	if(g == NULL) {
		printf("\nErreur gridWidth : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}

	return g->width;
}

int gridHeight(Grid *g) {
	if(g == NULL) {
		printf("\nErreur gridHeight : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}

	return g->height;
}

int gridIsFlagged(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridIsFlagged : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}
	if(!valid_pos(grid, x, y)) {
		printf("\nErreur gridIsFlagged : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	return grid->cells[x][y].isFlagged;
}
void gridSetFlag(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridSetFlag : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return;
	}
	if(!valid_pos(grid, x, y)) {
		printf("\nErreur gridSetFlag : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return;
	}

	if(!gridIsExplored(grid, x, y)) { // Marquer une case explorée est inutile.
		grid->cells[x][y].isFlagged = 1;
	}
}

int gridWon(Grid *grid) {
	int width = gridWidth(grid);
	int height = gridHeight(grid);
	int found_non_mine_unrevealed = 0;
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			if(gridIsMine(grid, x, y) && gridIsExplored(grid, x, y)) {
				return -1; // Une case est une bombe révélée -> Perdu.
			}

			// Si ça se déclenche, il existe au moins une case vide pas encore révélée.
			// On ne peut pas return 0 tout de suite cependant, car il se pourrait 
			// qu'une mine révélée se trouve autre part.
			if(!gridIsMine(grid, x, y) && !gridIsExplored(grid, x, y)) {
				found_non_mine_unrevealed = 1;
			}
		}
	}

	// Si found_non_mine_unrevealed = 1, la partie n'est pas finie donc on renvoie 0. 
	// Sinon on renvoie 1 car aucune bombe n'a été révélée et aucune case vide n'a pas encore été révélée.
	return !found_non_mine_unrevealed;
}
