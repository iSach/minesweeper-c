#include "Engine.h"
#include "Grid.h"

int play(int width, int height, int nbrBombs, Move (*computeMove)(Grid *), int show) {
    Grid *grid = gridInit(width, height, nbrBombs);
    while(gridWon(grid) == 0) {
        if(show) {
            gridPrint(grid);
        }
        Move move = (*computeMove)(grid);
        if(move.flag == -1) {
            return 0;
        } else if(move.flag == 1) {
            gridSetFlag(grid, move.x, move.y);
        } else {
            gridReveal(grid, move.x, move.y);
        }
    }
    gridPrint(grid);
    int result = gridWon(grid); // -1 = perdue, 1 = gagnée.
    return result;
}
