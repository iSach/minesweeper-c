//
//  Engine.h
//

#ifndef Engine_h
#define Engine_h

#include <stdio.h>
#include "Grid.h"

typedef struct Move_t Move;

struct Move_t {
    int x;
    int y;
    int flag;
};

int play(int width, int height, int nbrBombs, Move (*computeMove)(Grid *), int show);

#endif /* Engine_h */
