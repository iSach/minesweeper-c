//
//  Grid.h
//  


#ifndef Grid_h
#define Grid_h

#include <stdio.h>

typedef struct Grid_t Grid;

Grid *gridInit(int width, int height, int nbrBombs);
void gridFree(Grid *grid);
void gridPrint(Grid *grid);
int gridReveal(Grid *grid, int x, int y);

int gridValue(Grid *grid, int x, int y);
int gridIsExplored(Grid *grid, int x, int y);
int gridWidth(Grid *g);
int gridHeight(Grid *g);
int gridIsFlagged(Grid *grid, int x, int y);
void gridSetFlag(Grid *grid, int x, int y);
int gridWon(Grid *grid);


#endif /* Grid_h */
