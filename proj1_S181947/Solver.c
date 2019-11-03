#include "Solver.h"
#include "Grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static int valid_pos(Grid *grid, int x, int y);

Move human(Grid *grid) {
	if(grid == NULL) {
		printf("Erreur : Pointeur nul passé en argument. Arrêt du programme.");
		exit(-1);
	}

    Move m;
    m.x = -1;
    m.y = -1;
    m.flag = -2;
    while(m.x == -1) {
        printf("Veuillez entrer une action, et des coordonnées :\n");
        printf("Actions possibles : 1 (placer un drapeau), 0 (révéler une case), -1 (abandonner).\n");
        printf("Format : <action> <x> <y>\n");

        int entered_flag, entered_x, entered_y;
        scanf("%d %d %d", &(entered_flag), &(entered_x), &(entered_y));

        if(entered_x < 0 || entered_y < 0 || entered_x > gridWidth(grid) - 1 || entered_y > gridHeight(grid) - 1) {
            printf("Position invalide. Veuillez réessayer.\n\n");
            continue;
        }
		
        if(entered_flag < -1 || entered_flag > 1) {
            printf("Action invalide. Veuillez réessayer.\n\n");
            continue;
        }

        m.flag = entered_flag;
        m.x = entered_x;
        m.y = entered_y;
    }
    return m;
}

Move heuristic(Grid *grid) {
	if(grid == NULL) {
		printf("Erreur : Pointeur nul passé en argument. Arrêt du programme.");
		exit(-1);
	}

    for(int i = 0; i < gridWidth(grid); i++) {
        for(int j = 0; j < gridHeight(grid); j++) {
            // On a déjà révélé une case, ce n'est donc pas le 1er coup.
            if(gridIsExplored(grid, i, j)) {
                for(int x = 0; x < gridWidth(grid); x++) {
                    for(int y = 0; y < gridHeight(grid); y++) {
						if(gridIsExplored(grid, x, y)) {
							int cell_value = gridValue(grid, x, y);

							if(cell_value == 0) continue;

							int flagged_adj = 0, hidden_adj = 0;
							// On parcourt les cellules adjacentes pour compter les non révélées et marquées.
							for(int a = -1; a <= 1; a++) {
								for(int b = -1; b <= 1; b++) {
									if(a == 0 && b == 0) continue;
									if(!valid_pos(grid, x + a, y + b)) continue;
									if(gridIsFlagged(grid, x + a, y + b)) flagged_adj++;
									else if(!gridIsExplored(grid, x + a, y + b)) hidden_adj++; // Une case flag est évidemment non-révélée, mais ne compte pas dans hidden_adj.
								}
							}

							if(hidden_adj == 0) continue;

							// Révéler une case non révélée qui est adjacente à une case révélée dont la valeur
							// est égale au nombre de cases marquées qui lui sont adjacentes
							if(cell_value == flagged_adj) {
								for(int a = -1; a <= 1; a++) {
									for(int b = -1; b <= 1; b++) {
										if(a == 0 && b == 0) continue;
										if(!valid_pos(grid, x + a, y + b)) continue;
										if(gridIsExplored(grid, x + a, y + b) || gridIsFlagged(grid, x + a, y + b)) continue;
										
										Move m;
										m.x = x + a;
										m.y = y + b;
										m.flag = 0; // On révèle
										return m;
									}
								}
							}

							// Marquer une case non révélée qui est adjacente à une case révélée dont la valeur
							// est égale au nombre de cases marquées et de cases non révélées qui lui sont
							// adjacentes.
							if(cell_value == (flagged_adj + hidden_adj)) {
								for(int a = -1; a <= 1; a++) {
									for(int b = -1; b <= 1; b++) {
										if(a == 0 && b == 0) continue;
										if(!valid_pos(grid, x + a, y + b)) continue;
										if(gridIsExplored(grid, x + a, y + b) || gridIsFlagged(grid, x + a, y + b)) continue;
											
										Move m;
										m.x = x + a;
										m.y = y + b;
										m.flag = 1; // On flag
										return m;
									}
								}
							} 
						}
                    }
                }
				// Si on arrive ici, c'est qu'aucun des deux cas possibles n'a été trouvé.
				// Ainsi, on abandonne #PressF
                Move m;
				m.x = 0;
				m.y = 0;
				m.flag = -1;
				return m; 
            }
        }
    }
    // Si on atteint ici, c'est qu'aucune case n'a encore été révélée. On en révèle une au hasard.
    // Ce code ci-dessous doit donc être exécuté **une seule fois** !
    Move m;
    m.x = (int) (rand() % gridWidth(grid));
    m.y = (int) (rand() % gridHeight(grid));
    m.flag = 0;
    return m;
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
