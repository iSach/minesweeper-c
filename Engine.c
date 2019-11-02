#include "Engine.h"

int play(int width, int height, int nbrBombs, Move (*computeMove)(Grid *), int show) {
    int a = width + height + nbrBombs + show;
    Grid *g = NULL;
    Move m = (*computeMove)(g);
    return a - a + m.x - m.x;
}
