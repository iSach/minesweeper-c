#include "Grid.h"
#include <stdio.h>
#include <stdlib.h>

struct Grid_t {
  int width;
  int height;
  int mines_amount;
  int **mine_grid;   // -1 = pas encore initialisé, 0 = pas de mine, 1 = mine
  int **flag_grid;   // 0 = pas de drapeau, 1 = drapeau placé
  int **reveal_grid; // 0 = Caché. 1 = Révélé.
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

  /**
   * Ici, on alloue 3 tableaux 2D qui contiennent chacun une information pour
   * chaque case du tableau. Un tableau pour les mines, un pour les drapeaux, et
   * un pour si c'est révélé ou non.
   */
  grid->mine_grid = malloc(width * sizeof(int *));
  if (grid->mine_grid == NULL) {
    printf("Erreur d'allocation mémoire. Arrêt du programme.");
    exit(-1);
    return NULL;
  }
  grid->flag_grid = malloc(width * sizeof(int *));
  if (grid->flag_grid == NULL) {
    printf("Erreur d'allocation mémoire. Arrêt du programme.");
    exit(-1);
    return NULL;
  }
  grid->reveal_grid = malloc(width * sizeof(int *));
  if (grid->reveal_grid == NULL) {
    printf("Erreur d'allocation mémoire. Arrêt du programme.");
    exit(-1);
    return NULL;
  }

  for (int i = 0; i < width; i++) {
    grid->mine_grid[i] = malloc(height * sizeof(int *));
    if (grid->mine_grid[i] == NULL) {
      printf("Erreur d'allocation mémoire. Arrêt du programme.");
      exit(-1);
      return NULL;
    }
    grid->flag_grid[i] = malloc(height * sizeof(int *));
    if (grid->flag_grid[i] == NULL) {
      printf("Erreur d'allocation mémoire. Arrêt du programme.");
      exit(-1);
      return NULL;
    }
    grid->reveal_grid[i] = malloc(height * sizeof(int *));
    if (grid->reveal_grid[i] == NULL) {
      printf("Erreur d'allocation mémoire. Arrêt du programme.");
      exit(-1);
      return NULL;
    }

    // On insère les valeurs par défaut.
    for (int j = 0; j < height; j++) {
      grid->mine_grid[i][j] = -1;
      grid->flag_grid[i][j] = 0;
      grid->reveal_grid[i][j] = 0;
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
    	free(grid->mine_grid[i]);
    	free(grid->flag_grid[i]);
		free(grid->reveal_grid[i]);
	}
	free(grid->mine_grid);
	free(grid->flag_grid);
	free(grid->reveal_grid);
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
	for (int y = height - 1; y >= 0; y--) {
    	for (int x = 0; x < 2 * width + 1; x++) { // Sépare les lignes.
      		printf("-");
    	}
    	printf("\n");
    	for (int i = 0; i < 2 * width + 1; i++) {
      		if (i % 2 == 0) {
        		printf("|");
      		} else {
				int x = (i - 1) / 2;
				if(gridWon(grid) == 0 || (grid->mine_grid[x][y] < 1)) { // Partie en cours.
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
					if(grid->mine_grid[x][y] == 1) {
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
	if(x < 0 || y < 0 || x > gridWidth(grid) - 1 || y > gridHeight(grid) - 1) {
		printf("\nErreur gridReveal : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	return 0; // TODO
}

int gridValue(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridValue : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}
	if(x < 0 || y < 0 || x > gridWidth(grid) - 1 || y > gridHeight(grid) - 1) {
		printf("\nErreur gridValue : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	if (gridIsExplored(grid, x, y)) { // Case révélée.
		int count = 0;
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(i == 0 && j == 0) continue;
				if(grid->mine_grid[x + i][y + j] == 1) {
					count++;
				}
			}
		}
		return count;
	} else { // Case cachée.
		return -1;
	}
}

int gridIsExplored(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridIsExplored : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return 0;
	}
	if(x < 0 || y < 0 || x > gridWidth(grid) - 1 || y > gridHeight(grid) - 1) {
		printf("\nErreur gridIsExplored : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	return grid->reveal_grid[x][y];
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
	if(x < 0 || y < 0 || x > gridWidth(grid) - 1 || y > gridHeight(grid) - 1) {
		printf("\nErreur gridIsFlagged : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return 0;
	}

	return grid->flag_grid[x][y];
}
void gridSetFlag(Grid *grid, int x, int y) {
	if(grid == NULL) {
		printf("\nErreur gridSetFlag : Pointeur nul passé en argument. Arrêt du programme.\n");
		exit(-1);
		return;
	}
	if(x < 0 || y < 0 || x > gridWidth(grid) - 1 || y > gridHeight(grid) - 1) {
		printf("\nErreur gridSetFlag : Position impossible passée en argument : (%d, %d). Arrêt du programme.\n", x, y);
		exit(-1);
		return;
	}

	if(!gridIsExplored(grid, x, y)) { // Marquer une case explorée est inutile.
		grid->flag_grid[x][y] = 1;
	}
}

// TODO
int gridWon(Grid *grid) {
	return 0;
}
