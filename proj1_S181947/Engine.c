#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>
#include "Grid.h"

int play(int width, int height, int nbrBombs, Move (*computeMove)(Grid *), int show) {
	if(computeMove == NULL) {
		printf("Erreur : Pointeur nul passé en argument. Arrêt du programme.");
		exit(-1);
	}

    Grid *grid = gridInit(width, height, nbrBombs);
    while(gridWon(grid) == 0) {
        if(show) {
            gridPrint(grid);
        }
        Move move = (*computeMove)(grid);
        if(move.flag == -1) {
            gridFree(grid);
            return 0;
        } else if(move.flag == 1) {
            gridSetFlag(grid, move.x, move.y);
        } else {
            gridReveal(grid, move.x, move.y);
        }
    }
    gridPrint(grid);
    int result = gridWon(grid); // -1 = perdue, 1 = gagnée.
    gridFree(grid);
    return result;
}
